#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/util.h"
#include "../include/scan_funcs.h"

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


void *AM_FindNextEntry(int scanDesc) {
  printf("INDEX: %d\n",scanDesc);
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
          printf("Whattt\n");
          AM_errno = AME_EOF;
          return NULL;
        }
        printf("GUYSSS\n");
      /* Check if an error occured last time we called AM_FindNextEntry */
      } else if (Scans->open[scan_index].error == true) {
        printf("ME TPT\n");
        AM_errno = AME_EOF;
        return NULL;
      /* Everything ok, so proceed to the next entry */
      } else {
        Scans->open[scan_index].last_entry.no_entry++;
        printf("GOOOOD\n");
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
  // return NextEntry
  return value;
}


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
