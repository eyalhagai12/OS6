#include <stdio.h>
#include "Queue.h"

int main(int argc, char const *argv[])
{
    PQueue queue = createQ();
    enQ(queue, 10);
    enQ(queue, 19);
    deQ(queue);
    enQ(queue, 1);
    enQ(queue, 12);
    destroyQ(queue);
    return 0;
}
