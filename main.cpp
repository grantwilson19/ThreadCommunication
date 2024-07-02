/* 
 *CS480 ASSIGNMENT 2
 *written by James Marsh and Grant Wilson
 *Created on 9 - 20 - 2023 at 4:51 pm
 *Updated: 9 - 28 - 2023 at 2:00 AM
 NEW 
 WORKING!!!!!
 */

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "Trie.h"
#include <pthread.h>
#include <stdio.h>
#include "readvocab.h"
#include "readlines.h"
#include "countvocabstrings.h"

using namespace std;




/*
   PROGRESS BAR FOR COUNT VOCAB METHOD
   DISPLAYS LIVE PROGRESS.. CATCHES UP IF IT FALLS BEHIND THE THREAD
 */
void* countvocabProgressBar(void* args){
	SHARED_DATA* data = static_cast<SHARED_DATA*>(args);
	double progress = 0.0;	//how much the therad has done
	double tempProgress = 0.0;//measures progress relative to internal progress
	double internalProgress=0.0;//how much has been printed
	int numOfHashes = (data->numOfProgressMarks)/(data->hashmarkInterval);// p/m
	double placeHash = ((double)data->hashmarkInterval)/((double)data->numOfProgressMarks);//percentage of when a hashmark should be placed
	double placeDash = placeHash/((double)data->hashmarkInterval);//percentage of when to place a dash-progress mark ....unless it equals placeHash
	int markerCount = 0;
	int hashCount = 1;

	while(progress<1.0){
		progress =((double)data->numOfProcessedLines)/(((double)data->lineCountInFile[TESTFILEINDEX]));
		tempProgress = progress-internalProgress;//Temp is How Much Progress Made
		while(tempProgress>0){ //While we have Made any progress
			if(tempProgress>=placeDash){ //If Progress Made is more than the amount to place a Dash

				//check if next marker to place is Hashmark
				if ((markerCount+1)/hashCount == data->hashmarkInterval) {
					hashCount++;
					printf("#");
					cout.flush();
					internalProgress = internalProgress+placeDash;//increase internal progress for a mark
					markerCount++;
					tempProgress = tempProgress-placeDash;//decrease the relative progress
					continue;
				}

				//enough progress for a dash, but isn't hashmark interval
				printf("-");
				markerCount++;
				cout.flush();
				internalProgress = internalProgress+placeDash;
				tempProgress = tempProgress-placeDash;
				continue;
			}
			break;//needs more progress in order to place next marker
		}
	}
	//prints the last char and accounts for odd numbers
	if(markerCount!=data->numOfProgressMarks && data->numOfProgressMarks%data->hashmarkInterval!=0){
		printf("-");
		markerCount++;
		cout.flush();
	}
	else if(hashCount-1 != numOfHashes){
		printf("#");
		cout.flush();
	}
	printf("\nThere are %d lines in testfile.txt\n",data->lineCountInFile[TESTFILEINDEX]);
	return nullptr;
}



//PROGRESS BAR for readvocab thread...nearly identical to the countvocabstrings progress bar
void* readvocabProgressBar(void* args){
	SHARED_DATA* data = static_cast<SHARED_DATA*>(args);
	double progress = 0.0;
	double tempProgress = 0.0;
	double internalProgress=0.0;
	int numOfHashes = (data->numOfProgressMarks)/(data->hashmarkInterval);// p/m
	double placeHash = ((double)data->hashmarkInterval)/((double)data->numOfProgressMarks);//percentage of when a hashmark should be placed
	double placeDash = placeHash/((double)data->hashmarkInterval);//percentage of when to place a dash-progress mark...unless it equals placeHash
	int markerCount = 0;
	int hashCount = 1;

	while(progress<1.0){
		progress =((double)data->numOfCharsReadFromVocabFile)/((double)data->totalNumOfCharsInVocabFile);
		tempProgress = progress-internalProgress;//Temp is How Much Progress Made
		while(tempProgress>0){ //While we have Made any progress
			if(tempProgress>=placeDash){ //If Progress Made is more than the amount to place a Dash
				if ((markerCount+1)/hashCount == data->hashmarkInterval) {	
					hashCount++;
					printf("#");
					cout.flush();
					internalProgress = internalProgress+placeDash;
					markerCount++;
					tempProgress = tempProgress-placeDash;
					continue;
				}
				printf("-");
				markerCount++;
				cout.flush();
				internalProgress = internalProgress+placeDash;
				tempProgress = tempProgress-placeDash;
				continue;
			}
			break;
		}
	}
	//prints the last char and accounts for odd numbers
	if(markerCount!=data->numOfProgressMarks && data->numOfProgressMarks%data->hashmarkInterval!=0){
		printf("-");
		markerCount++;
		cout.flush();
	}
	else if(hashCount-1 != numOfHashes){
		printf("#");
		cout.flush();
	}
	printf("\nThere are %d lines in vocabulary.txt\n",data->lineCountInFile[VOCABFILEINDEX]);
	return nullptr;
}



