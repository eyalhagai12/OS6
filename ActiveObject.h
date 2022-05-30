#include <stdlib.h>

#include "Queue.h"

typedef struct ActiveObject
{
    PQueue queue;
    void (*per_element)(void *);
    void *(*end)();
} * PActive;

PActive newAO(PQueue queue, void (*per_element)(void *), void *(*end)())
{
    PActive ao = (PActive)malloc(sizeof(ActiveObject));
    ao->queue = queue;
    ao->per_element = per_element;
    ao->end = end;
    return ao;
}

void destroyAO(PActive ao)
{
    destroyQ(ao->queue);
    free(ao);
}

void *activate()
{

}
