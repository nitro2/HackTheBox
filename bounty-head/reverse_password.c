#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int iVar1;
    uint64_t local_80;
    char *local_38;
    uint32_t local_c;

    char local_88[16] = {0xf4, 0x98, 0x3a, 0x9a, 0x12, 0xa8, 0xf6, 0x12, 
                        0x12, 0xda, 0xaa, 0x12, 0x1a, 0xba, 0x14, 0x96};
    
    local_38 = (char *)malloc(0x10);
    for (local_c = 0; local_c < 0x10; local_c = local_c + 1)
    {
        // local_38[(int)local_c] = *(char *)((uint64_t)&local_88 + (uint64_t)(int)local_c) >> 1 ^ 0x39;
        local_38[local_c] = local_88[local_c] ;
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