//main thread to handle error checking, output file, and spawn other threads.
int main(int argc, char* argv[]){

	if(argc<3){
		printf("invalid arguments\n");
		exit(NORMAL_EXIT);
	}

	//Struct null and 0 initializer for all fields
	SHARED_DATA* data = new SHARED_DATA;

	data->numOfProgressMarks = DEFAULT_NUMOF_MARKS; //OR SET TO 0?
	data->hashmarkInterval = DEFAULT_HASHMARKINTERVAL;//^
	data->minNumOfVocabStringsContainedForPrinting = DEFAULT_MINNUM_OFVOCABSTRINGS_CONTAINED;//^
	for(int i = 0; i<NUMOFFILES; i++){
		data->fileName[i] = nullptr;
	}

	data->totalNumOfCharsInVocabFile = 0;//default before read
	data->numOfCharsReadFromVocabFile = 0;


	for(int i = 0; i<NUMOFFILES; i++){
		data->lineCountInFile[i] = 0;//default before read
	}

	//vector and queue initialized by default
	data->vocabVect = new vector<string>;
	data->lineQueue = new queue<string>;

	//do this in main
	//pthread_mutex_init(&(data->queue_mutex), nullptr);


	for(int i = 0; i<NUMOFFILES; i++){
		data->taskCompleted[i] = 0;
	}
	//return data pointer

	//integer for optional args.
	int option = 0;
	//value of arg
	int val = 0;
	//handle optional arguments 
	while((option = getopt(argc,argv,"p:m:v:")) != -1){
		switch(option)
		{
			case 'p':	//set num of progress marks..check range
				val = atoi(optarg);
				if(val<10){
					printf("Number of progress marks must be a number and at least 10\0");
					exit(NORMAL_EXIT);
				}

				data->numOfProgressMarks=val;	
				break;
			case 'm':	//set hash interval... check range	
				val = atoi(optarg);
				if(val<=0 || val>10){
					printf("Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10\0");		
					exit(NORMAL_EXIT);
				}

				data->hashmarkInterval=val;
				break;

			case 'v':	//ensure positive value	
				val = atoi(optarg);
				if(val>=0){
					data->minNumOfVocabStringsContainedForPrinting=val;
				}
				break;	

			default:
				printf("invalid argument to conuntvocabstrings\0");
				exit(NORMAL_EXIT);
		}
	}

	//file argument check handled in threads...set filenames here
	int idx = optind;
	if(idx<argc){
		data->fileName[VOCABFILEINDEX] = argv[idx];//path to first argument	
	}
	idx++;
	if(idx<argc){
		data->fileName[TESTFILEINDEX] = argv[idx];//path to second argument

	}
	//by convention, ignores any other arguments



	//create mutex +  threads
	pthread_t t1, t2, t3;
	pthread_mutex_init(&(data->queue_mutex), nullptr);

	if(pthread_create(&t1, nullptr, readvocab,data) != 0) {
		printf("COULD NOT CREATE readvocab() thread");
		exit(NORMAL_EXIT);
	}

	if(pthread_create(&t2, nullptr, readlines,data) != 0) {
		printf("COULD NOT CREATE readlines() thread");
		exit(NORMAL_EXIT);
	}

	if(pthread_create(&t3, nullptr,countvocabstrings,data) != 0) {
		printf("COULD NOT CREATE countvocabstrings() thread");
		exit(NORMAL_EXIT);
	}


	while(data->lineCountInFile[TESTFILEINDEX] == 0 && data->lineCountInFile[VOCABFILEINDEX]==0){
		//wait on data to populate
	}

	//display progress for threads
	readvocabProgressBar(data);
	countvocabProgressBar(data);

	//thread joins
	//	pthread_join(t1,nullptr);
	pthread_join(t2,nullptr);
	pthread_join(t3,nullptr);

	//destory mutex (DO THIS BEFORE LOSING THE SHARED DATA POINTER)
	pthread_mutex_destroy(&(data->queue_mutex));

	exit(NORMAL_EXIT);
	return 0;
}
