#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/init_destroy_funcs.h"
#include "../include/util.h"

#define AME_ERROR -1 //TODO: change this

void AM_Init() {
	BF_Init(MRU);
	open_files_init();
	open_scans_init();
}

int AM_OpenIndex(char *fileName) {
	/* Open the file. and catch a possible error */
  int fileDesc;
  BF_OpenFile(fileName,&fileDesc);
	/* Find the index of the array that we want to insert the file */
	int index = find_empty_index(Files->open);
	/* We can not open more files than allowed */
	if (index == EMPTY)
		return AME_ERROR; //TODO: Change to a true error
	/*Initialize the file info*/
	Files->open[index].file_index = fileDesc;
	memcpy(Files->open[index].file_name, fileName, sizeof(fileName));
	/* Initialize a pointer to the first block */
	BF_Block* first_block;
	BF_Block_Init(&first_block);
	/* And get access to it */
	BF_GetBlock(fileDesc, 0, first_block);
	/* Get access to its data */
	char* first_block_info = BF_Block_GetData(first_block);
	/* Check if it is a AM file, and return sucess or failure */
	if (first_block_info[0] != 'B')
		return AME_ERROR;
	int offset = sizeof(char);
	/* Copy the total records */
	memcpy(&Files->open[index].total_records, first_block_info + offset, sizeof(int));
	/* That is how much we must come through to go to the max_pointers counter */
	offset = sizeof(char) + 2 * sizeof(int);
	memcpy(&Files->open[index].max_pointers, first_block_info + offset, sizeof(int));
	/* That is how much we must come through to go to the max_entries counter */
	offset += sizeof(int);
	memcpy(&Files->open[index].max_entries, first_block_info + offset, sizeof(int));
	/* Increase number of open files */
	Files->total++;
	/* Unpin and destroy the block */
	BF_UnpinBlock(first_block);
	BF_Block_Destroy(&first_block);
	return AME_OK;
}


int AM_CloseIndex (int fileDesc) {
	/* We want to close the file, thus remove the entry from the open files array */
	/* We are going to remove the entry, and replace its data with -1 and spaces */
	int indexDesc = find_index(fileDesc);
	strcpy(Files->open[indexDesc].file_name,"");
	Files->open[indexDesc].file_index = EMPTY;
	Files->open[indexDesc].total_records = -1;
	Files->open[indexDesc].max_entries = -1;
	Files->open[indexDesc].max_pointers = -1;
	/* Reduce the number of open files */
	Files->total--;
	/* Now close the file by calling the bf function */
	BF_CloseFile(fileDesc);
  return AME_OK;
}


int AM_DestroyIndex(char *fileName) {
	/*if(kati pou prepei na vroume) {
			mhn to kleiseis to gamidi
			return kapoio error
	}*/
	/*else, remove the file*/
	char* to_remove = concat("../", fileName);
	remove(to_remove);
  return AME_OK;
}

void AM_PrintError(char *errString) {
  printf("%s\n", errString);
  switch (AME_errno) {
    case AME_BF_ERR:
      printf("Block File lever error occured!\n");
      break;
    case AME_MEM_ERR:
      printf("Memory allocation error occured!\n");
      break;
    case AME_MAX_FILES_ERR:
      printf("Tried to open too many files. Last requested file will not be opened!\n");
      break;
    case AME_TYPE_ERR:
      printf("Tried to initialize a file with unknown type of data!\n");
			break;
		default:
			printf("Unknown error occured!\n");
			break;
  }
}

void AM_Close() {
  BF_Close();
	open_files_destroy();
	open_scans_destroy();
}
