#include "stdafx.h"
//#include "vld.h"
#include "CMVSOGA.h"
#include "math.h"
#include "stdlib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMVSOGA.cpp
CMVSOGA::CMVSOGA()
{
	best_index=0;  
	worst_index=0;
	crossoverrate=0;            //交叉率
	mutationrate=0;            //变异率
	maxgeneration=0;
}
CMVSOGA::~CMVSOGA()
{
	best_index=0;  
	worst_index=0;
	crossoverrate=0;            //交叉率
	mutationrate=0;            //变异率
	maxgeneration=0;
	population.RemoveAll();   //种群
	newpopulation.RemoveAll();  //新种群
	cfitness.RemoveAll(); 
}
void CMVSOGA::initialpopulation(int ps, int gen ,double cr ,double mr,double *xtop,double *xbottom)  //第一步，初始化。
{
	//应该采用一定的策略来保证遗传算法的初始化合理，采用产生正态分布随机数初始化？选定中心点为多少？
	int i,j;
	popsize=ps;
	maxgeneration=gen;
	crossoverrate=cr;
	mutationrate =mr;
	for (i=0;i<variablenum;i++)
	{
		variabletop[i] =xtop[i];
		variablebottom[i] =xbottom[i];
	}
	//srand( (unsigned)time( NULL ) );  //寻找一个真正的随机数生成函数。
	for(i=0;i<popsize;i++)
	{ 
		for (j=0;j<variablenum ;j++)
		{
			current.chromosome[j]=double(rand()%10000)/10000*(variabletop[j]-variablebottom[j])+variablebottom[j];
		}
		current.fitness=0;
		current.value=0;
		population.InsertAfter(population.FindIndex(i),current);//除了初始化使用insertafter外,其他的用setat命令。
	}
}
void CMVSOGA::generatenextpopulation()//第三步，生成下一代。
{
	//srand( (unsigned)time( NULL ) );
	selectionoperator();
	crossoveroperator();
	mutationoperator();
}
//void CMVSOGA::evaluatepopulation()   //第二步，评价个体，求最佳个体
//{
// calculateobjectvalue();
// calculatefitnessvalue();   //在此步中因该按适应度值进行排序.链表的排序.
// findbestandworstindividual();
//}
void CMVSOGA:: calculateobjectvalue()  //计算函数值，应该由外部函数实现。主要因为目标函数很复杂。
{
	int i,j;
	double x[variablenum];
	for (i=0; i<popsize; i++)
	{
		current=population.GetAt(population.FindIndex(i));  
		current.value=0;
		//使用外部函数进行，在此只做结果的传递。
		for (j=0;j<variablenum;j++)
		{
			x[j]=current.chromosome[j];
			current.value=current.value+(j+1)*pow(x[j],4);
		}
		////使用外部函数进行，在此只做结果的传递。
		population.SetAt(population.FindIndex(i),current);
	}
}
/*
*按变异概率来变异
*对于浮点数编码，变异算子的选择具有决定意义。
*需要guass正态分布函数，生成方差为sigma，均值为浮点数编码值c。
*/
void CMVSOGA::mutationoperator()
{
	// srand((unsigned int) time (NULL));
	int i,j;
	double r1,r2,p,sigma;//sigma高斯变异参数

	for (i=0;i<popsize;i++)
	{
		current=population.GetAt(population.FindIndex(i));

		//生成均值为current.chromosome，方差为sigma的高斯分布数
		for(j=0; j<variablenum; j++)
		{   
			r1 = double(rand()%10001)/10000;
			r2 = double(rand()%10001)/10000;
			p = double(rand()%10000)/10000;
			if(p<mutationrate)
			{
				double sign;
				sign=rand()%2;
				sigma=0.01*(variabletop[j]-variablebottom [j]);
				//高斯变异
				if(sign)
				{
					current.chromosome[j] = (current.chromosome[j] 
					+ sigma*sqrt(-2*log(r1)/0.4323)*sin(2*3.1415926*r2));
				}
				else
				{
					current.chromosome[j] = (current.chromosome[j] 
					- sigma*sqrt(-2*log(r1)/0.4323)*sin(2*3.1415926*r2));
				}
				/*
				*Reset the chromosome
				*/
				if (current.chromosome[j]>variabletop[j])
				{
					current.chromosome[j]=double(rand()%10000)/10000*(variabletop[j]-variablebottom[j])+variablebottom[j];
				}
				if (current.chromosome[j]<variablebottom [j])
				{
					current.chromosome[j]=double(rand()%10000)/10000*(variabletop[j]-variablebottom[j])+variablebottom[j];
				}
			}
		}
		population.SetAt(population.FindIndex(i),current);
	}
}
/*
 *从当前个体中按概率选择新种群,应该加一个复制选择,提高种群的平均适应度
 */
