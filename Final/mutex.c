#include <stdio.h>
#include <pthread.h>
double balance = 1000.0; // Shared resource: bank account balance
pthread_mutex_t balance_mutex = PTHREAD_MUTEX_INITIALIZER;
void *deposit(void *amount)
{
    // pthread_mutex_lock(&balance_mutex);
    balance += *((double *)amount);
    printf("Deposited $%.2f. New balance: $%.2f\n", *((double *)amount),
           balance);
    // pthread_mutex_unlock(&balance_mutex);
    return NULL;
}
void *withdraw(void *amount)
{
    // pthread_mutex_lock(&balance_mutex);
    balance -= *((double *)amount);
    printf("Withdrew $%.2f. New balance: $%.2f\n", *((double *)amount),
           balance);
    // pthread_mutex_unlock(&balance_mutex);
    return NULL;
}
int main()
{
    pthread_t t1, t2;
    double deposit_amount = 500.0;
    double withdraw_amount = 200.0;
    pthread_create(&t1, NULL, deposit, &deposit_amount);
    pthread_create(&t2, NULL, withdraw, &withdraw_amount);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Final balance: $%.2f\n", balance);
    return 0;
}