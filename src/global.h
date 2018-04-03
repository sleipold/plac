/**************** global.h  - SS 2018    ************/

#ifndef GLOBAL_H
#define GLOBAL_H
#endif

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string.h>
using namespace std;

#define TRUE			1
#define FALSE			0
#define BSIZE			512			/* Puffergrösse */
#define NONE		 	-1		/* Vorbesetzung für num */
#define EOS			 	'\0'		/* End of String */

#define NESTMAX		10			/* Max. Schachtelungstiefe von Blöckeen */
#define STRMAX		999			/* Länge des Stringfeldes */
#define SYMMAX		20			/* Größe der Teil-Symboltabelle */


/* Definition der Codierung für Tokentypen */


#define INTNUM	 	2561			/* Int-Konstante */
#define REALNUM	 	2562			/* Real-Konstante */


#define ID  			257			/* Identifikator */
#define CONST 		258			/* Schlüsselwort const */
#define VAR  			259			/* Schlüsselwort var */
#define PROCEDURE	260			/* Procedure */
#define CALL 			261			/* call */
#define BEGIN 		262			/* begin */
#define END   		263			/* end */
#define IF  			264			/* if */
#define THEN 			2651		/* then */
#define ELSE			2652		/* else */
#define WHILE 		266			/* while */
#define DO 				267			/* do */
#define EQ 				268			/* = */
#define NE				269			/* != */
#define LT				270			/* <  */
#define	LE				271			/* <= */
#define	GT				272			/* >  */
#define	GE				273			/* >= */
#define ASS				274			/* := */
#define KOMMA	   	275			/* ,  */
#define SEMICOLON	276			/* ;  */
#define PLUS			277			/* +  */
#define MINUS			278			/* -  */
#define MULT			279			/* *  */
#define DIV				280			/* /  */
#define KLAUF			281			/* (  */
#define KLZU			282			/* )  */
#define PROGEND		283			/*  $ */
#define COLON			284			/*  :  */
#define INT				285			/* int */
#define REAL			286			/* real */
#define FI 				291			/* fi */


#define DONE			300			/* Ende-Token */

/* Definition der Art der Symboltabellen-Einträge  */

#define KONST 			310			/* Konstanten-Eintrag */
#define INTIDENT  	320			/* Identifikator vom Typ int  */
#define REALIDENT  	330			/* Identifikator vom Typ real */
#define PROC				400			/* Procedure */


/****************Typ und extern-Deklarationen***********************/

/* Symboltabelle  */
struct symtable ;

/* Aufbau eines Symboltabellen-Eintrags */
struct st_entry {
	int token ;					/* Art des Eintrags (KONST/INTIDENT/REALIDENT/PROC)  */
 	char *name;					/* Zeiger auf Namen */
 	int wertaddr;				/* Wert bei Konstanten (KONST);
									   		 relative Speicheradresse (offset) bei Variablen */
 	symtable * subsym;  /* Zeiger auf Teil-Symboltabelle bei Art PROC */
};

/* Aufbau der (Teil- )Symboltabelle */

struct symtable {
	symtable * precsym;				/* Zeiger auf Übergeordnete Symboltabelle; bei oberster NULL */
	int level;								/* Schachtelungstiefe  */
	int anzahl;								/* Anzahl der Symboltabelleneinträge */
	st_entry eintrag[SYMMAX];	/* Feld für Einträge */
};

extern ifstream fin;
extern ostream fout, ferr, fsym, trace;

extern int level;

extern char idname[];				/* Zeiger auf Namen bei Identifikator */
extern int num;							/* Wert einer integer-Konstanten/ Zahl */
extern double realnum; 			/* Wert einer Real-Konstanten */
extern int lineno;					/* Zeilennummer */
extern symtable *actsym,		/* Zeiger auf aktuelle Symboltabelle */
		*firstsym;							/* Zeiger auf oberste (globale) Symboltabelle */

extern int tracesw;					/* Kennung, ob Trace gewünscht */


/******************** Prototypen für Prozeduren *******************/

void initialize(int argc, char** argv);					/* Compiler initialisieren */
void stop(); 																		/* Beenden */

void initlexan();																/* Scanner initialisieren */
int lookforres( char * );	/* sucht in Tabelle der res. Symbole nach Zeichenkette */
int nextsymbol(); 				/* liest nächstes Symbol der Eingabe */



symtable * create_newsym() ;		/* Neue ST erzeugen */
st_entry * lookup( char *s);		/* Namen in ganzer Symboltabelle suchen */
st_entry * lookup_in_actsym ( char *s);	/* Namen in aktueller Symboltabelle suchen */
st_entry * insert(int);   			/* Neuen Eintrag in actsym erzeugen */




void constdecl();					/* Verarbeiten einer Konstantendeklaration */
void vardecl(); 					/* Verarbeiten einer VAriablendeklaration */
void procdecl(); 					/* Verarbeiten einer Prozedurdeklaration */
int factor(); 						/* Verarbeiten eines Faktors */
int term();								/* Verarbeiten eines Terms */
int exp(); 								/* Verarbeiten eines Ausdrucks */
int condition(); 					/* Verarbeiten einer Bedingung */
void statement();					/* Verarbeiten Statement */
void program(); 					/* Programm übersetzen */
void block( symtable * neusym);		/* Bearbeiten eines Blockes */



void error(int);								/* Fehlerausgabe */
void errortext(string);				/* Fehlerausgabe */
void warningtext(string);			/* Warnung ausgeben */
// void generate(int);					/* Codeerzeugung */
void printsymtab(symtable *);		/* Ausgabe der Symboltabelle */
