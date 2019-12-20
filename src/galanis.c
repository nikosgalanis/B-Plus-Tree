#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/util.h"

extern int AM_errno;
#define AME_ERROR -1 //TODO: change this
#define CHAR_SIZE 40

void AM_Init() {
	BF_Init(MRU);
	/* Initialize the files array */
	Files = malloc(sizeof(Open_Files));
	if (Files == NULL) {
		printf("Memory exhausted!\n");
		return;
	}
	Files->open = malloc(MAX_OPEN_FILES * sizeof(File_info));
	if (Files->open == NULL) {
		printf("Memory exhausted!\n");
		return;
	}
	for (int i = 0; i < MAX_OPEN_FILES; ++i) {
		Files->open[i].file_index = EMPTY;
	}
	Files->total = 0;
	/* Initialize the scans array */
	Scans = malloc(sizeof(Open_Scans));
	if (Scans == NULL) {
		printf("Memory exhausted!\n");
		return;
	}
	Scans->open = malloc(MAX_SCAN_FILES * sizeof(Scan_info));
	if (Scans->open == NULL) {
		printf("Memory exhausted!\n");
		return;
	}
	for (int i = 0; i < MAX_OPEN_SCANS; ++i) {
		Scans->open[i].file_index = NULL;
	}
	Scans->total = 0;
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
	memcpy(Files->open[Files->total].file_name, fileName, sizeof(fileName));
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
  /* print the wanted string */
  printf("%s\n",errString);
  /* print an error message according to our global variable */
  switch (AM_errno) {
    case BF_ERR:
      printf("The error occured in the Block File level\n");
      return;
    case MEM_ERR:
      printf("The error occured during allocation of memory in stack\n");
      return;
    case MAX_FILES_ERR:
      printf("Tried to open too many files. Last requested file will not be oppened\n");
      return;
    case WRONG_FILE_ERR:
      printf("Tried to open a non-AM file. This file will not be oppened\n");
      return;
    case TYPE_ERR:
      printf("Tried to initialize a file with unknown type of data.\n");
  }
}

void AM_Close() {
  /* close the BF level */
  BF_Close();
  /* free the struct for the open files */
  free(Files->open);
  free(Files);
}
