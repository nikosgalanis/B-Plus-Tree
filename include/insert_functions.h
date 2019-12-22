#pragma once

/* Includes */
#include "util.h"

/* Function declarations */

/** Get the key and the value, and create the struct that we are going to use
	  as a record in our file */
Record* create_record(int fileDesc, void* key, void* value);

/* Create a root as a result of a previous root split */
int create_root(int fileDesc, char* append, int key_size);

/** Create an empty root, for the first time that we want to insert something in
		our tree */
int create_empty_root(int fileDesc,  void *key);

/* Returns a string which contains exactly the info we want to append
  to the higher level block. */
char* split_data_block(int fileDesc, int block_num, Record* new_record, char key_type, int key_size);
char* split_index_block(int fileDesc, int block_num, char* new_entry, char key_type, int key_size);

boolean data_sorted_insert(int block_num, int fileDesc, Record* new_record, char key_type);
boolean index_sorted_insert(int block_num, int fileDesc, char* new_tuple, char key_type, int key_size);


boolean record_fits_data(int fileDesc, int target_block_index);
boolean key_fits_index(int fileDesc, int target_block_index);

Stack* find_data_block(int fileDesc, int root_num, void *key, char key_type, int key_size);
