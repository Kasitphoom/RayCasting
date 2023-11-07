#include <stdio.h>
#include <pthread.h>
int buffer;
int data_available = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void *producer(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        while (data_available)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        buffer = i; // Produce data
        data_available = 1;
        printf("Produced %d\n", buffer);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
void *consumer(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        while (!data_available)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Consumed %d\n", buffer);
        data_available = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main()
{
    pthread_t prod, cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    return 0;
}