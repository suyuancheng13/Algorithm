//#pragma once
//#include "Afxtempl.h"
//#define variablenum 14
//
//class CMVSOGA
//{
//public:
//	CMVSOGA();
//	~CMVSOGA();
//	void selectionoperator();//ѡ�����
//	void crossoveroperator();//��������
//	void mutationoperator();//�������
//
//	void initialpopulation(int, int ,double ,double,double *,double *);           //��Ⱥ��ʼ��
//	void generatenextpopulation();          //������һ����Ⱥ
//	void evaluatepopulation();           //���۸��壬����Ѹ���
//	void calculateobjectvalue();          //����Ŀ�꺯��ֵ
//	void calculatefitnessvalue();          //������Ӧ�Ⱥ���ֵ
//	void findbestandworstindividual();         //Ѱ����Ѹ����������
//	void performevolution();   
//	void GetResult(double *);
//	void GetPopData(CList <double,double>&);
//	void SetFitnessData(CList <double,double>&,CList <double,double>&,CList <double,double>&);
//private:
//	struct individual
//	{
//		double chromosome[variablenum];         //Ⱦɫ����볤��Ӧ��Ϊ�����ĸ���
//		double value;         
//		double fitness;             //��Ӧ��
//	};
//	double variabletop[variablenum];         //����ֵ
//	double variablebottom[variablenum];         //����ֵ
//	int popsize;              //��Ⱥ��С
//	// int generation;              //������
//	int best_index;  
//	int worst_index;
//	double crossoverrate;            //������
//	double mutationrate;            //������
//	int maxgeneration;             //���������
//	struct individual bestindividual;         //��Ѹ���
//	struct individual worstindividual;         //������
//	struct individual current;              //��ǰ����
//	struct individual tmp;              //��ǰ����
//	struct individual current1;              //��ǰ����
//	struct individual currentbest;          //��ǰ��Ѹ���
//	CList <struct individual,struct individual &> population;   //��Ⱥ
//	CList <struct individual,struct individual &> newpopulation;  //����Ⱥ
//	CList <double,double> cfitness;          //�洢��Ӧ��ֵ
//	//����ʹ�����������һ���ṹ��????��Ҫ�������Ⱥ����������ʡ�ռ䡣
//};
