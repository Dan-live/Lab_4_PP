#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define SIZE 15
#define CurrentSize (buff.ind2)

FILE *file;
//initialising threads p1 - p6
pthread_t thread1;
pthread_t thread2;
pthread_t thread3;
pthread_t thread4;
pthread_t thread5;
pthread_t thread6;
//initialising functions for each thread
void* P1(void*);
void* P2(void*);
void* P3(void*);
void* P4(void*);
void* P5(void*);
void* P6(void*)

pthread_mutex_t MCR1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t exit_mt = PTHREAD_MUTEX_INITIALIZER;

sem_t sem_q;

typedef struct
{
    int32_t data;
} Elem;

typedef struct
{
	Elem vec[SIZE];
	size_t ind1, ind2;
} VectorBuffer;

VectorBuffer buff = {{{0}}, 0, 0};

//initialising atomic variables
int atomic_int1 = 0, atomic_int2 = 0;
unsigned int atomic_uint1 = 0, atomic_uint2 = 0;
long atomic_long1 = 0, atomic_long2 = 0;
unsigned long atomic_ulong1 = 0, atomic_ulong2 = 0;
int amountExit = 0;

void checking()
{
    if(isFull() || isEmpty())
        amountExit++;
}
int isExit()
{
    return amountExit >= 2;
}

int isFull()
{
	return CurrentSize >= SIZE;
}

int isEmpty()
{
	return CurrentSize <= 0;
}

int Set(int data)
{
	if(buff.ind2)
	{
		buff.ind1 = (buff.ind1 + 1) % SIZE;
	}
	buff.vec[buff.ind1].data = data;
	buff.ind2++;
	return data;
}

int Get()
{
	int result = 0;
	result = buff.vec[(buff.ind1 - buff.ind2 + 1 + SIZE) % SIZE].data;
	buff.ind2--;
	return result;
}

void produceElement()
{
		mutex_lock_macro(MCR1);
		while(isFull())
		{
			overrun_control_macro();
			if(overrun_cnt >= 2 && underrun_cnt >= 2)
			{
				Flag = 1;
			}
			pthread_cond_wait(&Sig1, &MCR1);
		}
		fprintf(logFile, "%s: Element struct=%li; element %i PRODUSED; \n", __FUNCTION__, CurrentSize, Set(rand() % 1000));
		mutex_unlock_macro(MCR1);
		pthread_cond_signal(&Sig2);
}

void consumeElement()
{
        mutex_lock_macro(MCR1);
		while(isEmpty())
		{
			underrun_control_macro();
			pthread_cond_wait(&Sig2, &MCR1);
		}
		int32_t data = Get();
		mutex_unlock_macro(MCR1);
		pthread_cond_signal(&Sig1);
		fprintf(logFile, "%s: Element struct=%li; element %d CONSUMED; \n", __FUNCTION__, CurrentSize, data);
		elemProcces(data);
}

void atomic_change()
{

    __sync_fetch_and_sub (&k, 1);
    __sync_add_and_fetch (&k2, 1);

    __sync_fetch_and_or(&b, 2);
    __sync_fetch_and_nand(&b2, 3);

    __sync_and_and_fetch (&c, 4);
    __sync_xor_and_fetch (&c2, 5);

    __sync_bool_compare_and_swap(&d, 22222222, 3);
__sync_val_compare_and_swap(&d2, 2, 3);

}


