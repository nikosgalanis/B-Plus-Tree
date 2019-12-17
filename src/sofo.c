#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/util.h"
#include "../include/insert_functions.h"

#define AME_ERROR -1 //TODO: change this
int AM_errno;

int AM_CreateIndex(const char* fileName, char attrType1, int attrLength1,
									 char attrType2,int attrLength2) {
	int offset;
	/* Create the file */
  CALL_BF(BF_CreateFile(fileName));
  /* Open it, to edit the first block */
  int fileDesc;
  CALL_BF(BF_OpenFile(fileName, &fileDesc));
  /* Get access to the first block */
  BF_Block* first_block;
  /* Initialize the first block */
  BF_Block_Init(&first_block);
  CALL_BF(BF_AllocateBlock(fileDesc, first_block));
  /* Get the total number of available blocks(1) */
  int blocks_num;
  BF_GetBlockCounter(fileDesc, &blocks_num);
  /* Get access to the first block */
  BF_GetBlock(fileDesc, blocks_num - 1, first_block);
	/* Find out if we have a wrong input */
	int ok_a = ((attrType1 == 'c' && attrLength1 >= 1 && attrLength1 <= 255) ||
							(attrType1 == 'i' && attrLength1 == 4) ||
							(attrType1 == 'f' && attrLength1 == 4));

	int ok_b = ((attrType2 == 'c' && attrLength2 >= 1 && attrLength2 <= 255) ||
							(attrType2 == 'i' && attrLength2 == 4) ||
							(attrType2 == 'f' && attrLength2 == 4));
	/*if there are no errors*/
	if ((ok_a == 1) && (ok_b == 1)) {
	  /* Set its data to be the character 'B', so we can recognize the B+ files */
	  char* first_block_info = BF_Block_GetData(first_block);
	  char idf = 'B';
	  memcpy(first_block_info, &idf, sizeof(char));

	  /* Also, store the total number of records in the file */
	  int init_no_records = 0;
		/* Initialize our offset */
		offset = sizeof(char);
	  memcpy(first_block_info + offset, &init_no_records, sizeof(int));

	  /* Also, store the block that the root is in */
	  int root_block_no = -1;
		offset = sizeof(char) + sizeof(int);
	  memcpy(first_block_info + offset, &root_block_no, sizeof(int));

	  /* Also, max pointers of index and max records of a block */
		/* If we have a string as a key, inrease its length by 1 to store the \0 */
		int attrLength1new;
		if (attrType1 == 'c') {
			attrLength1new = attrLength1 + 1; // \O
		}
		else {
			attrLength1new = attrLength1;
		}
    /* the following math occur from the metadata that we must store */
	  int max_index_pointers = ((BF_BLOCK_SIZE - 2 * sizeof(int) - sizeof(char)) / (attrLength1new + sizeof(int))) + 1;
	  int max_records_block = (BF_BLOCK_SIZE - 3 * sizeof(int) - sizeof(char)) / sizeof(Record);
		offset = sizeof(char) + 2 * sizeof(int);
	  memcpy(first_block_info + offset, &max_index_pointers, sizeof(int));
		offset = sizeof(char) + 3 * sizeof(int);
	  memcpy(first_block_info + offset, &max_records_block, sizeof(int));

	  /* Also the attr1 and it's length , and attr2 and it's length */
		/* a */
		offset = sizeof(char) + 4 * sizeof(int);
		memcpy(first_block_info + offset, &attrType1, sizeof(char));
		offset = 2 * sizeof(char) + 4 * sizeof(int);
		memcpy(first_block_info +  offset, &attrLength1new, sizeof(int));
		/* b */
		offset = 2 * sizeof(char) + 5 * sizeof(int);
		memcpy(first_block_info + offset , &attrType2, sizeof(char));
		int attrLength2new;
		if (attrType2 == 'c') {
			attrLength2new = attrLength2 + 1;
		}
		else {
			attrLength2new = attrLength2;
		}
		offset = 3 * sizeof(char) + 5 * sizeof(int);
		memcpy(first_block_info + offset , &attrLength2new, sizeof(char));
	}
	else {
		return AME_ERROR; //TODO: Return error
  }
	//TODO change the n and n+1 for max
  /* We've changed the block data, so its dirty */
  BF_Block_SetDirty(first_block);
  /* Unpin the block from the memory */
  BF_UnpinBlock(first_block);
	/* Destroy the block pointer */
	BF_Block_Destroy(&first_block);
	/*close the file*/
	BF_CloseFile(fileDesc);
  return AME_OK;
}

int AM_InsertEntry(int fileDesc, void *value1, void *value2) {
	BF_Block *first_block;
	int offset;
	/* Get first block data */
	BF_Block_Init(&first_block);
	BF_GetBlock(fileDesc, 0, first_block);
	char *first_block_info = BF_Block_GetData(first_block);
	/* Get root number */
	offset = sizeof(char) + sizeof(int);
	int root_block_int;
	memcpy(&root_block_int, first_block_info + offset, sizeof(int));
	/* Check if -1 */
	if (root_block_int == -1) {
		root_block_int = create_root(fileDesc, value1);
	}
  find_data_block(fileDesc, root_block_int, value1);
	/////////////////////////////////
	//TODO find tou panteli . Epistrefei int me to no_block pou 8elw
	////////////////////////////////

	////////////////////////////////
	//TODO check an exei xwro auto to data block
	////////////////////////////////

	///////////////////////////////
	//TODO
	//An exei xwro apla insert mesa sto data block , me th sorted insert tou galani
	//Else ,
	//Αναδρομικά ή επαναληπτικά , βλέπουμε, μέχρι να βρεθεί χώρος για εγγραφες + ζεύγη
	//1: Σπασιμο του του μπλοκ σε 2.
	//2: Ισομερισμός εγγραφών(αν data block ) ή ζευγων(αν index block) στα 2 μπλοκ
	//3: Εισαγωγή νέου ζευγους (κλειδι , δεικτης) στο κατάλληλο επίπεδο
	//4: Αν σπάσει η ρίζα δημιουργείται νέα ρίζα και αντικατάσταση index και στο μηδενικό μπλοκ
	////////////////////////////////
	return AME_OK;
}
