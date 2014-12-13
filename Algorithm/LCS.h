//
//  LCS.h
//  Algorithm
//
//  Created by Suyuancheng on 14-12-12.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

/*
 Longest Common Subsequence (LCS)
 */
#ifndef Algorithm_LCS_h
#define Algorithm_LCS_h


#define Type char
enum{EQUAL=1,UP,LEFT};

int** LCS(Type * Seq1,Type *Seq2);
void freeMem(int **c,int len);
void ConstructLCS(Type *Seq1,int i,int j,int **b);
#endif
