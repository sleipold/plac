# Makefile zur Uebersetzung des Parsers bzw. Scanners aus Aufgabe 1
# J. Uhrmann, 09.11.2005

# Verwendung fuer Aufgabe 1:
#
#	make lexmain
#	make parsemain

# Hinweis zu den verwendeten "automatic variables":
# 	$@ kennzeichnet immer die zu erstellende Datei
#	$^ kennzeichnet immer die Liste der dazu benoetigten Dateien (ohne Duplikate)
#	$< kennzeichnet die _erste_ der benoetigten Dateien

# Hinweise zu den verwendeten Funktionen
#	$(wildcard muster)
#		sucht nach Dateien, auf die das Muster passt und gibt eine Liste
#		der Dateinamen zurueck. * und ? werden wie in der Shell verwendet
#
#	$(patsubst von, nach, eingabe)
#		ersetzt jedes Vorkommen von "von" in "eingabe" durch "nach"
#		Hier wird % als Wildcard verwendet.

# Variablendeklarationen
CFLAGS=-Wall -g
LDFLAGS=

# Regeln, die _keine_ Dateien mit dem Namen der Regel erstellen, sind "phony"
.PHONY: clean

# Uebersetzung des Scanners aus Aufgabe 1
lexmain: lexmain.o init.o error.o lexan.o symbol.o parser.o
	cd build && g++ $(LDFLAGS) -o ../$@ $^

# Uebersetzung des Parsers aus Aufgabe 1
parsemain: parsemain.o init.o error.o lexan.o symbol.o parser.o
	cd build && g++ $(LDFLAGS) -o ../$@ $^


# allgemeine Regel zur Erstellung eines Object-Files aus einer
# C++ Quelldatei
%.o: %.cxx
	g++ $(CFLAGS) -c -o build/$@ $<


# da C++-Dateien Header-Dateien inkludieren, sollten deren Abhaengigkeiten
# bekannt gemacht werden. Dies geschieht in den Dateien mit der Endung .d
# GCC kann diese automatisch mit dem Schalter -M aus Quelltexten erstellen.
%.d: %.cxx
	@mkdir -p build
	g++ -M $^ > build/$@

# hier werden sie inkludiert
# Falls sie nicht existieren, wird make das zwar melden, diese Dateien
# jedoch sofort mit der obigen Regel erzeugen und einbinden.
include  $(patsubst %.cxx,%.d,$(wildcard *.cxx))


# Aufraeumen
clean:
	-rm $(patsubst %.cxx,%.d,$(wildcard *.cxx))
	-rm $(patsubst %.cxx,%.o,$(wildcard *.cxx))
