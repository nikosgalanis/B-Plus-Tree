/* A simple main function that demonstrates the usage of our B+ tree */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "defn.h"
#include "AM.h"
#include "util.h"
#include "print_funcs.h"
/* Number of records that will be inserted in the 1st file. */
#define N_OF_NAMES 1200
char* errStr = "Error!";
/* Some names that will be generated */
char NamePrefix[][5] = {
	"Bel", "Nar", "Xan", "Nar", "Eva", "God", "Gal", "Pap", "Sto", "Pan"
};

char NameSuffix[][5] = {
  "usa", "ixs", "oxa", "ith",
	"ath", "umu", "ato", "tor","ara", "ere", "nam" , "luc", "are", "axa",
	"imu", "ais", "itu", "oex", "ore","yer"
};

const char NameStems[][10] = {
	"adur", "aesa", "anim", "apoll", "imac",
	"educ", "equs", "extr", "guis", "hann",
	"equi", "amor", "huma", "iace", "ille",
	"inet", "iuva", "obea", "ocul", "orbis"
};

/* Name generator */
char* NameGen() {
  char* name = malloc(11 * sizeof(int));
	//add the prefix...
	strcat(name, NamePrefix[(rand() % 10)]);
	//add the stem...
	strcat(name, NameStems[(rand() % 20)]);
	//add the suffix...
	strcat(name, NameSuffix[(rand() % 20)]);
	//Make the first letter capital...
	return name;
}

int main(void) {
  /* Initialize the AM index level */
  printf("Initializing our AM level...\n\n");
  AM_Init();
  /** Create a B+ tree file, in which we are going to store names as keys, with
      at most 21 chars, and integers(money of the person) as values */
  printf("Creating the file...\n\n");

  if (AM_CreateIndex("names_file.db", INTEGER, sizeof(int), INTEGER, sizeof(int)) != AME_OK) {
		AM_PrintError("Errror");
	}
  /* Open the file, and store its descriptor */
  printf("Opening the file...\n\n");

  int eNentry;
  if ((eNentry = AM_OpenIndex("names_file.db")) < 0) {
    AM_PrintError("Errror");
  }
  /* Now we are going to insert vcalues in our 1st B+ tree file */
  // char* name = malloc(11 * sizeof(int));
  int key,value;
  printf("Time to insert some entries \n\n");
  for(int i = 0; i < 10; ++i) {
    // name = NameGen();
		if (i == 5000) {

			printf("i %d\n",i );
		}
		key = rand() % 10000000 ;
    value = rand() % N_OF_NAMES;
    /* Catch a possible error */
		// if(key == 93151555){
		//
		// 	print_tree(eNentry, 'I'); printf("\n");
		// 	print_tree(eNentry, 'D');	printf("\n");
		// }

    if (AM_InsertEntry(eNentry, (void *) &key, (void*)&value) != AME_OK) {
      sprintf(errStr, "Error in AM_InsertEntry called on %s \n", "names_file.db");
      AM_PrintError(errStr);
      break;
    }

  }
  // /* Create a scan, to find the money of the entry before "Galanis" */
  // int scan1;
  // printf("Initialize a search in our tree\n\n");
  // char* search_name = "Galanis";
  // if ((scan1 = AM_OpenIndexScan(eNentry, LESS_THAN, (void *) search_name)) < 0) {
	// 	sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", "names_file.db");
	// 	AM_PrintError(errStr);
	// }
  // /* Search the tree */
  // int* ivalue = NULL;
  // if ((ivalue = (int*)AM_FindNextEntry(scan1)) != NULL)
  //   printf("Money of the first entry before %s is %d \n", search_name, *ivalue);
  // else {
  //   sprintf(errStr, "Error in AM_FindNextEntry called on scan %d \n\n", scan1);
  //   AM_PrintError(errStr);
  // }
  // /* Close the scan */
  // printf("Closing the scan\n\n");
  // if (AM_CloseIndexScan(scan1) != AME_OK) {
  //   sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
  //       scan1);
  //   AM_PrintError(errStr);
  // }

  /* Close the B+ tree file */
  printf("Closing the file\n\n");
  if (AM_CloseIndex(eNentry) != AME_OK) {
  	sprintf(errStr, "Error in AM_CloseIndex called on %s \n", "names_file.db");
  	AM_PrintError(errStr);
  }

  /* remove the file from the disk */
  printf("Destroying the file\n\n");
  if (AM_DestroyIndex("names_file.db") != AME_OK) {
     sprintf(errStr,"Error in AM_DestroyIndex called on %s \n","names_file");
     AM_PrintError(errStr);
  }

  AM_Close();



}
