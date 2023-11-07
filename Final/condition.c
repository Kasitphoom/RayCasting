#include <stdio.h>
#include <pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int queue = 0;
void *producer(){
    pthread_mutex_lock(&mutex);
    while(queue){
        pthread_cond_wait(&cond, &mutex);
    }
    queue = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void *consumer(){
    pthread_mutex_lock(&mutex);
    while(!queue){
        pthread_cond_wait(&cond, &mutex);
    }
    queue = 0;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

int main(){
    pthread_t pt, ct;
    pthread_create(&pt, NULL, producer, NULL);
    pthread_create(&ct, NULL, consumer, NULL);

    pthread_join(pt, NULL);
    pthread_join(ct, NULL);
    return 0;
}