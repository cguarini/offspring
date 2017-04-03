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
#include "queue.h"
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




///queue_tree
///Turns the tree into a priority queue, with the top of the tree having
///highest priority (golf rules, lower number is higher priority)
/// Uses recursion, returns pointer to queue of the tree
///@param tree - tree to enqueue
///       q    - Queue in which to enqueue all the elements
///       priority - priority in which to enqueue, pass 0 at start
void queue_tree(NTree * tree, queue * q, int priority){
  //BASE CASE
  if(!tree){
    return;//END OF SUBTREE
  }
  //Tree exists

  //enqueue the current node
  enqueue(q, tree, priority);
  //Queue all of the node's children
  for(int i=0; i < tree->child_count; i++){
    //Recursively queue all of nodes children
    //Priority corresponds to level on the family tree
    //0 is highest
    queue_tree(&tree->children[i],q,priority+1);
  }
}


///find
///Finds a person in the tree and returns their VMA
///@param tree - Tree to search
///       name - name to find in the tree
NTree* find(NTree * tree, char name[]){
  NTree * pointer = NULL;//Return variable

  //Create a queue to use for breadth first search
  queue * q = init_queue();
  //pass queue to helper function that enqueues entire tree from root
  queue_tree(tree,q,0);
  
  //Dequeue elements in the queue until name is found
  int qsize = queue_size(q);
  for(int i=0; i<qsize; i++){
    NTree * temp = dequeue(q);
    if(strcmp(temp->name,name)){
      //Return first instance of the name (oldest ancestor)
      pointer = temp;
      break;
    }
  }

  //Return pointer to NTree object if name is found
  //otherwise return null
  return pointer;
}




///insert_child
///Inserts a child into an NTree using find() method
///@param: tree - Tree to insert the child
///        parent - Name of the parent 
///        child  - Name of the child
void insert_child(NTree *  tree, char parent[], char child[]){
 //Use find function to search for the parent
 NTree * p = find(tree, parent);
 //Check if parent was found
 if(p){
 //Parent is found
  //Add a child
  p->child_count++;
  //reallocate memory to account for new child
  p->children = realloc(tree->children,(sizeof(NTree)*(tree->child_count)));
  p->capacity++;//might be useful later, who knows?

  //Add the child to the list of children
  NTree * birth = malloc(sizeof(NTree));//a new node is born
  if(birth){//malloc delivered the child
    birth->name=child;//name the child
    birth->children = NULL;//no children
    birth->child_count = 0;//not a single one
    birth->capacity = 0;//Can't hold any children
  }
  p->children[p->child_count] = * birth;//Add child to parent's list of children
 }
 //END OF INSERT_CHILD
 //END OF BIRTH PUNS
}



///main() Takes an optional argument to read a file,
///Otherwise will loop through waiting for commands
///To create a family tree.
int main(int argc, char * argv[]){
 //Command loop
  while(1){

  }
}









