#pragma once
#include<vector>
using namespace std;
class Individual
{
public:
	Individual(){}
	Individual(int _geneNum):geneNum(_geneNum),fitness(
		0),weight(0)
	{	}
	void initial(double _topLimit,double _downLimit);

public:/*Variables*/
	vector<double>chromosome;
	double fitness;
	double weight;
private:
	int geneNum;
	double topLimit;
	double bottomLimit;
};

class RCGA
{
public:
	RCGA():popSize(0),best_index(-1),worst_index(-1),crossoverRate(0),mutationRate(0){};
	~RCGA(){};
	
public:   /*Functions*/
	void initialRCGA(int _popSize,int _genNum,int _maxGeneration,double _crossoverRate,double _mutationRate, double _topLimit,double _downLimit,double _goal);
	void evolution();
	void result(double *);

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
	double topLimit;
	double bottomLimit;
	double goal;
	int t;

    vector<Individual> population;
	vector<Individual>nextPoputation;

	Individual bestindividual;         //最佳个体
	Individual worstindividual;         //最差个体
	Individual current;              //当前个体
	Individual tmp;              //当前个体
	Individual current1;              //当前个体
	Individual currentbest;          //当前最佳个体
};