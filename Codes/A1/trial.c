//
// Created by jing on 6/03/2021.
//
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct intSet{
     int index;
     int array[1];
}intSet;
int cmp_int(const void* _a , const void* _b);


int cmp_int(const void* _a , const void* _b)
{
    int* a = (int*)_a;
    int* b = (int*)_b;
    return *a - *b;
}

int main()
{
    char str[1024] = "{1, 2  , 3,4 ,11, 22, 1,1, 100,2,3}";
    char *delim = "{, }";
    char *token;
    int *array, length = 2, countNum = 0, f=0;
    //initial an array to temporarily store the initial int data
    array = (int *)malloc(sizeof(int)*length);
    token = strtok(str, delim);
    while( token != NULL ) {
        // if the array is full, realloc the new size
        if(countNum == length-1)
        {
            length = length*2;
            array = realloc(array,sizeof(int)*length);
        }
        // check if the value is distinct
        for (int i=0; i<countNum; i++) {
            if (array[i] == atoi(token)) {
                f = 1;
                break;
            }
        }
        if (!f) {
            array[countNum] = atoi(token);
            countNum++;
        }
        token = strtok(NULL, delim);
        f = 0;
    }
    // sort the value
    qsort(array, countNum,sizeof(int), cmp_int);


    for(int i=0; i<countNum; i++)
    {
        printf("%d\n", array[i]);
    }
    printf("count = %d",countNum);

    free(array);


}

