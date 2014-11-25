//
//  TrieTree.c
//  Algorithm
//
//  Created by Suyuancheng on 14-10-8.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "TrieTree.h"
#include<stdlib.h>
#include<string.h>
TrieNode* initialTrie()
{
    TrieNode *root = (TrieNode *)malloc(sizeof(TrieNode));
    if(NULL!=root)
        return root;
    else {
        return NULL;
    }
}
int insertWord(TrieNode *root,char word[])
{
    
    if(!root)
        return 0;//present fail
    int index = 0;
    int len = strlen(word);
    TrieNode *_troot = root;
    
    for (index = 0; index<len; index++) {
        if(!_troot->childs[word[index]-'a'])
        {
             _troot->childs[word[index]-'a'] = (TrieNode*)malloc(sizeof(TrieNode));
        }
         _troot = _troot->childs[word[index]-'a'];
        if(index == len-1)//the last letter marked the end sign of the word
        {
            if(_troot->complete)
            {
                _troot->hot++;//the  requency of this word
            }
            else {
                 _troot->complete = 1;
            }
           
        }
    }
    return 1;//present success
    
}
/*
 return value is the hot frequency
 */

int searchWord(TrieNode *root,char word[])
{
    if(!root)
        return 0;//present fail
    int index = 0;
    int len = strlen(word);
    TrieNode *_troot = root;
    
    for ( index = 0; index<len; index++) {
        if(!_troot->childs[word[index]-'a'])
        {
            return 0;//present not found!
        }
        _troot = _troot->childs[word[index]-'a'];
        if(index == len-1)//the last letter marked the end sign of the word
        {
            if(_troot->complete)
            {
                return _troot->hot;
            }
            else {//insert the new word if not found the word
//                _troot->complete=1;
//               return  _troot->hot++;
        
            }
                       
        }
    }
    return 0;//present success
}