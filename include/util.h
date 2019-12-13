#pragma once
/*includes*/
#include "AM.h"
#include "bf.h"
#include "defn.h"
/*defines*/
#define BUFF_SIZE 64
#define MAX_OPEN_FILES 20
#define EMPTY_FILE -1
/*structs definition*/
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
/*global variables definition*/
Open_Files* Files;
/*function headers definition*/
int find_empty_index();
int find_index(int fileDesc);
char* concat(const char *s1, const char *s2);
char* split_data_block(int fileDesc, int block_num, Record* new_record, int* new_block_num);
