#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defn.h"
#include "AM.h"
#include "util.h"

int main(void) {
  AM_Init();
  if (AM_CreateIndex("data.db", INTEGER, sizeof(int), INTEGER,
			sizeof(int)) != AME_OK) {
		// sprintf("Errror", "Error in AM_CreateIndex called on %s \n", "data.db");
		AM_PrintError("Errror");
	}
  int eNentry;
  if ((eNentry = AM_OpenIndex("data.db")) < 0) {
    AM_PrintError("Errror");
  }
  int k1 = 1, k2 = 2, k3 = 3, k4 = 4;
  int v1 = 1, v2 = 2, v3 = 3, v4 = 4;
  AM_InsertEntry(eNentry, (void*)&v1, (void*)&k1);
  // print_tree(eNentry, 'I');
  // print_tree(eNentry, 'D');
  printf("----------------------\n");
  AM_InsertEntry(eNentry, (void*)&v2, (void*)&k2);

}
