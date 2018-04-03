/************************ lexan.cxx  Sommersemester 2018 ********************/
/*************** Scanner *******************************/


#ifndef GLOBAL_H
#include "global.h"
#endif

#define NORW   14   		/* Zahl der reservierten Worte */
#define RWSYMLEN  15		/* Max. Länge reservierter Symbole */


int lineno;					/* Zeilennummer */

int num ;           /* Wert einer int-Konstanten  */
double realnum; 		/* Wert einer real-Konstanten*/

char idname[BSIZE];      /* Name eines Bezeichners ; wird vom Parser weiterverwendet */


char actchar;       /* gelesenes Zeichen */


/*  Um Bezeichner von reservierten Symbolene unterscheiden zu können,
wird eine Tabelle reservierte Worte verwendet (restable).

Die Tabelle enthält Einträge für jedes Schlüsselwort, bestehend aus
dem Schlüsselwort selbst und dem zugehörigen Tokentyp (Codierung vgl.global.h):

Bei Erkennen eines möglichen Bezeichners wird zuerst die Tabelle
der reservierten Symbole durchsucht (lookforres);
wird ein Schlüsselwort gefunden, liefert lookforres den dem Schlüsselwort
zugeordneten Tokentyp; sonst 0. Bei Ergebnis 0 liegt dann tatsächlich ein
Bezeichner vor.
*/


/* Struktur eines Eintrags in der Tabelle reservierter Symbole */
struct ressw {
  char ressymbol [RWSYMLEN];			/* Symbol */
  int token;							/* zugehöriger Tokentyp */
};

/* Tabelle reservierter Worte */
struct ressw restable [] = {
  {"const", CONST},
	{"var", VAR},
	{"procedure", PROCEDURE},
	{"call", CALL},
	{"begin", BEGIN},
	{"end", END},
	{"if", IF},
	{"then", THEN},
	{"else",ELSE},
	{"while",WHILE},
	{"do", DO},
	{"int", INT},
	{"real", REAL},
	{"fi", FI}
};

/* Suchen nach einem reservierten Symbol */

/* Sucht in Tabelle reservierter Worte nach s und liefert zugehörigen Token
   falls gefunden,
   sonst 0
*/
int lookforres( char *s) {
  struct ressw *ptr;
  for ( ptr = restable; ptr < &restable[NORW]; ptr++ ) {
    if (strcmp(ptr->ressymbol, s) == 0) {
			return (ptr->token); /* Symbol gefunden */
    }
  }
	return(0); /* Symbol nicht gefunden */
}

/******************* Initialisieren des Scanners **********************/

/* wird aufgerufen von  initialize () aus init.cxx ;
nimmt Vorbesetzungen der Variablen num, realnum,  idname und lineno vor;
liest das erste Zeichen aus der Eingabe */
void initlexan() {
  num = NONE;
  realnum = 0.0;
  idname [0] = '\0';
  lineno = 1;
  fin.get(actchar);			/* Erstes Zeichen der Eingabe lesen */
}

/*
*******************************************************************************
********** Funktion nextsymbol ************************************************
*******************************************************************************

**** zentrale Funktion zum Lesen des nächsten lexikalischen Symbols ***************
*****  identifiziert nächstes lexikalisches Symbol der Eingabe **********

***** WICHTIG!!!!!
		Bei Aufruf von nextsymbol muss  sich das nächste Eingabezeichen bereits in actchar befinden
*******

liefert Codierung des nächsten Symbols (token):
   - Konstante:				   token == INTNUM und Wert der Konstanten in Variable num
									       token == REALNUM und Wert in realnum
   - Bezeichner:				 token == ID  und Zeiger auf Name in idname
	 - Schlüsselwort:			 token == Tokentyp des reservierten Symbols nach Suche in restable
	 - Operatoren,Sonderzeichen :entsprechende Token
**/
int nextsymbol() {
	int token;
	char lexbuf[BSIZE];		/* lokaler Puffer für Eingabezeichen */
	while( !fin.eof() ) {			/* Eingabe-Dateiende nicht erreicht */
    /* Blank und Tab in Ausgabedatei kopieren und überlesen */
		if ( actchar== ' ' || actchar == '\t' ) {
      fout.put(actchar);
			fin.get(actchar);

    /* Newline in Ausgabedatei kopieren, überlesen/entfernen, Zeilennummer erhöhen */
    } else if (actchar== '\n'  ||  actchar == '\r') {
      fout.put(actchar);
		  fin.get(actchar);
		  lineno++;

    /***** actchar ist Ziffer --> Konstanten erkennen  *****/
		} else if ( isdigit(actchar) ) {
			char zahl [BSIZE];	 /* Puffer für Ziffern */
			int b = 0;				   /* Zeichenzahl*/

      // TODO

    /***** actchar ist Buchstabe -->  Identifikatoren erkennen ****/
    } else if ( isalpha(actchar) ) {
      int b = 0 ;				/* Zeichenzahl */
					/* reg. Ausdruck   letter (letter|digit)*  erkennen ==>
					    solange Buchstaben oder Ziffern folgen --> Identifikator */

      // TODO

      /***** Sonderzeichen oder Operatoren erkennen ***************/
		} else {
      fout.put(actchar);				/* Zeichen in Ausgabedatei */
	    switch(actchar) {
        case '=':
          fin.get(actchar);
					return(EQ);

        // TODO

        default: 	error(32);
			} /* end-switch */
		} /* end-else */
 	}/* end while */
 	return(DONE); 	/* EIngabe -Ende erreicht */
}
