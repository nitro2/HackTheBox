# Bounty Head

## CHALLENGE DESCRIPTION
While having a home-brewed Mezcal in your favourite tavern, one of the most wanted cyborgs in the galaxy entered the room, he wanted to exact revenge on you for putting him away back in your bounty hunting days. A intense melee involving hand-to-hand combat ensued. Once you pinned him down, you tried to backdoor his system and neutralise him but he managed to escape. You successfully extracted the firmware from his system and pulled off the inversion cable for his torsional magnesium drive capacitor, so he mustn't have gone far. Your mission is to identify the suspect's current location and apprehend him.


## Walkthrough

Unzip shows 2 file
```
-rw-r--r--@ 1   39K Jun 17  2020 boot_loader
-rw-r--r--@ 1  7.8M Jun 17  2020 efs.bin
```

The boot_loader is an INTEL_HEX format
```

```

So we decode it using python tool:

```python
import bincopy

f = bincopy.BinFile("boot_loader")
with open("boot.elf","wb+") as outfile:
    outfile.write(f.as_binary())
```

It's ELF file.
```
boot.elf: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=00b3b0209626fb55543d941467f253b9a2d26ec2, for GNU/Linux 3.2.0, stripped
```

So we decompile it using Ghidra

I find an interesting code inside it:
```C
void FUN_00101235(char *param_1)
{
  int iVar1;
  size_t sVar2;
  char local_108 [111];
  byte local_99;
  byte local_98 [16];
  undefined8 local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 local_70;
  undefined4 local_68;
  FILE *local_58;
  FILE *local_50;
  char *local_48;
  int local_3c;
  char *local_38;
  undefined4 local_30;
  uint local_2c;
  ulong local_28;
  char *local_20;
  int local_14;
  int local_10;
  uint local_c;
  
  local_78 = 0x6e69622e736665;
  local_70 = 0;
  local_68 = 0;
  local_88 = 0x12f6a8129a3a98f4;
  local_80 = 0x9614ba1a12aada12;
  local_30 = 0x10;
  local_38 = (char *)malloc(0x10);
  for (local_c = 0; local_c < 0x10; local_c = local_c + 1) {
    local_38[(int)local_c] = *(byte *)((long)&local_88 + (long)(int)local_c) >> 1 ^ 0x39;
  }
  iVar1 = strcmp(param_1,local_38);
  if (iVar1 == 0) {
    puts("\nLoading File System.");
    sVar2 = strlen(param_1);
    local_3c = (int)sVar2;
    local_14 = 0;
    for (local_10 = 0; local_10 < local_3c; local_10 = local_10 + 1) {
      sprintf(local_108 + local_14,"%02x",(ulong)((int)param_1[local_10] & 0xff));
      local_14 = local_14 + 2;
    }
    local_20 = local_108;
    for (local_28 = 0; local_28 < 0x10; local_28 = local_28 + 1) {
      __isoc99_sscanf(local_20,"%2hhx",local_98 + local_28);
      local_20 = local_20 + 2;
    }
    local_48 = "efs.bin";
    local_50 = fopen("efs.bin","rb");
    local_58 = fopen("temp_03455.bin","wb");
    if (local_50 == (FILE *)0x0) {
      puts("Error in reading filesystem");
    }
    else {
      while( true ) {
        iVar1 = feof(local_50);
        if (iVar1 != 0) break;
        fread(&local_99,1,1,local_50);
        for (local_2c = 0; local_2c < 0x10; local_2c = local_2c + 1) {
          local_99 = local_99 ^ local_98[(int)local_2c];
        }
        fwrite(&local_99,1,1,local_58);
      }
      puts("Error in loading filesystem");
    }
  }
  return;
}
```

So the bootloader open the `efs.bin` and write something to `temp_03455.bin`

Let make a similar C program to write our  `temp_03455.bin`

The output look strange and why `puts("Error in loading filesystem");` in the end?


Let check other places:

```c
void FUN_0010157f(void)

{
  char local_28 [28];
  int local_c;
  
  system("stty raw -echo min 0 time 10");
  local_c = getchar();
  if (local_c != 0x1b) {
    system("stty sane");
    putchar(10);
    puts("No working controllers found.");
    puts("Exiting..");
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  system("stty sane");
  puts("User input detected. Launching loader.");
  putchar(10);
  printf("password: ");
  fgets(local_28,0x11,stdin);
  FUN_00101235(local_28);
  system("stty sane");
  return;
}
```

What `stty raw -echo min 0 time 10` means?


Look like if we pipeline input, we can get something.

```
echo -ne "\x1b" | ./boot.elf

Secure Boot 2020.01 (April 02 2020 - 13:14:12 +0800)

DRAM: 128MiB
Message: Caches been cleared
Flash: 128MiB

Starting autoboot
stty: 'standard input': Inappropriate ioctl for device
stty: 'standard input': Inappropriate ioctl for device
User input detected. Launching loader.

stty: 'standard input': Inappropriate ioctl for device
```


Check service at server:

```s
nmap -sV -sC -Pn 161.35.36.167 -p 31202                                                                                 ─╯
Starting Nmap 7.93 ( https://nmap.org ) at 2023-04-21 22:33 +07
Nmap scan report for 161.35.36.167
Host is up (0.17s latency).

PORT      STATE SERVICE VERSION
31202/tcp open  unknown

```

Unknown :(


## Review

Look back previous functions, we have a summary of what is happening.

It's a linux like system. Upon bootup, it scans input from stty, requires a `password` of 17 bytes characters. `fgets(local_28,0x11,stdin);`

Then it put the password into `FUN_00101235(local_28);`
	
In this function, it compares the password by using this method: 

```c
  local_88 = 0x12f6a8129a3a98f4;
  ...
  local_38 = (char *)malloc(0x10);
  for (local_c = 0; local_c < 0x10; local_c = local_c + 1) {
    local_38[(int)local_c] = *(byte *)((long)&local_88 + (long)(int)local_c) >> 1 ^ 0x39;
  }
  iVar1 = strcmp(param_1,local_38);
```

If password is matched, it continues load `efs.bin` file and decode using this:

```c
        fread(&local_99,1,1,local_50);
        for (local_2c = 0; local_2c < 0x10; local_2c = local_2c + 1) {
          local_99 = local_99 ^ local_98[(int)local_2c];
        }
        fwrite(&local_99,1,1,local_58);
```

So the `temp_03455.bin` is what this statement said: `Once you pinned him down, you tried to backdoor his system and neutralise him but he managed to escape.`

Now our task is reverse the password.

Let create a small C program to print out the password:

[reverse_password.c](reverse_password.c)

I got this 

```c
char local_88[16] = {0xf4, 0x98, 0x3a, 0x9a, 0x12, 0xa8, 0xf6, 0x12, 
                        0x12, 0xda, 0xaa, 0x12, 0x1a, 0xba, 0x14, 0x96};
```

But I think I need to use other decompiler tool for x64 ELF. 

Look like ghidra cannot put these pieces together like I wanted

Let make it as input for our write2.c 