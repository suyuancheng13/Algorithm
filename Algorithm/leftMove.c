//
//  leftMove.c
//  Algorithm
//
//  Created by Suyuancheng on 14-9-29.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//


#include <stdio.h>
#include <string.h>
#include "leftMove.h"
/*
 TODO:reverse the string
 */
static void strrev(char *string)
{
    int len = strlen(string);
    int i=0,j=len-1;
    while (i<j) {
        string[i]+=string[j];
        string[j]= string[i]-string[j];
        string[i]= string[i]-string[j];
        i++;
        j--;
    }
    string[len]='\0';
}
/*
 (XY)=>>(YX):
 step1:(XY)'=>>Y'X'
 step2:Y''X''=>>(YX)
 param string: the source string to move left
 param offset: the positions to move left
 */
void leftmove_string(char *string,int offset)//循环右移offset位
{
    
    strrev(string);
    char temp[strlen(string)];
    strncpy(temp, string, offset);
    temp[offset]='\0';
    strrev(temp);
    memmove(string, string+offset, strlen(string)-offset);
    string[strlen(string)-offset]='\0';
    strrev(string);
    strcat(temp, string);
    printf("%s\n",temp);
}