//
//  TopKNum.c
//  Algorithm
//
//  Created by Suyuancheng on 14-9-30.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "TopKNum.h"
static void siftDown(int num[],int start,int heapSize)
{
    int i =0;
    int temp = num[start];
    for(i=2*start+1;i<=heapSize;i=2*i+1)//left child
    {
        if(i<heapSize&&num[i]>num[i+1])i++;//find the more little element
        if(temp<num[i])break;
        else {
            num[start]=num[i];
            start =  i;
        }
    }
    num[start]= temp;
}
static void createMinHeap(int num[],int heapSize)
{
    int i =0;
    for (i=(heapSize-2)/2; i>=0; i--) {
        siftDown(num, i, heapSize-1);
    }
}

static void print(int num[],int size)
{
    for(int i=0;i<size;i++)
    {
        printf("%d,",num[i]);
    }
    printf("\n");
}
/*
 TODO: Use a min heap to find the top k numbers
 The min heap just has k numbers, then compare the N_Knumbers in array whith the top elements
 in the heap, if the heap top more little then exchange otherwise do nothing.
 
 param num:the number array to find the top k numbers
 param k: the top K 
 param result: the top k numbers
 */
void TopKnum(int num[],int size,int K,int result[])
{
    if(K<=0||K>size)
        return;
    int i=0;
    createMinHeap(num, K);//create a min heap with K elements
    print(num,K);
    for (i=K; i<size; i++) {
        if(num[0]<num[i])//exchange the heap top with the more little number in array
        {
            num[0]^=num[i];
            num[i]=num[0]^num[i];
            num[0]=num[0]^num[i];
            siftDown(num, 0,K-1);
        }
         print(num,K);
    }
    for (i=0; i<K; i++) {
        result[i]=num[i];
    }
}