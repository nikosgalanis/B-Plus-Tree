#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/util.h"

int find_empty_index() {
  for (int i = 0; i < MAX_OPEN_FILES; ++i) {
    if (Files->open[i].file_index == EMPTY)
      return i;
  }
  return AME_MAX_FILES_ERR;
}

int find_empty_scan() {
  for (int i = 0; i < MAX_SCAN_FILES; ++i) {
    if (Scans->open[i].file_index == EMPTY)
      return i;
  }
  return AME_MAX_SCANS_ERR;
}

int find_index(int fileDesc) {
  for (int i = 0; i < MAX_OPEN_FILES; ++i) {
    if (Files->open[i].file_index == fileDesc)
      return i;
  }
  return EMPTY;
}

int find_scan(int fileDesc) {
  for (int i = 0; i < MAX_SCAN_FILES; ++i) {
    if (Scans->open[i].file_index == fileDesc)
      return i;
  }
  return EMPTY;
}

char* concat(const char *s1, const char *s2) {
  char* result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

int type_length_match(char type, int length) {
  switch(type) {
    case INTEGER:
      if (length != 4) {
        return AME_INTEGER_ERR;
      }
      break;
    case FLOAT:
      if (length != 4) {
        return AME_FLOAT_ERR;
      }
      break;
    case STRING:
      if (length < 1 || length > 255 ) {
			  return AME_STRING_ERR;
			}
			break;
    default:
      return AME_TYPE_ERR;
  }
  return AME_OK;
}

int compare(void *op1, int op, void *op2, char type){
	switch (type) {
		case INTEGER:
			switch (op) {
				case EQUAL:
					return (*((int *)op1) == *((int *)op2));
				case NOT_EQUAL:
					return (*((int *)op1) != *((int *)op2));
				case LESS_THAN:
					return (*((int *)op1) <  *((int *)op2));
				case GREATER_THAN:
					return (*((int *)op1) >  *((int *)op2));
				case LESS_THAN_OR_EQUAL:
					return (*((int *)op1) <= *((int *)op2));
				case GREATER_THAN_OR_EQUAL:
					return (*((int *)op1) >= *((int *)op2));
				default:
					return -1;
			}
			break;
		case FLOAT:
			switch (op) {
				case EQUAL:
					return (*((float *)op1) == *((float *)op2));
				case NOT_EQUAL:
					return (*((float *)op1) != *((float *)op2));
				case LESS_THAN:
					return (*((float *)op1) <  *((float *)op2));
				case GREATER_THAN:
					return (*((float *)op1) >  *((float *)op2));
				case LESS_THAN_OR_EQUAL:
					return (*((float *)op1) <= *((float *)op2));
				case GREATER_THAN_OR_EQUAL:
					return (*((float *)op1) >= *((float *)op2));
				default:
					return -1;
			}
			break;
		case STRING:;
			int result = strcmp(((char *)op1), ((char *)op2));
			switch (op) {
				case EQUAL:
					return (result == 0);
				case NOT_EQUAL:
					return (result != 0);
				case LESS_THAN:
					return (result <  0);
				case GREATER_THAN:
					return (result >  0);
				case LESS_THAN_OR_EQUAL:
					return (result <= 0);
				case GREATER_THAN_OR_EQUAL:
					return (result >= 0);
				default:
					return -1;
			}
			break;
		default:
			return -1;
	}
}


boolean print_data_block(int fileDesc, int target_block) {
	BF_Block *first_block, *block;
	int offset, max_records, block_records, attrLength1, attrLength2, size;
	char type1,type2;
	/*Get the data of the blocks*/
	BF_Block_Init(&first_block);
	BF_Block_Init(&block);
	BF_GetBlock(fileDesc, 0, first_block);
	BF_GetBlock(fileDesc, target_block, block);
	char* first_block_info = BF_Block_GetData(first_block);
	char* block_data = BF_Block_GetData(block);
	/*check if data block*/
	if (block_data[0] != 'D'){
		printf("That's not a data block\n");
		BF_UnpinBlock(block);
		BF_Block_Destroy(&block);
		BF_UnpinBlock(first_block);
		BF_Block_Destroy(&first_block);
		return false;
	}
	/*check if the records are as many as we want*/
	memcpy(&max_records, first_block_info + sizeof(char) + 3 * sizeof(int), sizeof(int));
	memcpy(&block_records, block_data + sizeof(char), sizeof(int));
	if (max_records < block_records) {
		printf("Error in records,max records are %d, this block has%d\n",max_records,block_records);
		BF_UnpinBlock(block);
		BF_Block_Destroy(&block);
		BF_UnpinBlock(first_block);
		BF_Block_Destroy(&first_block);
		return false;
	}
	if (block_records == 0) {
		printf("Empty data block\n");
		BF_UnpinBlock(block);
		BF_Block_Destroy(&block);
		BF_UnpinBlock(first_block);
		BF_Block_Destroy(&first_block);
		return true;
	}
	/*get type and length of a and b*/
	offset = sizeof(char) + 4 * sizeof(int);
  	memcpy(&type1, first_block_info + offset, sizeof(char));
	offset += sizeof(char);
	memcpy(&attrLength1, first_block_info + offset, sizeof(int));
  	offset += sizeof(int);
  	memcpy(&type2, first_block_info + offset, sizeof(char));
	offset += sizeof(char);
	memcpy(&attrLength2, first_block_info + offset, sizeof(int));
	/*Now let's print the records*/
	size = sizeof(Record) + attrLength1 + attrLength2;
	int i;
	Record* cur_record = malloc(size);
	offset = sizeof(char) + 3 * sizeof(int);
	int prev,next;
	memcpy(&prev, block_data + sizeof(char) + sizeof(int), sizeof(int));
	memcpy(&next, block_data + sizeof(char) + 2 * sizeof(int), sizeof(int));
	printf("block %d with %d records, prev %d, next %d : ",target_block, block_records,prev,next);
	int thekey,thevalue;
	for (i = 0; i < block_records; i++) {
		memcpy(cur_record, block_data + offset, size);
		printf("Record %d [ ",i);
		switch (type1) {
			case 'i':
				printf("%d ",*((int*) cur_record->key));
				break;
			case 'f':
				printf("%f ",*((float *)cur_record->key));
				break;
			case 'c':
				printf("%s ",(char*) cur_record->key);
				break;
			default:
				printf("error type\n");
				BF_UnpinBlock(block);
				BF_Block_Destroy(&block);
				BF_UnpinBlock(first_block);
				BF_Block_Destroy(&first_block);
				return false;
				break;
		}
		printf("| ");
		switch (type2) {
			case 'i':
				printf("%d ",*( (int *) cur_record->value));
				break;
			case 'f':
				printf("%f ",*( (float *) cur_record->value));
				break;
			case 'c':
				printf("%s ",(char*) cur_record->value);
				break;
			default:
				printf("error type\n");
				BF_UnpinBlock(block);
				BF_Block_Destroy(&block);
				BF_UnpinBlock(first_block);
				BF_Block_Destroy(&first_block);
				return false;
				break;
		}
		printf("] ");
		/*next record*/
		offset += size;
	}
	printf("\n");

	/*Unpin destroy*/
	BF_UnpinBlock(block);
	BF_Block_Destroy(&block);
	BF_UnpinBlock(first_block);
	BF_Block_Destroy(&first_block);
	return true;
}

boolean print_index_block(int fileDesc, int target_block){
	BF_Block *first_block, *block;
	int offset, max_keys, block_keys, attrLength1;
	char type1;
	/*Get the data of the blocks*/
	BF_Block_Init(&first_block);
	BF_Block_Init(&block);
	BF_GetBlock(fileDesc, 0, first_block);
	BF_GetBlock(fileDesc, target_block, block);
	char* first_block_info = BF_Block_GetData(first_block);
	char* block_data = BF_Block_GetData(block);
	/*check if data block*/
	if (block_data[0] != 'I'){
		printf("that's a data block\n");
		BF_UnpinBlock(block);
		BF_Block_Destroy(&block);
		BF_UnpinBlock(first_block);
		BF_Block_Destroy(&first_block);
		return false;
	}
	/*check if the keys are as many as we want*/
	memcpy(&max_keys, first_block_info + sizeof(char) + 2 * sizeof(int), sizeof(int));
	memcpy(&block_keys, block_data + sizeof(char), sizeof(int));
	if (max_keys < block_keys) {
		printf("Error in keys\n");
		BF_UnpinBlock(block);
		BF_Block_Destroy(&block);
		BF_UnpinBlock(first_block);
		BF_Block_Destroy(&first_block);
		return false;
	}
	if (block_keys == 0) {
		printf("Empty data block\n");
		BF_UnpinBlock(block);
		BF_Block_Destroy(&block);
		BF_UnpinBlock(first_block);
		BF_Block_Destroy(&first_block);
		return true;
	}
	/*get type and lenght of key*/
	offset = sizeof(char) + 4 * sizeof(int);
  	memcpy(&type1, first_block_info + offset, sizeof(char));
	offset += sizeof(char);
	memcpy(&attrLength1, first_block_info + offset, sizeof(int));
	/*Now let's start printing*/
	offset = sizeof(char) + sizeof(int);
	printf("block %d with %d keys: ",target_block, block_keys);
	int block_pointer;
	memcpy(&block_pointer, block_data + offset, sizeof(int));
	printf("|%d|", block_pointer);
	offset += sizeof(int);
	int i;
	void* key;
	int thekey ;
	for (i = 0; i < block_keys; i++) {
		switch (type1) {
			case 'i':
				key = malloc(attrLength1);
				memcpy(key ,block_data + offset, attrLength1);
				printf("| %d |",*((int *)key));
				break;
			case 'f':
				key = malloc(attrLength1);
				memcpy(key ,block_data + offset, attrLength1);
				printf("| %f |",*((float*)key));
				break;
			case 'c':
				key = (char*) malloc(attrLength1);
				memcpy(key ,block_data + offset, attrLength1);
				printf("| %s |",(char*)key);
				break;
			default:
				printf("error type\n");
				BF_UnpinBlock(block);
				BF_Block_Destroy(&block);
				BF_UnpinBlock(first_block);
				BF_Block_Destroy(&first_block);
				return false;
				break;
		}
		offset += attrLength1;
		memcpy(&block_pointer, block_data + offset, sizeof(int));
		printf("|%d|", block_pointer);
		offset += sizeof(int);
	}
	printf("\n");
	/*Unpin destroy*/
	BF_UnpinBlock(block);
	BF_Block_Destroy(&block);
	BF_UnpinBlock(first_block);
	BF_Block_Destroy(&first_block);
	return true;
}

boolean print_tree(int fileDesc, char print_type) {
	/*print type is 'D' if we want to print data blocks, or 'I' for index*/
	BF_Block* first_block, *cur_block;
	int offset;
	BF_Block_Init(&first_block);
	BF_Block_Init(&cur_block);
	BF_GetBlock(fileDesc, 0, first_block);
	char* first_block_info = BF_Block_GetData(first_block);
	int cur_block_index;
	offset = sizeof(char) + sizeof(int);
	memcpy(&cur_block_index,first_block_info + offset, sizeof(int));
	if (cur_block_index == -1) {
		printf("Empty root\n");
		BF_UnpinBlock(cur_block);
		BF_Block_Destroy(&cur_block);
		BF_UnpinBlock(first_block);
		BF_Block_Destroy(&first_block);
		return false;
	}
	BF_GetBlock(fileDesc, cur_block_index, cur_block);
	char* cur_block_info = BF_Block_GetData(cur_block);
	/*if we want to print index blocks*/
	if (print_type == 'I'){
		printf("---------------------------------------------------------------------\n");
		printf("Printing the index tree PRE-ORDER\n");
		/*need keylenght*/
	 	int key_size;
		offset = 2 * sizeof(char) + 4 * sizeof(int);
		memcpy(&key_size, first_block_info + offset, sizeof(int));
		/*we PRE-ORDER print them using the Stack*/
		if (cur_block_index == -1){
			printf("empty root\n");
			BF_UnpinBlock(cur_block);
			BF_Block_Destroy(&cur_block);
			BF_UnpinBlock(first_block);
			BF_Block_Destroy(&first_block);
			return false;
		}
		Stack* s = InitializeStack();
		/*push root*/
		Push(s,cur_block_index);
		int left,right;
		boolean check_index;
		while (!Empty(s)) {
			cur_block_index = Pop(s);
			check_index = print_index_block(fileDesc,cur_block_index);
			if (check_index == false) {
				continue;
			}
			/*right child is pushed first during pop, left is processed first*/
			BF_UnpinBlock(cur_block);
			BF_GetBlock(fileDesc,cur_block_index,cur_block);
			cur_block_info = BF_Block_GetData(cur_block);
			memcpy(&left,cur_block_info + sizeof(char) + sizeof(int),sizeof(int));
			memcpy(&right,cur_block_info + sizeof(char)+ 2 * sizeof(int) + key_size, sizeof(int));
			if (right != -1 ) {
				Push(s,right);
			}
			if (left != -1 ) {
				Push(s,left);
			}
		}
		printf("---------------------------------------------------------------------\n");
	}
	/*if we want to print data blocks*/
	else if (print_type == 'D') {
		printf("---------------------------------------------------------------------\n");
		printf("Printing the data blocks!\n");
		/*while we are not in data blocks*/
		while (cur_block_info[0] != 'D') {
			memcpy(&cur_block_index, cur_block_info + sizeof(char) + sizeof(int), sizeof(int));
			BF_UnpinBlock(cur_block);
			BF_GetBlock(fileDesc, cur_block_index, cur_block);
			cur_block_info = BF_Block_GetData(cur_block);
		}
		/*now we are at the first data block*/
		int prev,next;
		offset = sizeof(char) + sizeof(int);
		memcpy(&prev,cur_block_info + offset, sizeof(int));
		if (prev!= -1) {
			printf("There is a previous data block, problem\n");
			BF_UnpinBlock(cur_block);
			BF_Block_Destroy(&cur_block);
			BF_UnpinBlock(first_block);
			BF_Block_Destroy(&first_block);
			return false;
		}
		offset += sizeof(int);
		memcpy(&next, cur_block_info + offset, sizeof(int));
		print_data_block(fileDesc,cur_block_index);
		while (next != -1) {
			/*print data of next*/
			print_data_block(fileDesc,next);
			/*now we want a new next*/
			BF_UnpinBlock(cur_block);
			cur_block_index = next;
			BF_GetBlock(fileDesc,cur_block_index,cur_block);
			cur_block_info = BF_Block_GetData(cur_block);
			memcpy(&next, cur_block_info + offset, sizeof(int));
		}
		printf("---------------------------------------------------------------------\n");
	}
	else {
		printf("print_type error\n");
		BF_UnpinBlock(cur_block);
		BF_Block_Destroy(&cur_block);
		BF_UnpinBlock(first_block);
		BF_Block_Destroy(&first_block);
		return false;
	}
	/*Unpin and Destroy*/
	BF_UnpinBlock(cur_block);
	BF_Block_Destroy(&cur_block);
	BF_UnpinBlock(first_block);
	BF_Block_Destroy(&first_block);
	return true;
}
