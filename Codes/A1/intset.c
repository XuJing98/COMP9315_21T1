#include "postgres.h"
#include "utils/builtins.h"
#include "fmgr.h"
#include "access/hash.h"

PG_MODULE_MAGIC;

typedef struct
{
    int length;
	int array[FLEXIBLE_ARRAY_MEMBER];
}intSet;
int cmp_int(const void* _a , const void* _b);
int cmp_int(const void* _a , const void* _b)
{
    int* a = (int*)_a;
    int* b = (int*)_b;
    return *a - *b;
}


/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(intset_in);

Datum
intset_in(PG_FUNCTION_ARGS)
{
	char *str = PG_GETARG_CSTRING(0);
    intSet *result;
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
    result = (intSet *)palloc(VARHDRSZ+sizeof(int)*(countNum+1));
    SET_VARSIZE(result, VARHDRSZ+sizeof(int)*(countNum+1));
    result->array[0] = countNum;
    for (int i=1; i<countNum+1; i++)
    {
        result->array[i] = array[i-1];
        elog(NOTICE, "index :%d, value:%d", i, result->array[i]);
    }
    free(array);


//	if (sscanf(str, " ( %lf , %lf )", &x, &y) != 2)
//		ereport(ERROR,
//				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
//				 errmsg("invalid input syntax for type %s: \"%s\"",
//						"complex", str)));

	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(intset_out);

Datum
intset_out(PG_FUNCTION_ARGS)
{
	intSet *intPut = (intSet *) PG_GETARG_POINTER(0);
	char result[1024];
	char str[24];
    result[0] = '\0';
	strcat(result, "{");
	if (intPut->array[0] > 0)
    {
        pg_ltoa(intPut->array[1], str);
        strcat(result, str);
	    for(int i = 2; i < intPut->array[0]+1; i++ )
        {
	        strcat(result,",");
	        pg_ltoa(intPut->array[i], str);
	        strcat(result, str);
        }
    }
    strcat(result, "}");

	PG_RETURN_CSTRING(psprintf("%s", result));
}



//PG_FUNCTION_INFO_V1(intset_con);
//
//Datum
//intset_con(PG_FUNCTION_ARGS)
//{
//    intSet *a = (intSet *) PG_GETARG_POINTER(0);
//    int32 i = PG_GETARG_INT32(0);
//    int32 result = -1;
//    for (int32 k=1; k < a->array[0]+1; k++)
//    {
//        if (i == a->array[k])
//        {
//            result = 1;
//            break;
//        }
//    }
//
//    PG_RETURN_BOOL(result>0);
//}
//

PG_FUNCTION_INFO_V1(intset_card);

Datum
intset_card(PG_FUNCTION_ARGS)
{
	intSet *a = (intSet *) PG_GETARG_POINTER(0);

	PG_RETURN_INT32(a->array[0]);
}

// compare the two intSet
// return 0 when a=b, else return -1
static int intset_compare(intSet *a, intSet *b)
{
    int result = 0;
    int f;
    if (a->array[0] != b->array[0])
    {
        result = -1;
    }
    for (int i=1; i<a->array[0]+1; i++)
    {
        f = 0;
        if (result==-1) break;

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

// check if a is a superset of b
//if true return 0 else return -1
static int intset_sup(intSet *a, intSet *b)
{
    int result = 0;
    int f;
    if (a->array[0] < b->array[0])
    {
        result = -1;
    }
    for (int i=1; i<b->array[0]+1; i++)
    {
        f = 0;
        if (result == -1) break;

        for (int j=1; j<a->array[0]+1; j++)
        {
            if (b->array[i] == a->array[j])
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


PG_FUNCTION_INFO_V1(intset_equal);

Datum
intset_equal(PG_FUNCTION_ARGS)
{
    intSet *a = (intSet *) PG_GETARG_POINTER(0);
    intSet *b = (intSet *) PG_GETARG_POINTER(1);


    PG_RETURN_BOOL(intset_compare(a,b) == 0);
}



PG_FUNCTION_INFO_V1(intset_notequal);

Datum
intset_notequal(PG_FUNCTION_ARGS)
{
    intSet *a = (intSet *) PG_GETARG_POINTER(0);
    intSet *b = (intSet *) PG_GETARG_POINTER(1);


    PG_RETURN_BOOL(intset_compare(a,b) == -1);
}


PG_FUNCTION_INFO_V1(intset_superset);

Datum
intset_superset(PG_FUNCTION_ARGS)
{
    intSet *a = (intSet *) PG_GETARG_POINTER(0);
    intSet *b = (intSet *) PG_GETARG_POINTER(1);


    PG_RETURN_BOOL(intset_sup(a,b) == 0);
}


PG_FUNCTION_INFO_V1(intset_subset);

Datum
intset_subset(PG_FUNCTION_ARGS)
{
    intSet *a = (intSet *) PG_GETARG_POINTER(0);
    intSet *b = (intSet *) PG_GETARG_POINTER(1);


    PG_RETURN_BOOL(intset_sup(b,a) == 0);
}


PG_FUNCTION_INFO_V1(intset_intersection);

Datum
intset_intersection(PG_FUNCTION_ARGS)
{
    intSet *a = (intSet *) PG_GETARG_POINTER(0);
    intSet *b = (intSet *) PG_GETARG_POINTER(1);
    intSet *result;
    int countNuma = a->array[0];
    int countNumb = b->array[0];
    int index=0;
    int *array = (int *)malloc(sizeof(int)*(countNuma+countNumb));

    for (int i=1; i<countNuma+1; i++)
    {
        for (int j=1; j<countNumb+1; j++)
        {
            if (a->array[i] == b->array[j])
            {
                array[index] = a->array[i];
                index++;
            }
        }

    }
    qsort(array, index,sizeof(int), cmp_int);
    result = (intSet *)palloc(VARHDRSZ+sizeof(int)*(index+1));
    SET_VARSIZE(result, VARHDRSZ+sizeof(int)*(index+1));
    result->array[0] = index;
    for (int i=1; i<index+1; i++)
    {
        result->array[i] = array[i-1];
    }
    free(array);
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(intset_differ);

Datum
intset_differ(PG_FUNCTION_ARGS)
{
    intSet *a = (intSet *) PG_GETARG_POINTER(0);
    intSet *b = (intSet *) PG_GETARG_POINTER(1);
    intSet *result;
    int countNuma = a->array[0];
    int countNumb = b->array[0];
    int index=0, f=0;
    int *array = (int *)malloc(sizeof(int)*(countNuma+countNumb));

    for (int i=1; i<countNuma+1; i++)
    {
        f = 0;
        for (int j=1; j<countNumb+1; j++)
        {
            if (a->array[i] == b->array[j])
            {
                f=1;
                break;

            }
        }
        if (f==0)
        {
            array[index] = a->array[i];
            index++;
        }

    }
    qsort(array, index,sizeof(int), cmp_int);
    result = (intSet *)palloc(VARHDRSZ+sizeof(int)*(index+1));
    SET_VARSIZE(result, VARHDRSZ+sizeof(int)*(index+1));
    result->array[0] = index;
    for (int i=1; i<index+1; i++)
    {
        result->array[i] = array[i-1];
    }
    free(array);
    PG_RETURN_POINTER(result);
}


PG_FUNCTION_INFO_V1(intset_union);

Datum
intset_union(PG_FUNCTION_ARGS)
{
    intSet *a = (intSet *) PG_GETARG_POINTER(0);
    intSet *b = (intSet *) PG_GETARG_POINTER(1);
    intSet *result;
    int countNuma = a->array[0];
    int countNumb = b->array[0];
    int index=0, f=0;
    int *array = (int *)malloc(sizeof(int)*(countNuma+countNumb));

    for (int i=1; i<countNuma+1; i++)
    {
        f = 0;
        for (int j=1; j<countNumb+1; j++)
        {
            if (a->array[i] == b->array[j])
            {
                f=1;
                break;
            }
        }
        if (f==0)
        {
            array[index] = a->array[i];
            index++;
        }
    }
    for (int j=1; j<countNumb+1; j++)
    {
        array[index] = b->array[j];
        index++;
    }
    qsort(array, index,sizeof(int), cmp_int);
    result = (intSet *)palloc(VARHDRSZ+sizeof(int)*(index+1));
    SET_VARSIZE(result, VARHDRSZ+sizeof(int)*(index+1));
    result->array[0] = index;
    for (int i=1; i<index+1; i++)
    {
        result->array[i] = array[i-1];
    }

    free(array);
    PG_RETURN_POINTER(result);
}





//PG_FUNCTION_INFO_V1(intset_eq);
//
//Datum
//intset_eq(PG_FUNCTION_ARGS)
//{
//	Complex    *a = (Complex *) PG_GETARG_POINTER(0);
//	Complex    *b = (Complex *) PG_GETARG_POINTER(1);
//
//	PG_RETURN_BOOL(complex_abs_cmp_internal(a, b) == 0);
//}

