#ifndef	READVOCAB_H
#define READVOCAB_H


#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "Trie.h"





/*
readvocab thread to populate shared_data vocabVect with words to search against testfile
*/
void* readvocab(void* args);










#endif // READVOCAB_H
