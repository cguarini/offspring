/**File: offspring.c
**Creates a family tree from an input file or command line input.
**author: Chris Guarini cdg6285
**CSCI243 Project 2
**/

//INCLUDES
//libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//headers
#include "trimit.h"
#include "queue.h"
//DEFINES
#define MAX_INPUT (100) ///MAX_INPUT - maximum input allowed through terminal

//Function Declarations
int strcmp(const char *str1, const char *str2);
void *realloc(void *ptr, size_t size);
char * strtok(char * str, const char * delimiters);
char *strcpy(char *dest, const char *src);
size_t strlen(const char *str);

//NTree_S implementation for N-ary tree structure
typedef
struct NTree {
  char * name;    //Name of the person
  struct NTree * * children; //collection of children
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
    queue_tree(tree->children[i],q,priority+1);
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
    int debug = strcmp(temp->name,name);
    if(!strcmp(temp->name,name)){
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
 
 //check if tree is empty
 if(!tree){
    char * parent_cpy = malloc(strlen(parent)+1);
    strcpy(parent_cpy,parent);
    //If tree is empty, initialize tree with parent as the root
    tree = malloc(sizeof(NTree));
    //Set root's name to parent's name
    tree->name = parent_cpy;
    //set root to childless
    tree->children = NULL;
    tree->child_count = 0;
    //root can hold no children
    tree->capacity = 0;
 }

 if(child){//Check if child exists
 //Continue with adding child
  
  //Use find function to search for the parent
  NTree * p = find(tree, parent);
  //Check if parent was found
  if(p){
    //Parent is found

    //check if child is in children list
    for(int i=0; i < p->child_count; i++){
      //check all of the children
      if(!strcmp(child,p->children[i]->name)){//child's name is found
        return tree;//return
      }
    }
    //copy names into new addresses
    char * child_cpy = malloc(strlen(child)+1);
    strcpy(child_cpy,child);
    //Add a child
    p->child_count++;
    //reallocate memory to account for new child
    p->children = realloc(p->children,(sizeof(NTree)*(p->child_count)));
    p->capacity++;//might be useful later, who knows?

    //Add the child to the list of children
    NTree * birth = malloc(sizeof(NTree));//a new node is born
    if(birth){//malloc delivered the child
      birth->name = child_cpy;//name the child
      birth->children = NULL;//no children
      birth->child_count = 0;//not a single one
      birth->capacity = 0;//Can't hold any children
    }
    p->children[p->child_count-1] = birth;//Add child to parent's list of children
    //free(birth);
  }

  //Parent not found in tree, check if child is root.
  else if(!strcmp(tree->name,child)){
    //set parent as new root, and add child to children list
    NTree * p = malloc(sizeof(NTree));
    if(p){//malloc worked
      //copy name into new pointer
      char * p_cpy = malloc(strlen(parent)+1);
      strcpy(p_cpy,parent);
      p->name=p_cpy;
      //add root to list of children
      p->child_count = 1;
      p->children = malloc((sizeof(tree)*(p->child_count)));
      p->children[0] = tree;
      p->capacity = 1;
      return p;
    }
  }
  else{
    fprintf(stderr,"Unable to add %s, %s not found in tree.\n",child,parent);
    free(p);
  }
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
    //Go through queue, printing the person's name and the childre they had
    NTree * p = dequeue(q);//Current parent
    printf("%s had ",p->name);//Print the parent's name
    
    if(p->child_count){//If the parent has children, print their names
      for(int i=0; i < p->child_count; i++){
        
        //Formatting code
        
        //Has only 1 child
        if(p->child_count == 1){
          printf("%s.\n",p->children[i]->name);
        }

        //Has more than one child

        else if(i == p->child_count-1){//Last child
          printf(" and %s.\n",p->children[i]->name);//Print last child's name
        }
        
        else if(i == 0){
          printf("%s",p->children[i]->name);//Print first child's name
        }
        else{//Middle child
          printf(", %s",p->children[i]->name);//Print middle child's name
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

///print_person
///prints the current node and their children
///@param : p - The node in which to print
void print_person(NTree * p){
  //Print the person's name
  printf("%s had ",p->name);
  
  //Print the person's offspring
  if(p->child_count){//If the person has children, print their names
    for(int i=0; i< p->child_count; i++){
      //Formatting code
      
      //Has only 1 child
      if(p->child_count == 1){
        printf("%s. \n",p->children[i]->name);
      }

      //Has more than one child
      else if(i == p->child_count-1){//Last child
        printf(" and %s.\n",p->children[i]->name);//Print last child's name
      }
      else if(i==0){
        printf("%s",p->children[i]->name);//Print first child's name
      }
      else{//Middle child
        printf(", %s",p->children[i]->name);//Print middle child's name
      }

    }
  }
  //Person has no children
  else{
    printf("no offspring.\n");
  }
}



///tree_size
///Returns the amount of nodes in the tree by
///using queue_tree function
///@param : tree - the tree to find the size of.
int tree_size(NTree * tree){
  //Create the queue that is used by queue_tree
  queue * q = init_queue();
  //Get the tree in queue form
  queue_tree(tree,q,0);
  
  //return the sizse of the queue
  int size = queue_size(q);//get the size of the queue
  destroy_queue(q);//free the queue's memory
  return size;
}

///tree_height
///uses queue_tree to determine the height of the tree
///@param : tree - tree in which to inspect
int tree_height(NTree * tree){
  if(!tree){
    return 0;
  }
  //Create the queue
  queue * q = init_queue();
  //put tree into queue
  queue_tree(tree,q,0);
  //get priority of last node in queue
  int height = queue_priority(q);
  //destroy the queue
  destroy_queue(q);
  return height+1;//return height of tree
}

///scrub_node
///Destroys the members of the tree node given to the function
///@param : n - the node of the tree to be destroyed
void scrub_node(NTree * n){
  //Frees all malloced parts of the node
  free(n->name);
  free(n->children);
}


///destroy_tree
///Uses queue_tree to destroy the entire tree
///one node at a time with the help of scrub_node
///@param : tree - the tree to destroy
void destroy_tree(NTree * tree){
  //create a queue for queue_tree
  queue * q = init_queue();
  //enqueue the entire tree to be destroyed
  queue_tree(tree,q,0);

  //empty the queue, destroying each node in the tree
  while(queue_size(q)){//while elements in queue
    NTree * n = dequeue(q);//get next in line to kill
    scrub_node(n);//kill the malloced parts
    //free the memory of that node
    free(n);
  }
  //destroy the queue
  destroy_queue(q);
  //END OF DESTROY_TREE
}




///main() Takes an optional argument to read a file,
///Otherwise will loop through waiting for commands
///To create a family tree.
int main(int argc, char * argv[]){
  NTree * tree = NULL;
  
  //Read from file
  if(argc > 1){//If argument is present
    FILE * fp;
    fp = fopen(argv[1],"r");
    char finput[MAX_INPUT];
    //Read file line by line
    while(EOF != fscanf(fp,"%100[^\n]\n", finput)){
      char * parent;
      parent = strtok(finput,",");
      if(parent){//Parent is on this line
        tree = insert_child(tree,parent,NULL);
        char * child = strtok(NULL,",");
        while(child){//Add children to parent
          strcpy(child,trim(child));//trim whitespace
          tree = insert_child(tree,parent,child);//insert child
          child = strtok(NULL,",");//move on to next child
        }
      }
      //Read next line
    }
    //Close file
    fclose(fp);
  }
  
  //After file read, allow for terminal use


  //Command loop
  while(1){
    printf(" offspring> ");
    //Get command from terminal
    char input[MAX_INPUT];
    input[0]=0x00;
    //Get input
    fgets(input, MAX_INPUT, stdin);
        
    //Check what the command is
    char command[MAX_INPUT];
    strcpy(command , strtok(input," ,"));
    strcpy(command , trim(command));
    
    //Check if command is add
    if(!strcmp(command,"add")){
      //Retrieve arguments
      
      //parent argument
      char * parent = strtok(NULL,",");
      if(parent){//Check if passed parent argument
          strcpy(parent , trim(parent));
          //child argument
          char * child = strtok(NULL,",");
          if(child){//Check if passed child argument
          strcpy(child , trim(child));
      
          //DEBUG INPUTS///////////////////////////////
          //printf("add %s %s\n",parent,child);

          //Run insert_child on those arguments
          tree = insert_child(tree,parent,child);
      
          }
          else{
            fprintf(stderr,"add parent_name child_name\n");
          }
      }
      else{
        fprintf(stderr,"add parent_name child_name\n");
      }
    }

    //Check if command is print
    if(!strcmp(command,"print")){
      //Check if optional argument is there
      char * name;
      name = strtok(NULL, ",");
      
      if(name){//name exists
        strcpy(name, trim(name));
        if(find(tree,name)){//name in tree
          print_tree(find(tree,name));
        }
        else{//name not in tree
          fprintf(stderr, "Unable to find %s in tree.\n", name);
        }
      }
      //otherwise print entire tree
      else{
        print_tree(tree);
      }
    }

    //Check if command is find
    if(!strcmp(command,"find")){
      //Check for arguments
      char * name;
      name = strtok(NULL, ",");
      if(name){
        strcpy(name, trim(name));
        if(find(tree,name)){//name in tree
          print_person(find(tree,name));
        }
        else{//Can't find name
          fprintf(stderr, "Unable to find %s in tree.\n", name);
        }
      }
    }

    //Check if command is size
    if(!strcmp(command,"size")){
      //print the size of the tree
      char * name = strtok(NULL, ",");
      if(name){//Check for optional argument
        strcpy(name,trim(name));
        if(find(tree,name)){
          printf("size: %d\n",tree_size(find(tree,name)));
        }
        else{
          fprintf(stderr, "Unable to find %s in tree.\n", name);
        }
      }
      //Otherwise perform on whole tree
      else{
        printf("%d\n",tree_size(tree));
      }
    }

    //Check if command is height
    if(!strcmp(command,"height")){
      //print the height of the tree
      char * name = strtok(NULL, ",");
      if(name){//check for optional argument
        strcpy(name,trim(name));//trim whitespace
        if(find(tree,name)){//name in tree
          printf("height: %d\n",tree_height(find(tree,name)));
        }
        else{//name not in tree
          fprintf(stderr, "Unable to find %s in tree.\n", name);
        }
      }
      //Otherwise perform on whole tree
      else{
        printf("%d\n",tree_height(tree));
      }
    }
    

    //Check if command is quit
    if(!strcmp(command,"quit")){
      //Destroy the tree and exit the program
      destroy_tree(tree);
      printf("\n");
      return EXIT_SUCCESS;
    }

    //Check if command is init
    if(!strcmp(command,"init")){
      //Destroy the tree and reinitialize tree pointer
      destroy_tree(tree);
      tree=NULL;
    }

    //Check if command is help
    if(!strcmp(command,"help")){
      //Print information of valid commands
      printf("User commands for offspring:\n"
          "add parent-name, child-name # create a parent-child relation.\n"
          "find   [name] # find and print the name and its children list.\n"
          "print  [name] # print a breadth first traversal of all the offspring from named person down.\n"
          "size   [name] # print the size of all members in the [sub]tree.\n"
          "height [name] # print the height of [sub]tree\n"
          "init # delete the current tree and start with an empty tree.\n"
          "help # print information on valid commands.\n"
          "quit # delete current tree and exit.\n");
    }
  }
}

/* TEST FUNCTION
int main(void){
  NTree * tree = NULL;
  tree = insert_child(tree,"Chris","Kevin");
  tree = insert_child(tree,"Chris","Yoda");
  tree = insert_child(tree,"Yoda","Luke");
  print_tree(tree);
  printf("%s\n",tree->name);
  printf("%s\n",tree->children[0].name);
}
*/














