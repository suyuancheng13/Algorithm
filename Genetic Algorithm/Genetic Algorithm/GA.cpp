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
	crossoverrate=0;            //������
	mutationrate=0;            //������
	maxgeneration=0;
}
CMVSOGA::~CMVSOGA()
{
	best_index=0;  
	worst_index=0;
	crossoverrate=0;            //������
	mutationrate=0;            //������
	maxgeneration=0;
	population.RemoveAll();   //��Ⱥ
	newpopulation.RemoveAll();  //����Ⱥ
	cfitness.RemoveAll(); 
}
void CMVSOGA::initialpopulation(int ps, int gen ,double cr ,double mr,double *xtop,double *xbottom)  //��һ������ʼ����
{
	//Ӧ�ò���һ���Ĳ�������֤�Ŵ��㷨�ĳ�ʼ���������ò�����̬�ֲ��������ʼ����ѡ�����ĵ�Ϊ���٣�
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
	//srand( (unsigned)time( NULL ) );  //Ѱ��һ����������������ɺ�����
	for(i=0;i<popsize;i++)
	{ 
		for (j=0;j<variablenum ;j++)
		{
			current.chromosome[j]=double(rand()%10000)/10000*(variabletop[j]-variablebottom[j])+variablebottom[j];
		}
		current.fitness=0;
		current.value=0;
		population.InsertAfter(population.FindIndex(i),current);//���˳�ʼ��ʹ��insertafter��,��������setat���
	}
}
void CMVSOGA::generatenextpopulation()//��������������һ����
{
	//srand( (unsigned)time( NULL ) );
	selectionoperator();
	crossoveroperator();
	mutationoperator();
}
//void CMVSOGA::evaluatepopulation()   //�ڶ��������۸��壬����Ѹ���
//{
// calculateobjectvalue();
// calculatefitnessvalue();   //�ڴ˲�����ð���Ӧ��ֵ��������.���������.
// findbestandworstindividual();
//}
void CMVSOGA:: calculateobjectvalue()  //���㺯��ֵ��Ӧ�����ⲿ����ʵ�֡���Ҫ��ΪĿ�꺯���ܸ��ӡ�
{
	int i,j;
	double x[variablenum];
	for (i=0; i<popsize; i++)
	{
		current=population.GetAt(population.FindIndex(i));  
		current.value=0;
		//ʹ���ⲿ�������У��ڴ�ֻ������Ĵ��ݡ�
		for (j=0;j<variablenum;j++)
		{
			x[j]=current.chromosome[j];
			current.value=current.value+(j+1)*pow(x[j],4);
		}
		////ʹ���ⲿ�������У��ڴ�ֻ������Ĵ��ݡ�
		population.SetAt(population.FindIndex(i),current);
	}
}
/*
*���������������
*���ڸ��������룬�������ӵ�ѡ����о������塣
*��Ҫguass��̬�ֲ����������ɷ���Ϊsigma����ֵΪ����������ֵc��
*/
void CMVSOGA::mutationoperator()
{
	// srand((unsigned int) time (NULL));
	int i,j;
	double r1,r2,p,sigma;//sigma��˹�������

	for (i=0;i<popsize;i++)
	{
		current=population.GetAt(population.FindIndex(i));

		//���ɾ�ֵΪcurrent.chromosome������Ϊsigma�ĸ�˹�ֲ���
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
				//��˹����
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
 *�ӵ�ǰ�����а�����ѡ������Ⱥ,Ӧ�ü�һ������ѡ��,�����Ⱥ��ƽ����Ӧ��
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
	//�������� ---ð������
	// population.SetAt (population.FindIndex(0),current); //�������
	for (i=1;i<popsize;i++)
	{ 
		
		for(j=0;j<i;j++)   //��С������before���С�
		{
			current1=population.GetAt(population.FindIndex(j));//��ʱ���ñ���
			current=population.GetAt(population.FindIndex(j+1));			
			if(current.fitness<=current1.fitness)  
			{
				population.InsertBefore(population.FindIndex(j),current);
				population.RemoveAt(population.FindIndex(j+1));
				
			}
		}
		//  m=population.GetCount();
	}

	for(i=0;i<popsize;i++)//����Ӧ����ֵ���Ա��һ��,���Ѿ�����õ�����
	{
		current=population.GetAt(population.FindIndex(i)); //ȡ������ֵ��������.
		sum+=current.fitness;
	}

	for(i=0;i<popsize; i++)//��һ��
	{
		current=population.GetAt(population.FindIndex(i)); //population ��ֵ,Ϊʲôȡ�����Ĳ���ȷ��??
		//current.fitness=current.fitness/sum;
		cfitness.InsertAfter (cfitness .FindIndex(i),current.fitness/sum);
	}

	for(i=1;i<popsize; i++)//����ֵ��С����;
	{
		double fitness=cfitness.GetAt (cfitness.FindIndex(i-1))
			+cfitness.GetAt(cfitness.FindIndex(i));  //�����ۼƸ���
		cfitness.SetAt (cfitness .FindIndex(i),fitness);
		/*population.SetAt(population.FindIndex(i),current);*/
	}
	for (i=0;i<popsize;)//���̶ĸ���ѡ�񡣱��λ������⡣
	{
		p=double(rand()%999)/1000+0.0001;  //������ɸ���
		pindex=0;  //��������
		pc=cfitness.GetAt(cfitness.FindIndex(0));  //Ϊʲôȡ������ֵ???20060910
		while(p>pc&&pindex<popsize) //�������ڡ�
		{
			pc=cfitness.GetAt(cfitness .FindIndex(pindex));
			pindex++;
		}
		//�����Ǵ�index~popsize��ѡ��߸��ʵ����������ڸ���p����Ӧ�ñ�ѡ��ѡ����������´�ѡ��
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

//current   �仯������û�������ˡ�
/*
 //�Ǿ����������Խ��棬����������,alpha ,beta��(0��1)֮��������
	//����Ⱥ����������ĸ���ѡ��Ҳ�����ѡ��ġ���ȡbeta=1-alpha;
	//current�ı仯����һЩ�ı䡣
*/
void CMVSOGA:: crossoveroperator()  
{
	int i,j;
	double alpha,beta;
	CList <int,int> index;
	int point,temp;
	double p;
	// srand( (unsigned)time( NULL ) );
	for (i=0;i<popsize;i++)//�������
	{
		index.InsertAfter (index.FindIndex(i),i);
	}
	for (i=0;i<popsize;i++)//�������
	{
		point=rand()%(popsize-1);
		temp=index.GetAt(index.FindIndex(i));
		index.SetAt(index.FindIndex(i),
			index.GetAt(index.FindIndex(point)));  
		index.SetAt(index.FindIndex(point),temp);
	}
	for (i=0;i<popsize-1;i+=2)
	{//��˳�����,�����ѡ������ĸ����н��������
		p=double(rand()%10000)/10000.0;
		if (p<crossoverrate)
		{   
			alpha=double(rand()%10000)/10000.0;
			beta=double(rand()%10000)/10000.0;
			
			tmp = current=population.GetAt(population.FindIndex(index.GetAt(index.FindIndex(i))));
			current1=population.GetAt(population.FindIndex(index.GetAt(index.FindIndex(i+1))));//��ʱʹ��current1����
			for(j=0;j<variablenum;j++)			
			{ 
				//����	
	    			current.chromosome[j]=(alpha)*current.chromosome[j]+
						(1-alpha)*current1.chromosome[j];	
					 current1.chromosome[j]=(alpha)*current1.chromosome[j]+
						(1-alpha)*tmp.chromosome[j];	
				
				if (current.chromosome[j]>variabletop[j])  //�ж��Ƿ񳬽�.
				{
					current.chromosome[j]=double(rand()%10000)/10000*(variabletop[j]-variablebottom[j])+variablebottom[j];
				}
				if (current.chromosome[j]<variablebottom [j])
				{
					current.chromosome[j]=double(rand()%10000)/10000*(variabletop[j]-variablebottom[j])+variablebottom[j];
				}
				
			}
			//�ش�	
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
	//����õ�������ġ�
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
void CMVSOGA:: calculatefitnessvalue() //��Ӧ�Ⱥ���ֵ���㣬�ؼ�����Ӧ�Ⱥ��������
	//current�仯����γ���仯�ϴ��ر�������
{
	int  i;
	double temp;//alpha,beta;//��Ӧ�Ⱥ����ĳ߶ȱ仯ϵ��
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
void CMVSOGA:: performevolution() //��ʾ���۽��,���������,current�仯������Ӧ�øı�ϴ�
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
		current=population.GetAt(population.FindIndex(i)); //����Ϊ��һ�䣬�����˺ܴ�����⡣ 
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