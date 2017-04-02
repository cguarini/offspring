/**File: offspring.c
**Creates a family tree from an input file or command line input.
**author: Chris Guarini cdg6285
**CSCI243 Project 2
**/

//INCLUDES
//libraries
#include <stdio.h>
#include <stdlib.h>
//headers
#include "trimit.h"
//Function Declarations
int strcmp(const char *str1, const char *str2);
void *realloc(void *ptr, size_t size);


//NTree_S implementation for N-ary tree structure
typedef
struct NTree {
  const char * name;    //Name of the person
  struct NTree * children; //collection of children
  size_t child_count;  //Current number of children
  size_t capacity;    //Capacity of children collection
} NTree;              //NTree is an alias for the struct

//NTree_p is the "pointer to N-ary tree struct" type.
typedef struct NTree * NTree_p;


///insert_child
///Inserts a child into an NTree using recursion
///@param: tree - Tree to insert the child
///        parent - Name of the parent 
///        child  - Name of the child
void insert_child(NTree *  tree, char parent[], char child[]){
 //Base case
 if(tree == NULL){
    //NOP, end of subtree
 }

 //Parent is found
 else if(strcmp(tree->name,parent)){
    //Parent of child
    //Add a child
    tree->child_count++;
    //reallocate memory to account for new child
    tree->children = realloc(tree->children,(sizeof(NTree)*(tree->child_count)));
    tree->capacity++;//MIGHT BE USEFUL LATER

    //Add the child to list of children
    NTree * birth = malloc(sizeof(NTree));//a new node is born
    if(birth != NULL){//malloc delivered the child
      birth->name=child;//name the child
      birth->children = NULL;//no children
      birth->child_count=0;//Not a single one
      birth->capacity=0;//Can't hold any children
    }
    tree->children[tree->child_count] = *birth;//Add child to parent's list of children
 }

 //Not the parent
 else{//YOU ARE NOT THE FATHER!
   for(int i=0; i < (tree->child_count); i++){
      //Recursively check all of their kids
      insert_child(&(tree->children[i]),parent,child);
   }
 }

}


///main() Takes an optional argument to read a file,
///Otherwise will loop through waiting for commands
///To create a family tree.
int main(int argc, char * argv[]){
 //Command loop
  while(1){

  }
}









