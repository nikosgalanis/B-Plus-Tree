#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/util.h"
#include "../include/insert_functions.h"

int create_root(int fileDesc,  void *key) {
  BF_Block *first_block, *root_block, *first_data_block, *second_data_block;
  int offset;
  /* Get data of 1st block */
  BF_Block_Init(&first_block);
  BF_GetBlock(fileDesc, 0, first_block);
  char *first_block_info = BF_Block_GetData(first_block);
  /* Type of key and the length */
  char keyType;
  int keyLength;
  offset = sizeof(char) + 4 * sizeof(int);
  memcpy(&keyType, first_block_info + offset, sizeof(char));
  offset = 2 * sizeof(char) + 4 * sizeof(int);
  memcpy(&keyLength, first_block_info + offset, sizeof(int));
  /* key to a new variable */
  /* Create root block */
  int root_block_index = -1;
  int blocks_num;
  BF_Block_Init(&root_block);

  BF_AllocateBlock(fileDesc, root_block);
  BF_GetBlockCounter(fileDesc,  &blocks_num);
  root_block_index = blocks_num - 1;

  char* root_block_info = BF_Block_GetData(root_block);
  /* Place 'I' and number of indexes in the block */
  char I = 'I';
  offset = 0;
  memcpy(root_block_info + offset, &I, sizeof(char) );
  int no_indxs = 1;
  offset = sizeof(char);
  memcpy(root_block_info + offset, &no_indxs, sizeof(int));
  /* Place the key and 2 data block indexes later*/

  /* Create 2 data blocks */
  BF_Block_Init(&first_data_block);
  BF_Block_Init(&second_data_block);
  /* Allocate blocks */
  BF_AllocateBlock(fileDesc, first_data_block);
  BF_GetBlockCounter(fileDesc,  &blocks_num);
  int first_data_block_index = blocks_num - 1;
  BF_AllocateBlock(fileDesc, first_data_block);
  BF_GetBlockCounter(fileDesc,  &blocks_num);
  int second_data_block_index = blocks_num - 1;

  char* first_data_block_info = BF_Block_GetData(first_data_block);
  char* second_data_block_info= BF_Block_GetData(second_data_block);
  /* Place 'D' and number of records in the block */
  char D = 'D';
  offset = 0;
  memcpy(first_data_block_info + offset, &D, sizeof(char));
  memcpy(second_data_block_info + offset, &D, sizeof(char));
  int no_records = 0;
  offset = sizeof(char);
  memcpy(first_data_block_info + offset, &no_records, sizeof(int));
  memcpy(second_data_block_info + offset, &no_records, sizeof(int));

  /* Now place the 2 data block indexes in the root block,
    and the key between them */
  offset = sizeof(char) + sizeof(int);
  memcpy(root_block_info + offset, &first_data_block_index, sizeof(int));
  offset = sizeof(char) + 2 * sizeof(int);
  memcpy(root_block_info + offset, key, keyLength);
  offset = sizeof(char) + 2 * sizeof(int) + keyLength;
  memcpy(root_block_info + offset, &second_data_block_index, sizeof(int));

  /* We place the index block of root in the first block */
  offset = sizeof(char) + sizeof(int);
  memcpy(first_block_info + offset, &root_block_index, sizeof(int));

  /*Set dirty, Unpin, Destroy*/
  BF_Block_SetDirty(first_block);
  BF_Block_SetDirty(root_block);
  BF_Block_SetDirty(first_data_block);
  BF_Block_SetDirty(second_data_block);
  BF_UnpinBlock(first_block);
  BF_UnpinBlock(root_block);
  BF_UnpinBlock(first_data_block);
  BF_UnpinBlock(second_data_block);
  BF_Block_Destroy(&root_block);
  BF_Block_Destroy(&first_block);
  BF_Block_Destroy(&second_data_block);
  BF_Block_Destroy(&first_data_block);
  //TODO in insert , call key as a pointer
  //TODO CALL_BF
  return root_block_index;
}

