#pragma once

/* Includes */
#include "util.h"

boolean print_tree(int fileDesc, char print_type);
boolean print_index_block(int fileDesc, int target_block);
boolean print_data_block(int fileDesc, int target_block);
