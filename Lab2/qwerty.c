#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *threadA_void(void*), *threadB_void(void*);
pthread_cond_t cond1;
pthread_mutex_t lock;
int ready;

int main()
{
    pthread_t A, B;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond1, NULL);
    ready = 0;

    printf("Press Enter to close threads.\n\n");

    pthread_create(&A, NULL, &threadA_void, NULL);
    pthread_create(&B, NULL, &threadB_void, NULL);

    scanf("1"); 
    pthread_detach(A);
    pthread_detach(B);
    pthread_cancel(A);
    pthread_cancel(B);

    pthread_cond_destroy(&cond1);
    pthread_mutex_destroy(&lock);
    return 0;
}

void *threadA_void(void *param)
{
    while(1) 
    {
        sleep(1);
        pthread_mutex_lock(&lock);
        if (ready == 1)
        {
            pthread_mutex_unlock(&lock);
            continue;
        }
        ready = 1;
        printf ("Provided\n");
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock);
    } 
}

void *threadB_void(void *param)
{
    while(1)
{
        pthread_mutex_lock(&lock);
        while (ready == 0)
        {
            pthread_cond_wait(&cond1, &lock);
            printf ("Awoke...");
        }
        ready = 0;
        printf (" Consumed\n");
        pthread_mutex_unlock(&lock);
    } 
}
