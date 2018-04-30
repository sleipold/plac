
/**************** parser.cxx  Sommersemester 2018******************/

/*******   **************/

#ifndef GLOBAL_H
#include "global.h"
#endif

int lookahead;					/* lookahead enthält nächsten Eingabetoken */

int exp();
int nextsymbol();


/******************  factor  **********************************************/
/* analysiert wird der korrekte Aufbau eines Faktors

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
*/
int factor() {
  int kind;
  st_entry *found;		// Zeiger auf Eintrag in ST
	int factor_typ;

	if (tracesw)
	 trace << "\n Zeile:" << lineno << "	Faktor" << endl;

  // je nach nächstem Eingabesymbol in lookahead
	switch(lookahead) {
		case KLAUF:	/* Symbol '(' folgt --> (EXPRESSION) erwartet*/
			lookahead=nextsymbol();
			exp();
			if(lookahead== KLZU)
				// korrekt ; nächstes Symbol lesen --> Ende
				lookahead = nextsymbol();
			else
				error(27); // kein Faktor
			break;

		case INTNUM:
 			/* Int-Zahl (INTNUMBER) gefunden --> okay */
			lookahead=nextsymbol();
			break;

		case REALNUM: 		/* Real-Zahl (REALNUMBER) gefunden --> okay */
			lookahead=nextsymbol();
			break;

		case ID:	/* Identifikator (ID) gefunden  */
			/* Suche Identifikator in Symboltabelle ;
				angewandtes Auftreten -->
				Deklaration muss vorhanden sein
				und also Eintrag in ST */

			found = lookup(idname);

      /* nicht gefunden --> Fehler: Id nicht deklariert*/
			if (found == NULL)
				error(10);

      // Id in ST gefunden ; Art prüfen
			else {
        kind = found->token;	// Art des ST-Eintrags

				switch(kind) {
          case KONST:	// Konstantenname --> okay
						break;

				  case INTIDENT:// einfache Variable, Typ int --> okay
						break;

				  case REALIDENT:// einfache Variable, Typ real --> okay
						break;

				  case PROC:	// Name einer Prozedur in
						// Factor nicht erlaubt
						error(20); // --> exit
						// break;
				} // endswitch (kind)

			  // nächstes Symbol lesen
        lookahead=nextsymbol();
	    } // endif
			break;

		default:	// kein korrekter Faktor
			error (27);
	}	// endswitch (lookahead)
	return (0);
} 	// end factor


/******************  term ***************************************************/
/* analysiert wird der korrekte Aufbau eines Terms nach folgender Syntax:

			TERM	::=		FACTOR  { '*' FACTOR |  '/' FACTOR }*

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
							Typ des Terms ist Funktionswert

*/
int term() {
  int ret;

	if (tracesw)
    trace << "\n Zeile:" << lineno << "Term:" << endl;
	ret = factor();
	// korrekter Factor

	while(lookahead == MULT || lookahead == DIV) {
	  // solange * oder / folgt, muss Factor kommen
    // nächstes Symbol lesen
	  lookahead=nextsymbol();
		ret = factor();
	}
	return(0);
 }

/******************  exp ***************************************************/
/* analysiert wird der korrekte Aufbau eines Ausdrucks nach folgender Syntax:

			EXPRESSION	::=		TERM { '+' TERM |  '-' TERM}*

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
							Funktionswert ist Typ des Ausdrucks
*/
int exp() {
	int typ_left,typ_right;
	if (tracesw)
	    trace<<"\n Zeile:"<< lineno<<"Ausdruck" << endl;

	typ_left = term();
	// korrekter Term

	while (lookahead == PLUS || lookahead == MINUS ) {
		// solange + oder - folgt, muss Term kommen
    // nächstes Symbol lesen
		lookahead=nextsymbol();
		// Term prüfen
		typ_right = term();
		// nach korrektem Ende wurde nächstes Symbol gelesen
	}
	return (0);
}

/******************  condition ***************************************************/
/* analysiert wird der korrekte Aufbau einer Bedingung nach folgender Syntax:

			CONDITION	::=		EXPRESSION  RELOP  EXPRESSION

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead
*/
int condition() {
  int typ_left, typ_right;

	if (tracesw)
	    trace<<"\n Zeile:"<< lineno<<"Condition" << endl;

	typ_left = exp();
	// korrekter Ausdruck
	// relationaler Operator muss folgen
	switch(lookahead) {
		case EQ:
		case NE:
		case LT:
		case LE:
		case GT:
		case GE:// nächstes Symbol lesen
				lookahead=nextsymbol();
				// Ausdruck muss folgen
				typ_right = exp();
				break;

		default: // kein relationaler Operator
				 error(19);
	}
	if (typ_left != typ_right)
		errortext("Typen der Operanden nicht kompatibel");

	return(typ_left);
}

