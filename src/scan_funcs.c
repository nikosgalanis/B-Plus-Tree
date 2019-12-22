#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/scan_funcs.h"

int init_entry(int scan_index) {
  /* Get useful info */
  int fd = Scans->open[scan_index].file_index;
  int fileDesc =  Files->open[fd].file_index;
  /* To get access to the root, get to the first block */
  int root;
  BF_Block* first_block;
  BF_Block_Init(&first_block);
  CALL_BF(BF_GetBlock(fileDesc, 0, first_block));
  char* data = BF_Block_GetData(first_block);
  memcpy(&root, data + sizeof(char) + sizeof(int), sizeof(int));
  /* Unpin and Destroy the block pointer */
  BF_UnpinBlock(first_block);
  BF_Block_Destroy(&first_block);
  char key_type = Files->open[fd].attr_type_1;
  int key_size = Files->open[fd].attr_length_1;
  void* key = malloc(key_size);
  memcpy(key, Scans->open[scan_index].value, key_size);
  /* First find data block in which value is stored */
  Stack* path;
  path = find_data_block(fileDesc, root, key, key_type, key_size);
  int target_block_index = Pop(path);
  printf("DATA BLOCK %d\n", target_block_index);

  return AME_OK;
}

int find_first_entry(int file_desc, int block_num, void* key, char key_type,
  int key_size, int* entry_num) {

  int attr_length_1 = Files->open[file_desc].attr_length_1;
  int attr_length_2 = Files->open[file_desc].attr_length_2;
  /* Get data block */
  BF_Block* block;
	BF_Block_Init(&block);
	BF_GetBlock(file_desc, block_num, block);
	char* block_data = BF_Block_GetData(block);

  /* Find entry */
  int offset = sizeof(char);
  int no_records;
  memcpy(&no_records, block_data + offset, sizeof(int));
  offset += 3 * sizeof(int);

  /* Initialize a new record */
  Record* record = malloc(sizeof(Record) + attr_length_1 + attr_length_2);

  /* Iterate over records */
  boolean found = false;
  for (int i = 0; i < no_records; ++i) {
    memcpy(record, block_data + offset, sizeof(Record) + attr_length_1 + attr_length_2);
    int result = compare(key, LESS_THAN_OR_EQUAL, record->key, key_type);
    if (result) {
      *entry_num = i;
      found = true;
      break;
    }
    offset += sizeof(record->size);
  }

  /* Unpin and destroy block */
  BF_UnpinBlock(block);
	BF_Block_Destroy(&block);
  free(record);

  /* Check if entry was found */
  if (!found) {
    return AME_EOF;
  }

  return AME_OK;
}

int get_entry_value(int scan_index, void* value) {
  /* Extract info from scan_index open scan */
  int fd = Scans->open[scan_index].file_index;
  int block_num = Scans->open[scan_index].last_entry.no_block;
  int entry_num = Scans->open[scan_index].last_entry.no_entry;
  int attr_length_1 = Files->open[fd].attr_length_1;
  int attr_length_2 = Files->open[fd].attr_length_2;

  /* Get data block */
  BF_Block* block;
  BF_Block_Init(&block);
  BF_GetBlock(fd, block_num, block);
  char* block_data = BF_Block_GetData(block);

  int offset = sizeof(char);
  int no_records;
  memcpy(&no_records, block_data + offset, sizeof(int));
  offset += 2 * sizeof(int);
  int next_block_num;
  memcpy(&next_block_num, block_data + offset, sizeof(int));

  /* Initialize a new record */
  Record* record = malloc(sizeof(Record) + attr_length_1 + attr_length_2);

  /* Check if entry_num exists in current data block */
  if (entry_num < no_records) {
    offset += entry_num * (sizeof(Record) + attr_length_1 + attr_length_2);
    memcpy(record, block_data + offset, sizeof(Record) + attr_length_1 + attr_length_2);
    memcpy(value, record->value, attr_length_2);
  /* Go to the next data block if that exists */
  } else {
    if (next_block_num != -1) {
      /* Update last entry */
      Scans->open[scan_index].last_entry.no_block = next_block_num;
      Scans->open[scan_index].last_entry.no_entry = 0;
      /* Unpin past block */
      BF_UnpinBlock(block);
      /* Get new block and store value */
      BF_GetBlock(fd, next_block_num, block);
      block_data = BF_Block_GetData(block);
      memcpy(record, block_data + offset, sizeof(Record) + attr_length_1 + attr_length_2);
      memcpy(value, record->value, attr_length_2);
    } else {
      Scans->open[scan_index].error = true;
      /* Unpin and destroy block */
      BF_UnpinBlock(block);
      BF_Block_Destroy(&block);
      free(record);
      /* Return error */
      return AME_EOF;
    }
  }

  /* Everything ok, so unpin and destroy block */
  BF_UnpinBlock(block);
  BF_Block_Destroy(&block);
  free(record);

  return AME_OK;
}

Stack* find_data_block(int file_desc, int root_num, void* key, char key_type, int key_size) {
	/* Initialize a stack to keep our path*/
	Stack* path = InitializeStack();
	/* Initialize a new block */
	BF_Block* block;
	BF_Block_Init(&block);
	/* Starting from the root */
	int block_num = root_num;
	char block_idf;
	do {
		BF_GetBlock(file_desc, block_num, block);
		/* Push the block number to the stack */
    Push(path, block_num);
		char* block_info = BF_Block_GetData(block);
		/* Get block identifier */
		int block_offset = 0;
		memcpy(&block_idf, block_info, sizeof(char));
		block_offset += sizeof(char);
		if (block_idf == 'D') {
			BF_UnpinBlock(block);
			BF_Block_Destroy(&block);
			return path;
		}
		/* Get number of indexes stored into the block */
		int no_indxs;
		memcpy(&no_indxs, block_info + block_offset, sizeof(int));
		block_offset += sizeof(int);
		boolean found = false;
		for (int i = 0; i < no_indxs; ++i) {
			/* surpass index to block */
			block_offset += sizeof(int);
			void* block_key = malloc(key_size);
			memcpy(block_key, block_info + block_offset, key_size);
			/* Compare key with block key */
			int result = compare(key, LESS_THAN_OR_EQUAL, block_key, key_type);
			/* if given key <= block key go to corresponding block */
			if (result) {
				/* Update the found variable */
				found = true;
				/* Get corresponding index */
				memcpy(&block_num, block_info + block_offset - sizeof(int), sizeof(int));
				/* Unpin index block */
				BF_UnpinBlock(block);
				/* Go one level down to the B+ tree */
				break;
			}
			block_offset += key_size;
			free(block_key);
		}
		if (found == false) {
			/* We reached the end of the index block, so visit the most right branch */
			memcpy(&block_num, block_info + block_offset, sizeof(int));
			BF_UnpinBlock(block);
		}
	}	while (block_idf == 'I');
}
