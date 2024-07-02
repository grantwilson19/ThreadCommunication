


#include "Trie.h"
#include <iostream>
#include <pthread.h>
#include <string.h>
using namespace std;


//readlines thread function: reads in lines from a test file to shared_data struct given as void*
void* readlines(void* data);


