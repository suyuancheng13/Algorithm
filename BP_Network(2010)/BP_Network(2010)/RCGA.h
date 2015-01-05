#pragma once
#include<vector>
#include"chromosome.h"
using namespace std;


/*
*����ֻ�����Ż����ֵ�����Խ���������ת��������Сֵ
*/
class CBackProp;
class RCGA
{
public:
	RCGA(CBackProp &_bp):popSize(0),best_index(-1),worst_index(-1),crossoverRate(0),mutationRate(0),bp(_bp){};
	~RCGA(){};
	
public:   /*Functions*/
	void initialRCGA(int _popSize,int _genNum,int _maxGeneration,double _crossoverRate,double _mutationRate, double _topLimit,double _downLimit,double _goal);
	void evolution();
	void result(double *);
	void gaOptions(double *,double*);

private:/*Functions*/
   void selectOperator();
	void mutationOperator();
	void crossoverOperator();
	void fitness();
	void updateBestIndividual();

private:/*Variables*/

	int popSize;
	int geneNum;
	int best_index;  
	int worst_index;
	double crossoverRate;            //������
	double mutationRate;            //������
	int maxGeneration;             //���������
	int gen;
	double topLimit;
	double bottomLimit;
	double goal;
	double *input;
	double *target;
public:
    vector<Individual> population;
private:
	vector<Individual>nextPoputation;
public:
	Individual bestindividual;         //��Ѹ���
private:
	Individual current;              //��ǰ����
	Individual tmp;              //��ǰ����
	Individual current1;              //��ǰ����

	CBackProp &bp;
	
};