boolean data_sorted_insert(int block_num, int fileDesc, Record new_record) {
  /* Initialize a pointer to the first block, and get its data */
  BF_Block* first_block;
  BF_Block_Init(&first_block);
  BF_GetBlock(fileDesc, 0, first_block);
  char* first_block_info = BF_Block_GetData(first_block);
  int	offset = sizeof(char) + 3 * sizeof(int);
  /* Get the max possible records in a data block */
  int max_records;
  memcpy(&max_records, first_block_info + offset, sizeof(int));
  BF_Block* block;
  BF_Block_Init(&block);
  BF_GetBlock(fileDesc, block_num - 1, block);
  char* data = BF_Block_GetData(block);
  if (data[0] != 'D') {
    printf("Not a data block\n");
    //TODO Umpin and Destroy
    return false;
  }
  /* Initialize the offset to find where to store the new record */
  offset = sizeof(char) + sizeof(int);
  int total_records;
  /*If the block is full, return an error*/
  if (total_records == max_records)
    return false;
  int pos = 0;
  /* Traverse all the current records */
  for (int i = 0; i < total_records; ++i) {
    Record* curr_rec;
    memcpy(curr_rec, data + offset, sizeof(new_record));
    /* If the key of the record we want to insert is smaller than the current one */
    if (new_record.key < curr_rec->key) {
      /* Shift all records to the right to create space */
      memmove(data + offset + sizeof(new_record), data + offset, (total_records - i) * sizeof(new_record));
      /* Insert the new record */
      memcpy(data + offset, &new_record, sizeof(new_record));
      //TODO: Unpin and destroy
      return true;
    }
    /* Each time, update the offset */
    offset += sizeof(new_record);
   }
   /* If we haven't yet inserted the record, now its time(at the end of the block) */
   memcpy(data + offset, &new_record, sizeof(new_record));
   //TODO: Unpin and destroy
   return true;
}

char* split_data_block(int fileDesc, int block_num, Record* new_record, int* new_block_num) {
  /* Initialize a pointer to the block */
  BF_Block* block;
  BF_Block_Init(&block);
  /* And get access to it */
  BF_GetBlock(fileDesc, block_num, block);
  /* Get access to its data */
  char* block_info = BF_Block_GetData(block);
  /* Get the total records that the block contains*/
  int total_records;
  memcpy(&total_records, block_info + sizeof(char), sizeof(int));
  int offset = sizeof(char) + sizeof(int);
  /* Get the position that we want to split. If the block contains odd number of
    records, we want one more on the right */
  int split_pos = total_records / 2 + (total_records % 2 == 1);
  offset += split_pos * sizeof(new_record);
  /* Allocate and initialize a new data block */
  BF_Block* new_block;
  BF_Block_Init(&new_block);
  BF_AllocateBlock(fileDesc, new_block);
  char* new_block_data = BF_Block_GetData(new_block);
  /* Save its info: the fact that is a data block + the amount of records we are
    going to insert */
  char type = 'D'; int init_records = total_records - split_pos;
  memcpy(new_block_data, &type, sizeof(char));
  memcpy(new_block_data + sizeof(char), &init_records, sizeof(int));
  /* Copy the data from the full block to the empty, so we have two half-full
    data blocks */
  int new_block_offset = sizeof(char) + sizeof(int);
  memcpy(new_block_data + new_block_offset, block_info + offset, init_records * sizeof(new_record));
  /* Fill the rest of the first block with -1 values */
  memset(block_info + offset, -1, init_records * sizeof(new_record));
  /* Find and store the record that we want as a pointer for the index blocks
    level */
  char* record_data_to_return;
  offset -= sizeof(new_record);
  memcpy(record_data_to_return, block_info + offset, sizeof(new_record));
  Record* record_to_return = (Record*) record_data_to_return;
  /* Call BF_GetBlockCounter to find the number of the newly allocated block */
  BF_GetBlockCounter(fileDesc, new_block_num);
  (*new_block_num)--;
  /* Set the previously edited blocks dirty, and unpin the, from the memory */
  BF_Block_SetDirty(block);
  BF_Block_SetDirty(new_block);
  BF_UnpinBlock(block);
  BF_UnpinBlock(new_block);
  BF_Block_Destroy(&block);
  BF_Block_Destroy(&new_block);
  /* Return the key of the record we want as an index */
  return (char*)record_to_return -> key;
}

int find_data_block_to_insert(int fileDesc, int root_num, void *key) {
  /* Initialize a pointer to a block */
  BF_Block* first_block;
  BF_Block_Init(&first_block);
  /* Get acces to the data of the very first block */
  BF_GetBlock(fileDesc, 0, first_block);
  /* Get access to its data */
  char* first_block_info = BF_Block_GetData(first_block);
  /* Get the attribute type of key */
  char key_type;
  memcpy(&key_type, first_block_info + sizeof(char) + 4 * sizeof(int), sizeof(char));
  printf("%c\n",key_type);
  /* Unpin and destroy block */
  BF_UnpinBlock(first_block);
  BF_Block_Destroy(&first_block);

  return 1;
}
