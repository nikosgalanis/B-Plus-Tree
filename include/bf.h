#ifndef BF_H
#define BF_H

#ifdef __cplusplus
extern "C" {
#endif

#define BF_BLOCK_SIZE 512
#define BF_BUFFER_SIZE 250
#define BF_MAX_OPEN_FILES 100

typedef enum BF_ErrorCode {
  BF_OK,
  BF_OPEN_FILES_LIMIT_ERROR,
  BF_INVALID_FILE_ERROR,
  BF_ACTIVE_ERROR,
  BF_FILE_ALREADY_EXISTS,
  BF_FULL_MEMORY_ERROR,
  BF_INVALID_BLOCK_NUMBER_ERROR,
  BF_ERROR
} BF_ErrorCode;

typedef enum ReplacementAlgorithm {
  LRU,
  MRU
} ReplacementAlgorithm;


typedef struct BF_Block BF_Block;

void BF_Block_Init(BF_Block **block);

void BF_Block_Destroy(BF_Block **block);

void BF_Block_SetDirty(BF_Block *block);

char* BF_Block_GetData(const BF_Block *block);

BF_ErrorCode BF_Init(const ReplacementAlgorithm repl_alg);

BF_ErrorCode BF_CreateFile(const char* filename);

BF_ErrorCode BF_OpenFile(const char* filename, int *file_desc);

BF_ErrorCode BF_CloseFile(const int file_desc);

BF_ErrorCode BF_GetBlockCounter(const int file_desc, int *blocks_num);

BF_ErrorCode BF_AllocateBlock(const int file_desc, BF_Block *block);

BF_ErrorCode BF_GetBlock(const int file_desc,
                         const int block_num,
                         BF_Block *block);

BF_ErrorCode BF_UnpinBlock(BF_Block *block);

void BF_PrintError(BF_ErrorCode err);

void BF_Close();

#ifdef __cplusplus
}
#endif
#endif // BF_H
