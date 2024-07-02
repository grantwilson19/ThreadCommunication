# CXX Make variable for compiler
CC=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb 
#hide debug flag to improve performance CCFLAGS=-std=c++11 -Wall -g3 -c
CCFLAGS=-std=c++11 -O0 -Wall -c

# object files
OBJS = Trie.o readvocab.o readlines.o countvocabstrings.o main.o 

# Program name
PROGRAM = countvocabstrings

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -lpthread -o $(PROGRAM) $(OBJS)

main.o : main.cpp
	$(CC) $(CCFLAGS) main.cpp
	
readvocab.o : readvocab.cpp readvocab.h
	$(CC) $(CCFLAGS) readvocab.cpp

readlines.o: readlines.cpp readlines.h
	$(CC) $(CCFLAGS) readlines.cpp
    
countvocabstrings.o: countvocabstrings.cpp countvocabstrings.h
	$(CC) $(CCFLAGS) countvocabstrings.cpp

Trie.o : Trie.cpp Trie.h
	$(CC) $(CCFLAGS) Trie.cpp
# Once things work, people frequently delete their object files.
# If you use "make clean", this will do it for you.
# As we use gnuemacs which leaves auto save files termintating
# with ~, we will delete those as well.
clean :
	rm -f *.o *~ $(PROGRAM)


