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
	double crossoverRate;            //������
	double mutationRate;            //������
	int maxGeneration;             //���������
	double topLimit;
	double bottomLimit;
	double goal;
	int t;

    vector<Individual> population;
	vector<Individual>nextPoputation;

	Individual bestindividual;         //��Ѹ���
	Individual worstindividual;         //������
	Individual current;              //��ǰ����
	Individual tmp;              //��ǰ����
	Individual current1;              //��ǰ����
	Individual currentbest;          //��ǰ��Ѹ���
};