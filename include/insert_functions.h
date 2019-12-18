#pragma once

/* Includes */
#include "AM.h"
#include "bf.h"
#include "defn.h"

#include "./util.h"

/* Returns a string which contains exactly the info we want to append
  to the higher level block. */
char* split_data_block(int fileDesc, int block_num, Record* new_record);
/* Crates the root of B+ tree and returns its block index */
int create_root(int fileDesc, void* key);

boolean data_sorted_insert(int block_num, int fileDesc, Record new_record);

boolean record_fits_data(int fileDesc, int target_block_index);

int find_data_block(int fileDesc, int root_num, void *key);
