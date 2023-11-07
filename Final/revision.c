#include <stdio.h>
#include <pthread.h>
int shared[100];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *task1(){
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 100; i+=2){
        shared[i] += 1;
    }
    printf("SHARED value (+1) = %d\n", shared);
    pthread_mutex_unlock(&mutex);
}

void *task2(){
    pthread_mutex_lock(&mutex);
    for (int i = 1; i < 100; i+=2){
        shared[i] += 1;
    }
    printf("SHARED value (-1) = %d\n", shared);
    pthread_mutex_unlock(&mutex);
}

void *task3(){
    pthread_mutex_lock(&mutex);
    int sum = 0;
    for (int i = 0; i < 100; i++){
        sum += shared[i];
    }
    printf("SUM = %d", sum);
    pthread_mutex_unlock(&mutex);
}

void init_shared(){
    for (int i = 0; i < 100; i++){
        shared[i] = 0;
    }
}

int main(){
    init_shared();

    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, task1, NULL);
    pthread_create(&t2, NULL, task2, NULL);
    pthread_create(&t3, NULL, task3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
}