void* P1(void* arg)//зроблено
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(1)
    {
        if(isExit()) break;

        sem_getvalue(&sem_q,&sem_value);
        if (sem_value < max_queue_length) {
/* Захоплення м'ютекса черги для додавання нового елементу */
        pthread_mutex_lock (&MCR1);
        fprintf(file, "Thread 1 locked MCR1\n");
        Set(rand() % 1000)

        sem_getvalue(&sem_q,&sem_value);
        fprintf(file, "Thread 1 added value %d into CR1\n", CurrentSize);
        
        pthread_mutex_unlock(&MCR1);
        fprintf(file, "Thread 1 unlocked mutex MCR1\n");
        sem_post (&sem_q)
        fprintf(file, "Thread 1 end\n");
        }
    }
    pthread_cancel(thread2);
    fprintf(file, "p1| p2 canceled\n");
    pthread_cancel(thread3);
    fprintf(file, "p1| p3 canceled\n");
    pthread_cancel(thread4);
    fprintf(file, "p1| p4 canceled\n");
    pthread_cancel(thread5);
    fprintf(file, "p1| p5 canceled\n");
    pthread_cancel(thread6);
    fprintf(file, "p1| p6 canceled\n");
    return NULL;
}
void* P2(void* arg)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    fprintf(file,"Thread 2 is enable\n");
    long getvalue;
    while(1)
    {
        if(isExit()) break;
        pthread_mutex_lock(&Sig21_mutex);
        while(!flag_p1p4_p2)
        {
            fprintf(file,"Thread 2 waits Sig21\n");
            pthread_cond_wait(&Sig21, &Sig21_mutex);
        }
        fprintf(file,"Thread 2 got Sig21\n");
        flag_p1p4_p2 = 0;
        fprintf(file,"Thread 2 changed flag14_2\n");
        pthread_mutex_unlock(&Sig21_mutex);
        fprintf(file, "Thread 2 unlocked Sig21_mutex\n");
        getvalue = __sync_or_and_fetch(&atomic_long1, 0);
        fprintf(file,"Thread 2 writing %ld to the CR1\n", getvalue);
        pthread_mutex_lock(&MCR1);
        if(isFull())
        {
            while(isFull())
            {
//Sig1 for producer
                pthread_cond_wait(&Sig1,&MCR1);
            }
        }
        push_back(getvalue);
        fprintf(file, "Thread 2 pushed_back value %ld into CR1\n", getvalue);
        pthread_mutex_unlock(&MCR1);
        fprintf(file,"Thread 2 unlocked mutex MCR1\n");
        pthread_cond_broadcast(&Sig2); // notFull
        fprintf(file, "Thread 2 send signal Sig2 to all threads\n");
    }
    pthread_cancel(thread1);
    fprintf(file, "p2| p1 canceled\n");
    pthread_cancel(thread3);
    fprintf(file, "p2| p3 canceled\n");
    pthread_cancel(thread4);
    fprintf(file, "p2| p4 canceled\n");
    pthread_cancel(thread5);
    fprintf(file, "p2| p5 canceled\n");
    pthread_cancel(thread6);
    fprintf(file, "p2| p6 canceled\n");
    return NULL;
}
void* P3(void* arg)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(1)
    {
        if(isExit()) break;
        pthread_mutex_lock(&MCR1);
        fprintf(file,"thread 3 locked MC1\n");
        if(isEmpty())
        {
            while (isEmpty())
            {
                fprintf(file,"Thread 3 is waiting for Sig2\n");
                pthread_cond_wait(&Sig2, &MCR1);
            }
        }
        pop_back();
        fprintf(file,"Thread 3 uses pop_back\n");
        pthread_mutex_unlock(&MCR1);
        fprintf(file, "Thread 3 unlocked mutex\n");
        pthread_cond_broadcast(&Sig1); // notFull
        fprintf(file, "Thread 3 sent Sig1\n");
    }
    return NULL;
    pthread_cancel(thread1);
    fprintf(file, "p3| p1 canceled\n");
    pthread_cancel(thread2);
    fprintf(file, "p3| p2 canceled\n");
    pthread_cancel(thread4);
    fprintf(file, "p3| p4 canceled\n");
    pthread_cancel(thread5);
    fprintf(file, "p3| p5 canceled\n");
    pthread_cancel(thread6);
    fprintf(file, "p3| p6 canceled\n");
}
void* P4(void* arg)//зроблено
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(1)
    {
        if(isExit()) break;

        sem_getvalue(&sem_q,&sem_value);
        if (sem_value < max_queue_length) {
/* Захоплення м'ютекса черги для додавання нового елементу */
        pthread_mutex_lock (&MCR1);
        fprintf(file, "Thread 4 locked MCR1\n");
        Get();

        sem_getvalue(&sem_q,&sem_value);
        fprintf(file, "Thread 4 get value %d into CR1\n", CurrentSize);
        
        pthread_mutex_unlock(&MCR1);
        fprintf(file, "Thread 4 unlocked mutex MCR1\n");
        sem_post (&sem_q)
        fprintf(file, "Thread 4 end\n");
        }
    }
    pthread_cancel(thread1);
    fprintf(file, "p4| p1 canceled\n");
    pthread_cancel(thread2);
    fprintf(file, "p4| p2 canceled\n");
    pthread_cancel(thread3);
    fprintf(file, "p4| p3 canceled\n");
    pthread_cancel(thread5);
    fprintf(file, "p4| p5 canceled\n");
    pthread_cancel(thread6);
    fprintf(file, "p4| p6 canceled\n");
    return NULL;
}
void* P5(void* arg)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    unsigned long ul_getvalue;
    while(1)
    {
        if(isExit()) break;
        pthread_mutex_lock(&Sig21_mutex);
        fprintf(file,"Thread 5 locked Sig21_mutex\n");
        if(!flag_p1p4_p5)
        {
            while(!flag_p1p4_p5)
            {
                fprintf(file,"Thread 5 is waiting for Sig21\n");
                pthread_cond_wait(&Sig21,&Sig21_mutex);
            }
        }
        fprintf(file, "Thread 5 got a Sig21\n");
        flag_p1p4_p5 = 0;
        fprintf(file,"Thread 5 changed flag14_5\n");
        pthread_mutex_unlock(&Sig21_mutex);
        fprintf(file, "Thread 5 unlock sig21_mutex\n");
        ul_getvalue = __sync_or_and_fetch(&atomic_ulong1, 0);
        fprintf(file,"Thread 5 got variable %ld\n", ul_getvalue);
        pthread_mutex_lock(&MCR1);
        if(isFull())
        {
            while (isFull())
            {
                pthread_cond_wait(&Sig1, &MCR1);
            }
        }
        push_back(ul_getvalue);
        fprintf(file, "Thread 5 wrote %ld to the CR1\n", ul_getvalue);
        pthread_mutex_unlock(&MCR1);
        fprintf(file,"Thread 5 unlocked mutex CR1\n");
        pthread_cond_broadcast(&Sig2); // notEmpty
        fprintf(file, "Thread 5 sending Sig2\n");
    }
    pthread_cancel(thread1);
    fprintf(file, "p5| p1 canceled\n");
    pthread_cancel(thread3);
    fprintf(file, "p5| p3 canceled\n");
    pthread_cancel(thread4);
    fprintf(file, "p5| p4 canceled\n");
    pthread_cancel(thread2);
    fprintf(file, "p5| p2 canceled\n");
    pthread_cancel(thread6);
    fprintf(file, "p5| p6 canceled\n");
    return NULL;
}
void* P6(void* arg)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(1)
    {
        if(isExit()) break;
        pthread_mutex_lock(&MCR1);
        fprintf(file, "Thread 6 locked MCR1\n");
        if(isFull())
        {
            while(isFull())
            {
//waits for notFULL;
                pthread_cond_wait(&Sig1,&MCR1);
            }
        }
        current++;
        push_back(current + 1);
        fprintf(file, "Thread 6 pushed_back value %d into CR1\n", current);
        pthread_mutex_unlock(&MCR1);
        fprintf(file, "Thread 6 unlocked mutex MCR1\n");
        pthread_cond_broadcast(&Sig2); // notEmpty
        fprintf(file, "Thread 6 sending Sig2\n");
    }
    pthread_cancel(thread1);
    fprintf(file, "p6| p1 canceled\n");
    pthread_cancel(thread3);
    fprintf(file, "p6| p3 canceled\n");
    pthread_cancel(thread4);
    fprintf(file, "p6| p4 canceled\n");
    pthread_cancel(thread5);
    fprintf(file, "p6| p5 canceled\n");
    pthread_cancel(thread2);
    fprintf(file, "p6| p2 canceled\n");
    return NULL;
}


int main(int argc, char *argv[])
{
    logFile = stdout;
	if(argc > 1)
	{
		producerSleepMSEnabled = false;
		consumerSleepMSEnabled = false;
	}
    if(0)
    {
		logFile = fopen("/student/Univer/PP/Lab_4/Tests/Test_2/logFile.txt", "wt");
	}

    sem_init(&SCR21, 0, 0);
    sem_init(&SCR22, 0, 0);
    thread_func_start_macro();
    pthread_create(&thread1, NULL, &P1, NULL);
    pthread_create(&thread2, NULL, &P2, NULL);
    pthread_create(&thread3, NULL, &P3, NULL);
    pthread_create(&thread4, NULL, &P4, NULL);
    pthread_create(&thread5, NULL, &P5, NULL);
    pthread_create(&thread6, NULL, &P6, NULL);

    thread_join_macro(thread1, NULL);
    thread_join_macro(thread2, NULL);
    thread_join_macro(thread3, NULL);
    thread_join_macro(thread4, NULL);
    thread_join_macro(thread5, NULL);
    thread_join_macro(thread6, NULL);
    thread_func_finish_macro();

    fclose(logFile);
    return 0;
}
