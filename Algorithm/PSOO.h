//
//  PSO.h
//  Algorithm
//
//  Created by Suyuancheng on 14-11-29.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#ifndef Algorithm_PSO_h
#define Algorithm_PSO_h

#define Dim 10//粒子维数
#define number 60//粒子个数

#define Pi 3.1415926

double c1=2;//粒子自身的认识
double c2=2;//粒子整个群体的认识

double w;//惯性权重
double wmax=0.9;//最大惯性权重
double wmin=0.4;//最小惯性权重

int T;//当前迭代次数
int Tmax=1000;//最大迭代次数

float pmax;//粒子位置上界
float pmin;//粒子位置下界

double vmax;//速度最大值


int glbindex;//粒子最好适应度编号
double gbest[Dim];//全局最优解
double glbest;//最好适应度

double m;//介于1，0之间的随机数
double n;//介于1，0之间的随机数

typedef struct{
	double v[Dim];//粒子本身速度
	double position[Dim];//粒子位置
	double pbest[Dim];//粒子本身最优解
    
	double fitness;//粒子适应度
	double bestfitness;//粒子历史最好适应度
    
}particle;

particle particles[number];


void fitness();
void limit_PSO(float max,float min);//动态区间的设置
void initial_PSO();
void update_gbest();
void update_speed();
void update_position();
void update_Interweight();//更新权重
#endif
