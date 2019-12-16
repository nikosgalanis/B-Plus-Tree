#pragma once

/* Includes */
#include "AM.h"
#include "bf.h"
#include "defn.h"

/* Macro instructions for error catching */


#define CALL_BF(call)           \
{                               \
  BF_ErrorCode code = call;     \
  if (code != BF_OK) {          \
    CALL_AM(BF_ERR);            \
    BF_PrintError(code);        \
  }                             \
}

#define CALL_AM(call)                         \
{                                             \
  AM_ErrorCode code = call;                   \
  if (code != AM_OK) {                        \
    AM_PrintError("An error has occured");    \
    AM_errno = code;                          \
  }                                           \
}

/* Defines */
#define BUFF_SIZE 64
#define MAX_OPEN_FILES 20
#define EMPTY_FILE -1

/* Enumerations */
typedef enum s{
  false, true
} boolean;

typedef enum {
  AM_OK,
  BF_ERR,
  MEM_ERR,
  MAX_FILES_ERR,
  WRONG_FILE_ERR,
  TYPE_ERR
} AM_ErrorCode;

/* Structs definition */
typedef struct {
  void* key;
  void* value;
} Record;

typedef struct {
  int file_index;
  char file_name[BUFF_SIZE];
  int total_records;
  int max_pointers;
  int max_entries;
} File_info;

typedef struct {
  File_info* open;
  int total;
} Open_Files;

/* Global variables definition */
Open_Files* Files;

/* Function headers definition */
int find_empty_index();
int find_index(int fileDesc);
char* concat(const char *s1, const char *s2);
int compare(void *op1, int op, void *op2, char type);
