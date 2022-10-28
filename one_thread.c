/* Программа считает сумму делителей всех чисел в промежутке от P до Q, которые передаются через аргументы 
Ипользуется 1 поток */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long long int sum = 0; //переменная для подсчета суммы делителей

void number_divisors(long int n); // функция для подсчета суммы делителей числа n

int main(int argc, char *argv[]) {

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

    long int P = atoi(argv[1]);
    long int Q = atoi(argv[2]);

    for (long int i = P ; i <= Q; ++i)
        number_divisors(i);

    printf("Count of divisors = %lld\n", sum);

    return 0;
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
