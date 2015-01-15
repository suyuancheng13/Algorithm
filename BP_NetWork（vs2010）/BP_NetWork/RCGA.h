#pragma once
#include<vector>
#include"chromosome.h"
using namespace std;


/*
*现在只做了优化最大值，可以将函数进行转换后求最小值
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
	double crossoverRate;            //交叉率
	double mutationRate;            //变异率
	int maxGeneration;             //最大世代数
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
	Individual bestindividual;         //最佳个体
private:
	Individual current;              //当前个体
	Individual tmp;              //当前个体
	Individual current1;              //当前个体

	CBackProp &bp;
	
};