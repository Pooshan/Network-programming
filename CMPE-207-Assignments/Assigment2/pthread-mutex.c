#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<time.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct Queue
{
        int capacity;
        int size;
        int front;
        int rear;
        int *elements;
}Queue;
Queue * createQueue(int maxElements)
{
        /* Create a Queue */
        Queue *Q;

        Q = (Queue *)malloc(sizeof(Queue));
        /* Initialise its properties */
        Q->elements = (int *)malloc(sizeof(int)*maxElements);
        Q->size = 0;
        Q->capacity = maxElements;
        Q->front = 0;
        Q->rear = 0;
        /* Return the pointer */
        return Q;
}
void Dequeue(Queue *Q)
{
        /* If Queue size is zero then it is empty. So we cannot pop */
        if(Q->size==0)
        {
                printf("Queue is Empty\n");
                return;
        }
        /* Removing an element is equivalent to incrementing index of front by one */
        else
        {
                Q->size--;
                Q->front++;
                /* As we fill elements in circular fashion */
                if(Q->front==Q->capacity)
                {
                        Q->front=0;
                }
        }
        return;
}
int front(Queue *Q)
{
        if(Q->size==0)
        {
                printf("Queue is Empty\n");
                exit(0);
        }
        /* Return the element which is at the front*/
        return Q->elements[Q->front];
}

void *producer(void *t)
{
  int rear;
  printf("t1 value = %d %lu\n",*((int *)t),pthread_self());
  while(1)
  {
   if(rear < 5)
   {
    pthread_mutex_lock(&mutex);
    printf("Producer...\n");
    addTail();

     pthread_cond_signal(&cond);

     sleep(1);
     pthread_mutex_unlock(&mutex);
    }

   }
}
void *consumer1(void *t)
{
  printf("t2 value = %d %lu\n",*((int *)t),pthread_self());
  while(1)
  {
   pthread_mutex_lock(&mutex);
   printf("Consumer...1\n");

  // while(rear == front)//(done == 0)
   {

  //   rear=0;front=0;
     pthread_cond_wait(&cond,&mutex);
     printf("front needed1\n");
   }
   readHead();//done--;
   printf("front consumer1 = %d\n",front);
   pthread_cond_signal(&cond);

   sleep(1);
   pthread_mutex_unlock(&mutex);

  }
}
