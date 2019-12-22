#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/util.h"

int AM_OpenIndexScan(int fileDesc, int op, void *value) {
  /* Check if fileDesc is open. If not return corresponding error */
  int file_index;
  CALL_OR_DIE(find_index(fileDesc, &file_index));
  /* Find the index of the array that we want to insert the scan */
  int scan_index;
  CALL_OR_DIE(find_empty_scan(&scan_index))
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
  Scans->open[scan_index].value = malloc(Files->open[file_index].attr_length_1);
  /* Check if memory exhausted */
  if (Scans->open[scan_index].value == NULL) {
    AM_errno = AME_MEM_ERR;
    return AME_MEM_ERR;
  }
  /* If not store value */
  memcpy(&Scans->open[scan_index].value, value, Files->open[file_index].attr_length_1);
  /* Increase number of open Scans */
  Scans->total++;
  return AME_OK;
}


void *AM_FindNextEntry(int scanDesc) {

}


int AM_CloseIndexScan(int scanDesc) {
  return AME_OK;
}
