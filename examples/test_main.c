#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defn.h"
#include "AM.h"
#include "util.h"

int main(void) {
  AM_Init();
  if (AM_CreateIndex("data.db", INTEGER, sizeof(int), STRING,
			sizeof(int)) != AME_OK) {
		// sprintf("Errror", "Error in AM_CreateIndex called on %s \n", "data.db");
		AM_PrintError("Errror");
	}
  int eNentry;
  if ((eNentry = AM_OpenIndex("data.db")) < 0) {
    AM_PrintError("Errror");
  }
  int v1 = 10; char* k1 = "lalala";
  for(int i = 0; i < 100; ++i) {
    int v1 = i; char* k1 = "lallala";
    printf("--------Inserting %d ---------------\n",v1 );
    // if (i == 1863) {
    //     // print_data_block(eNentry, 6);
    // }
    // if (i > 100) {
    //   print_data_block(eNentry, 5);
    // }
    AM_InsertEntry(eNentry, (void*)&v1, (void*)k1);
  }
   print_tree(eNentry, 'I');
  print_tree(eNentry, 'D');
}
