//
//  PSO.c
//  Algorithm
//
//  Created by Suyuancheng on 14-11-29.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include "PSO.h"



void fitness()//求适应度
{
	int i,j;
	double sum=0;
	for(i=0;i<number;i++)
	{
		sum=0;
	
		particles[i].fitness=sum;
	}
}

void limit_PSO(float max,float min){//输入粒子的速度和位置信息
//	float a=10,b=-5;
	pmax=max;pmin=min;
    vmax=pmax-pmin;
	
	return;
}

void initial_PSO(){//初始化粒子
	int i,j;
    /*
     initial the velocity,position
     */
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{
			particles[i].v[j]=vmax*1.0*rand()/RAND_MAX;;//粒子的各维速度
			particles[i].position[j]=1.0*rand()/RAND_MAX*(pmax-pmin)+(pmin);//粒子的位置
			particles[i].pbest[j]=particles[i].position[j];
			
		}
	}
    
    /*
     calculate the fitness value
     */
	fitness();
    
    /*
     initial the individual best history particle
     */
		for(i=0;i<number;i++)//寻找最好的适应度的粒子
	{
		particles[i].bestfitness=particles[i].fitness;	
		for(j=0;j<Dim;j++)
            particles[i].pbest[j]=particles[i].position[j];//初始化时的个体最好位置
	}
    
    /*
     initial global best particle
     */
    glbindex=0;//初始化时的粒子群的最好适应度粒子编号
	double s=particles[glbindex].bestfitness;
	for(i=0;i<number;i++)
	{
		if(particles[i].bestfitness<s)
		{
			s=particles[i].bestfitness;
            glbindex=i;	
		}
	}
    
	for(j=0;j<Dim;j++)
    {
        gbest[j]=particles[glbindex].position[j];//初始化时全局最好位置
    }
	glbest=particles[glbindex].bestfitness;
	
}

void update_Interweight(){//更新粒子的惯性权重,线性递减。
	w=wmax-T*(wmax-wmin)/Tmax;
}

void update_speed(){//更新粒子的的速度
	int i,j;
    //	m=(double((double)(rand()%(int)(16384)/(16383.0))));
    //	n=(double((double)(rand()%(int)(16384)/(16383.0))));
	
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{ 
			particles[i].v[j]=w*particles[i].v[j]+c1*1.0*rand()/RAND_MAX*(particles[i].pbest[j]-particles[i].position[j]) +c2*1.0*rand()/RAND_MAX*(gbest[j]-particles[i].position[j]);
		    if(particles[i].v[j]>vmax)
				particles[i].v[j]=vmax;
			if(particles[i].v[j]<-vmax)
				particles[i].v[j]=-vmax;
			
		}
        
	}
	return;
}
void update_position(){//更新粒子的位置
    int i,j;
	for(i=0;i<number;i++){                                          
		for(j=0;j<Dim;j++)
		{	
			particles[i].position[j]=particles[i].position[j]+particles[i].v[j];
			if(particles[i].position[j]>pmax)
				particles[i].position[j]=2*pmax-particles[i].position[j];
			if(particles[i].position[j]<pmin)
				particles[i].position[j]=2*pmin-particles[i].position[j];
			
		}
	}
	
	fitness();
	for(i=0;i<number;i++)
	{
		if(particles[i].fitness<particles[i].bestfitness)//更新粒子的历史个体最好位置
		{
			particles[i].bestfitness=particles[i].fitness;
			for(j=0;j<Dim;j++)
			{
				particles[i].pbest[j]=particles[i].position[j];
			}
        }
	}
	return;
}
void update_gbest(){//更新全体粒子的全局最优位置
	int i;
//    FILE *fp;
	//fp=fopen("result2.txt","a");
    
	for(i=0;i<number;i++)
	{
		if(particles[i].bestfitness<glbest)
		{
            glbindex=i;
            glbest=particles[glbindex].bestfitness;
            for(int j=0;j<Dim;j++)
            {
                gbest[j]=particles[glbindex].pbest[j];
            }
			
		}
	}
    
	return;
}
/*
int main(){
    double sum=0;
	time_t a=0,b=0;
	time(&b);
	srand((unsigned)time(NULL));
	for(int i=0;i<1;i++)
	{
		limit_PSO();
		initial_PSO();
		for(T=0;T<=Tmax;T++)
		{
			update_Interweight();//更新惯性权重
            update_speed();//更新速度
			update_position();//更新位置及个体最优
			update_gbest();//更新全局最优
		}
		sum+=glbest; 
        printf("第%d次，最优值%e最优解\n：",i+1,glbest);
        
	}
	time(&a);
	//printf("%lf\n",difftime(a,b)/500);
	//printf("%.10lf",sum/50);
	
}
*/