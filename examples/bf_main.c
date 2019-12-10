#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bf.h"

#define CALL_OR_DIE(call)     \
  {                           \
    BF_ErrorCode code = call; \
    if (code != BF_OK) {      \
      BF_PrintError(code);    \
      exit(code);             \
    }                         \
  }

int main() {
  int fd;
  BF_Block *block;
  BF_Block_Init(&block);

  CALL_OR_DIE(BF_Init(LRU));
  CALL_OR_DIE(BF_CreateFile("data.db"))
  CALL_OR_DIE(BF_OpenFile("data.db", &fd));

  char* data;
  for (int i = 0; i < 1000; ++i) {
    CALL_OR_DIE(BF_AllocateBlock(fd, block));
    data = BF_Block_GetData(block);
    memset(data, i % 127, BF_BUFFER_SIZE);
    BF_Block_SetDirty(block);
    CALL_OR_DIE(BF_UnpinBlock(block));
  }
  for (int i = 0; i < 1000; ++i) {
    CALL_OR_DIE(BF_GetBlock(fd, i, block));
    data = BF_Block_GetData(block);
    printf("block = %d and data = %d\n", i, data[0]);
    CALL_OR_DIE(BF_UnpinBlock(block));
  }

  BF_CloseFile(fd);
  BF_Close();

  CALL_OR_DIE(BF_Init(LRU));
  CALL_OR_DIE(BF_OpenFile("data.db", &fd));
  int blocks_num;
  CALL_OR_DIE(BF_GetBlockCounter(fd, &blocks_num));

  for (int i = 0; i < blocks_num; ++i) {
    CALL_OR_DIE(BF_GetBlock(fd, i, block));
    data = BF_Block_GetData(block);
    printf("block = %d and data = %d\n", i, data[i % BF_BUFFER_SIZE]);
    CALL_OR_DIE(BF_UnpinBlock(block));
  }

  BF_Block_Destroy(&block);
  BF_CloseFile(fd);
  BF_Close();
}
