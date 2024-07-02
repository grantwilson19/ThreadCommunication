


#include <iostream>
#include <pthread.h>
#include <string.h>
#include "readlines.h"
using namespace std;

//thread function to read lines from the testfile into MUTEX queue to be processed by countvocabstrings
void* readlines(void* args){
	//cast data pointer
	SHARED_DATA* data = static_cast<SHARED_DATA*>(args);	
	ifstream textstream;//stream for the text from testfile	
	
	//OPEN testfile text
	textstream.open(static_cast<string>(data->fileName[TESTFILEINDEX]));

	//validate file	
	if(!textstream){
        	cout << "Unable to open <<testfile.txt>>" << endl;
        	exit(NORMAL_EXIT);
	}


	string line;//line to read text

	//iterate every line in testfile
	while(getline(textstream,line)){

		//lock shared data linequeue to avoid concurrent access
		pthread_mutex_lock(&(data->queue_mutex));//proper syntax??
		
		data->lineQueue->push(line);//push string onto queue
		data->lineCountInFile[TESTFILEINDEX]=data->lineCountInFile[TESTFILEINDEX]+1;//increment line count
		
		pthread_mutex_unlock(&(data->queue_mutex));//release lock
	}
	//close the textstream
	textstream.close();

	data->taskCompleted[TESTFILEINDEX]=true;
	return nullptr;
}//end readlines
