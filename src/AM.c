#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "init_destroy_funcs.h"
#include "util.h"
#include "insert_functions.h"
#include "scan_funcs.h"
#include "stack.h"

/* Initialize the AM level */
void AM_Init() {
	BF_Init(MRU);
	open_files_init();
	open_scans_init();
}

/* Create a new file, based on a B+ tree index */
int AM_CreateIndex(const char* fileName, char attrType1, int attrLength1,
									 char attrType2, int attrLength2) {
	/**
		At first check if type and length fields match each other. Otherwise do not
		create any AM file
	*/
	CALL_OR_DIE(type_length_match(attrType1, attrLength1));
	CALL_OR_DIE(type_length_match(attrType2, attrLength2));

	int offset;
	/* Create the file */
  CALL_BF(BF_CreateFile(fileName));
  /* Open it, to edit the first block */
  int fileDesc;
  CALL_BF(BF_OpenFile(fileName, &fileDesc));
  /* Get access to the first block */
  BF_Block* first_block;
  /* Initialize the first block */
  BF_Block_Init(&first_block);
  CALL_BF(BF_AllocateBlock(fileDesc, first_block));
  /* Get the total number of available blocks(1) */
  int blocks_num;
  CALL_BF(BF_GetBlockCounter(fileDesc, &blocks_num));
  /* Get access to the first block */
  CALL_BF(BF_GetBlock(fileDesc, blocks_num - 1, first_block));

  /* Set its data to be the character 'B', so we can recognize the B+ files */
  char* first_block_info = BF_Block_GetData(first_block);
  char idf = 'B';
  memcpy(first_block_info, &idf, sizeof(char));

  /* Also, store the total number of records in the file */
  int init_no_records = 0;
	/* Initialize our offset */
	offset = sizeof(char);
  memcpy(first_block_info + offset, &init_no_records, sizeof(int));

  /* Also, store the block that the root is in */
  int root_block_no = -1;
	offset = sizeof(char) + sizeof(int);
  memcpy(first_block_info + offset, &root_block_no, sizeof(int));

  /* Also, max pointers of index and max records of a block */
	/* If we have a string as a key, inrease its length by 1 to store the \0 */
	int attrLength1new;
	int attrLength2new;
	if (attrType1 == 'c') {
		attrLength1new = attrLength1 + 1;
	}
	else {
		attrLength1new = attrLength1;
	}
	if (attrType2 == 'c') {
		attrLength2new = attrLength2 + 1;
	}
	else {
		attrLength2new = attrLength2;
	}
  /* the following math occur from the metadata that we must store */
  int max_index_keys = ((BF_BLOCK_SIZE - 2 * sizeof(int) - sizeof(char)) / (attrLength1new + sizeof(int)));
  int max_records_block = (BF_BLOCK_SIZE - 3 * sizeof(int) - sizeof(char)) / (sizeof(Record) + attrLength1new + attrLength2new);
	offset = sizeof(char) + 2 * sizeof(int);
  memcpy(first_block_info + offset, &max_index_keys, sizeof(int));
	offset = sizeof(char) + 3 * sizeof(int);
  memcpy(first_block_info + offset, &max_records_block, sizeof(int));
  /* Also the attr1 and it's length , and attr2 and it's length */
	/* a */
	offset = sizeof(char) + 4 * sizeof(int);
	memcpy(first_block_info + offset, &attrType1, sizeof(char));
	offset = 2 * sizeof(char) + 4 * sizeof(int);
	memcpy(first_block_info +  offset, &attrLength1new, sizeof(int));
	/* b */
	offset = 2 * sizeof(char) + 5 * sizeof(int);
	memcpy(first_block_info + offset , &attrType2, sizeof(char));

	offset = 3 * sizeof(char) + 5 * sizeof(int);
	memcpy(first_block_info + offset , &attrLength2new, sizeof(int));

  /* We've changed the block data, so its dirty */
  BF_Block_SetDirty(first_block);
  /* Unpin the block from the memory */
  BF_UnpinBlock(first_block);
	/* Destroy the block pointer */
	BF_Block_Destroy(&first_block);
	/* Close the file */
	BF_CloseFile(fileDesc);

  return AME_OK;
}

/* Destroy the existing file from the disc */
int AM_DestroyIndex(char *fileName) {
	/* Check if file is open */
	if( access( fileName, F_OK ) != -1 ) {
			remove(fileName);
	} else {
		printf("File does not exist\n");
	}
  return AME_OK;
}

/* Open the file named "fileName", if found on the disk */
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
	/* Return file descriptor */
	return fileDesc;
}

