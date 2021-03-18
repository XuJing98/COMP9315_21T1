#include "postgres.h"
#include "utils/builtins.h"
#include "fmgr.h"
#include "access/hash.h"
#include "regex.h"

PG_MODULE_MAGIC;

typedef struct
{
    int length;
	int array[FLEXIBLE_ARRAY_MEMBER];
}intSet;
static int intset_compare(intSet *a, intSet *b);
static int cmp_int(const void* _a , const void* _b);
static int intset_sup(intSet *a, intSet *b);
static int re_compare(char *str, char *pattern);
static int input_valid(char *str);


// int compare function for qsort function
static int cmp_int(const void* _a , const void* _b)
{
    int* a = (int*)_a;
    int* b = (int*)_b;
    return *a - *b;
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


static int re_compare(char *str, char *pattern)
{
    regex_t re;
    int result;
    result = 0;
    if (regcomp(&re, pattern, REG_EXTENDED) !=0)
    {
        printf("error\n");
    }
    if (regexec(&re, str,0, NULL, 0)==0)
    {
        result = 1;
    }
    regfree(&re);
    return result;
}

static int input_valid(char *str)
{
    int f1=0, f2=0;
    char * p1 = "\\{\\s*[0-9]+\\s*(\\s*,\\s*[0-9]+\\s*)*\\}";
    char * p2 = "\\{\\s*\\}";
    char a1 = '{';
    char a2 = '}';
    for(int i = 0; str[i] != '\0'; ++i)
    {
        if(a1 == str[i])
            ++f1;
        if(a2 == str[i])
            ++f2;
    }
    if (f1 >1)
        return 0;
    if (f2 >1)
        return 0;
    if (re_compare(str,p1)||re_compare(str, p2))
    {
        return 1;
    }

    return 0;
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
    char *token;
    int *array, length = 5, countNum = 0, f=0;
    char *delim = "{, }";
    if (input_valid(str)==0)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type %s: \"%s\"",
						"intSet", str)));


    //initial an array to temporarily store the initial int data
    array = (int *)malloc(sizeof(int)*length);
    token = strtok(str, delim);
    while( token != NULL ) {
        // if the array is full, realloc the new size
        if(countNum == length-1)
        {
            // add 1000 more data size
            length = length+1000;
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
    }
    free(array);



	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(intset_out);

Datum
intset_out(PG_FUNCTION_ARGS)
{
	intSet *intPut = (intSet *) PG_GETARG_POINTER(0);
	int length;
	char *result;
	char str[24];
	length = (intPut->array[0]+2)*24;
    result = (char *)palloc(sizeof(char)*length);
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




PG_FUNCTION_INFO_V1(intset_card);

Datum
intset_card(PG_FUNCTION_ARGS)
{
	intSet *a = (intSet *) PG_GETARG_POINTER(0);

	PG_RETURN_INT32(a->array[0]);
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

PG_FUNCTION_INFO_V1(intset_disjunction);

Datum
intset_disjunction(PG_FUNCTION_ARGS)
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
    for (int i=1; i<countNumb+1; i++)
    {
        f = 0;
        for (int j=1; j<countNuma+1; j++)
        {
            if (a->array[j] == b->array[i])
            {
                f=1;
                break;

            }
        }
        if (f==0)
        {
            array[index] = b->array[i];
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


PG_FUNCTION_INFO_V1(intset_con);

Datum
intset_con(PG_FUNCTION_ARGS)
{
    intSet *a = (intSet *) PG_GETARG_POINTER(1);
    int i, result = -1;
    i = PG_GETARG_INT32(0);
    for (int k=1; k < a->array[0]+1; k++)
    {
        if (i == a->array[k])
        {
            result = 1;
            break;
        }
    }

    PG_RETURN_BOOL(result>0);
}





