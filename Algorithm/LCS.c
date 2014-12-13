//
//  LCS.cpp
//  Algorithm
//
//  Created by Suyuancheng on 14-12-12.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LCS.h"
/*
 * TODO:calculate the length of array
 * CAUTION: just for string now
 */
static int getLength(Type *data)
{
   int len = strlen(data);
    return len;
}
int** LCS(Type * Seq1,Type *Seq2)
{
    int len1 = getLength(Seq1);
    int len2 = getLength(Seq2);
    int **c;
    int **b;
    
    /*
     *initial the lcs length array
     */
    c = (int**)malloc(sizeof(int*)*(len1+1));
    b = (int**)malloc(sizeof(int*)*(len1+1));

    for (int i=0; i<=len1; i++) {
         c[i] = (int*)malloc(sizeof(int)*(len2+1));
         b[i] = (int*)malloc(sizeof(int)*(len2+1));

    }
    for (int i=0; i<=len1; i++) {
        memset(c[i], 0, len2+1);
        memset(b[i], 0, len2+1);

    }
 
    /*           0                          if i=0 or j=0
     *  c[i,j]=  c[i-1,j-1]+1               if seq1[i]=seq2[j]
     *           max(c[i-1,j],c[i,j-1])     if seq1[i]!= seq2[j]
     */          
    for (int i=1; i<=len1; i++) {
        for (int j=1 ; j<=len2; j++) {
           
            if(Seq1[i-1] == Seq2[j-1])//index of array start from 0
            {
                c[i][j]=c[i-1][j-1] + 1;
                b[i][j]=EQUAL;
            }
            else if(c[i-1][j]>= c[i][j-1]){
                c[i][j] = c[i-1][j];
                b[i][j]=LEFT;
                
            }
            else {
                c[i][j] = c[i][j-1];
                b[i][j]=UP;
                  
            }
        }
    }
    ConstructLCS(Seq1, len1, len2,b);
    return c;
}
void ConstructLCS(Type *Seq1,int i,int j,int **b)
{
    if (i == 0 || j== 0 )
        return;
    if(EQUAL == b[i][j])
    {
        ConstructLCS(Seq1, i-1, j-1, b);
        printf("%c",Seq1[i-1]);
    }
    else if(UP==b[i][j]){
        ConstructLCS(Seq1, i, j-1, b);
    }
    else if(LEFT == b[i][j]){
        ConstructLCS(Seq1, i-1, j, b);
    }
    
    
}
void freeMem(int **c,int len)
{
    for (int i=0; i<=len; i++) {
        free(c[i]);
    }
    free(c);
}