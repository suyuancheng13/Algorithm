//
//  TopKNum.h
//  Algorithm
//
//  Created by Suyuancheng on 14-9-30.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#ifndef Algorithm_TopKNum_h
#define Algorithm_TopKNum_h
/*
 TODO: Use a min heap to find the top k numbers
       The min heap just has k numbers, then compare the N_Knumbers in array whith the top elements
        in the heap, if the heap top more little then exchange otherwise do nothing.
 
 param num:the number array to find the top k numbers
 param k: the top K 
 param result: the top k numbers
 */
void TopKnum(int num[],int size,int K,int result[]);
/*
 TODO: use divide-conque algorithm to find the top k numbers
 param num:the number array to find the top k numbers
 param k: the top K 
 param result: the top k numbers
 */
void TOPKNumDC(int num[],int size,int K,int result[]);

#endif
