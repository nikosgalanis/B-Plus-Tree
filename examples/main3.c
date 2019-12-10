/********************************************************************************
 * main3.c                                                                    *
 * Αυτό το πρόγραμμα παίζει τον ρόλο ενός αρθρώματος υψηλού επιπέλου ενος ΣΔΒΔ  *
 * το οποίο διαγράφει όλα τα b+-δενδρα λόγω της διαγραφής όλης της βάσης.       *
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defn.h"
#include "AM.h"

int main()
{
	char empName[40];
	char empAge[40];
	char empSal[40];
	char empDname[40];
	char deptName[40];

	char errStr[200];
	
	
	strcpy(empName,"EMP-NAME");
	strcpy(empAge,"EMP-AGE");
	strcpy(empSal,"EMP-SAL");
	strcpy(empDname,"EMP-DNAME");
	strcpy(deptName,"DEPT-NAME");

/********************************************************************************
 *  Αρχικοποίηση των εσωτερικών δομών του λογισμικού των ΒΔ                     *
 ********************************************************************************/
	AM_Init();

/********************************************************************************
 *  Διαγραφή των b+-δένδρων , για παράδειγμα λόγω της διαγραφής                 *
 *  ολόκληρης της βάσης                                                         *
 ********************************************************************************/
	if (AM_DestroyIndex(empName) != AME_OK) {
  	 sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",empName);
	   AM_PrintError(errStr);
	}

	if (AM_DestroyIndex(empAge) != AME_OK) {
  	 sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",empAge);
	   AM_PrintError(errStr);
	}

	if (AM_DestroyIndex(empSal) != AME_OK) {
  	 sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",empSal);
	   AM_PrintError(errStr);
	}

	if (AM_DestroyIndex(empDname) != AME_OK) {
  	 sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",empDname);
   	AM_PrintError(errStr);
	}

	if (AM_DestroyIndex(deptName) != AME_OK) {
  	 sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",deptName);
	   AM_PrintError(errStr);
	}
	
	AM_Close();
	
	return 0;
}
