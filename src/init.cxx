/******************************** init.cxx ************************/



/* Initialisierungen für Compiler PLA

		Stand Sommersemester 2018


*/


#ifndef GLOBAL_H
#include "global.h"
#endif

ifstream fin;					/* Zu Übersetzende Eingaben */
ostream fout(nullptr),					/* Ausgabedatei */
	 ferr(nullptr),					/* Fehlerausgaben */
	 fsym(nullptr),					/* Ausgabe für Symboltabelle */
	 trace(nullptr);					/* Ausgabe für Trace */

int tracesw;
int level=0;

#define MAX_SWITCH_LENGTH 10

/***************** Initialisierungen durchführen ******************/

/**
* Print help to stdout and stop program.
* @param program Program that was executed. (lexmain/parsemain)
*/
void help( const string program ) {
	cout << "Usage: " << program << " [-O] [-t trace] [-e errfile] [-s symtable] <input> <output>\n";
	cout << "Help: \n";
	cout << "\t-O --old: old modus\n";
	cout << "\t-t --trace: output trace to file\n";
	cout << "\t-e --error: output error to file if not to stderr\n";
	cout << "\t-s --sym: output syboltable to file\n";
	cout << "\tinput: input file\n";
	cout << "\toutput: output file\n";
	exit(0);
}

/**
* Searching for the first index of argument.
* @param argc count of arguments
* @param argv array of arguments
* @param small small version of the argument switch
* @param big big version of the argument switch
* @return index of argument or -1
*/
int indexOfArg( int argc, char** argv, const string small, const string big ) {
	for (int i = 0; i < argc; i++ ) {
		if ( strncmp(argv[i], small.c_str(), MAX_SWITCH_LENGTH) == 0 || strncmp(argv[i], big.c_str(), MAX_SWITCH_LENGTH) == 0 ) {
			return i;
		}
	}
	return -1;
}

/**
* Look if index is in array range. If not calls help
* @param argc count of arguments
* @param argv arguments of the program
* @param index index to check
* @return argument
*/
char* safeArg( int argc, char** argv, int index ) {
	if ( index >= argc ) {
		help( argv[0] );
		return nullptr;
	}
	return argv[index];
}

/**
* Opens File for write access and returns buffer
* @param file filename to open
* @return streambuf of the filestream
*/
std::streambuf* openWriteFile( string file ) {
	ofstream* filestream = new ofstream();
	filestream->open( file, ios::out );
	return filestream->rdbuf();
}

/**
 * Initialize program with switches
 */
void newInitalize( int argc, char** argv ) {
	if ( argc < 3 ) {
		help( argv[0] );
		return;
	}
	int usedArgs = 1;

	int tracefile = indexOfArg( argc, argv, "-t", "--trace");
	if ( tracefile == -1 ) {
		tracesw = FALSE;
	} else {
		usedArgs++;
		trace.rdbuf(openWriteFile( safeArg( argc, argv, tracefile + 1) ));
		tracesw = TRUE;
	}
	int errorfile = indexOfArg( argc, argv, "-e", "--error");
	if ( errorfile == -1 ) {
		ferr.rdbuf(cerr.rdbuf());
	} else {
		usedArgs++;
		ferr.rdbuf(openWriteFile( safeArg( argc, argv, errorfile + 1) ));
	}
	int symtable = indexOfArg( argc, argv, "-s", "--sym");
	if ( symtable == -1 ) {
		fsym.rdbuf(cout.rdbuf());
	} else {
		usedArgs++;
		fsym.rdbuf(openWriteFile( safeArg( argc, argv, symtable + 1) ));
	}

	if ( (argc - usedArgs) < 2 ) {
		help( argv[0] );
		return;
	}

	char* inputfile = argv[argc - 2];
	char* outputfile = argv[argc - 1];

	if ( access( inputfile, F_OK ) == -1 ) {
		cerr << "Input file doesn't exist\n";
		exit(1);
	}

	fin.open(inputfile, ifstream::in);

	if ( fin.fail() ) {
		ferr << "opening file: " << inputfile << " failed" << endl;
		exit(1);
	}

	fout.rdbuf(openWriteFile( outputfile ));

	initlexan();
}

void initialize( int argc, char** argv ) {
	if ( argc == 1 ) {
		help( argv[0] );
		return;
	}

	if ( indexOfArg( argc, argv, "-h", "--help" ) != -1 ) {
		help( argv[0] );
		return;
	}

	if ( indexOfArg( argc, argv, "-O", "--old" ) == -1 ) {
		newInitalize( argc, argv );
		return;
	}

  char c;
  char filename[20];

  cout << "\n Name der Eingabedatei eingeben-->";
  cin >> filename;

  /* Eingabefile öffnen*/
	fin.open(filename, ios::in);

 	cout << "\n Name der Ausgabedatei eingeben-->";
 	cin >> filename;

  /* Ausgabedatei öffnen */
	fout.rdbuf(openWriteFile( filename ));

  fout << "\n\n***************** Ausgabe ***************\n";

 	cout << "\n Name der Fehlerdatei eingeben-->";
 	cin >> filename;

  /* Fehlerdatei öffnen */
	ferr.rdbuf(openWriteFile( filename ));

  ferr<<"\n\n**************** Fehlermeldungen*************\n";

  cout << "\n Trace gewünscht ? (y/n): ";
  cin >> c;
  if ( c == 'y') {
		/* Datei für Trace  öffnen */
		trace.rdbuf(openWriteFile( "trace.out" ));
    tracesw = TRUE;
  } else {
		tracesw = FALSE;
	}

	/* Datei für Symboltabellenausgabe öffnen */
	fsym.rdbuf(openWriteFile( "symtable.out" ));
  fsym<<"\n\n**************** Symboltabellen*************\n";

 	/* Initialisieren des Scanners */
  initlexan();
 }

/************ Endebehandlung bei fehlerfreier Übersetzung  *****/
/* zur Zeit nur Ausgabe der Symboltabelle */


/* Symboltabellen ausgeben  */
void stop() {
  printsymtab(firstsym);
}
