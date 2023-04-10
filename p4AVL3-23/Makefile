CC=g++
DEV=-Wall -g -std=c++14
OPT=-O3 -std=c++14

.PHONY: all
all: AVL.o AVLCommands BSTSanityCheck AVLSanityCheck CreateData BST.o 

CreateData: CreateData.cxx json.hpp
	$(CC) $(OPT) CreateData.cxx -o CreateData.exe

BSTSanityCheck: BSTSanityCheck.cxx BST.o
	$(CC) $(DEV) BSTSanityCheck.cxx BST.o -o BSTSanityCheck.exe

AVLSanityCheck: AVLSanityCheck.cxx AVL.o
	$(CC) $(DEV) AVLSanityCheck.cxx AVL.o -o AVLSanityCheck.exe

BST.o: BST.cpp BST.h
	$(CC) $(DEV) -c BST.cpp

AVL.o: AVL.cpp AVL.h
	$(CC) $(DEV) -c AVL.cpp

AVLCommands: AVLCommands.cxx json.hpp AVL.o
	$(CC) $(OPT) AVLCommands.cxx AVL.o -o AVLCommands.exe

# Build
.PHONY: clean
clean:
	rm -f *.o
	rm -f *.exe
	rm -rf *dSYM

.PHONY: update
update:
	make clean
	make all
