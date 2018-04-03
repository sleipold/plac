
/****************** parsemain.cxx ***************
			Stand Sommersemester 2018
*************************************************/



#ifndef GLOBAL_H
#include "global.h"
#endif


// Mainprogramm für Parser

int main(int argc, char** argv) {
	// Compiler  intialisieren
	initialize(argc, argv);
	// Analyse PROGRAM
	program();
	// Analyse korrekt beendet --> ST ausgeben; Abschlussarbeiten
	stop();
	exit(0); 			// Erfolg
}
