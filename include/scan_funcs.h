#pragma once

/* Includes */
#include "util.h"
#include "stack.h"

int init_entry(int scan_index);

int find_first_entry(int file_desc, int block_num, void *key, char key_type,
                     int key_size, int* entry_num);

int get_entry_value(int scan_index, void* value);

Stack* find_data_block(int file_desc, int root_num, void *key, char key_type,
                       int key_size);