void CMVSOGA::selectionoperator()  {
	int i,j,pindex=0;
	double p,pc,sum;
	i=0;
	j=0;
	pindex=0;
	p=0;
	pc=0;
	sum=0.001;
	newpopulation.RemoveAll();
	cfitness.RemoveAll();
	//链表排序 ---冒泡排序
	// population.SetAt (population.FindIndex(0),current); //多余代码
	for (i=1;i<popsize;i++)
	{ 
		
		for(j=0;j<i;j++)   //从小到大用before排列。
		{
			current1=population.GetAt(population.FindIndex(j));//临时借用变量
			current=population.GetAt(population.FindIndex(j+1));			
			if(current.fitness<=current1.fitness)  
			{
				population.InsertBefore(population.FindIndex(j),current);
				population.RemoveAt(population.FindIndex(j+1));
				
			}
		}
		//  m=population.GetCount();
	}

	for(i=0;i<popsize;i++)//求适应度总值，以便归一化,是已经排序好的链。
	{
		current=population.GetAt(population.FindIndex(i)); //取出来的值出现问题.
		sum+=current.fitness;
	}

	for(i=0;i<popsize; i++)//归一化
	{
		current=population.GetAt(population.FindIndex(i)); //population 有值,为什么取出来的不正确呢??
		//current.fitness=current.fitness/sum;
		cfitness.InsertAfter (cfitness .FindIndex(i),current.fitness/sum);
	}

	for(i=1;i<popsize; i++)//概率值从小到大;
	{
		double fitness=cfitness.GetAt (cfitness.FindIndex(i-1))
			+cfitness.GetAt(cfitness.FindIndex(i));  //计算累计概率
		cfitness.SetAt (cfitness .FindIndex(i),fitness);
		/*population.SetAt(population.FindIndex(i),current);*/
	}
	for (i=0;i<popsize;)//轮盘赌概率选择。本段还有问题。
	{
		p=double(rand()%999)/1000+0.0001;  //随机生成概率
		pindex=0;  //遍历索引
		pc=cfitness.GetAt(cfitness.FindIndex(0));  //为什么取不到数值???20060910
		while(p>pc&&pindex<popsize) //问题所在。
		{
			pc=cfitness.GetAt(cfitness .FindIndex(pindex));
			pindex++;
		}
		//必须是从index~popsize，选择高概率的数。即大于概率p的数应该被选择，选择不满则进行下次选择。
		for (j=popsize-1;j<pindex&&i<popsize;j--)
		{
			newpopulation.InsertAfter (newpopulation.FindIndex(0),
				population.GetAt (population.FindIndex(j)));
			i++;
		}
	}
	for(i=0;i<popsize; i++)
	{
		population.SetAt (population.FindIndex(i),
			newpopulation.GetAt (newpopulation.FindIndex(i)));
	}

	newpopulation.RemoveAll();
}

