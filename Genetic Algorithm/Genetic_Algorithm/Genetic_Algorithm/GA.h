//#pragma once
//#include "Afxtempl.h"
//#define variablenum 14
//
//class CMVSOGA
//{
//public:
//	CMVSOGA();
//	~CMVSOGA();
//	void selectionoperator();//选择操作
//	void crossoveroperator();//交换操作
//	void mutationoperator();//变异操作
//
//	void initialpopulation(int, int ,double ,double,double *,double *);           //种群初始化
//	void generatenextpopulation();          //生成下一代种群
//	void evaluatepopulation();           //评价个体，求最佳个体
//	void calculateobjectvalue();          //计算目标函数值
//	void calculatefitnessvalue();          //计算适应度函数值
//	void findbestandworstindividual();         //寻找最佳个体和最差个体
//	void performevolution();   
//	void GetResult(double *);
//	void GetPopData(CList <double,double>&);
//	void SetFitnessData(CList <double,double>&,CList <double,double>&,CList <double,double>&);
//private:
//	struct individual
//	{
//		double chromosome[variablenum];         //染色体编码长度应该为变量的个数
//		double value;         
//		double fitness;             //适应度
//	};
//	double variabletop[variablenum];         //变量值
//	double variablebottom[variablenum];         //变量值
//	int popsize;              //种群大小
//	// int generation;              //世代数
//	int best_index;  
//	int worst_index;
//	double crossoverrate;            //交叉率
//	double mutationrate;            //变异率
//	int maxgeneration;             //最大世代数
//	struct individual bestindividual;         //最佳个体
//	struct individual worstindividual;         //最差个体
//	struct individual current;              //当前个体
//	struct individual tmp;              //当前个体
//	struct individual current1;              //当前个体
//	struct individual currentbest;          //当前最佳个体
//	CList <struct individual,struct individual &> population;   //种群
//	CList <struct individual,struct individual &> newpopulation;  //新种群
//	CList <double,double> cfitness;          //存储适应度值
//	//怎样使链表的数据是一个结构体????主要是想把种群作成链表。节省空间。
//};
