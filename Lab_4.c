#include <stdio.h>
#include <stdlib.h>
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
// void* P1(void*);
// void* P2(void*);
// void* P3(void*);
// void* P4(void*);
// void* P5(void*);
// void* P6(void*)

sem_t sem_q;
pthread_mutex_t MCR1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Sig21_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Sig21 = PTHREAD_COND_INITIALIZER;
pthread_barrier_t BCR2;


int flag21_P3P6_P2 = 0;
int flag21_P3P6_P5 = 0;

typedef struct
{
    int data;
} Elem;

typedef struct
{
	Elem vec[SIZE];
	int ind1, ind2;
} VectorBuffer;

VectorBuffer buff = {{{0}}, 0, 0};
////////////
//блок атомарных переменных
int k = 12;
int k2 = -1222;
unsigned int b = 12123;
unsigned int b2 = 10;
long int c = 11111111;
long int c2 = -3333333;
unsigned long int d = 22222222;
unsigned long int d2 = 121212123;
//////////////////////
//int amountExit = 0;

// void checking()
// {
//     if(isFull() || isEmpty())
//         amountExit++;
// }
// int isExit()
// {
//     return amountExit >= 2;
// }

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

void atomic_change()
{

    __sync_fetch_and_and (&k, 1);
    __sync_fetch_and_add (&k2, 1);

    __sync_fetch_and_or(&b, 2);
    __sync_nand_and_fetch(&b2, 3);

    __sync_and_and_fetch (&c, 4);
    __sync_xor_and_fetch (&c2, 5);

    __sync_bool_compare_and_swap(&d, 22222222, 3);
__sync_val_compare_and_swap(&d2, 2, 3);

}