//current   变化后，以上没有问题了。
/*
 //非均匀算术线性交叉，浮点数适用,alpha ,beta是(0，1)之间的随机数
	//对种群中两两交叉的个体选择也是随机选择的。可取beta=1-alpha;
	//current的变化会有一些改变。
*/
void CMVSOGA:: crossoveroperator()  
{
	int i,j;
	double alpha,beta;
	CList <int,int> index;
	int point,temp;
	double p;
	// srand( (unsigned)time( NULL ) );
	for (i=0;i<popsize;i++)//生成序号
	{
		index.InsertAfter (index.FindIndex(i),i);
	}
	for (i=0;i<popsize;i++)//打乱序号
	{
		point=rand()%(popsize-1);
		temp=index.GetAt(index.FindIndex(i));
		index.SetAt(index.FindIndex(i),
			index.GetAt(index.FindIndex(point)));  
		index.SetAt(index.FindIndex(point),temp);
	}
	for (i=0;i<popsize-1;i+=2)
	{//按顺序序号,按序号选择两个母体进行交叉操作。
		p=double(rand()%10000)/10000.0;
		if (p<crossoverrate)
		{   
			alpha=double(rand()%10000)/10000.0;
			beta=double(rand()%10000)/10000.0;
			
			tmp = current=population.GetAt(population.FindIndex(index.GetAt(index.FindIndex(i))));
			current1=population.GetAt(population.FindIndex(index.GetAt(index.FindIndex(i+1))));//临时使用current1代替
			for(j=0;j<variablenum;j++)			
			{ 
				//交叉	
	    			current.chromosome[j]=(alpha)*current.chromosome[j]+
						(1-alpha)*current1.chromosome[j];	
					 current1.chromosome[j]=(alpha)*current1.chromosome[j]+
						(1-alpha)*tmp.chromosome[j];	
				
				if (current.chromosome[j]>variabletop[j])  //判断是否超界.
				{
					current.chromosome[j]=double(rand()%10000)/10000*(variabletop[j]-variablebottom[j])+variablebottom[j];
				}
				if (current.chromosome[j]<variablebottom [j])
				{
					current.chromosome[j]=double(rand()%10000)/10000*(variabletop[j]-variablebottom[j])+variablebottom[j];
				}
				
			}
			//回代	
			newpopulation.setAt  (newpopulation.FindIndex(i),current);
			newpopulation.setAt  (newpopulation.FindIndex(i+1),current1);
		}

	}
	ASSERT(newpopulation.GetCount()==popsize);
	for (i=0;i<popsize;i++)
	{
		population.SetAt (population.FindIndex(i),
			newpopulation.GetAt (newpopulation.FindIndex(i)));
	}
	newpopulation.RemoveAll();
	index.RemoveAll();
}
void CMVSOGA:: findbestandworstindividual( )  
{
	int i;
	bestindividual=population.GetAt(population.FindIndex(best_index));
	worstindividual=population.GetAt(population.FindIndex(worst_index));
	for (i=1;i<popsize; i++)
	{
		current=population.GetAt(population.FindIndex(i));
		if (current.fitness>bestindividual.fitness)
		{
			bestindividual=current;
			best_index=i;
		}
		else if (current.fitness<worstindividual.fitness)
		{
			worstindividual=current;
			worst_index=i;
		}
	}
	population.SetAt(population.FindIndex(worst_index),
		population.GetAt(population.FindIndex(best_index)));
	//用最好的替代最差的。
	if (maxgeneration==0)
	{
		currentbest=bestindividual;
	}
	else
	{
		if(bestindividual.fitness>=currentbest.fitness)
		{
			currentbest=bestindividual;
		}
	}
}
void CMVSOGA:: calculatefitnessvalue() //适应度函数值计算，关键是适应度函数的设计
	//current变化，这段程序变化较大，特别是排序。
{
	int  i;
	double temp;//alpha,beta;//适应度函数的尺度变化系数
	double cmax=100;
	for(i=0;i<popsize;i++)
	{
		current=population.GetAt(population.FindIndex(i));
		if(current.value<cmax)
		{
			temp=cmax-current.value;
		}
		else
		{
			temp=0.0;
		}
		/*
		if((population[i].value+cmin)>0.0)
		{temp=cmin+population[i].value;}
		else
		{temp=0.0;
		}
		*/
		current.fitness=temp;
		population.SetAt(population.FindIndex(i),current); 
	}
}
void CMVSOGA:: performevolution() //演示评价结果,有冗余代码,current变化，程序应该改变较大
{
	if (bestindividual.fitness>currentbest.fitness)
	{
		currentbest=population.GetAt(population.FindIndex(best_index));
	}
	else
	{
		population.SetAt(population.FindIndex(worst_index),currentbest);
	}
}
void CMVSOGA::GetResult(double *Result)
{
	int i;
	for (i=0;i<variablenum;i++)
	{
		Result[i]=currentbest.chromosome[i];
	}
	Result[i]=currentbest.value;
}

void CMVSOGA::GetPopData(CList <double,double>&PopData)  
{
	PopData.RemoveAll();
	int i,j;
	for (i=0;i<popsize;i++)
	{
		current=population.GetAt(population.FindIndex(i));
		for (j=0;j<variablenum;j++)
		{
			PopData.AddTail(current.chromosome[j]);
		}
	}
}
void CMVSOGA::SetFitnessData(CList <double,double>&PopData,CList <double,double>&FitnessData,CList <double,double>&ValueData)
{
	int i,j;
	for (i=0;i<popsize;i++)
	{  
		current=population.GetAt(population.FindIndex(i)); //就因为这一句，出现了很大的问题。 
		for (j=0;j<variablenum;j++)
		{
			current.chromosome[j]=PopData.GetAt(PopData.FindIndex(i*variablenum+j));
		}
		current.fitness=FitnessData.GetAt(FitnessData.FindIndex(i));
		current.value=ValueData.GetAt(ValueData.FindIndex(i));
		population.SetAt(population.FindIndex(i),current);
	}
	FitnessData.RemoveAll();
	PopData.RemoveAll();
	ValueData.RemoveAll();
}