//
//  leftMove.h
//  Algorithm
//
//  Created by Suyuancheng on 14-9-29.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#ifndef Algorithm_leftMove_h
#define Algorithm_leftMove_h
/*
 (XY)=>>(YX):
 step1:(XY)'=>>Y'X'
 step2:Y''X''=>>(YX)
 param string: the source string to move left
 param offset: the positions to move left
 */
void leftmove_string(char *string,int offset);

#endif
