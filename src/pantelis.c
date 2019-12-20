#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/util.h"

extern int AM_errno;

int AM_OpenIndexScan(int fileDesc, int op, void *value) {
  /* Check if fileDesc is open */
  boolean file_is_open = false;
  for (int i = 0; i < MAX_OPEN_FILES; ++i) {
    if (Files->open[i].file_index == fileDesc) {
      file_is_open = true;
      break;
    }
  }
  /* If fileDesc has not been found in open files return corresponding ErrCode */
  if (file_is_open == false) {
    return -1; // TODO change it
  }
  /* Find the index of the array that we want to insert the scan */
  return AME_OK;
}


void *AM_FindNextEntry(int scanDesc) {

}


int AM_CloseIndexScan(int scanDesc) {
  return AME_OK;
}
