#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void FUN_00101235(char *param_1)

{
  int iVar1;
  size_t sVar2;
  char local_108 [111];
  uint8_t local_99;
  uint8_t local_98 [16];
  uint64_t local_88;
  uint64_t local_80;
  uint64_t local_78;
  uint64_t local_70;
  uint64_t local_68;
  FILE *local_58;
  FILE *local_50;
  char *local_48;
  int local_3c;
  char *local_38;
  uint32_t local_30;
  uint32_t local_2c;
  uint32_t local_28;
  char *local_20;
  int local_14;
  int local_10;
  uint32_t local_c;
  
  local_78 = 0x6e69622e736665;
  local_70 = 0;
  local_68 = 0;
  local_88 = 0x12f6a8129a3a98f4;
  local_80 = 0x9614ba1a12aada12;
  local_30 = 0x10;
  // local_38 = (char *)malloc(0x10);
  // for (local_c = 0; local_c < 0x10; local_c = local_c + 1) {
  //   local_38[(int)local_c] = *(uint8_t *)((long)&local_88 + (long)(int)local_c) >> 1 ^ 0x39;
  // }
  // iVar1 = strcmp(param_1,local_38);
  iVar1 = 0;
  if (iVar1 == 0) {
    puts("\nLoading File System.");
    sVar2 = strlen(param_1);
    local_3c = (int)sVar2;
    local_14 = 0;
    for (local_10 = 0; local_10 < local_3c; local_10 = local_10 + 1) {
      sprintf(local_108 + local_14,"%02x",(uint32_t)((int)param_1[local_10] & 0xff));
      local_14 = local_14 + 2;
    }
    local_20 = local_108;
    for (local_28 = 0; local_28 < 0x10; local_28 = local_28 + 1) {
      sscanf(local_20,"%2hhx",local_98 + local_28);
      local_20 = local_20 + 2;
    }
    printf("%s\n", local_20);
    local_48 = "efs.bin";
    local_50 = fopen("efs.bin","rb");
    local_58 = fopen("temp_03455.bin","wb");
    if (local_50 == (FILE *)0x0) {
      puts("Error in reading filesystem");
    }
    else {
      while( 1 ) {
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
  else {
    printf("Wrong pass\n");
  }
  return;
}

int main()
{
    // What is the password?
    char pass [28];
    fgets(pass,0x11,stdin);
    FUN_00101235(pass);
}