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
