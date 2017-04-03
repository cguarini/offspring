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
//DEFINES
#define MAX_INPUT (100) ///MAX_INPUT - maximum input allowed through terminal

//Function Declarations
int strcmp(const char *str1, const char *str2);
void *realloc(void *ptr, size_t size);
char * strtok(char * str, const char * delimiters);

//NTree_S implementation for N-ary tree structure
typedef
struct NTree {
  const char * name;    //Name of the person
  struct NTree ** children; //collection of children
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
  
  //Destroy the queue
  destroy_queue(q);

  //Return pointer to NTree object if name is found
  //otherwise return null
  return pointer;
}




///insert_child
///Inserts a child into an NTree using find() method
///@param: tree - Tree to insert the child
///        parent - Name of the parent 
///        child  - Name of the child
NTree *  insert_child(NTree *  tree, char parent[], char child[]){
 //Check if tree is empty
 if(!tree){
    //If tree is empty, initialize tree with parent as the root
    tree = malloc(sizeof(NTree));
    //Set root's name to parent's name
    tree->name = parent;
    //set root to childless
    tree->children = NULL;
    tree->child_count = 0;
    //root can hold no children
    tree->capacity = 0;
 }
 //Continue with adding child
  
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
    birth->name = child;//name the child
    birth->children = NULL;//no children
    birth->child_count = 0;//not a single one
    birth->capacity = 0;//Can't hold any children
  }
  p->children[p->child_count] = * birth;//Add child to parent's list of children
 }
 return tree;
 //END OF INSERT_CHILD
 //END OF BIRTH PUNS
}

///print_tree
///Prints the tree or subtree using breadth first traversal and 
/// the queue_tree function
///@param : tree - The tree in which to print
void print_tree(NTree * tree){
  //Use queue_tree function to get the tree in priority queue form
  queue * q = init_queue();//get a pointer to an empty queue
  queue_tree(tree,q,0);//put the tree into queue form
   
  while(queue_size(q)){//while items are in the queue
    //Go through queue, printing the person's name and the children they had
    NTree * p = dequeue(q);//Current parent
    printf("%s had ",p->name);//Print the parent's name
    
    if(p->child_count){//If the parent has children, print their names
      for(int i=0; i < p->child_count; i++){
        
        //Formatting code
        
        //Has only 1 child
        if(p->child_count == 1){
          printf("%s.\n",p->children[i].name);
        }

        //Has more than one child

        else if(i == p->child_count-1){//Last child
          printf(" and %s.\n",p->children[i].name);//Print last child's name
        }
        
        else if(i == 0){
          printf("%s",p->children[i].name);//Print first child's name
        }
        else{//Middle child
          printf(", %s",p->children[i].name);//Print middle child's name
        }
      }
    }
    //Parent has no children
    else{
      printf("no offspring.\n");
    }
  }

  //Destroy the queue
  destroy_queue(q);
}

///main() Takes an optional argument to read a file,
///Otherwise will loop through waiting for commands
///To create a family tree.
/*
int main(int argc, char * argv[]){
  NTree * tree = NULL;
  //Command loop
  while(1){
    //Get command from terminal
    char input[MAX_INPUT];
    input[0]=0x00;
    //Get input
    fgets(input, MAX_INPUT, stdin);
    
    //Check what the command is
    char * command;
    command = strtok(input," ,");
    command = trim(command);
    
    //Check if command is add
    if(!strcmp(command,"add")){
      //Retrieve arguments
      
      //parent argument
      char * parent = strtok(NULL," ,");
      parent = trim(parent);
      //child argument
      char * child = strtok(NULL," ,");
      child = trim(child);
      
      //Run insert_child on those arguments
      tree = insert_child(tree,parent,child);
    }

    //Check if command is print
    if(!strcmp(command,"print")){
      print_tree(tree);
    }
  }
}
*/

int main(void){
  NTree * tree = NULL;
  tree = insert_child(tree,"Chris","Kevin");
  printf("%s\n",tree->name);
  printf("%s\n",tree->children[0]->name);
}
















