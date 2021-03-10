#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>


static int re_compare(char *str, char *pattern);
static int input_valid(char *str);


static int re_compare(char *str, char *pattern)
{
    regex_t re;
    int result;
    result = 0;
    if (regcomp(&re, pattern, 0) !=0)
    {
        printf("error\n");
    }
    if (regexec(&re, str,0, NULL, 0)==0)
    {
        printf("compare is 0\n");
        result = 1;
    }
    regfree(&re);
    return result;
}

////static int regexMatch(char * str, char * regexPattern) {
////    regex_t regex;
////    int match = FALSE;
////    // compile the regex
////    if(regcomp(&regex, regexPattern, REG_EXTENDED)){
////        return FALSE;
////    }
////    // execute the regex
////    if(regexec(&regex, str, 0, NULL, 0) == 0) {
////        match = TRUE;
////    }
////    // free the regex
////    regfree(&regex);
////    return match;
////}

static int input_valid(char *str)
{
    char * p1 = "^\{{1}\}{1}$";
    if (re_compare(str,p1))
    {
        return 1;
    }

    return 0;
}

char* delete_space(char *str)
{
    int i=0, j=0;
    char* str_c = (char*)malloc((strlen(str) + 1) * sizeof(char));
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != ' ')
            str_c[j++] = str[i];
    }
    str_c[j] = '\0';
    return str_c;
}


int main()
{
    char *s1 = "{1, 2, 3, 4, five}";
    char *s2 = "{10  , 9   , 8,    7,    6   ,5 ,    4,3,2,1}";
    char *str = "{ }";
    s2 = delete_space(s2);
    s1 = delete_space(s1);
    str = delete_space(str);
    if (!input_valid(s1))
    {
        printf("%s\n", s1);
    }
    if (!input_valid(s2))
    {
        printf("%s\n", s2);
    }
    if (!input_valid(str))
    {
        printf("%s\n", str);
    }

    return 0;
}


