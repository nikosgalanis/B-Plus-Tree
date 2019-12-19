#pragma once

/* Includes */
#include "util.h"

Record* create_record(int fileDesc, void* key, void* value);
/* Returns a string which contains exactly the info we want to append
  to the higher level block. */
char* split_data_block(int fileDesc, int block_num, Record* new_record, char key_type, int key_size);
/* Crates the root of B+ tree and returns its block index */
int create_root(int fileDesc, char* append);

int create_empty_root(int fileDesc,  void *key);

boolean data_sorted_insert(int block_num, int fileDesc, Record* new_record, char key_type);

boolean index_sorted_insert(int block_num, int fileDesc, char* new_tuple, char key_type, int key_size);


boolean record_fits_data(int fileDesc, int target_block_index);

Stack* find_data_block(int fileDesc, int root_num, void *key);
