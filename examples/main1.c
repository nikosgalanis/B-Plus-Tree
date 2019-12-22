/********************************************************************************
 *  main1.c                                                                   *
 *  Το αρχείο αυτό προσομοιώνει τον ρόλο ενός υψηλότερου-επιπέδου αρθρώματος    *
 *  ενός ΣΔΒΔ το οποίο καλεί συναρτήσεις του επιπέδου που ασχολείται με  τον    *
 *  χειρισμό b+-δένδρων (ΒΔ). Η προσοχή του αρχείου επικεντρώνεται              * 
 *  στην δημιουργία ΒΔ δένδρων και στην εισαγωγή και αναζήτηση εγγραφών         *
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defn.h"
#include "AM.h"

char empName[40];
char empAge[40];
char empSal[40];
char empDname[40];

void insertEntries(int eNentry, int eAentry, int eSentry, int eDentry,
		char* ename, int eage, float esal, char* edname, int recid) {
	char errStr[40];

	strcpy(errStr, "Error in AM_InsertEntry");

	if (AM_InsertEntry(eNentry, (void *) ename, &recid) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empName);
		AM_PrintError(errStr);
	}
	if (AM_InsertEntry(eAentry, (void *) &eage, ename) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empAge);
		AM_PrintError(errStr);
	}
	if (AM_InsertEntry(eSentry, (void *) &esal, ename) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empSal);
		AM_PrintError(errStr);
	}
	if (AM_InsertEntry(eDentry, (void *) edname, &esal) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empDname);
		AM_PrintError(errStr);
	}

}

int main() {
	int scan1;

	int eNentry;
	int eAentry;
	int eSentry;
	int eDentry;

	int eage;
	float esal;
	char edname[40];
	char ename[40];
	char fltname[40];

	char deptName[40];

	int recordid = 1;

	char errStr[200];

	int* ivalue = NULL;
	char* cvalue = NULL;

	/********************************************************************************
	 *  Αρχικοποίηση των εσωτερικών δομών του λογισμικού των ΒΔ                     *
	 ********************************************************************************/
	AM_Init();

	strcpy(empName, "EMP-NAME");
	strcpy(empAge, "EMP-AGE");
	strcpy(empSal, "EMP-SAL");
	strcpy(empDname, "EMP-DNAME");
	strcpy(deptName, "DEPT-NAME");
	strcpy(fltname, "EMP-FAULT");

	/********************************************************************************
	 *  Δημιουργία ΒΔ που θα περιέχουν πληροφορίες για υπαλλήλους                   *
	 ********************************************************************************/

	if (AM_CreateIndex(empName, STRING, sizeof(empName) - 1, INTEGER,
			sizeof(int)) != AME_OK) {
		sprintf(errStr, "Error in AM_CreateIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	if (AM_CreateIndex(empAge, INTEGER, sizeof(int), STRING, 39) != AME_OK) {
		sprintf(errStr, "Error in AM_CreateIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	if (AM_CreateIndex(empSal, FLOAT, sizeof(float), STRING, 39) != AME_OK) {
		sprintf(errStr, "Error in AM_CreateIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	if (AM_CreateIndex(fltname, FLOAT, 39, STRING, 39) != AME_OK) {
		sprintf(errStr, "Expected error in AM_CreateIndex called on %s \n",
				fltname);
		AM_PrintError(errStr);
	} else {
		sprintf(errStr, "Creating %s index should have failed \n", fltname);
		AM_PrintError(errStr);
	}

	if (AM_CreateIndex(empDname, STRING, 10, FLOAT, sizeof(float)) != AME_OK) {
		sprintf(errStr, "Error in AM_CreateIndex called on %s \n", empDname);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  ¶νοιγμα των τεσσάρων ΑΚ (με σκοπό την εισαγωγή εγγραφών)                    *
	 ********************************************************************************/

	if ((eNentry = AM_OpenIndex(empName)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	if ((eAentry = AM_OpenIndex(empAge)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	if ((eSentry = AM_OpenIndex(empSal)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	if ((eDentry = AM_OpenIndex(empDname)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empDname);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Εισαγωγή των δυάδων (τιμή1, τιμή2) στα ΑΚ. Υπάρχουν 100 συνολικά εισαγωγές    *
	 *  σε κάθε αρχείο, οι οποίες διαχωρίζονται με παύλες για δική σας ευκολία         *
	 ********************************************************************************/

	/* -----------------------------------------------------------------------------*/
	recordid = 0;

	strcpy(ename, "XRHSTOFORIDHS ANTWNHS");
	eage = 25;
	esal = 155.5;
	strcpy(edname, "GALATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PARAGWGOY MARIA");
	eage = 30;
	esal = 208.9;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "IWAKEIMIDHS ELEYUERIOS");
	eage = 49;
	esal = 255.1;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "SYRIANOS ARHS");
	eage = 59;
	esal = 306.9;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KYRIAKIDHS GRHGORIOS");
	eage = 27;
	esal = 448.1;
	strcpy(edname, "PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "XANTZHKYRIAKOS ANTWNIOS");
	eage = 69;
	esal = 357.5;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "APELPISIADOY IWSHFINA");
	eage = 21;
	esal = 400.0;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PANTAZHS LEYTERHS");
	eage = 19;
	esal = 367.6;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KOYROPETROGLOY KWSTAS");
	eage = 33;
	esal = 315.8;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "TSAXPINH EYLAMPIA");
	eage = 42;
	esal = 267.9;
	strcpy(edname, "SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "TRIGWNIDHS PYQAGORAS");
	eage = 51;
	esal = 218.9;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PISIDOY ANTIGONH");
	eage = 61;
	esal = 169.1;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);
	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ROYBAS APOSTOLOS");
	eage = 25;
	esal = 175.2;
	strcpy(edname, "GLYKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "APOSBOLHS ANDREAS");
	eage = 36;
	esal = 227.2;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ASTRONOMOGLOY ISAAK");
	eage = 42;
	esal = 278.2;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "MOLOTOFIDHS AYGOYSTHS");
	eage = 53;
	esal = 328.5;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KYTTAROGLOY RODOYLA");
	eage = 65;
	esal = 378.5;
	strcpy(edname, "SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "MPALADOROS PETROS");
	eage = 22;
	esal = 428.1;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "OIKONOMIDHS ALKHS");
	eage = 34;
	esal = 478.8;
	strcpy(edname, "GLYKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ARTEMIADH PETROYLA");
	eage = 19;
	esal = 436.5;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PANTELIADOY ELENH");
	eage = 44;
	esal = 387.1;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PERISTERIADH ANNA");
	eage = 52;
	esal = 337.1;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "GEWRGAKOPOYLOS EYQYMIOS");
	eage = 64;
	esal = 286.7;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PANAGIWTOPOYLOY QEODWRA");
	eage = 23;
	esal = 238.0;
	strcpy(edname, "LAXANIKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "LASKARIDHS SWKRATHS");
	eage = 36;
	esal = 189.2;
	strcpy(edname, "PSARIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ZWGRAFOY ALEXANDRA");
	eage = 19;
	esal = 198.2;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "NIKOLAKOPOYLOS IWANNHS");
	eage = 41;
	esal = 248.6;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "STAMOS IASWNAS");
	eage = 56;
	esal = 295.5;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "TERZOPOYLOY ANASTASIA");
	eage = 65;
	esal = 347.1;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "AFENTOYLIDHS AXILLEAS");
	eage = 23;
	esal = 398.1;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ANASTASIADHS ALEXANDROS");
	eage = 36;
	esal = 449.2;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "BELIBASSAKH AIMILIA");
	eage = 44;
	esal = 458.8;
	strcpy(edname, "GLYKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "XRISTODOYLIDOY ARISTEA");
	eage = 56;
	esal = 406.9;
	strcpy(edname, "PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "MARKOY ANTWNIOS");
	eage = 69;
	esal = 359.9;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "EYAGGELOY LEWNIDAS");
	eage = 70;
	esal = 308.9;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "SPAQARAKH SOFIA");
	eage = 18;
	esal = 257.8;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "STEQANAKHS APOSTOLOS");
	eage = 19;
	esal = 205.6;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PARASKEYOPOYLOS ARGYRHS");
	eage = 18;
	esal = 157.9;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ANTWNOPOYLOY ELISABET");
	eage = 22;
	esal = 169.0;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "GEWRGIADOY MARIA");
	eage = 33;
	esal = 217.0;
	strcpy(edname, "KRASIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "SKLABOY MARQA");
	eage = 45;
	esal = 268.1;
	strcpy(edname, "PSARIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "BASLEIADHS ARISTARXOS");
	eage = 56;
	esal = 317.8;
	strcpy(edname, "PSARIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);
	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "SEREMETHS ANASTASIOS");
	eage = 63;
	esal = 368.8;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "STAMATIADOY ARTEMIS");
	eage = 18;
	esal = 415.1;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KONTOGIANNIDHS XRHSTOS");
	eage = 27;
	esal = 468.1;
	strcpy(edname, "LAXANIKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "WRAIOPOYLOS KVNSTANTINOS");
	eage = 30;
	esal = 479.5;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ELEYQERIADHS MARKOS");
	eage = 49;
	esal = 428.1;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ADAMOPOYLOS ANTWNIOS");
	eage = 57;
	esal = 377.0;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "EYSTAQIOY XRISTINA");
	eage = 63;
	esal = 328.1;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "MARKOGIANNAKHS EYSTAQIOS");
	eage = 71;
	esal = 279.4;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KOKKINOS ARISTOTELHS");
	eage = 28;
	esal = 228.4;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ZORMPAS KVNSTANTINOS");
	eage = 33;
	esal = 177.1;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "POLYKANDRIWTHS DHMHTRIOS");
	eage = 41;
	esal = 126.9;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "MANWLIKIDHS ZHSHS");
	eage = 58;
	esal = 186.9;
	strcpy(edname, "PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ZENEMPISHS IAKWBOS");
	eage = 69;
	esal = 237.9;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KALOGHROY ALKMHNH");
	eage = 19;
	esal = 288.2;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "QEOTOKAS PARASKEYAS");
	eage = 25;
	esal = 337.9;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KANARH EYQALEIA");
	eage = 38;
	esal = 385.1;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "LYRISTHS EYSTAQIOS");
	eage = 18;
	esal = 438.9;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "TREMPELHS ALKHBIADHS");
	eage = 40;
	esal = 486.2;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ARGYRIADHS ANASTASIOS");
	eage = 52;
	esal = 447.9;
	strcpy(edname, "KRASIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "QWMADAKHS EMMANOYHL");
	eage = 66;
	esal = 398.1;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "SIDHROPYLOS XRYSANQOS");
	eage = 21;
	esal = 349.3;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "STELLAS PETROS");
	eage = 33;
	esal = 298.6;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "DHMHTRIADOY XARALAMPIA");
	eage = 44;
	esal = 245.9;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PALAIOLOGOS QWMAS");
	eage = 50;
	esal = 198.1;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "TSAKIROGLOY ANASTASIOS");
	eage = 61;
	esal = 147.1;
	strcpy(edname, "PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "STAMATIOY DHMHTRIOS");
	eage = 24;
	esal = 159.6;
	strcpy(edname, "XYMOI");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "DEMERTZH AIMILIA");
	eage = 31;
	esal = 207.0;
	strcpy(edname, "SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "EYSTAQIOY MARIA");
	eage = 45;
	esal = 258.0;
	strcpy(edname, "PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "MARKOGIANNAKH PARASKEYH");
	eage = 53;
	esal = 309.2;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "DIAMANTOPOYLOS QEOXARHS");
	eage = 60;
	esal = 356.9;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "LEONTIOS STAMATHS");
	eage = 24;
	esal = 408.7;
	strcpy(edname, "LAXANIKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PAPPA EYQYMIA");
	eage = 31;
	esal = 457.0;
	strcpy(edname, "PSARIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "LAMPRINOYDHS NIKOLAOS");
	eage = 45;
	esal = 508.1;
	strcpy(edname, "SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "LOYKOPOYLOY BASILIKH");
	eage = 56;
	esal = 469.1;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "SKLABOS APOSTOLOS");
	eage = 61;
	esal = 418.7;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PANAGOPOYLOY EYANQIA");
	eage = 54;
	esal = 369.2;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "MAMALH NIKOLETA");
	eage = 53;
	esal = 315.8;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PANAGIWTOPOYLOS XARALAMPOS");
	eage = 34;
	esal = 268.2;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "XRYSANQAKOPOYLOS DHMHTRIOS");
	eage = 25;
	esal = 218.7;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "XRISTOOYLOS PANAGIWTHS");
	eage = 32;
	esal = 166.0;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "BASILAKAKH STAMATOYLA");
	eage = 43;
	esal = 175.0;
	strcpy(edname, "SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "TSOLAKAS KWNSTANTINOS");
	eage = 52;
	esal = 228.9;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KOLLIA AIKATERINH");
	eage = 65;
	esal = 277.2;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "GKOYRHS NIKOLAOS");
	eage = 56;
	esal = 328.2;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KAZAKOS STEFANOS");
	eage = 41;
	esal = 376.8;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "APOSTOLOPOYLOS KWNSTANTINOS");
	eage = 33;
	esal = 428.6;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PAPAQANASIOY ELENH");
	eage = 27;
	esal = 479.2;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "SOYTSOS IWANNHS");
	eage = 20;
	esal = 439.9;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "MATSARIDHS XARALAMPOS");
	eage = 31;
	esal = 388.9;
	strcpy(edname, "LAXANIKA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "STAYROY FILIPPOS");
	eage = 19;
	esal = 339.1;
	strcpy(edname, "PSWMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "TAGKOPOYLOS DHMHTRIOS");
	eage = 44;
	esal = 287.1;
	strcpy(edname, "KRASIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "REGKLEITHS STEFANOS");
	eage = 51;
	esal = 235.7;
	strcpy(edname, "KRASIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "LASKOS NIKOLAOS");
	eage = 60;
	esal = 188.3;
	strcpy(edname, "SAPOUNIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "ZANAKHS MIXAHL");
	eage = 18;
	esal = 198.1;
	strcpy(edname, "KREATA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "AMORIANOS KWNSTANTINOS");
	eage = 63;
	esal = 247.4;
	strcpy(edname, "SALAMIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "KOYTSOYKOS GEWRGIOS");
	eage = 54;
	esal = 297.9;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "PAPASWTHRIOY KYRIAKOS");
	eage = 46;
	esal = 348.6;
	strcpy(edname, "TYRIA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	strcpy(ename, "LAMPROPOYLOY XRISTINA");
	eage = 31;
	esal = 399.0;
	strcpy(edname, "FROUTA");
	insertEntries(eNentry, eAentry, eSentry, eDentry, ename, eage, esal,
			edname, ++recordid);

	/* -----------------------------------------------------------------------------*/

	/********************************************************************************
	 *  Τέλος των εισαγωγών στα τέσσερα αρχεία ΒΔ                                   *
	 ********************************************************************************/

	if (AM_CloseIndex(eNentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eAentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eSentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eDentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empDname);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *      Επερώτηση #1                                                            *
	 *  Έυρεση της εγγραφής "KOKKINOS ARISTOTELHS", γνωρίζοντας ότι το όνομα είναι  *
	 *	μοναδικό                                                                    *
	 ********************************************************************************/

	strcpy(ename, "KOKKINOS ARISTOTELHS");
	printf("\nRESULT OF QUERY #1\n\n");

	if ((eNentry = AM_OpenIndex(empName)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Άνοιγμα της αναζήτησης                                                      *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eNentry, EQUAL, (void *) ename)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Εύρεση της πρώτης εγγραφής με όνομα "KOKKINOS ARISTOTELHS"                  *
	 ********************************************************************************/
	if ((ivalue = (int*) AM_FindNextEntry(scan1)) != NULL)
		printf("%d \n\n", *ivalue);
	else {
		sprintf(errStr, "Error in AM_FindNextEntry called on scan %d \n\n",
				scan1);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ                                  *
	 ********************************************************************************/
	if (AM_CloseIndexScan(scan1) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
				scan1);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eNentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *      Επερώτηση #2                                                            *
	 *  Εύρεση των ονομάτων των υπαλλήλων με μισθό μικρότερο από 398.100            *
	 ********************************************************************************/

	esal = 398.1;
	printf("\nRESULT OF QUERY #2\n\n");

	if ((eSentry = AM_OpenIndex(empSal)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  ¶νοιγμα της αναζήτησης                                                      *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eSentry, LESS_THAN_OR_EQUAL, (void *) &esal))
			< 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Σάρωση των σχετικών εγγραφών και εκτύπωση των αντίστοιχων ονομάτων          *
	 ********************************************************************************/
	while ((cvalue = (char*) AM_FindNextEntry(scan1)) != NULL) {
		printf("'%s'\n", cvalue);
	}

	if (AM_errno != AME_EOF) {
		sprintf(errStr, "Error in AM_FindNextEntry called on scan %d \n", scan1);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ                                  *
	 ********************************************************************************/
	if (AM_CloseIndexScan(scan1) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
				scan1);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eSentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *      Επερώτηση #3                                                            *
	 *  Αναζήτηση στο αρχείο για τα ονόματα αυτών που δεν είναι 19 χρονών           *
	 ********************************************************************************/

	eage = 19;
	printf("\nRESULT OF QUERY #3\n\n");

	if ((eAentry = AM_OpenIndex(empAge)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  'Aνοιγμα της αναζήτησης                                                      *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eAentry, NOT_EQUAL, (void *) &eage)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Σάρωση των σχετικών εγγραφών και εκτύπωση των αντίστοιχων ονομάτων          *
	 ********************************************************************************/
	while ((cvalue = (char*) AM_FindNextEntry(scan1)) != NULL) {
		printf("'%s' \n", cvalue);
	}

	if (AM_errno != AME_EOF) {
		sprintf(errStr, "Error in AM_FindNextEntry called on scan %d \n", scan1);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Κλείσιμο της αναζήτησης και του αρχείου ΒΔ                                  *
	 ********************************************************************************/
	if (AM_CloseIndexScan(scan1) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
				scan1);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eAentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *      Eπερώτηση #4                                                            *
	 *  Αναζήτηση στο enames για να βρεθεί το αναγνωριστικό της εγγραφής ANEMONH    *
	 ********************************************************************************/

	strcpy(ename, "ANEMONH THETIS");
	printf("\nRESULT OF QUERY #4\n\n");

	if ((eNentry = AM_OpenIndex(empName)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  'Aνοιγμα της αναζήτησης                                                      *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eNentry, EQUAL, (void *) ename)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Eύρεση της εγγραφής ANEMONH.                                                *
	 ********************************************************************************/
	if ((ivalue = (int*) AM_FindNextEntry(scan1)) != NULL)
		printf("%d \n\n", *ivalue);
	else if (AM_errno != AME_EOF) {
		sprintf(errStr, "ANEMONH was not found due to an error");
		AM_PrintError(errStr);
	} else {
		sprintf(errStr,
				"ANEMONH was not found... (Note: This is the correct behaviour!)");
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ                                  *
	 ********************************************************************************/
	if (AM_CloseIndexScan(scan1) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
				scan1);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eNentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Τέλος του δοκιμαστικού προγράμματος για εισαγωγές/ αναζητήσεις    *
	 ********************************************************************************/
	 
	AM_Close();

	return 0;
}
