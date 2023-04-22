#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int iVar1;
    long local_88;
    char *local_38;
    uint32_t local_c;

    local_88 = 0x12f6a8129a3a98f4;

    local_38 = (char *)malloc(0x10);
    for (local_c = 0; local_c < 0x10; local_c = local_c + 1)
    {
        local_38[(int)local_c] = *(char *)((long)&local_88 + (long)(int)local_c) >> 1 ^ 0x39;
    }
    // printf("%s", local_38);

    // for (int i = 0; i < 0x10; i++) {
    //   printf("%02X",local_38[i]);
    // }
    // printf("\n");

    for (int i = 0; i < 0x10; i++) {
      printf("%c",local_38[i]);
    }
    return 0;
}
