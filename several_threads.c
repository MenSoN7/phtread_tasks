/* Программа считает сумму делителей всех чисел в интервалах, которые генерирует программа interval_generator
Ипользуются несколько потоков */ 

#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
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
  
  //дескрипторы для пассивного(слушающего) и присоединенного сокетов
  int sockfd, newsockfd;
  int clilen, n;
  long int count = sysconf(_SC_NPROCESSORS_CONF);
  //структуры для размещения полных адресов сервера и клиента
  struct sockaddr_in servaddr, cliaddr;
  //создаем TCP-сокет
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  //обнуляем всю структуру перед заполнением
  memset(&servaddr, 0, sizeof(servaddr));
  //заполняем структуру для адреса сервера
  servaddr.sin_family = AF_INET; //семейство
  servaddr.sin_port = htons(51000);//номер порта
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
  //связываем созданный сокет с адресом (3 параметр - размер структуры в байтах)
  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  //переводим сокет в пассивное состояние
  // второй параметр - кол-во одновременных подключений
  if (listen(sockfd, 5) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  // заносим в clilen максимальную длину ожидаемого адреса клиента
  clilen = sizeof(cliaddr);
  // принимаем соединение на сокете
  // при нормальном завершении в структуре будет адрес, а в clilen - фактическая длина
  if ((newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  
    //read(newsockfd, &count, sizeof(count));
  
    //выделение памяти для интервалов
    //(переменная count_of_lines)
    interval_t* intervals;
    intervals = (interval_t *) malloc(count * sizeof(interval_t));

    //выделение памяти для  потоков
    //(переменная count_of_lines)
    pthread_t *p;
    p = (pthread_t *) malloc(count * sizeof(pthread_t));
    
    //инициализация мьютекса
    pthread_mutex_init(&mutex,NULL);
     
    
    //считываем интервалы из файла и создаем потоки
    for(int i=0;i<count;++i)
    {
        n=read(newsockfd, &intervals[i],sizeof(intervals[i]));
        //если при чтении возникла ошибка - завершаем работу
        if (n < 0) {
          perror(NULL);
          close(sockfd);
          close(newsockfd);
          exit(1);
        }
        pthread_create(&p[i],NULL,thread_job,&intervals[i]);
    }

    for(int i=0; i < count ; ++i)
        pthread_join(p[i],NULL);

    pthread_mutex_destroy(&mutex);
   // write(newsockfd,&sum,sizeof(sum));
    printf("Count of divisors = %lld\n", sum);

    //освобождаем выделенную память
    free(p);
    free(intervals);
    close(sockfd);
    close(newsockfd);
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
