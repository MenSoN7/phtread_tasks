/* Программа считает сумму делителей всех чисел в промежутке от P до Q, которые передаются через аргументы 
Ипользуются 2 потока */ 



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* #define P 1
#define Q 800000 */

long long int sum = 0; // общие данные для потоков  - переменная для подсчета суммы делителей
pthread_mutex_t mutex;

struct interval_t
{
  long int p;
  long int q;
};

void number_divisors(long int n); // функция для подсчета суммы делителей числа n
void *thread_job(void *param); // потоковая функция  

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Неверное количество аргументов!\n");
    return -1;
  }

  struct interval_t interval_1, interval_2;
  pthread_t p1,p2;       // идентификаторы потоков 

  long int P = atoi(argv[1]);
  long int Q = atoi(argv[2]);
  
  interval_1.p=P;
  interval_1.q=Q/2;
  
  interval_2.p=Q/2+1;
  interval_2.q=Q;

  //инициализация мьютекса
  pthread_mutex_init(&mutex,NULL);
  
  /* создаем  два новых потока */
  pthread_create(&p1, NULL, thread_job, &interval_1);
  pthread_create(&p2, NULL, thread_job, &interval_2);
  
  /* ждем завершения исполнения потоков */
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
 
  pthread_mutex_destroy(&mutex);
  
  printf("Count of divisors = %lld\n", sum);

  return 0;
}

// Потоковая функция 
void *thread_job(void *interval) 
{
  struct interval_t*  interval_ = (struct interval_t*) interval;
  for (long int i = interval_->p; i <= interval_->q; ++i)
  {
     number_divisors(i);
  }
  return NULL;
}

//функция, которая считает количество делителей для числа
void number_divisors(long int n) {

 for (long int j = 1; j <= sqrt(n); ++j)
      {
        if (!(n % j))
         {
           pthread_mutex_lock(&mutex);
           if (j != n/j)
            {
             sum += 2;             
            }
            else
            {
             sum++;     
            }
           pthread_mutex_unlock(&mutex);
         }
      } 
  
}