/****************** statement ***************************************************/
/* analysiert wird der korrekte Aufbau eines Statements nach folgender Syntax:

			STATEMENT 	::=		IDENT  ':=' EXPRESSION
							|	call IDENT
							|	begin STATEMENT { ';' STATEMENT }* end
							|	if CONDITION then STATEMENT [else STATEMENT ] fi
							|	while CONDITION do STATEMENT



Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void statement() {
	st_entry *found;		// Zeiger auf ST-Eintrag
	int typ_left, typ_right;

	if (tracesw)
		trace<<"\n Zeile:"<< lineno<<"Statement" << endl;

	// überprüfung des aktuellen lex. Symbols
	// TODO
	// IDENT  ':=' EXPRESSION
	if(lookahead == ID) {
 
		// ST nach idname durchsuchen, idname muss bereits existieren
		found = lookup(idname);
		if(found == NULL) {
			error(10);
		}
                
		lookahead = nextsymbol();
		if(lookahead != ASS) {
			error(40);
		}

		if(found->token == KONST || found->token == PROC) {
			error(11);
		}
                
		lookahead = nextsymbol();
		exp();
	}

	// call IDENT 
	else if(lookahead == CALL) {
		lookahead = nextsymbol();
		if(lookahead != ID) {
			error(13);
		}

		// ST nach idname durchsuchen, idname muss bereits existieren
		found = lookup(idname);
		if(found == NULL) {
			error(10);
		}

		// in CALL-Anweisung darf ausschließlich PROC auftreten
		if(found->token != PROC) {
			error(14);
		}
		
		lookahead = nextsymbol();
	}

	// begin STATEMENT { ';' STATEMENT }* end
	else if(lookahead == BEGIN) {
		lookahead = nextsymbol();
		statement();
                
		while(lookahead == SEMICOLON) {
			lookahead = nextsymbol();
			statement();
		}

		if(lookahead != END) {
			error(16);
		}
	}

	// if CONDITION then STATEMENT [else STATEMENT ] fi
	else if(lookahead == IF) {
		lookahead = nextsymbol();
		condition();
                
		if(lookahead != THEN) {
			error(15);
		}

		lookahead = nextsymbol();
		statement();

		// hier darf entweder else oder fi folgen
		if(lookahead == ELSE) {
			lookahead = nextsymbol();
			statement();
		}

		if(lookahead != FI) {
			error(39);
		}
                lookahead = nextsymbol();
	}

	// while CONDITION do STATEMENT
	else if(lookahead == WHILE) {
		lookahead = nextsymbol();
		condition();
		if(lookahead != DO) {
			error(17);
		}

		lookahead = nextsymbol();
		statement();
	}

	else {
		errortext("Statement wird erwartet");
	}

	//lookahead = nextsymbol();
	return;	// end statement
}



/****************** procdecl ***************************************************/
/* analysiert wird der korrekte Aufbau einer Prozedurdeklaration
nach folgender Syntax:

			PROCDECL 	::=		{procedure IDENT ';' BLOCK ';' }*



Schnittstelle:
	bei Aufruf :			erkannt wurde das Schlüsselwort procedure
							nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void procdecl() {
	st_entry* neu, *found;          // Zeiger auf ST-Eintrag

	symtable* neusym;		// Zeiger auf Symboltabelle

	if (tracesw)
		trace<<"\n Zeile:"<< lineno<<"Procdeklaration:" << endl;

	// TODO
	if(lookahead != ID) {
		error(4);
	} 

	// Überprüfung ob idname bereits im aktuellen scope bekannt
	if(lookup_in_actsym(idname) != NULL) {
		error(34);
	}

	insert(PROC);

	// liefert den neuen Eintrag in der neuen ST
	neu = lookup_in_actsym(idname);

	lookahead = nextsymbol();
	if(lookahead != SEMICOLON) {
		error(5);
	}

	lookahead = nextsymbol();
	block(neu->subsym);
	lookahead = nextsymbol();
        
	if(lookahead != SEMICOLON) {
		error(5);
	}

	lookahead = nextsymbol();
	while(lookahead == PROCEDURE) {
		lookahead = nextsymbol();
		procdecl();
	}

	return;   // end procdecl
}


/****************** vardecl ***************************************************/
/* analysiert wird der korrekte Aufbau einer Variablendeklaration
nach folgender Syntax:

			VARDECL 	::=		  var IDENT ' : ' TYP  { ',' IDENT ' : ' TYP} *  ';'


Schnittstelle:
	bei Aufruf :			erkannt wurde das Schlüsselwort var
							nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void vardecl() {
	st_entry* neu, *found;

	if (tracesw)
		trace << "\n Zeile:" << lineno << "Variablendeklaration:" << endl;

	// nach var muss Identifikator folgen
	// TODO
	if(lookahead != ID) {
		error(4);
	}

	lookahead = nextsymbol();
	if(lookahead != COLON) {
		error(35);
	}

	// Prüfen auf Doppeldekleration
	if(lookup_in_actsym(idname) != NULL) {
		error(34);
	}

	lookahead = nextsymbol();
	// Überprüfung des Variablen-Typs
	switch(lookahead) {
		case INT:
			insert(INTIDENT);
			break;
		case REAL:
			insert(REALIDENT);
			break;
		default:
			error(36);
			break;
	}

	lookahead = nextsymbol();
	// mehrere var-Deklerationen können folgen
	switch(lookahead) {
		case KOMMA:
			lookahead = nextsymbol();
			vardecl();
			break;
		case SEMICOLON:
			lookahead = nextsymbol();
			return;
		default:
			error(5);
	}
	return;	// end vardecl
}

/****************** constdecl ***************************************************/
/* analysiert wird der korrekte Aufbau einer Variablen(Konstanten-)deklaration
nach folgender Syntax:

			CONSTDECL 	::=	 const IDENT '=' INTNUMBER {',' IDENT '=' INTNUMBER } * ';'



Schnittstelle:
	bei Aufruf :			erkannt wurde das Schlüsselwort const
							nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void constdecl() {
	st_entry *neu, *found;

	if (tracesw)
		trace<<"\n Zeile:"<< lineno<<"Konstantendeklaration:" << endl;

	// auf const muss IDENT folgen
	// TODO
	if(lookahead != ID) {
		error(4);
	}

	lookahead = nextsymbol();
	if(lookahead != EQ) {
		error(3);
	}

	lookahead = nextsymbol();
	if(lookahead != INTNUM) {
		error(2);
	}

	if(lookup_in_actsym(idname) != NULL) {
		error(34);
	}

	insert(KONST);

	// mehrere const-Deklerationen können folgen
	lookahead = nextsymbol();
	switch(lookahead) {
		case KOMMA:
			lookahead = nextsymbol();
			constdecl();
			break;
		case SEMICOLON:
			lookahead = nextsymbol();
			return;
		default:
			error(5);
	}
	return;		// end constdecl
}




/****************** block ***************************************************/

