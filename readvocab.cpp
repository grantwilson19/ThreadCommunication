#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include "readvocab.h"
using namespace std;

/*
   readvocab.cpp implementation of readvocab.h

this thread reads in vocab lines from vocabulary.txt and stores them in a vector in the shared data for access via other threads
 */
void* readvocab(void* args){


	SHARED_DATA* data = static_cast<SHARED_DATA*>(args);


		ifstream dictstream;//stream for vocabulary file

		//open vocabulary dictionary file
		dictstream.open(static_cast<string>(data->fileName[VOCABFILEINDEX]));

		//verify file exists and open
		if(!dictstream){
			cout << "Unable to open <<vocabulary.txt>>" << endl;
			exit(NORMAL_EXIT);
		}
		
		//get byte count from file
		struct stat st;
		stat(data->fileName[VOCABFILEINDEX],&st);
		off_t size = st.st_size;
		if((unsigned long)size == 0){
			printf("vocabulary.txt is empty\0");
			exit(NORMAL_EXIT);
		}
		data->totalNumOfCharsInVocabFile=(unsigned long)size;
		
		//variable to store dictionary word
		string line;

		//read lines from vocab file
		while(getline(dictstream,line)){

			//update linecount
			data->lineCountInFile[VOCABFILEINDEX]++;

			//add lines to shared data
			data->vocabVect->push_back(line);

			//increment charCounter with the length of each line
			data->numOfCharsReadFromVocabFile+=(1+line.size());
		}

		//close vocab  file stream
		dictstream.close();

		//thread complete
		data->taskCompleted[VOCABFILEINDEX]=true;
		
		return nullptr;
}//end readvocab thread function












