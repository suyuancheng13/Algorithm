//
//  TrieTree.h
//  Algorithm
//
//  Created by Suyuancheng on 14-10-8.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#ifndef Algorithm_TrieTree_h
#define Algorithm_TrieTree_h
typedef struct TrieNode
{
    int complete;
    int hot;
    char letter;
    struct TrieNode *childs[26];//26 characters
}TrieNode;
TrieNode* initialTrie();
int insertWord(TrieNode *root,char word[]);
int searchWord(TrieNode *root,char word[]);

#endif
