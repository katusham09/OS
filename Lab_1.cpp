#include <pthread.h> 
#include <unistd.h> 
#include <iostream> 

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
int ready = 0;                                   


void*  provide(void*) 
{
    while (true) 
    {
        pthread_mutex_lock(&lock); 
        if (ready)
        {

            pthread_mutex_unlock(&lock);  
            continue;
        }
        ready = 1;                   
        printf("Provided\n");           
        pthread_cond_signal(&cond);  
        pthread_mutex_unlock(&lock); 
        sleep(1);                    
    }
    return nullptr;
}

void* consumer(void*) 
{
    while (true) 
    {
        pthread_mutex_lock(&lock);  
        while (!ready) 
        {
            pthread_cond_wait(&cond, &lock);
        }
        ready = 0;                  
        printf("Consumed\n");      
        pthread_mutex_unlock(&lock); 
    }
    return nullptr;
}

int main() 
{
    pthread_t producerThread, consumerThread; 

    pthread_create(&producerThread, nullptr,  provide, nullptr);
    pthread_create(&consumerThread, nullptr, consumer, nullptr); 
    pthread_join(producerThread, nullptr); 

    return 0;  
}
