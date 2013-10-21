COMPIL_OPTIONS = -g -Wall -ansi -pedantic
LINK_OPTIONS = -lm -lSDL -lGLU -lglut -lGL
COMPILER = gcc


# Liste des fichiers objets .o
OBJECTS = exempleGlut.o

#Règle pour compiler l'exécutable

executable : $(OBJECTS)
	$(COMPILER) $(OBJECTS) ${LINK_OPTIONS} -o $@ 

#Règle pour compiler tous les .cpp en .o

.cpp.o:
	$(COMPILER) ${COMPIL_OPTIONS} -c $< -o $@

#Règle pour créer automatiquement le fichier des dépendances
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
