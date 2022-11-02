/* Программа считает сумму делителей всех чисел в интервалах, которые генерирует программа interval_generator
Ипользуются несколько потоков */ 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

long long int sum = 0; // общие данные для потоков  - переменная для подсчета суммы делителей
pthread_mutex_t mutex;

typedef struct interval
{
    long int p;
    long int q;
} interval_t;

void number_divisors(long int n); // функция для подсчета суммы делителей числа n
void *thread_job(void *param); // потоковая функция

int main(int argc, char *argv[]) {
    FILE* file=fopen("intervals.txt", "r");
    
    //количество строк в файле - количество интервалов
    int count_of_lines=0;
    rewind(file);
    while(!feof(file))
    {   
       if (fgetc(fd) == '\n') 
           ++count_of_lines;
    }    
    
    //выделение памяти для n-ого количества интервалов, где n - количество строк в файле
    //(переменная count_of_lines)
    interval_t* intervals;
    intervals = (interval_t *) malloc(count_of_lines * sizeof(interval_t));

    //выделение памяти для n-ого количества потоков, где n - количество строк в файле
    //(переменная count_of_lines)
    pthread_t *p;
    p = (pthread_t *) malloc(count_of_lines * sizeof(pthread_t));
    
    //инициализация мьютекса
    pthread_mutex_init(&mutex,NULL);
     
    //указатель на начало файла
    rewind(file);
    //считываем интервалы из файла и создаем потоки
    for(int i=0;i<count_of_lines;++i)
    {
        fscanf(file,"%ld %ld",&intervals[i].p,&intervals[i].q);
        pthread_create(&p[i],NULL,thread_job,&intervals[i]);
    }

    for(int i=0; i < count_of_lines ; ++i)
        pthread_join(p[i],NULL);

    pthread_mutex_destroy(&mutex);
    printf("Count of divisors = %lld\n", sum);

    //освобождаем выделенную память
    free(p);
    free(intervals);

    fclose(file);
    return 0;
}

// Потоковая функция
void *thread_job(void *interval)
{
    interval_t*  interval_= (interval_t*) interval;
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
