#pragma once

/* Includes */
#include "AM.h"
#include "bf.h"
#include "defn.h"

/* Defines */
#define BUFF_SIZE 64
#define MAX_OPEN_FILES 20
#define EMPTY_FILE -1

/* Enumerations */
typedef enum s{
  false, true
} boolean;

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
