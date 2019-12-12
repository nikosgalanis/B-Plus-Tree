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

/*append two strings*/
char* concat(const char *s1, const char *s2) {
    char* result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