void* P1(void* arg)//зроблено
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    int sem_value;
    fprintf(file,"Thread 1 is started\n");
    while(1)
    {
        if(isFull() || isEmpty()) break;

        sem_getvalue(&sem_q,&sem_value);
        if (sem_value < SIZE) 
        {

            pthread_mutex_lock (&MCR1);
            fprintf(file, "Thread 1 locked MCR1\n");

            int num = rand() % 1000; 
            Set(num);

            sem_getvalue(&sem_q,&sem_value);
            fprintf(file, "P1(Produser) write %d to CR1", num);
        
            pthread_mutex_unlock(&MCR1);
            fprintf(file, "Thread 1 unlocked mutex MCR1\n");
            sem_post (&sem_q);
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
    int sem_value;
    //long getvalue;
    fprintf(file,"Thread 2 is started\n");
    while(1)
    {
        if(isFull() || isEmpty()) break;
        //доступ до буфера за допомогою семафора та м'ютекса
        
        sem_getvalue(&sem_q,&sem_value);
        if (sem_value < SIZE) 
        {

            pthread_mutex_lock (&MCR1);
            fprintf(file, "Thread 2 locked MCR1\n");

            int num = rand() % 1000; 
            Set(num);

            sem_getvalue(&sem_q,&sem_value);
            fprintf(file, "P2(Produser) write %d to CR1", num);
        
            pthread_mutex_unlock(&MCR1);
            fprintf(file, "Thread 2 unlocked mutex MCR1\n");
            sem_post (&sem_q);
        }
        //Sig21 !!Зроблено!!

        pthread_mutex_lock(&Sig21_mutex);

        while(flag21_P3P6_P2 == 0)
        {
            fprintf(file,"Thread 2 waits Sig21\n");
            pthread_cond_wait(&Sig21, &Sig21_mutex);
        }
        fprintf(file,"Thread 2 got Sig21\n");
        flag21_P3P6_P2 = 0;
        fprintf(file,"Thread 2 changed flag21_P3P6_P2\n");

        pthread_mutex_unlock(&Sig21_mutex);
        fprintf(file,"Thread 2 unlocked Sig21_mutex\n");

        //використання а потім модифікація сигнальних змінних 
        
        int s = __sync_fetch_and_sub(&b, 0);
        s = s + __sync_add_and_fetch(&k2, 0);
        fprintf(file,"Thread 2 use atomic\n");
        atomic_change();
        fprintf(file,"Thread 2 modified atomic\n");


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
    fprintf(file,"Thread 3 is started\n");
    while(1)
    {
        if(isEmpty()) break;
        //модифікація атомарних змінних !!Зроблено!! 
        fprintf(file,"Thread 3 modified atomic\n");
        atomic_change();
        //синхронізуємо через бар'єр Р3 і Р6

        pthread_barrier_wait(&BCR2);
        //Sig21 !!Зроблено!!
        
        pthread_mutex_lock(&Sig21_mutex);
        fprintf(file,"Tread 3 locked Sig21_mutex \n");
        flag21_P3P6_P2 = 1;
        pthread_cond_signal(&Sig21);
        fprintf(file, "Thread 3 UNlocked Sig21_mutex \n");
        pthread_mutex_unlock(&Sig21_mutex);

        //використання атормарних змінних
        fprintf(file,"Thread 3 use atomic\n");
        int s = __sync_fetch_and_add(&k, 0);
        s = s + __sync_add_and_fetch(&k2, 0);
    

    }

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
    return NULL;
}
void* P4(void* arg)//зроблено
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    int sem_value;
    fprintf(file,"Thread 4 is started\n");
    while(1)
    {
        if(isFull() || isEmpty()) break;
        //доступ до буфера за допомогою семафора та м'ютекса 
        sem_wait(&sem_q);
        pthread_mutex_lock(&MCR1);
        int value = Get();

        sem_getvalue(&sem_q, &sem_value);
        fprintf(file, "Thread 4 get value %d from CR1\n", value);
        
        pthread_mutex_unlock(&MCR1);
        fprintf(file, "Thread 4 unlocked mutex MCR1\n");
        fprintf(file, "Thread 4 end\n");
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
    fprintf(file,"Thread 5 is started\n");
    int sem_value;
    while(1)
    {
        if(isFull() || isEmpty()) break;
        //доступ до буфера за допомогою семафора та м'ютекса 
        sem_wait(&sem_q);
        pthread_mutex_lock(&MCR1);
        int value = Get();

        sem_getvalue(&sem_q, &sem_value);
        fprintf(file, "Thread 5 get value %d from CR1\n", value);
        
        pthread_mutex_unlock(&MCR1);
        fprintf(file, "Thread 5 unlocked mutex MCR1\n");
        //fprintf(file, "Thread 5 end\n");

        //Sig21  !!Зроблено!!
        pthread_mutex_lock(&Sig21_mutex);

        while(flag21_P3P6_P5 == 0)
        {
            fprintf(file,"Thread 5 waits Sig21\n");
            pthread_cond_wait(&Sig21, &Sig21_mutex);
        }
        fprintf(file, "Thread 5 got a Sig21\n");
        flag21_P3P6_P5 = 0;
        fprintf(file,"Thread 5 changed flag21_P3P6_P5\n");

        pthread_mutex_unlock(&Sig21_mutex);
        fprintf(file,"Thread 5 unlocked Sig2_mutex\n");

        //використання атомарних змінних
        fprintf(file,"Thread 5 use atomic\n");
        int s = __sync_fetch_and_sub(&k, 0);
        s = s + __sync_add_and_fetch(&c2, 0);


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
    fprintf(file,"Thread 6 is started\n");
    while(1)
    {
        if(isEmpty()) break;
        //модифікація атомарних !!Зроблено!!
        fprintf(file, "P6 modifies atomic\n");
        atomic_change();
        //if(isExit()) break;
        //синхронізація через бар'єр Р3 і Р6 

        pthread_barrier_wait(&BCR2);
        //Sig21 !!Зроблено!! 

        pthread_mutex_lock(&Sig21_mutex);
        fprintf(file,"Tread 6 locked Sig21_mutex \n");
        flag21_P3P6_P2 = 1;
        flag21_P3P6_P5 = 1;
        pthread_cond_broadcast(&Sig21);
        fprintf(file, "Thread 6 UNlocked Sig21_mutex \n");
        pthread_mutex_unlock(&Sig21_mutex);




        //використання атомарних операцій
        fprintf(file, "P6 using atomic\n");//операция над атомарными переменными
        int s = __sync_fetch_and_add(&k, 0);
        s = s + __sync_add_and_fetch(&k2, 0);

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
    file = fopen("out.log", "w");
    if(file == NULL) return 2;
//initializing semapthores with closed status
    sem_init (&sem_q, 0, 0);
    pthread_barrier_init(&BCR2, NULL, 2);

    int sem_value;
    sem_getvalue(&sem_q, &sem_value);
    fprintf(file, "Semaphore = %d\n",sem_value);

    int length_at_start = 2;

    for(int i = 0; i < length_at_start; i++){
        Set(rand() % 1000);
    }

    fprintf(file, "Array filled by elements from 0-th to %d-th \n",length_at_start - 1);


    sem_getvalue(&sem_q,&sem_value);
    fprintf(file, "Semaphore = %d\n",sem_value);

    pthread_create(&thread1, NULL, &P1, NULL);
    pthread_create(&thread2, NULL, &P2, NULL);
    pthread_create(&thread3, NULL, &P3, NULL);
    pthread_create(&thread4, NULL, &P4, NULL);
    pthread_create(&thread5, NULL, &P5, NULL);
    pthread_create(&thread6, NULL, &P6, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);


   // fclose(logFile);
    return 0;
}
