#pragma once

/* Includes */
#include "AM.h"
#include "bf.h"
#include "defn.h"
#include "stack.h"

/* Defines */
#define MAX_OPEN_FILES 20
#define MAX_SCAN_FILES 20
#define BUFF_SIZE 64
#define EMPTY -1

/* Enumerations */
typedef enum s {
  false, true
} boolean;

/* Structs definition */
typedef struct {
  void* key;
  void* value;
  int size;
} Record;

typedef struct {
  int file_index;
  char file_name[BUFF_SIZE];
  int total_records;
  int root_id;
  int max_pointers;
  int max_entries;
  char attr_type_1;
  int attr_length_1;
  char attr_type_2;
  int attr_length_2;
} File_info;

typedef struct {
  File_info* open;
  int total;
} Open_Files;

typedef struct {
	int no_block;
	int no_entry;
} Entry_index;

typedef struct {
  int file_index;
  char file_name[BUFF_SIZE];
  int op;
  void* value;
  Entry_index last_entry;
} Scan_info;

typedef struct {
  Scan_info* open;
  int total;
} Open_Scans;

/* Global variables definition */
Open_Files* Files;
Open_Scans* Scans;

/* Macro instructions for error catching */
#define CALL_BF(call)         \
{                             \
  BF_ErrorCode code = call;   \
  if (code != BF_OK) {        \
    BF_PrintError(code);      \
    CALL_OR_DIE(AME_BF_ERR);  \
  }                           \
}

#define CALL_OR_DIE(call)       \
{                               \
  int code = call;              \
  if (code != AME_OK) {         \
    AM_errno = code;            \
    return code;                \
  }                             \
}

/**
  Returns the index in Files->open array to store a new open file in this
  position, otherwise AM_MAX_FILES_ERR is returned
*/
int find_empty_index(int* index);
/**
  Returns the index in Scans->open array to store a new scan file in this
  position, otherwise AM_MAX_SCAN_ERR is returned
*/
int find_empty_scan(int* index);
/**
  Returns the index in Files->open array in which fileDesc was found, otherwise
  AM_NOT_OPEN_FILE_ERR is returned
*/
int find_index(int fileDesc, int* index);
/**
  Returns the index in Scans->open array in which fileDesc was found, otherwise
  AM_NOT_OPEN_SCAN_ERR is returned
*/
int find_scan(int fileDesc, int* index);
/* Given two strings, this method returns the concatenated string */
char* concat(const char *s1, const char *s2);
/**
 Checks if type is integer, float or string and if the length parameter is
 compatible with the given type. In case of failure it returns AM_TYPE_ERR
*/
int type_length_match(char type, int length);
/* A generic compar function give the two operands, the operator and the type */
int compare(void *op1, int op, void *op2, char type);


boolean print_data_block(int fileDesc, int target_block);
boolean print_index_block(int fileDesc, int target_block);
boolean print_tree(int fileDesc, char print_type);
