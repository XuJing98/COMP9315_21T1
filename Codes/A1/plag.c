//
// Created by jing on 18/03/2021.
//

PG_FUNCTION_INFO_V1(intset_in);

Datum
intset_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    int *eleArray;
    int length = 2;
    int elember = 0;
    int whe =0;
    eleArray = malloc(sizeof(int)*length);
    for (int i=0; i<a->intset[0];i++){
	whe =0;
               }



    PG_RETURN_POINTER(result);
}


PG_FUNCTION_INFO_V1(intset_out);//delcare function

Datum
intset_out(PG_FUNCTION_ARGS)
{

    intSet *paramSet = (intSet *) PG_GETARG_POINTER(0);
    //in order to test,using fixed length.
    char strNum[32];//something can be changed.
    char result[1024];

    //SET
    SET_VARSIZE(result ,VARHDRSZ+(sizeof(char)*(1024)));
    result[0]='\0';
    strcat(result,"{");
    //int len=paramSet->intset[0];
    //char *reslut=
    //int element=1;
    if (paramSet->intset[0]>0)
    {
        pg_ltoa(paramSet->intset[1],strNum);
        strcat(result,strNum);
        for(int i=1;i<paramSet->intset[0];i++)
        {
            strcat(result,",");
            pg_itoa(paramSet->intset[i+1],strNum);
            strcat(result,strNum);
            //element=element+2;
        }
    }
    strcat(result,"}");
    PG_RETURN_CSTRING(psprintf("%s",result));
}