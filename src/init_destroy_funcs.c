#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/util.h"

void open_files_init() {
  /* Allocate memory for Files struct */
  Files = malloc(sizeof(Open_Files));
	assert(Files != NULL);
  /* Allocate memory for open files array */
	Files->open = malloc(MAX_OPEN_FILES * sizeof(File_info));
  assert(Files->open != NULL);
  /* Initialize open files array to NULL */
	for (int i = 0; i < MAX_OPEN_FILES; ++i) {
		Files->open[i].file_index = EMPTY;
	}
  /* In the initialization step, zero files are open */
	Files->total = 0;
}

void open_scans_init() {
  /* Allocate memory for Scans struct */
  Scans = malloc(sizeof(Open_Scans));
  assert(Scans != NULL);
  /* Allocate memory for open scans array */
  Scans->open = malloc(MAX_SCAN_FILES * sizeof(Scan_info));
  assert(Scans->open != NULL);
  /* Initialize open scans array to NULL */
  for (int i = 0; i < MAX_SCAN_FILES; ++i) {
    Scans->open[i].file_index = EMPTY;
  }
  /* In the initialization step, zero scans are open */
  Scans->total = 0;
}

void open_files_destroy() {
  free(Files->open);
  free(Files);
}

void open_scans_destroy() {
  // TODO free value
  free(Scans->open);
	free(Scans);
}
