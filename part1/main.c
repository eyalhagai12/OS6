#include <stdio.h>
#include <string.h>

#include "Queue.h"
#include "ActiveObject.h"

void *ceaser_code(void *wordle)
{
    char *word = (char *)wordle;
    int word_len = strlen(word);
    int key = 1;
    for (int i = 0; i < word_len; i++)
    {
        if (word[i] >= 'a' && word[i] <= 'z')
        {
            word[i] = word[i] + key;
            if (word[i] > 'z')
            {
                word[i] = word[i] + 'a' - 'z' - 1;
            }
        }
        else if (word[i] >= 'A' && word[i] <= 'Z')
        {
            word[i] = word[i] + key;
            if (word[i] > 'Z')
            {
                word[i] = word[i] + 'A' - 'Z' - 1;
            }
        }
    }
    return wordle;
}

void *switch_letters(void *wordle)
{
    char *word = (char *)wordle;
    int i = 0;
    while (word[i] != '\0')
    {
        if (word[i] >= 'a' && word[i] <= 'z')
        {
            word[i] -= 32;
        }
        else
        {
            word[i] += 32;
        }
        i++;
    }
    return wordle;
}

void worst_end_ever(PActive ao)
{
    printf("this does nothing\n");
}

void *per(void *that)
{
    return that;
}

int main(int argc, char const *argv[])
{
    // int num1 = 10;
    // int num2 = 2;
    // int num3 = 16;
    // int num4 = 17;
    // PQueue queue = createQ();
    // PQueue queue2 = createQ();
    // enQ(queue, (void *)&num1);
    // enQ(queue, (void *)&num2);
    // enQ(queue, (void *)&num3);
    // enQ(queue, (void *)&num4);
    // PActive ao = newAO(queue, &fun, &end);
    // PActive ao2 = newAO(queue2, &fun, &end);
    // ao->next = ao2;
    // activate(&ao);
    // size_t len = ao2->queue->size;
    // for (int i = 0; i < len; i++)
    // {
    //     printf("%d\n", *(int *)deQ(ao2->queue));
    // }
    PQueue q1 = createQ();
    PQueue q2 = createQ();
    PQueue q3 = createQ();

    PActive ao1 = newAO(q1, &ceaser_code, &worst_end_ever);
    PActive ao2 = newAO(q1, &switch_letters, &worst_end_ever);
    PActive ao3 = newAO(q1, &per, &worst_end_ever);

    char buff[] = "asdadASDASD";
    enQ(q1, (void *)buff);

    ao1->next = ao2;
    ao2->next = ao3;
    activate(&ao1);
    printf("%s", (char *)ao3->queue->head->data);
    return 0;
}
