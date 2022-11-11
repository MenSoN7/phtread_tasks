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

typedef struct interval {
    long int p;
    long int q;
} interval_t;

int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Неверное количество аргументов\nВведите IP-адрес и две границы интервала\n");
        return -1;
    }

    if (atoi(argv[2]) > atoi(argv[3])) {
        printf("Первый аргумент должен быть меньше второго!\n");
        return -1;
    }
    //структура для размещения адреса сервера
    struct sockaddr_in servaddr;
    int sockfd;
    int n;
    // long long int sum = 0;
    long int p = atoi(argv[2]);
    long int q = atoi(argv[3]);
    long int count_of_numbers = q - p + 1;

    //создаем TCP-сокет
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror(NULL);
        exit(1);
    }
    //перед заполнением обнуляем всю структуру
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; //семейство
    servaddr.sin_port = htons(51000); //хост

    //преобразует адрес в нужную форму и сохраняет в поле структуры
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
        printf("Неверный IP-адрес\n");
        close(sockfd);
        exit(1);
    }
    //устанавливаем соединение через созданный сокет с сокетом сервера, адрес которого в структуре
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    long int count_of_cores = sysconf(_SC_NPROCESSORS_CONF);

    if (count_of_numbers < count_of_cores * 2) {
        interval_t* interval;
        //long int count = 1;
        //write(sockfd, &count, sizeof(count));
        interval->p = p;
        interval->q = q;
        if ((n = write(sockfd, &interval, sizeof(interval))) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        };
        close(sockfd);
        return 0;
    }

    interval_t* intervals;
    intervals = (interval_t*)malloc(count_of_cores * sizeof(interval_t));

    // write(sockfd, &count_of_cores, sizeof(count_of_cores));
    //если левая граница = 1
    if (p == 1) {
        intervals[0].p = p;
        intervals[0].q = (count_of_numbers / count_of_cores);
        if ((n = write(sockfd, &intervals[0], sizeof(intervals[0]))) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        };

        for (int i = 1; i < count_of_cores; ++i) {
            intervals[i].p = i * (count_of_numbers / count_of_cores) + 1;
            if (i != count_of_cores -1)
                intervals[i].q = (i + 1) * (count_of_numbers / count_of_cores);
            else
                intervals[i].q = q;
            if ((n = write(sockfd, &intervals[i], sizeof(intervals[i]))) < 0) {
                perror(NULL);
                close(sockfd);
                exit(1);
            };
        }
        //read(sockfd, &sum, sizeof(sum));
    }

    //если левая граница не равна 1
    else {
        intervals[0].p = p;
        intervals[0].q = p + (count_of_numbers / count_of_cores);
        if ((n = write(sockfd, &intervals[0], sizeof(intervals[0]))) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        };

        for (int i = 1; i < count_of_cores; ++i) {
            // вторая граница интервала  = Q - в случае последней итерации и = p+ (i + 1) *(count_of_numbers / count_of_cores) в других случаях
            intervals[i].p = i * (count_of_numbers / count_of_cores) + 1 + atoi(argv[2]);
            if (i != count_of_cores -1)
                intervals[i].q = p + (i + 1) * (count_of_numbers / count_of_cores);
            else
                intervals[i].q = q;
            if ((n = write(sockfd, &intervals[i], sizeof(intervals[i]))) < 0) {
                perror(NULL);
                close(sockfd);
                exit(1);
            };
        }
        // read(sockfd, &sum, sizeof(sum));
    }

    close(sockfd);
    free(intervals);
    return 0;
}
