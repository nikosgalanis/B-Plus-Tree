#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/util.h"


int find_empty_index() {
  for (int i = 0; i < MAX_OPEN_FILES; ++i) {
    if (Files->open[i].file_index == EMPTY_FILE)
      return i;
  }
  return EMPTY_FILE;
}

int find_index(int fileDesc) {
  for (int i = 0; i < MAX_OPEN_FILES; ++i) {
    if (Files->open[i].file_index == fileDesc)
      return i;
  }
  return EMPTY_FILE;
}

/* Append two strings */
char* concat(const char *s1, const char *s2) {
    char* result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/* Generic compare function */
int compare(void *op1, int op, void *op2, char type){
	switch(type){
		case INTEGER:
			switch(op){
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
			switch(op){
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
			switch(op){
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
	if (max_records <= block_records) {
		printf("Error in records\n");
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
	Record* cur_record;
	offset = sizeof(char) + 3 * sizeof(int);
	printf("block %d with %d records: ",target_block, block_records);
	for (i = 0, i < block_records, i++) {
		memcpy(&cur_record, block_data + offset, size);
		printf("Record %d [ ",i);
		switch (type1) {
			case 'i':
				printf("%d ",cur_record->*key);
				break;
			case 'f':
				printf("%f ",cur_record->*key);
				break;
			case 'c':
				printf("%s ",cur_record->*key);
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
				printf("%d ",cur_record->*value);
				break;
			case 'f':
				printf("%f ",cur_record->*value);
				break;
			case 'c':
				printf("%s ",cur_record->*value);
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
		printf("That's not an index block\n");
		BF_UnpinBlock(block);
		BF_Block_Destroy(&block);
		BF_UnpinBlock(first_block);
		BF_Block_Destroy(&first_block);
		return false;
	}
	/*check if the keys are as many as we want*/
	memcpy(&max_keys, first_block_info + sizeof(char) + 2 * sizeof(int), sizeof(int));
	memcpy(&block_keys, block_data + sizeof(char), sizeof(int));
	if (max_records <= block_records) {
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
	printf("|%d | ", block_pointer);
	offset += sizeof(int);
	int i;
	void* key;
	for (i = 0; i < block_keys, i++) {
		memcpy(key ,block_data + offset, attrLength1);
		switch (type1) {
			case 'i':			
				printf("%d | ",(int *) key);
				break;
			case 'f':
				printf("%f | ",(float *) key);
				break;
			case 'c':
				printf("%s | ",(char* *) key);
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
	}
	/*Unpin destroy*/
	BF_UnpinBlock(block);
	BF_Block_Destroy(&block);
	BF_UnpinBlock(first_block);
	BF_Block_Destroy(&first_block);
	return true
}
boolean print_tree(int fileDesc) {

	return true;
}