/* Close the file, if it is already opened */
int AM_CloseIndex(int fileDesc) {
	/* Check if file is open */
	int index_desc;
	CALL_OR_DIE(find_index(fileDesc, &index_desc));
	/* Furthermore, check if file is in Scans->open array */
	for (int i = 0; i < Files->total; ++i) {
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

/* Insert an entry in the B+ tree file */
int AM_InsertEntry(int fileDesc, void *value1, void *value2) {
	/* Create a struct record, with the desired (key, value) tuple */
	Record* new_record = create_record(fileDesc, value1, value2);
	BF_Block *first_block;
	int offset;
	/* Get first block data */
	BF_Block_Init(&first_block);
	BF_GetBlock(fileDesc, 0, first_block);
	char *first_block_info = BF_Block_GetData(first_block);
	/* Get root number */
	offset = sizeof(char) + sizeof(int);
	int root_block_int;
	memcpy(&root_block_int, first_block_info + offset, sizeof(int));
	/* Check if there is no root yet. In that case, create an empty one */
	/* Get the size and the type of the key from the 1st block */
	char key_type; int key_size;
	offset = sizeof(char) + 4 * sizeof(int);
	memcpy(&key_type, first_block_info + offset, sizeof(char));
	offset = 2 * sizeof(char) + 4 * sizeof(int);
	memcpy(&key_size, first_block_info +  offset, sizeof(int));
	BF_Block_SetDirty(first_block);
	BF_UnpinBlock(first_block);
	if (root_block_int == -1) {
		root_block_int = create_empty_root(fileDesc, value1, key_size);
	}
	/** Create a stack in which we will hold the path from the root to the data
	   block. In order to implement the possible recursive split of the blocks,
		 we will pop from that stack to access higher levels of the tree */
	 Stack* path;
   path = find_data_block(fileDesc, root_block_int, value1, key_type, key_size);
	 char* append = malloc(new_record->size);
	 memcpy(append, new_record, new_record->size);
	 /* Pop the first element from the queue. It will be a data block */
	 int target_block_index = Pop(path);
	 /* If there is enough room in that data block */
	 if (record_fits_data(fileDesc,target_block_index) == true) {
			/* Just insert the new record */
			data_sorted_insert(target_block_index, fileDesc, new_record, key_type);
			/*all records by 1*/
			int all_records;
			offset = sizeof(char);
			/* re-gain access to the 1st block */
			BF_GetBlock(fileDesc, 0, first_block);
			first_block_info = BF_Block_GetData(first_block);

			memcpy(&all_records,first_block_info + offset, sizeof(int));
			all_records++;
			memcpy(first_block_info + offset, &all_records,sizeof(int));
			/* Set dirty and unpin */
			BF_Block_SetDirty(first_block);
			BF_UnpinBlock(first_block);
			BF_Block_Destroy(&first_block);
			return AME_OK;
	 	}
	 /** If there is no more room, split the block into 2, so the both can hold
	 		 more records */
	 else {
		 append = split_data_block(fileDesc, target_block_index, new_record, key_type, key_size);
	 }
	/** while there are still index blocks in the stack (aka we have not reached
		  the root) */
	while (! Empty(path)) {
		/* Pop the upeer level */
		target_block_index = Pop(path);
		/* if there is room for the extra key in that index block */
		if (key_fits_index(fileDesc, target_block_index)) {
			/* Insert it there */
			index_sorted_insert(target_block_index, fileDesc, append, key_type, key_size);
			/* and break the loop */
			break;
		}
		/* If there is not enough space for the key */
		else {
			/** Split the block into 2, and keep the tuple (pointer, key, pointer)
			    to append to the higher level */
			append = split_index_block(fileDesc, target_block_index, append, key_type, key_size);

		}
		/*If we have reached the root */
		if (target_block_index == root_block_int) {
			/* re-gain access to the 1st block */
			BF_GetBlock(fileDesc, 0, first_block);
			first_block_info = BF_Block_GetData(first_block);
			/* create a new root, and insert the tuple from the previous level */
			int new_root_block_int = create_root(fileDesc, append, key_size);
			/* Save the new root pointer in the first block */
			offset = sizeof(char) + sizeof(int);
			memcpy(first_block_info + offset, &new_root_block_int, sizeof(int));

			/*Break the loop */
			break;
		}
	}
	/*all records in file raised by 1*/
	int all_records;
	offset = sizeof(char);
	memcpy(&all_records,first_block_info + offset, sizeof(int));
	all_records++;
	memcpy(first_block_info + offset, &all_records,sizeof(int));
	/* Set dirty and unpin */
	BF_Block_SetDirty(first_block);
	BF_UnpinBlock(first_block);
	BF_Block_Destroy(&first_block);
	return AME_OK;
}

/* Open a new scan for the tree */
int AM_OpenIndexScan(int fileDesc, int op, void *value) {
  /* Check if fileDesc is open. If not return corresponding error */
  int file_index;
  CALL_OR_DIE(find_index(fileDesc, &file_index));
  /* Find the index of the array that we want to insert the scan */
  int scan_index;
  CALL_OR_DIE(find_empty_scan(&scan_index));
  /* Check if operator is valid */
  if (op < 1 || op > 6) {
    AM_errno = AME_OP_ERR;
    return AME_OP_ERR;
  }
  /* Everything ok, so proceed to open the index scan */
  /* Initialize the scan info */
  Scans->open[scan_index].file_index = fileDesc;
  strcpy(Scans->open[scan_index].file_name, Files->open[file_index].file_name);
  Scans->open[scan_index].op = op;
  Scans->open[scan_index].last_entry.no_block = -1;
  Scans->open[scan_index].last_entry.no_entry = -1;
  Scans->open[scan_index].error = false;
  Scans->open[scan_index].value = malloc(Files->open[file_index].attr_length_1);
  /* Check if memory exhausted */
  if (Scans->open[scan_index].value == NULL) {
    AM_errno = AME_MEM_ERR;
    return AME_MEM_ERR;
  }
  /* If not store value */
  memcpy(Scans->open[scan_index].value, value, Files->open[file_index].attr_length_1);
  /* Increase number of open Scans */
  Scans->total++;
  /* Return file descriptor */
  return fileDesc;
}

/*  Returns the keu of the next entry that satisfies the condition given on an
    allreadey open scan */
void *AM_FindNextEntry(int scanDesc) {
  /* Check if scan is open */
  boolean scan_is_open = false;
  int scan_index;
  for (int i = 0; i < MAX_SCAN_FILES; ++i) {
    if (Scans->open[i].file_index == scanDesc) {
      scan_is_open = true;
      scan_index = i;
      break;
    }
  }
  /* Allocate memory for value */
  int index = Scans->open[scan_index].file_index;
  void* value = malloc(Files->open[index].attr_length_2);
  if (value == NULL) {
    AM_errno = AME_MEM_ERR;
    return NULL;
  }

  /* Check if scan is open */
  if (scan_is_open) {
    /* Check if value is NULL */
    if (Scans->open[scan_index].value != NULL) {
      /* Check if it is the first entry */
      if (Scans->open[scan_index].last_entry.no_block == -1 && \
        Scans->open[scan_index].last_entry.no_entry == -1) {
        /* Initialize the first entry */
        if (init_entry(scan_index) == AME_EOF) {
          AM_errno = AME_EOF;
          return NULL;
        }
      /* Check if an error occured last time we called AM_FindNextEntry */
      } else if (Scans->open[scan_index].error == true) {
        AM_errno = AME_EOF;
        return NULL;
      /* Everything ok, so proceed to the next entry */
      } else {
        Scans->open[scan_index].last_entry.no_entry++;
      }
      /* Get value */
      if (get_entry_value(scan_index, value) == AME_EOF) {
        AM_errno = AME_EOF;
        return NULL;
      }
    } else {
      AM_errno = AME_SCAN_ERR;
      return NULL;
    }
  } else {
    AM_errno = AME_SCAN_ERR;
    return NULL;
  }

  return value;
}

/* Close an already opened scan */
int AM_CloseIndexScan(int scanDesc) {
  /* Check if scan is open */
  int scan_index;
	CALL_OR_DIE(find_scan(scanDesc, &scan_index));
  /* Free memory and replace its data with -1 */
  strcpy(Scans->open[scan_index].file_name, "");
  Scans->open[scan_index].file_index = EMPTY;
  Scans->open[scan_index].op = -1;
  Scans->open[scan_index].last_entry.no_block = -1;
  Scans->open[scan_index].last_entry.no_entry = -1;
  Scans->open[scan_index].error = false;
  free(Scans->open[scan_index].value);
  /* Reduce the number of open scans */
  Scans->total--;
  return AME_OK;
}

/* Catch a possible error in the previous functions, and print a message */
void AM_PrintError(char *errString) {
  printf("%s\n", errString);
  switch (AM_errno) {
    case AME_BF_ERR:
      printf("BF error occured!\n");
      break;
		case AME_EOF:
			printf("There are no other entries which meet the requirements of the open scan.\n\n");
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
		case AME_SCAN_ERR:
			printf("AN error occured while scanning the file.\n\n");
			break;
		default:
			printf("Unknown error occured!\n");
			break;
  }
}

/* Close the AM level */
void AM_Close() {
  BF_Close();
	open_files_destroy();
	open_scans_destroy();
}
