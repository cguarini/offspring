/** queue.h
**  outlines a priority queue structure and functions
**  author: cdg6285 Chris Guarini
**  for use in offspring.c
**/

///Node - For linked list implementation of queue
typedef struct node {
  void * data;
  struct node * next;
  int priority;
} node;

///Queue - structure of the queue
typedef struct queue { 
  node * back;
  node * front;
  int itemsInQueue;
} queue;

///enqueue
///Enqueues an object at the back of the queue
///@param : q - queue onto which to enqueue an object
///         data - data to enqueue
///         priority - priority of the object enqueued
void enqueue(queue* q, void* data, int priority);

///dequeue
///Dequeues the objects at the front of the queue, returns NULL if queue is
///empty
///@param : q- queue onto which the queue will be performed
void* dequeue(queue* q);

///rev_dequeue
///Dequeues an element from the back of the queue, returns NULL if the queue is
///empty
///@param : q - queue in which to operate with
void* rev_dequeue(queue* q);


///destroy_queue
///Destroys the queue by dequeueing all elements and freeing the queue struct
///@param : q - the queue to destroy
void destroy_queue(queue* q);

///init_queue
///Initialize the queue and return a pointer to it
queue * init_queue(void);

///queue_size
///Returns the amount of items in the queue
///@param : q - Queue to inspect
int queue_size(queue * q);

///queue_priority
///Returns the priority of the last item in the queue
///Which is the lowest priority, but highest number
int queue_priority(queue * q);
