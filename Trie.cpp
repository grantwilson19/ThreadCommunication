/* Author: James Marsh
* REDID: 826209118
* CS480 Operating Systems
* PA1 trie tree
*/

#include "Trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h> //use strlen method (won't include \0 )
using namespace std;

//class constructor
Trie::Trie(){
	//creates an empty node for the root
	root = nullptr;
	root = createNode();

}

//class destructor
Trie::~Trie(){
	charNode* next = root;
	//calls deallocate memory to free all nodes in the tree
	for(int i =0; i<NCHILD; i++){
			deallocate(next->next[i]);
	}
	//aka delete root
	delete next;
	//delete root;
}


//dsetructor aid. given a trie tree, this method takes the root pointer, and recursively traverses to the last node, securely deleting all nodes
void Trie::deallocate(charNode* currentNode){
	if(currentNode == nullptr){
		return;
	}
	for(int i = 0; i<NCHILD; i++){
				deallocate(currentNode->next[i]);
	}
	delete currentNode;
	return;
}
	
	







//recursively inserts a string and its suffixes from the root
bool Trie::insert(charNode* currentNode, const char* stringBeingInserted){
	//checks if the end of the string as been reached indicating that all characters have been inserted
	if(stringBeingInserted[0] == '\0'){
//HANDLES EMPTY STRING
		if(currentNode->next[NULLTERMINATORINDEX] == nullptr){
			currentNode->next[NULLTERMINATORINDEX] = createNode();
		}
		return true;
	}
	//skips insert if the index is invalid 
	if(charToIndex(stringBeingInserted[0])<0){
		return insert(currentNode,stringBeingInserted+1);
	}
		
	//calls the recursive insert for this suffix
	insertSuffix(currentNode,stringBeingInserted);
	
	
	//increments the string to the next possible suffix to be inserted
	insert(currentNode,stringBeingInserted+1);

	return true;
}//end insert()


//HELPER METHOD recursively inserts each char for a suffix
bool Trie::insertSuffix(charNode* currentNode, const char* suffix){

	//checks if the suffix has reached the end (all characters inserted)
	if(suffix[0] == '\0'){
		//sets an empty node at the null terminator index indicatin the end of that suffix
		if(currentNode->next[NULLTERMINATORINDEX] == nullptr){
			currentNode->next[NULLTERMINATORINDEX] = createNode();
			return true;
		}
		return true;
	}

	
	//get index of first char in suffix
	int index = charToIndex(suffix[0]);
		
	//skip invalid char
	if(index == -1){
		return insertSuffix(currentNode, suffix+1);
	}
		

	//ensures that no existing  nodes are overwritten for overlapping suffix
	if(currentNode->next[index] == nullptr){
		//creates new node for a valid char at the mapped index position if there is not already a node
		currentNode->next[index] = createNode();
	}

	//points currentNode to the next node for recursive traverse
	currentNode = currentNode->next[index];
	
	//recsurive insert for next char, incrementing the suffix pointer to the next character
	insertSuffix(currentNode,(suffix+1));
	return true;
}




//recursively searches each string character for a node with matching index in the trie  beginning with the root 
bool Trie::search(charNode* currentNode, const char* strBeingSearched){
	//if the stringBeingSearched reaches null, then a match for each char was found.
	if(strBeingSearched[0] == '\0'){
		return true;
	}
	//gets index of character being searched
	int index = charToIndex(strBeingSearched[0]);
	
	//checks that index is valid, and that the currentNode has the mapped index filled
	if(index >= 0 && currentNode->next[index] != nullptr){
		//updates currentNode to next node to search for the next char	
		currentNode = currentNode->next[index];
		
		//recrusive search for next char, incrementing the string pointer
		return search(currentNode,(strBeingSearched+1));
	}

	//if no matching index was found in the (if) then there is no matching substring.
	return false;
}



//HELPER METHOD to fetch mapped index of valid string characters using ascii values..returns-1 for invalid chars...uses defintions from header file
int Trie::charToIndex(char c){
	switch (c){
		case '\0':
			return NULLTERMINATORINDEX;
		case ' ':
			return SPACEINDEX;
		case '\'':
			return APOSTROPHEINDEX;
		case '-':
			return HYPHENINDEX;
		case '_':
			return UNDERSCOREINDEX;
		default:
			//index uses 'a' as a base reference to map a-z 0-25, with identical index for uppercase and lowercase
			int index = ((char)tolower(c))-'a';
			if( index >= 0 && index < 26){
				return index;
			}
			return -1;//case for invalid char...CAUTION SEGMENTATION FAULT
	}//end switch

}//end charToIndex()



