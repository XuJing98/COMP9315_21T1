//
// Created by jing on 6/03/2021.
//
#include <stdio.h> //预编译符
#include <stdbool.h>
#include <stdlib.h>

/**
 * main method
 * @return 0
 */
 typedef struct intSet{
     int index;
     int array[1];
 }intSet;
int cmp_int(const void* _a , const void* _b)
{
    int* a = (int*)_a;
    int* b = (int*)_b;
    return *a - *b;
}
int main()
{
    //variable name should be meaningful
//    char *string = "{1,2,3}";
//    intSet *intSetpointer;
//    intSetpointer = (intSet *)malloc(sizeof(intSet));
//    sscanf(string,"{%d,%d,%d}",&intSetpointer->array[1],&intSetpointer->array[2],&intSetpointer->array[3]);
//
//    intSetpointer->array[0] = 3;
////    intSetpointer->array[1] = 1;
////    intSetpointer->array[2] = 2;
////    intSetpointer->array[3] = 3;
//    for (int i=0; i<4; i++)
//    {
//        printf("array[%d]=%d\n",i,intSetpointer->array[i]);
//    }

//    int array1[]= {3,1,1,1,1};
//    int array2[] = {2,2,2,2};
//    int array3[9];
//    int n1 = 5;
//    int index1 = 0, index2=0, index3=0;
//    int n2 = 4;
//    int n3 = 9;
//    //sort the array1 and array2
//    qsort(array1,n1,sizeof(array1[0]),cmp_int);
//    qsort(array2,n2,sizeof(array2[0]),cmp_int);
/*
 * calculate union set
 */
//    while(index1 < n1 && index2<n2)
//    {
//        if (array1[index1] <=array2[index2] )
//        {
//            array3[index3] = array1[index1];
//            index1++;
//            index3++;
//        }
//        else{
//            array3[index3] = array2[index2];
//            index2++;
//            index3++;
//        }
//    }
//    while (index1<n1)
//    {
//        array3[index3] = array1[index1];
//        index1++;
//        index3++;
//    }
//    while (index2<n2)
//    {
//        array3[index3] = array2[index2];
//        index2++;
//        index3++;
//    }
//    for(int i=0;i<n3; i++)
//    {
//        printf("%d\n",array3[i]);
//    }
//    printf("n3 = %d\n",n3);
//
///*
// * distinct set array4
// * n4: the length of array4
// */
//    int index_f1 = 0;
//    int index_f2 = 0;
//    int index4 = 0;
//    int n4 = n3;
//    int array4[n4];
//    while (index_f1 < n3)
//    {
//        if(index_f1 == n3-1)
//        {
//            array4[index4] = array3[index_f1];
//            break;
//        }
//        index_f2 = index_f1+1;
//        while (index_f2 < n3 )
//        {
//            if (array3[index_f1] != array3[index_f2])
//            {
//                array4[index4] = array3[index_f1];
//                index4++;
//                index_f1++;
//                break;
//            }
//            else{
//                index_f1++;
//                index_f2++;
//                n4--;
//            }
//        }
//
//
//    }
//
//    for(int i=0;i<n4; i++)
//    {
//        printf("%d\n n=%d\n",array4[i],n4);
//    }






    return 0;
}

