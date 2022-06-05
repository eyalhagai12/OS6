#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Queue.h"

PQueue createQ()
{
    // allocate memory for a new queue
    PQueue queue = (PQueue)malloc(sizeof(struct Queue));

    // initiate queue attributes
    queue->head = NULL;
    queue->size = 0;

    return queue;
}

void enQ(PQueue queue, void *element)
{
    pthread_mutex_lock(&(queue->lock));
    // create new node to add
    PNode new_data = (PNode)malloc(sizeof(struct QueueNode));
    new_data->data = element;
    new_data->next = NULL;

    // add it to the queue
    if (queue->head == NULL)
    {
        queue->head = new_data;
    }
    else
    {
        PNode temp = queue->head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_data;
    }
    queue->size += 1;
    pthread_mutex_unlock(&(queue->lock));
}

void *deQ(PQueue queue)
{
    pthread_mutex_lock(&(queue->lock));
    if (queue->head != NULL)
    {
        PNode out = queue->head;
        void *value = out->data;
        queue->head = queue->head->next;
        free(out);
        pthread_mutex_unlock(&(queue->lock));
        queue->size -= 1;
        return value;
    }
    pthread_mutex_unlock(&(queue->lock));
    return NULL;
}

void destroyQ(PQueue queue)
{
    pthread_mutex_lock(&(queue->lock));
    PNode current = queue->head;

    while (current != NULL)
    {
        PNode temp = current;
        current = current->next;
        free(temp);
    }

    pthread_mutex_unlock(&(queue->lock));
    free(queue);
}
