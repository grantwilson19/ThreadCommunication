


#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
#include "countvocabstrings.h"
using namespace std;



//this thread processes lines from a MUTEX queue. inserts into trie and searches recursively for mathcing strings in the vocab file
void* countvocabstrings(void* args){
	//cast shared data ptr
	SHARED_DATA* data = static_cast<SHARED_DATA*>(args);

	while(!(data->taskCompleted[VOCABFILEINDEX])){
	//empty loop to busy wait for readvocab thread to finish
	}

	//need to declare outputstream for writing out to file!!!!
	ofstream output;
	output.open("countNumOfContainedVocab.txt");
	
	//verify output file
	if(!output.is_open()){
		cout << "ERROR: COULD NOT OPEN OUTPUT FILE countNumOfContainedVocab.txt" << endl;
		exit(NORMAL_EXIT);
	}


	//string to process lines from queue
	string line = "";
	
	//count matching vocab
	int count = 0;


	//only loop while lines are in queue
	while( (data->lineQueue->size() != 0) || ((data->lineQueue->size() == 0) && !(data->taskCompleted[TESTFILEINDEX]) )){

		//create tree	
        	Trie* tree = new Trie();
	
		//lock mutex
		pthread_mutex_lock(&(data->queue_mutex));
		
		//save and pop front element from linequue
		line = data->lineQueue->front();
		data->lineQueue->pop();
		
		//release lock
		pthread_mutex_unlock(&(data->queue_mutex));
		
		//reset counter for match
		count = 0;	

		tree->insert(tree->root,line.c_str());//insert line as a const char*

		 //iterate through dictionary to search each suffix for a match
                for(string suffix:*(data->vocabVect)){

                        //if search returns true for the suffix.. add to the counter
                        //uses c_str() to get the suffix string as a const char*
                        if(tree->search(tree->root,suffix.c_str()) == true){
                                count++;
                        }
                }
		
		if(count >= data->minNumOfVocabStringsContainedForPrinting){
			//write to output file if count is valid
			output << count << endl;
		}

		//increment processed lines counter
		data->numOfProcessedLines = data->numOfProcessedLines+1;
		
		//delete tree  and deallocate
		delete tree;
			
		
	}//end while
	//CLOSE OUTPUTSTREAM AFTER WHILE LOOP
	output.close();
	return nullptr;
}//end countvocabstrings

