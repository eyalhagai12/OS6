#include <stdio.h>

struct QueueNode
{
    void *data;
    struct QueueNode *next;
};

typedef struct QueueNode *PNode;

typedef struct Queue
{
    PNode head;
    size_t size;
    pthread_mutex_t lock;
} * PQueue;

// create a new queue
PQueue createQ();

// delete a given queue
void destroyQ(PQueue queue);

// push an element to the queue
void enQ(PQueue queue, void *element);

// pop an element from the queue
void *deQ(PQueue queue);