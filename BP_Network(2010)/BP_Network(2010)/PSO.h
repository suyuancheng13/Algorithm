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
	void limit_PSO();//��̬���������
	void initial_PSO(double *_in,double *_tgt);
	void initialBest(bool _initial);
	void update_gbest();
	void update_speed();
	void update_position();
	void update_Interweight();
	void Print();
private:
	int Dim;
	int number;

	//
	double c1;//�����������ʶ
	double c2;//��������Ⱥ�����ʶ

	double w;//����Ȩ��
	double wmax;//������Ȩ��
	double wmin;//��С����Ȩ��



	float pmax;//����λ���Ͻ�
	float pmin;//����λ���½�

	double vmax;//�ٶ����ֵ

public:
	int T;//��ǰ��������
	int Tmax;//����������
	int glbindex;//���������Ӧ�ȱ��
	int glbindex1;//���������Ӧ�ȱ��
	int glbindex2;//���������Ӧ�ȱ��
	int GLBINDEX;
	double *gBest;//ȫ�����Ž�
	double *gbest[3];//ȫ�����Ž�
	double glbest;//�����Ӧ��
	double glbest0;//�����Ӧ��
	double glbest1;
	double glbest2;
private:
	double m;//����1��0֮��������
	double n;//����1��0֮��������
	FILE *out;
	//bp 
	CBackProp &bp;

public:
	/*vector<Particle> particles;*/
	Particle *particles;
	//taret from bp network
	double *tgt;
	double *in;
};