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
  for(int i = 0; i < 1500; ++i) {
    int v1 = rand()%10000, k1 = i;
    printf("--------Inserting %d ---------------\n",v1 );
    AM_InsertEntry(eNentry, (void*)&v1, (void*)&k1);
  }
}
