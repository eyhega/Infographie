COMPIL_OPTIONS = -g -Wall -ansi -pedantic
LINK_OPTIONS = -lm -lGLU -lglut -lGL
COMPILER = gcc


# Liste des fichiers objets .o
OBJECTS = exempleGlut.o

#R�gle pour compiler l'ex�cutable

executable : $(OBJECTS)
	$(COMPILER) $(OBJECTS) ${LINK_OPTIONS} -o $@ 

#R�gle pour compiler tous les .cpp en .o

.cpp.o:
	$(COMPILER) ${COMPIL_OPTIONS} -c $< -o $@

#R�gle pour cr�er automatiquement le fichier des d�pendances
dep:
	$(COMPILER) -MM *.cpp > makefile.dep

#On efface sur demande les objets pour recommencer proprement 
clean :
	rm -f executable
	rm -f $(OBJECTS) core

#include makefile.dep

#usage : 1) touch makefile.dep
#	 2) make dep
#	 3) make
