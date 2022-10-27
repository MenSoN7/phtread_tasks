/* Программа считает сумму делителей всех чисел в промежутке от P до Q, которые передаются через аргументы 
Ипользуется 1 поток */ 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*#define P 1
#define Q 800000*/

long long int sum = 0; //переменная для подсчета суммы делителей

struct interval_t
{
  long int p;
  long int q;
};

void number_divisors(long int n); // функция для подсчета суммы делителей числа n
void *thread_job(void *param); /* потоковая функция */

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Неверное количество аргументов!\n");
    return -1;
  }

  long int P = atoi(argv[1]);
  long int Q = atoi(argv[2]);
  
  struct interval_t interval;
  interval.p=P;
  interval.q=Q;

  pthread_t p1;       /* идентификатор потока */
  
  /* создаем новый поток */
  pthread_create(&p1, NULL, thread_job, &interval);

  /* ждем завершения исполнения потока */
  pthread_join(p1, NULL);

  printf("Count of divisors = %lld\n", sum);

  return 0;
}

/* Потоковая функция */
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
           if (j != n/j)
            {
             sum += 2;             
            }
            else
            {
             sum++;     
            }
         }
      } 
  
}
