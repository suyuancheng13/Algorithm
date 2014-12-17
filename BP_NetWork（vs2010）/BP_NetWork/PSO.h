#pragma once
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#include<time.h>
#include<vector>
#include"Particle.h"
using namespace std;
#define Pi 3.1415926
/*
* particle swarm class
*/
class CBackProp;
class PSO
{
public:
	PSO(int _Dim,int _Number,CBackProp &_bp);
	~PSO();
	/*~PSO()
	{
		delete[] gbest;

		free( particles);
	}*/
public:
	void fitness();
	void limit_PSO();//动态区间的设置
	void initial_PSO(double *_in,double *_tgt);
	void initialBest();
	void update_gbest();
	void update_speed();
	void update_position();
	void update_Interweight();
	void Print();
private:
	int Dim;
	int number;

	//
	double c1;//粒子自身的认识
	double c2;//粒子整个群体的认识

	double w;//惯性权重
	double wmax;//最大惯性权重
	double wmin;//最小惯性权重



	float pmax;//粒子位置上界
	float pmin;//粒子位置下界

	double vmax;//速度最大值

public:
	int T;//当前迭代次数
	int Tmax;//最大迭代次数
	int glbindex;//粒子最好适应度编号
	double *gbest;//全局最优解
	double glbest;//最好适应度
private:
	double m;//介于1，0之间的随机数
	double n;//介于1，0之间的随机数

	//bp 
	CBackProp &bp;

public:
	/*vector<Particle> particles;*/
	Particle *particles;
	//taret from bp network
	double *tgt;
	double *in;
};