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
    if (regcomp(&re, pattern, REG_EXTENDED) !=0)
    {
        result = 0;
    }
    if (regexec(&re, str,0, NULL, 0)==0)
    {
        result = 1;
    }
    regfree(&re);
    return result;
}

//static int regexMatch(char * str, char * regexPattern) {
//    regex_t regex;
//    int match = FALSE;
//    // compile the regex
//    if(regcomp(&regex, regexPattern, REG_EXTENDED)){
//        return FALSE;
//    }
//    // execute the regex
//    if(regexec(&regex, str, 0, NULL, 0) == 0) {
//        match = TRUE;
//    }
//    // free the regex
//    regfree(&regex);
//    return match;
//}

static int input_valid(char *str)
{
    char * p1 = "\{{1}\s*\}{1}";
    char * p2 = "\{{1}\s*[0-9]+\s*(\s*,\s*[0-9]+\s*)*\}{1}";
    if (re_compare(str,p1) || re_compare(str,p2) )
    {
        return 1;
    }

    return 0;
}


int main()
{
    char *s1 = "{1, 2, 3, 4, five}";
    char *s2 = "{10, 9, 8, 7, 6,5,4,3,2,1}";
    char *s3 = "{ }";
    if (!input_valid(s1))
    {
        printf("111\n");
    }

    if (input_valid(s2))
    {
        printf("222\n");
    }

    if (input_valid(s3))
    {
        printf("333\n");
    }



    return 0;
}


