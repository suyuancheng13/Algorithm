//
//  main.c
//  Algorithm
//
//  Created by Suyuancheng on 14-9-29.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#include "leftMove.h"
#include "TopKNum.h"
#include <time.h>
int main(int argc, const char * argv[])
{
   // char string[] = "suyuancheng";

   // leftmove_string(string, 4);
  
    /*
     test the top k numbers function
     */
    int num[16];
    int result[6];
    srand(time(NULL));
    for (int i =0 ; i<16; i++) {
        num[i]=rand()%1000;
        printf("%d,",num[i]);
    }
    printf("\n");
    TOPKNumDC(num, 16, 6, result);
    for(int i=0;i<6;i++)
    {
        printf("%d,",result[i]);
    }
    return 0;
}

