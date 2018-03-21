
/****************** parsemain.cxx ***************
			Stand Sommersemester 2018
*************************************************/



#ifndef GLOBAL_H
#include "global.h"
#endif 


// Mainprogramm für Parser

int main()
{


	// Compiler  intialisieren 
	initialize();			
	// Analyse PROGRAM
	program();		
	// Analyse korrekt beendet --> ST ausgeben; Abschlussarbeiten 
	stop ();				
	exit(0); 			// Erfolg 
}

