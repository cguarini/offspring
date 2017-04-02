/**File: offspring.c
**Creates a family tree from an input file or command line input.
**author: Chris Guarini cdg6285
**CSCI243 Project 2
**/

//INCLUDES
#include <stdio.h>
#include <stdlib.h>

//NTree_S implementation for N-ary tree structure
typedef
struct NTree_S {
  const char * name;    //Name of the person
  struct NTree_S * children; //collection of children
  size_t child_cout;  //Current number of children
  size_t capacity;    //Capacity of children collection
} NTree;              //NTree is an alias for the struct

//NTree_p is the "pointer to N-ary tree struct" type.
typedef struct NTree_S * NTree_p;
