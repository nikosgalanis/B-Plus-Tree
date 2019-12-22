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
		AM_PrintError("Errror");
	}
  int eNentry;
  if ((eNentry = AM_OpenIndex("data.db")) < 0) {
    AM_PrintError("Errror");
  }
  for(int i = 0; i < 100; ++i) {
    int v1 = i, k1 = i;
    printf("--------Inserting %d ---------------\n",i );
    AM_InsertEntry(eNentry, (void*)&v1, (void*)&k1);
  }
  //print_tree(eNentry, 'D');
  //print_tree(eNentry, 'I');
}
