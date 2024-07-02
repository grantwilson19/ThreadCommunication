/* Author: James Marsh
 * REDID: 826209118
 * CS480 Operating Systems
 * PA1 trie tree
 */


#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;
#define NULLTERMINATORINDEX 26
#define SPACEINDEX 27
#define APOSTROPHEINDEX 28
#define HYPHENINDEX 29
#define UNDERSCOREINDEX 30
#define NCHILD 31

#define NORMAL_EXIT 0


/**
 * Shared constants and data structures among threads
 **/
#define NUMOFFILES 2
#define VOCABFILEINDEX 0
#define TESTFILEINDEX 1
/* default number of progress marks for representing 100% progress */
#define DEFAULT_NUMOF_MARKS 50
/* minimum number of progress marks for representing 100% progress */
#define MIN_NUMOF_MARKS 10
/* place hash marks in the progress bar every N marks */
#define DEFAULT_HASHMARKINTERVAL 10
/* default minimum number of vocab strings contained in a request string for printing or writing
   to the output */
#define DEFAULT_MINNUM_OFVOCABSTRINGS_CONTAINED 0





/* Common data shared between threads */
typedef struct {
	/**
	 * parameters for printing progress bar
	 */
	unsigned int numOfProgressMarks;
	unsigned int hashmarkInterval;



	/**
	 * print the string count to the output file only if the
	 * number of vocab strings contained in the line
	 * is equal to or greater than this number
	 */
	unsigned int minNumOfVocabStringsContainedForPrinting;



	/**
	 * fileName[0] - file path for the vocabulary file
	 * fileName[1] - file path for the test file providing lines for testing
	 */
	const char *fileName [NUMOFFILES];



	/** store total number of characters in files
	 * totalNumOfCharsInVocabFile
	 * - number of total chars in the dictionary vocabulary file.
	 * use stat, lstat, or fstat system call
	 *
	 * Hints: see man stat, lstat, or fstat to get the total number of bytes of the file
	 * stat or lstat uses a filename, whereas fstat requires a file descriptor from a
	 * low-level I/O call: e.g. open. If you are using high-level I/O, either use stat
	 * (or lstat), or open the file first with the low-level I/O, then call fstat,
	 * then close it.)
	 *
	 * Important: assume all text files are in Unix text file format, meaning, the end
	 * of each line only has a line feed character. The stat, lstat, fstat would include the
	 * count of the line feed character from each line.
	 */
	unsigned long totalNumOfCharsInVocabFile;


	/**
	 * Use numOfCharsReadFromVocabFile to track ongoing progress of
	 * number of characters read in from vocab file.
	 *
	 * This progress indicator is updated by readvocab worker thread, and used by the main
	 * thread to display the progress for tracking the execution of the readvocab thread
	 *
	 * Important: this number can be incremented by the number of characters in
	 * the line that is being read and processed, plus one to include the
	 * line feed character at the end of each line of a Unix text file.
	 * Again, do NOT convert the text files to a Windows DOS format.
	 */
	long numOfCharsReadFromVocabFile;


	/**
	 * lineCountInFile[VOCABFILEINDEX]
	 * - number of total lines (or words) in the vocabulary file.
	 * lineCountInFile[TESTFILEINDEX]
	 * - number of total lines in the test file.
	 */
	long lineCountInFile[NUMOFFILES];


	/**
	 * number of lines that have been removed and processed from
	 * the line queue,
	 * this is used for tracking the progress of the countvocabstrings thread
	 */
	long numOfProcessedLines;


	/**
	 * store strings read from vocabulary file
	 */
	std::vector<std::string> *vocabVect;


	/**
	 * buffer lines read from test file
	 */
	std::queue <std::string> *lineQueue;


	/**
	 * pthread mutex for accessing the line Queue
	 */
	pthread_mutex_t queue_mutex;


	/**
	 * completed flags indicate the completion of the thread logic, one
	 * for the readvocab thread, one for readlines thread
	 * Important: the completed flag of readvocab thread may be
	 * used to force the precedence constraint that the
	 * countvocabstrings thread has to wait for readvocab
	 * thread logic to complete before executing its thread logic
	 *
	 * taskCompleted[VOCABFILEINDEX]
	 * - boolean flag to indicate whether the readvocab
	 * thread has completed the task: read strings from the
	 * vocab file and store them into a linear data structure
	 *
	 * important: you may want to set the completed flag for the
	 * readvocab thread to true only after the main thread fully displays
	 * the progress bar upon the completion of the readvocab thread
	 *
	 * taskCompleted[TESTFILEINDEX]
	 * - boolean flag to indicate whether the readlines
	 * thread has completed the task: read lines from the
	 * a test file, and insert each line to a queue
	 * - main thread progress display would use this flag (when it is true) to
	 * start to show the countvocabstrings progress bar
	 */
	bool taskCompleted[NUMOFFILES];
} SHARED_DATA;






		//defines the charNode structure to store a char in the tree..each index in a charNode is mapped to valid alphabet characters, with some index defined above. 
		typedef struct charNode{
			charNode* next[NCHILD];


			//charnode destructor to remove nodes
			~charNode(){
				if(next != 0x0)
				{
					for(int i = 0;i<NCHILD; i++){
						if(next[i] != nullptr){
							 next[i] = nullptr;
						}
					}
		
				}
			}
		};



//Trie tree object and associated methods for inserting words and searching for substrings
class Trie {
	private:


	public:

		//pointer to the first node in the tree
		charNode* root;

		//new node initializer...sets each index pointer to null
		charNode* createNode(){
			charNode* newNode = new charNode;
			for(int i =0;i<NCHILD;i++){
				newNode->next[i] = nullptr;
			}
			return newNode;
		}



		//class constructor
		Trie();

		//class destructor
		~Trie();

		//recursively deallocate Trie tree
		void deallocate(charNode* currentNode);

		//recursively inserts every possible suffix of a word  beginning from the root node
		bool insert(charNode* currentNode, const char* stringBeingInserted);

		//helper method to recursively insert a suffix. creates new charNodes for each character in the suffix
		bool insertSuffix(charNode* currentNode,const char* suffix);


		//recursively searches for a matching substring in the trie tree 
		bool search (charNode* currentNode, const char* strBeingSearched);

		//helper method to return the mapped  index of each valid alphabet character
		int charToIndex(char c);

};



#endif // TRIE_H
