#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/scan_funcs.h"

int init_entry(int scan_index) {

	switch (Scans->open[scan_index].op) {
    /* Get the lowest key which is always in second block */
    case NOT_EQUAL:
    case LESS_THAN_OR_EQUAL:
    case LESS_THAN:
      Scans->open[scan_index].no_block = 2;
      Scans->open[scan_index].no_entry = 0;
      break;
  	/* Search B+ for the first entry */
    case EQUAL:
		case GREATER_THAN_OR_EQUAL:
		case GREATER_THAN:
      break;
	}
  return AME_OK;
}
