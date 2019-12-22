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
		AM_PrintError("Errror");
	}
  int eNentry;
  if ((eNentry = AM_OpenIndex("data.db")) < 0) {
    AM_PrintError("Errror");
  }
  int v1 = 10; char* k1 = "lalala";
  for(int i = 0; i < 10000; ++i) {
    int v1 = rand()%100; char* k1 = "lallala";
    printf("--------Inserting %d ---------------\n",i );
    // if (i == 1863) {
    //     // print_data_block(eNentry, 6);
    // }
    // if (i > 100) {
    //   print_data_block(eNentry, 5);
    // }
    AM_InsertEntry(eNentry, (void*)&v1, (void*)k1);
  }
  //print_tree(eNentry, 'D');
  //print_tree(eNentry, 'I');
}