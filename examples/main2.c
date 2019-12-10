/********************************************************************************
 *  main2.c                                                                   *
 *  Το πρόγραμμα αυτό παίζει τον ρόλο ενός αρθρώματος υψηλού επιπέδου eνός ΣΔΒΔ *
 *  το οποίο καλεί συναρτήσεις από το επίπεδο που ασχολείται με τα b+-δένδρα.   *
 *  Στην προσομοίωση που κάνει αυτό το πρόγραμμα, δίνεται                       *
 *  ιδιαίτερη προσοχή σε αναζητήσεις                                            *
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defn.h"
#include "AM.h"

int main() {
	int scan1;
	int scan2;

	char empName[40];
	char empAge[40];
	char empSal[40];
	char empDname[40];
	char deptName[40];

	int eNentry = 0;
	int eAentry = 0;
	int eSentry = 0;
	int eDentry = 0;
	int dNentry = 0;

	char ename[40];
	int eage;
	float esal;
	char edname[30];

	int count;

	char errStr[200];

	int* ivalue = NULL;
	float* fvalue = NULL;
	char* cvalue = NULL;
	int recordid = 0;

	strcpy(empName, "EMP-NAME");
	strcpy(empAge, "EMP-AGE");
	strcpy(empSal, "EMP-SAL");
	strcpy(empDname, "EMP-DNAME");
	strcpy(deptName, "DEPT-NAME");

	/********************************************************************************
	 *  Αρχικοποίηση των εσωτερικών δομών του λογισμικού των ΒΔ.                    *
	 ********************************************************************************/
	AM_Init();

	/********************************************************************************
	 *  Άνοιγμα των τεσσάρων ΒΔ με σκοπό την διαγραφή μίας εγγραφής από κάθε αρχείο *
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

	strcpy(ename, "LYRISTHS EYSTAQIOS");
	eage = 20;
	esal = 438.9;
	strcpy(edname, "FROUTA");
	printf("\nRESULT OF DELETION #1\n\n");
	recordid = 59;
	ivalue = &recordid;

	/********************************************************************************
	 *  Άνοιγμα της σάρωσης για αναζήτηση βάσει ηλικίας                             *
	 ********************************************************************************/
	eage = 20;
	if ((scan1 = AM_OpenIndexScan(eAentry, EQUAL, (void *) &eage)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Εύρεση των σχετικών εγγραφών 							                    *
	 ********************************************************************************/
	count = 0;
	while ((cvalue = AM_FindNextEntry(scan1)) != NULL) {
		printf("Found %s\n", cvalue);
		count++;
	}
	printf("%d records found with age %d\n", count, eage);

	/********************************************************************************
	 *  Κλείσιμο του αρχείου και της αναζήτησης                                     *
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
	 *      Επερώτηση #1                                                            *
	 *  Εύρεση των ονομάτων των  υπαλλήλων που παίρνουν περισσότερα από 398.100euro *
	 ********************************************************************************/

	esal = 398.10;
	printf("\nRESULT OF QUERY #2a\n\n");

	if ((eSentry = AM_OpenIndex(empSal)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  'Ανοιγμα της σάρωσης                                                         *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eSentry, GREATER_THAN, (void *) &esal)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Αναζήτηση των σχετικών εγγραφών.                                            *
	 ********************************************************************************/
	while ((cvalue = AM_FindNextEntry(scan1)) != NULL) {
		printf("'%s' \n", cvalue);
	}

	/********************************************************************************
	 *  Κλείσιμο του αρχείου και της αναζήτησης                                     *
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
	 *      Επερώτηση #2                                                            *
	 *  Εύρεση των ονομάτων των υπαλλήλων που είναι μεγαλύτεροι ή ίσοι              *
	 *  των 19 χρονών.                                                              *
	 ********************************************************************************/

	eage = 19;
	printf("\nRESULT OF QUERY #3a\n\n");

	if ((eAentry = AM_OpenIndex(empAge)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  'Ανοιγμα της σάρωσης                                                         *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eAentry, GREATER_THAN_OR_EQUAL,
			(void *) &eage)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Εύρεση των σχετικών εγγραφών                                                *
	 ********************************************************************************/
	while ((cvalue = AM_FindNextEntry(scan1)) != NULL) {
		printf("'%s' \n", cvalue);
	}

	/********************************************************************************
	 *  Κλείσιμο του αρχείου και της αναζήτησης                                     *
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
	 *  Δημιουργία ενός ΒΔ που θα περιέχει πληροφορίες για τμήματα                  *
	 ********************************************************************************/
	if (AM_CreateIndex(deptName, STRING, 15, INTEGER, sizeof(int)) != AME_OK) {
		sprintf(errStr, "Error in AM_CreateIndex called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  ¶νοιγμα του νέου ΒΔ με σκοπό την εισαγωγή εγγραφών                          *
	 ********************************************************************************/

	if ((dNentry = AM_OpenIndex(deptName)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Εισαγωγή των δυάδων (τιμή1,τιμή2) στο νέο αρχείο                            *
	 ********************************************************************************/
	recordid = 200;
	ivalue = &recordid;

	strcpy(edname, "GALATA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "SALAMIA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "TYRIA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "FROUTA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "XYMOI");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "KRASIA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "PSWMIA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "KREATA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "GLYKA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "SAPOUNIA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "LAXANIKA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	strcpy(edname, "PSARIA");
	++recordid;

	if (AM_InsertEntry(dNentry, (void *) edname, ivalue) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Τέλος των εισαγωγών                                                         *
	 ********************************************************************************/

	if (AM_CloseIndex(dNentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *      Eπερώτηση # 3                                                           *
	 *  Εύρεση σχετικών εγγραφών σε δύο ΒΔ ταυτόχρονα. Η επερώτηση αυτή αποτελείται *
	 *  από μία (εξωτερική) αναζήτηση η οποία περιέχει πολλαπλές εμφωλιασμένες      *
	 *  επερωτήσεις.                                                                *
	 ********************************************************************************/

	strcpy(edname, "TYRIA");
	printf("\nRESULT OF QUERY #5\n\n");

	if ((eDentry = AM_OpenIndex(empDname)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empDname);
		AM_PrintError(errStr);
	}

	if ((dNentry = AM_OpenIndex(deptName)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  ¶ναζήτηση στο εξωτερικό αρχείο                                              *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eDentry, EQUAL, (void *) edname)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Εύρεση των σχετικών εγγραφων στο εξωτερικό αρχείο και για κάθε              *
	 *  εγγραφή που βρίσκεται, έναρξη μίας εμφωλιασμένης αναζήτησης                 *
	 ********************************************************************************/
	while ((fvalue = (float*) AM_FindNextEntry(scan1)) != NULL) {
		printf("salary: %f \n", *fvalue);

		/*************************************************************************
		 *  Άνοιγμα της εμφωλιασμένης αναζήτησης                                 *
		 *************************************************************************/
		if ((scan2 = AM_OpenIndexScan(dNentry, EQUAL, (void *) edname)) < 0) {
			sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n",
					empDname);
			AM_PrintError(errStr);
		}

		/*************************************************************************
		 *  Εμφωλιασμένη αναζήτηση και εκτύπωση των σχετικών εγγραφών            *
		 *************************************************************************/
		printf("empids:  ");
		while ((ivalue = AM_FindNextEntry(scan2)) != NULL) {
			printf("%d \n", *ivalue);
		}
		printf("\n");

		if (AM_errno != AME_EOF) {
			sprintf(errStr, "Error in AM_FindNextEntry called on scan %d \n",
					scan2);
			AM_PrintError(errStr);
		}

		/*************************************************************************
		 *  Κλείσιμο της εμφωλιασμένης αναζήτησης                                *
		 *************************************************************************/
		if (AM_CloseIndexScan(scan2) != AME_OK) {
			sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
					scan2);
			AM_PrintError(errStr);
		}
	}

	if (AM_errno != AME_EOF) {
		sprintf(errStr, "Error in AM_FindNextEntry called on scan %d \n", scan1);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Κλείσιμο της εξωτερικής αναζήτησης και των δύο αρχείων                      *
	 ********************************************************************************/
	if (AM_CloseIndexScan(scan1) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
				scan1);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eDentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empDname);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(dNentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", deptName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Τέλος του προγράμματος                                                      *
	 ********************************************************************************/
	 
	AM_Close();

	return 0;
}
