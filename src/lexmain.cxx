
/****************** lexmain.cxx *********************/

/* Hauptprogramm zum Test der lexikalischen Analyse

		Stand: Sommersemester 2018






************************************************/


#ifndef GLOBAL_H
#include "global.h"
#endif

int main(int argc, char** argv) {
	int lookahead = -1;

	/* Initialisierungen der lexikalischen Analyse durchführen */
	initialize(argc, argv);

	/* 1. Symbol lesen  */
	lookahead = nextsymbol();

	/* Symbol ausgeben */
	fout << "		Tokentyp=    " << lookahead << endl;

 /* Symbole aus Eingabedatei lesen bis Dateiende */
  while (lookahead != DONE) {
		lookahead = nextsymbol();
		fout << "		Tokentyp=    " << lookahead << endl;
 	}
	exit(0); 	/* Erfolg */
}
