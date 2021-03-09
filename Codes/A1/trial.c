//
// Created by jing on 6/03/2021.
//
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
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
static int intset_compare(intSet *a, intSet *b)
{
    int result = 0;
    int f=0;
    if (a->array[0] != b->array[0])
    {
        result = -1;
    }
    for (int i=1; i<a->array[0]+1; i++)
    {
        if (result == -1) break;

        for (int j=1; j<b->array[0]+1; j++)
        {
            if (a->array[i] == b->array[j])
            {
                f = 1;
                break;
            }

        }
        if (f==0)
        {
            result = -1;
            break;
        }

    }
    return result;
}

int main()
{
    char str[1024] = "{1, 2  , 3,4 ,11, 22, 1,1, 100,2,3}";
    char *delim = "{, }";
    char *token;
    intSet *result;
    intSet *result2;
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

    result = (intSet *)malloc(sizeof(int)*(countNum+2));

    result->array[0] = countNum;
    for (int i=1; i<countNum+1; i++)
    {
        result->array[i] = array[i-1];
        printf("index :%d, value:%d\n", i, result->array[i]);
    }

    result2 = (intSet *)malloc(sizeof(int)*(countNum+2));

    result2->array[0] = countNum;
    for (int i=1; i<countNum+1; i++)
    {
        result2->array[i] = array[i-1]+1;
        printf("index :%d, value:%d\n", i, result2->array[i]);
    }



    int t = intset_compare(result, result);
    printf("compare: %d\n",t);
    int t2= intset_compare(result, result2);
    printf("compare: %d\n",t2);
    free(result);
    free(array);




}

