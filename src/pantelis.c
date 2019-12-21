#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/util.h"

int AM_OpenIndexScan(int fileDesc, int op, void *value) {
  /* Check if fileDesc is open. If not return corresponding error */
  int file_index = find_index(fileDesc);
  if (file_index == EMPTY) {
    return -1; // TODO
  }
  /* Find the index of the array that we want to insert the scan */
  int scan_index = find_empty_scan();
  if (scan_index == EMPTY) {
    /* We reached the max capacity of Open_Scans, so return corresponding error */
    return -1; // TODO
  }
  /* Everything ok, so proceed to open the index scan */
  /* Initialize the scan info */
  Scans->open[scan_index].file_index = fileDesc;
  Scans->open[scan_index].op = op;
  Scans->open[scan_index].value = value;
  Scans->open[scan_index].last_entry.no_block = -1;
  Scans->open[scan_index].last_entry.no_entry = -1;
  /* Increase number of open Scans */
  Scans->total++;
  return AME_OK;
}


void *AM_FindNextEntry(int scanDesc) {

}


int AM_CloseIndexScan(int scanDesc) {
  return AME_OK;
}