/* analysiert wird der korrekte Aufbau eines Blockes nach folgender Syntax:


		BLOCK		::= 	[ CONSTDECL ]
							[ VARDECL ]
							  PROCDECL
							  STATEMENT



Der Parameter neusym ist ein Zeiger auf die Symboltabelle des zu
analysierenden Blockes
===>
		Äussersten (globalen)  Block:		firstsym
		bei Prozeduren:		Zeiger auf neu angelegte ST für Prozedur

Zu Beginn muss der globale Zeiger actsym auf die neue ST gesetzt werden
Vor Verlassen muss actsym wieder auf den vorigen Wert gesetzt werden


Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead


*/
/* symtable * neusym :	Zeiger auf neue ST */
void block(symtable * neusym) {
	if (tracesw)
		trace<<"\n Zeile:"<< lineno<<"Block" << endl;


	// actsym auf neue Symboltabelle setzen
	actsym = neusym;

	// TODO
	if( lookahead == CONST ) {
		lookahead = nextsymbol();
		constdecl();
	}

	if( lookahead == VAR ) {
		lookahead = nextsymbol();
		vardecl();
	}

	if( lookahead == PROCEDURE ) {
		lookahead = nextsymbol();
              	procdecl();
                
	}

	statement();
        lookahead = nextsymbol();
	// bei Blockende : Symboltabelle zurücksetzen
	// actsym = Zeiger auf vorherige Symboltabelle
	actsym = neusym->precsym;
	// TODO
	return;
}





/****************** program    ***************************************************/
/* analysiert wird der korrekte Aufbau eines Programmes nach folgender Syntax:

			PROGRAM		::=		BLOCK	'$'

Schnittstelle:
	bei Aufruf :			nächstes Eingabesymbol befindet sich in lookahead
	bei korrektem Ende:		nächstes Eingabesymbol befindet sich in lookahead

*/
void program() {
  if (tracesw)
	  trace <<"\n Zeile:"<< lineno<<"Programm" << endl;

	// globale Symboltabelle  anlegen (firstsym
	firstsym = create_newsym();

	// erstes Symbol lesen
	lookahead=nextsymbol();

	// Block muss folgen
	block (firstsym);
	//  nach Block muss '$' folgen
	if (lookahead == PROGEND)
		// nächstes Symbol lesen
		lookahead=nextsymbol();
	else
		 // korrektes Programmende fehlt
		 error(31);

	// Dateiende erreicht ?
	if (lookahead != DONE)
		error (33); // noch Symbole in Eingabedatei nach RPOGRAM
}	// end program
