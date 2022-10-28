/* Программа считает сумму делителей всех чисел в промежутке от P до Q, которые передаются через аргументы 
Ипользуются 2 потока */ 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/* #define P 1
#define Q 800000 */

long long int sum = 0; // общие данные для потоков  - переменная для подсчета суммы делителей
pthread_mutex_t mutex;

typedef struct interval
{
  long int p;
  long int q;
} interval_t;

void number_divisors(long int n); // функция для подсчета суммы делителей числа n
void *thread_job(void *param); // потоковая функция  

int main(int argc, char *argv[]) 
{
  if (argc != 3) 
  {
    printf("Неверное количество аргументов!\n");
    return -1;
  }

  if (atoi(argv[1]) > atoi(argv[2])) 
  {
    printf("Первый аргумент должен быть меньше второго!\n");
    return -1;
  }

  //узнаем количество ядер
  long int count_of_cores = sysconf(_SC_NPROCESSORS_CONF);

  //выделение памяти для n-ого количества интервалов, где n - количество ядер
  //(переменная count_of_cores)
  interval_t* intervals;
  intervals = (interval_t *) malloc(count_of_cores * sizeof(interval_t));

  //выделение памяти для n-ого количества потоков, где n - количество ядер
  //(переменная count_of_cores)
  pthread_t *p;       
  p = (pthread_t *) malloc(count_of_cores * sizeof(pthread_t));

  long int P = atoi(argv[1]);
  long int Q = atoi(argv[2]);

  //инициализация мьютекса
  pthread_mutex_init(&mutex,NULL);

  for (int i=0; i < count_of_cores; ++i)
    {
        intervals[i].p = P+i;
        intervals[i].q = Q;
        pthread_create(&p[i],NULL,thread_job,&intervals[i]);
    }

  for(int i=0; i < count_of_cores ; ++i)
        pthread_join(p[i],NULL);
    
  pthread_mutex_destroy(&mutex);
  printf("Count of divisors = %lld\n", sum);

  //освобождаем выделенную память
  free(p);
  free(intervals);

  return 0;
}

// Потоковая функция 
void *thread_job(void *interval) 
{
  //узнаем количество ядер
  long int count_of_cores = sysconf(_SC_NPROCESSORS_CONF);
  interval_t*  interval_= (interval_t*) interval;
  for (long int i = interval_->p; i <= interval_->q; i+=count_of_cores)
  {
     number_divisors(i);
  }
  return NULL;
}

//функция, которая считает количество делителей для числа
void number_divisors(long int n) 
{
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
