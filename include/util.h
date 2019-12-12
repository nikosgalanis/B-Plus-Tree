#pragma once
/*includes*/
#include "../include/AM.h"
#include "../include/bf.h"
#include "../include/defn.h"
/*defines*/
#define MAX_OPEN_FILES 20
#define EMPTY_FILE -1
/*structs definition*/
typedef struct {
  void* a;
  void* b;
} Record;

typedef struct {
  int file_index;
  char* file_name;
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
int find_index(File_info* open);
char* concat(const char *s1, const char *s2);
