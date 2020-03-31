#include<iostream>
using namespace std;
#include<pthread.h>
#include<stdlib.h>
#include <unistd.h>

////////////////////////FUNCTIONS||DECLARATION\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void * barrierPoint(void *numthreads);
void barrierInIt(int n_threads);
int decrement();
int wait_barrier();

//////////////////////////////////////////////////////
int invoke_barrier = 0;
int barrierSize;
pthread_cond_t  finish_cond  = PTHREAD_COND_INITIALIZER;
int barrier = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int counter=0;
int T_TC;





////////////////////////////////MAIN||FUNCTION\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

int main() 
{

    cout<<"Enter the size of the barrier\n";
   cin>>barrierSize;

    cout<<"Enter no. of thread\n";
    cin>>T_TC;

     

    if (barrierSize>=0 && T_TC>=0)
	 {
        pthread_t tid[T_TC];

        barrierInIt(barrierSize);

        for(int i =0; i < T_TC; i++)
        {
            pthread_create(&(tid[i]), NULL, &barrierPoint, &T_TC);
        }


        for(int j = 0; j < T_TC; j++)
        {
            pthread_join(tid[j], NULL);
        }
    }
  	else{
      cout<<"You are entering wrong data.\n";
      main();
    }

    return 0;
}

//////////////////////////////////////END||OF||MAIN||FUNCTION\\\\\\\\\\\\\\\\\\\\\\\




///////////////////////////////////DEFINITIONS||OF||FUNCTIONS\\\\\\\\\\\\\\\\\\\\\\\\/

int decrement()
{
    if (barrier == 0) {

        return 0;
    }

    if(pthread_mutex_lock(&lock) != 0)
    {
        perror("Failed to take lock.");
        return -1;
    }

    barrier--;

    if(pthread_mutex_unlock(&lock) != 0)
    {
        perror("Failed to unlock.");
        return -1;
    }

    return 0;
}


//////////////////////////////////
void barrierInIt(int n_threads)
{
    if ( T_TC < barrierSize ) { barrier = T_TC; return; }
    barrier = n_threads;
}

/////////////////////////

void * barrierPoint(void *numthreads)
{

     int r = rand() % 5;

     cout<<"\nThread"<<++counter<< "\nPerforming init task of length "<<r<<" sec\n";
     sleep(r);

     wait_barrier();
     if (barrierSize!=0) {
       if ((T_TC - (invoke_barrier++) ) % barrierSize == 0) {
         cout<<"\nBarrier is Released\n";
       }
       cout<<"\n This is task after barrier\n";

     }
     

     return  NULL;
}

////////////////////////////////////

int wait_barrier()
{
    if(decrement() < 0)
    {
        return -1;
    }

    while (barrier)
    {
        if(pthread_mutex_lock(&lock) != 0)
        {
            perror("\n Error in locking mutex");
            return -1;
        }

        if(pthread_cond_wait(&finish_cond, &lock) != 0)
        {
            perror("\n Error in cond wait.");
            return -1;
        }
    }

    /*
     * last thread will execute this.
     */
    if(0 == barrier)
    {
        if(pthread_mutex_unlock(&lock) != 0)
        {
            perror("\n Error in locking mutex");
            return -1;
        }
        if(pthread_cond_signal(&finish_cond) != 0)
        {
            perror("\n Error while signaling.");
            return -1;
        }
    }

    return 0;
}


/////////////////////////////////END||OF||CODE\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
