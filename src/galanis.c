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
  CALL_BF(BF_OpenFile(fileName, &fileDesc));
	/* Find the index of the array that we want to store the file descriptor */
	int index;
	CALL_OR_DIE(find_empty_index(&index));
	/* Initialize the file info */
	Files->open[index].file_index = fileDesc;
	memcpy(Files->open[index].file_name, fileName, sizeof(fileName));
	/* Initialize a pointer to the first block */
	BF_Block* first_block;
	BF_Block_Init(&first_block);
	/* And get access to it */
	CALL_BF(BF_GetBlock(fileDesc, 0, first_block));
	/* Get access to its data */
	char* first_block_info = BF_Block_GetData(first_block);
	/* Check if it is a AM file, and return sucess or failure */
	if (first_block_info[0] != 'B') {
		AM_errno = AME_OPEN_TYPE_ERR;
		return AME_OPEN_TYPE_ERR;
	}
	/* Copy the total records */
	int offset = sizeof(char);
	memcpy(&Files->open[index].total_records, first_block_info + offset, sizeof(int));
	/* Get the root_id */
	offset += sizeof(int);
	memcpy(&Files->open[index].root_id, first_block_info + offset, sizeof(int));
	/* That is how much we must come through to go to the max_pointers counter */
	offset += sizeof(int);
	memcpy(&Files->open[index].max_pointers, first_block_info + offset, sizeof(int));
	/* That is how much we must come through to go to the max_entries counter */
	offset += sizeof(int);
	memcpy(&Files->open[index].max_entries, first_block_info + offset, sizeof(int));
	/* Get attribute 1 type */
	offset += sizeof(int);
	memcpy(&Files->open[index].attr_type_1, first_block_info + offset, sizeof(char));
	/* Get attribute 1 length */
	offset += sizeof(char);
	memcpy(&Files->open[index].attr_length_1, first_block_info + offset, sizeof(int));
	/* Check if type and length of attribute 1 is compatible */
	CALL_OR_DIE(type_length_match(Files->open[index].attr_type_1, Files->open[index].attr_length_1));
	/* Get attribute 2 type */
	offset += sizeof(int);
	memcpy(&Files->open[index].attr_type_2, first_block_info + offset, sizeof(char));
	/* Get attribute 2 length */
	offset += sizeof(char);
	memcpy(&Files->open[index].attr_length_2, first_block_info + offset, sizeof(int));
	/* Check if type and length of attribute 2 is compatible */
	CALL_OR_DIE(type_length_match(Files->open[index].attr_type_2, Files->open[index].attr_length_2));
	/* Increase number of open files */
	Files->total++;
	/* Unpin and destroy the block */
	BF_UnpinBlock(first_block);
	BF_Block_Destroy(&first_block);
	return AME_OK;
}


int AM_CloseIndex(int fileDesc) {
	/* Check if file is open */
	int index_desc;
	CALL_OR_DIE(find_index(fileDesc, &index_desc));
	/* Furthermore, check if file is in Scans->open array */
	for (int i = 0; i < MAX_SCAN_FILES; ++i) {
    if (Scans->open[i].file_index == fileDesc) {
			AM_errno = AME_SCAN_IS_USED_ERR;
      return AME_SCAN_IS_USED_ERR;
    }
  }
	/**
		File is open and there is no open_scan on this file so we can proceed to
		close this index. We want to close the file, thus remove the entry f
		rom the open files array. We are going to remove the entry, and replace its
		data with -1 and spaces.
	*/
	strcpy(Files->open[index_desc].file_name, "");
	Files->open[index_desc].file_index = EMPTY;
	Files->open[index_desc].total_records = -1;
	Files->open[index_desc].root_id = -1;
	Files->open[index_desc].max_entries = -1;
	Files->open[index_desc].max_pointers = -1;
	Files->open[index_desc].attr_type_1 = '\0';
	Files->open[index_desc].attr_length_1 = -1;
	Files->open[index_desc].attr_type_2 = '\0';
	Files->open[index_desc].attr_length_2 = -1;
	/* Reduce the number of open files */
	Files->total--;
	/* Now close the file by calling the bf function */
	CALL_BF(BF_CloseFile(fileDesc));
  return AME_OK;
}


int AM_DestroyIndex(char *fileName) {
	/* Check if file is open */
	boolean file_is_open = false;
	for (int i = 0; i < MAX_OPEN_FILES; ++i) {
    if (!strcmp(Files->open[i].file_name, fileName)) {
			file_is_open = true;
			break;
    }
  }
	if (!file_is_open) {
		AM_errno = AME_FILE_NOT_OPEN_ERR;
		return AME_FILE_NOT_OPEN_ERR;
	}
	/* Check if scan is open. If not trigger corresponding error */
	boolean scan_is_open = false;
	for (int i = 0; i < MAX_SCAN_FILES; ++i) {
		if (!strcmp(Scans->open[i].file_name, fileName)) {
			scan_is_open = true;
			break;
		}
	}
	if (scan_is_open) {
		AM_errno = AME_SCAN_IS_USED_ERR;
		return AME_SCAN_IS_USED_ERR;
	}
	/* Everything ok so destroy index */
	char* to_remove = concat("../", fileName);
	remove(to_remove);
  return AME_OK;
}

void AM_PrintError(char *errString) {
  printf("%s\n", errString);
  switch (AM_errno) {
    case AME_BF_ERR:
      printf("BF error occured!\n");
      break;
    case AME_MEM_ERR:
      printf("Memory allocation error occured!\n");
      break;
    case AME_MAX_FILES_ERR:
      printf("Tried to open too many files. Last requested file will not be opened!\n");
      break;
		case AME_MAX_SCANS_ERR:
			printf("Tried to open too many scans. Last requested scan will not be opened!\n");
			break;
		case AME_INTEGER_ERR:
			printf("Tried to initialize a file with incompatible integer length!\n");
			break;
		case AME_FLOAT_ERR:
			printf("Tried to initialize a file with incompatible float length!\n");
			break;
		case AME_STRING_ERR:
			printf("Tried to initialize a file with incompatible string length!\n");
			break;
    case AME_TYPE_ERR:
      printf("Tried to initialize a file with unknown type of data!\n");
			break;
		case AME_OPEN_TYPE_ERR:
			printf("Tried to open unknown type of file!\n");
			break;
		case AME_FILE_NOT_OPEN_ERR:
			printf("Requested file is not open error!");
			break;
		case AME_SCAN_NOT_OPEN_ERR:
			printf("Requested scan is not open error!");
			break;
		case AME_FILE_IS_USED_ERR:
			printf("This file is already being used error!");
			break;
		case AME_SCAN_IS_USED_ERR:
			printf("This scan is already being used error!");
			break;
		case AME_OP_ERR:
			printf("Tried to use an unknown type of operator!");
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
