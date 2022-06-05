#include <stdlib.h>
#include "Queue.h"

typedef struct ActiveObject
{
    PQueue queue;
    void *(*per_element)(void *);
    void (*end)(struct ActiveObject *);
    struct ActiveObject *next;
} * PActive;

PActive newAO(PQueue queue, void *(*per_element)(void *), void (*end)(PActive))
{
    PActive ao = (PActive)malloc(sizeof(struct ActiveObject));
    ao->queue = queue;
    ao->per_element = per_element;
    ao->end = end;
    ao->next = NULL;
    return ao;
}

void destroyAO(PActive ao)
{
    destroyQ(ao->queue);
    free(ao);
}

void insert_data(PActive obj, void *data)
{
    enQ(obj->queue, data);
}

void *pop(PActive obj)
{
    return deQ(obj->queue);
}

void activate(PActive *head)
{
    PActive tmp = *head;
    while (tmp != NULL)
    {
        for (size_t i = 0; i < tmp->queue->size; i++)
        {
            void *poped = tmp->per_element(pop(tmp));
            insert_data(tmp, poped);
        }
        tmp->end(tmp);
        if (tmp->next != NULL)
        {
            size_t len = tmp->queue->size;
            for (size_t i = 0; i < len; i++)
            {
                void *poped = pop(tmp);
                insert_data(tmp->next, poped);
            }
        }
        tmp = tmp->next;
    }
}