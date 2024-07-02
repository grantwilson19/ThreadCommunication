#ifndef COUNTVOCABSTRINGS_H
#define COUNTVOCABSTRINGS_H



#include <fstream>
#include <iostream>
#include "Trie.h"
#include <string.h>
using namespace std;

//thread function to process lines from shared_data line queue, and search for matches from vocabVect in shared datausing the Trie class. Also writes to output file for match counts.
void* countvocabstrings(void* args);

#endif // COUNTVOCABSTRINGS_H
