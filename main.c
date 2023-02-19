#include <stdlib.h>
#include <stdio.h>
#include <time.h>
static unsigned int number_of_comparisons = 0;    // создаем глобальные переменные кол-ва сравнений и обменов
static unsigned int number_of_exchanges = 0;
static int min = 0;
static int max = 0;

int inc(char operation){                            // функция для измерения кол-ва сравнений и обменов
    if (operation == '=') number_of_exchanges++;     // будем использовать '=' как обозначение обмена
    if (operation == '>') number_of_comparisons++;   // будем использовать '>' как обозначение обмена
    return 1;
}

double absf(double value){                        // модуль для double'ов
    return (value > 0) ? value : -value;
}

void BubbleSort(double *a, int n){                // реализация сортировки пузырьком
    int flag = 1;                                 // ставим флаг для оптимизации (если внутренний цикл ничего не делает => остановка)
    for (int i = 0; i < n; i++){
        flag = 0;
        for (int j = n - 1; j > i; j--){
            inc('>');
            if (absf(a[j]) > absf(a[j - 1])){     // рассматриваем (1)j-ое и (2)j-1-ое числа, если (1) больше (2) меняем их местами,
                double tmp = a[j - 1];           // т.е. самое большое число из диапазона [a[i], a[n-1]] на i-ой операции будет уходить
                a[j - 1] = a[j];                // в начало ("всплывать вверх будет самый большой пузырек")
                a[j] = tmp;
                flag = 1;
                inc('=');
            }
        }
        if (!flag) break;                         // вот и остановка
    }
}

void PartitionSort(double *a, int first, int last){  // реализация сортировки для qsort'а (задание требует неудобного ввода параметров)
    int left = first;                                // создаем левые и правые индексы (позже будет ясно для чего)
    int right = last;
    double pivot = absf(a[(left + right)/2]);        // создаем pivot - эл-т около которого и будем делать partitioning(разбиение)
    do {                                             // наш главный цикл находится именно тут
        while (inc('>') && absf(a[left]) > pivot) {              // находим эл-т больший pivot'а слева
            left++;
        }
        while (inc('>') &&pivot > absf(a[right])){              // находим эл-т меньший  pivot'а справа
            right--;
        }
        inc('>');
        if (left <= right){                          // если левый индекс <= правого индекса, меняем местами найденные числа
            double tmp = a[left];
            a[left] = a[right];
            a[right] = tmp;
            inc('=');
            left++;                                  // инкрементируем левый индекс  и декрементируем правый индекс на единицу
            right--;                                // т.к. значения при них уже стоят на нужных местах, надо идти к след. значениям
        }                                                   // перемещение эл-тов до момента того, левый индекс не превысил правый
    }while (left <= right);                                  // таким образом мы получаем слева от pivot все эл-ты больше pivot
    if (first < right) PartitionSort(a, first, right);      // а справа pivot все эл-ты меньше pivot (для этого и вводили left и right)
    if (left < last) PartitionSort(a, left, last);         // осталось таким же образом отсортировать подмассивы слева и справа
}

void QuickSort(double *a, int n){                 // просто используем наш partitionsort в qsort'е
    PartitionSort(a, 0, n - 1);
}

void printArr(double *a, int n){                  // дополнительная функция для удобного вывода массива
    for (int i = 0; i < n; i++){
        printf("%lf ", a[i]);
    }
}

void randArr(double *a, int n){                   // дополнительная функция для генерации рандомизированного массива
    for (int i = 0; i < n; i++){
        a[i] = ((double)rand()) / RAND_MAX * (max - min) + min;
    }
}

void straightlineArr(double *a, int n){                   // дополнительная функция для генерации упорядоченного массива
    for (int i = 0; i < n; i++){
        a[i] = n - i - ((double)rand()) / RAND_MAX;
    }
}

void reverselineArr(double *a, int n){                   // дополнительная функция для генерации обратно упорядоченного массива
    for (int i = 0; i < n; i++){
        a[i] = i + 1 + ((double)rand()) / RAND_MAX;
    }
}

int main(void){
    srand((unsigned)time(NULL));
    int n, choice;
    printf("Put length of array:\n");
    scanf("%d", &n);
    max = n;
    min = -n;
    double *a = calloc(n, sizeof(double));
    printf("how generate an array: \n 0)random \n 1)ordered \n 2)back ordered\n");
    scanf("%d", &choice);
    switch (choice){
        case 0: randArr(a, n);
            break;
        case 1: straightlineArr(a, n);
            break;
        case 2: reverselineArr(a, n);
            break;
        default: printf("wrong choice, so array will be [0]*n");
            break;
    }
    double *b = calloc(n, sizeof(double));
    for (int i = 0; i < n; i++) b[i] = a[i];
    printf("Non-sorted array:\n");
    printArr(a, n);
    QuickSort(a, n);
    printf("\nSorted array:\n");
    printArr(a, n);
    printf("\nQuicksort:\n number of comparisons: %u\n number of exchanges: %u\n", number_of_comparisons, number_of_exchanges);
    number_of_comparisons = 0;
    number_of_exchanges = 0;
    BubbleSort(b, n);
    printf("Bubblesort:\n number of comparisons: %u\n number of exchanges: %u\n", number_of_comparisons, number_of_exchanges);
    return 0;
}
