/** queue.c
**  outlines a priority queue structure and functions
**  author: cdg6285 Chris Guarini
**  modified for use in offspring.c
**/
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

///init_queue
///Initialize the queue and return a pointer to it
queue* init_queue(void){
  //allocate space
  queue * q = malloc(sizeof(queue));
  //Initialize all values to empty / 0
  q->front = NULL;
  q->back = NULL;
  q->itemsInQueue = 0;
  //return pointer
  return q;
}

///enqueue
///Enqueues an object at the back of the queue
/// LOWER NUMBER HAS HIGHER PRIORITY
///@param : q - queue onto which to enqueue an object
///         data - data to enqueue
void enqueue(queue* q, void* data, int priority){
  //Put data into a node
  node * n = malloc(sizeof(node));
  n->data = data;
  n->priority = priority;
  n->next = NULL;//Initialize next to 0x00

  if(q->itemsInQueue){//Check to make sure that there are items in queue
    //Insert node into queue and link it to the list
    if(q->back->priority <= n->priority){
      //Back of the queue has higher or equal priority to n
      //Add n to back of queue
      q->back->next = n;//add to linked list
      q->back = n;//set to back
    }
    else if(q->front->priority > n->priority){
      //n has higher priority than anything in the queue
      //Put n at front of the queue
      n->next = q->front;
      q->front = n;
    }
    else{//n belongs somewhere in middle of queue
      node * cursor = q->front;//cursor variable
      
      //Search for where to put n
      while(cursor){//not at end of queue
        if(cursor->next->priority > n->priority){
          //n belongs after cursor
          //Place n after cursor
          n->next = cursor->next;
          cursor->next = n;//point cursor to n
          break;//stop searching
        }
        //move cursor to next in line
        cursor = cursor->next;
      }
    }
  }
  else{//No items in queue, add first item to front and back
    //insert the node into the queue
    q->front = n;
    q->back = n;
  }

  q->itemsInQueue++;//Increment queue counter
  //ITEM ENQUEUED!
}

///dequeue
///Dequeues the object at the front of the queue, returns NULL if queue is
///empty
///@param : q - queue onto which the dequeue will be performed
void* dequeue(queue* q){
  //get the node at front of the queue
  node * n = q->front;
  if(q->itemsInQueue){//Queue wasn't empty
    //point the front of the queue to the next in line
    q->front = q->front->next;
    
    if(q->itemsInQueue == 1){//Only 1 item in queue
      //Set queue to empty
      q->back = NULL;
      q->front = NULL;
    }
    q->itemsInQueue--;

    //retrieve data from the node
    void * data = n->data;
    //delete the node
    free(n);
    //return the data
    return data;
  }
  else{//queue is empty
    return NULL;
  }
  //END OF DEQUEUE
}

///rev_dequeue
///Dequeues the object from the back of the queue, returns NULL if queue is
///empty
///@param : q -queue onto which to perform the operation
void * rev_dequeue(queue * q){
  //get the node at the back of the queue
  node * n = q->back;
  if(q->itemsInQueue){//Queue wasn't empty
    //point the back of the queue to next in line
    if(q->itemsInQueue == 1){//Only 1 item in queue
      //set queue to empty
      q->back = NULL;
      q->front = NULL;
    }
    q->itemsInQueue--;
    //set q->back to the new back of queue
    node * temp = q->front;
    for(int i=0; i<q->itemsInQueue; i++){
      temp = temp->next;
    }
    q->back = temp;

    //retrieve data from the node
    void * data = n->data;
    //delete the node
    free(n);
    //return the data;
    return data;
  }
  else{//queue is empty
    return NULL;
  }
}


///destroy_queue
///Destroys the queue by dequeueing all elements and freeing the queue struct
///@param : q - the queue to destroy
void destroy_queue(queue* q){
  //Empty the queue of all elements
  while(q->itemsInQueue){
    dequeue(q);//dequeue frees all nodes
  }
  //Free the space used by q
  free(q);
  //END OF DESTROY_QUEUE
}

///queue_size
///Returns the amount of elements in the queue
///@param: q - the queue to inspect
int queue_size(queue * q){
  return q->itemsInQueue;
}


///queue_priority
///Returns the priority of the last element in the queue
///@param : q - queue on which to find out lowest priority
///returns null if queue is empty
int queue_priority(queue * q){
  //Check if queue is empty
  if(!q->itemsInQueue){
    return 0x00;
  }
  //Items are in queue, return back's priority
  return q->back->priority;

}









