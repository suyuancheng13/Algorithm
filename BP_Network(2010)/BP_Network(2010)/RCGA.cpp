#include"RCGA.h"
#include"BackPro.h"
#include<time.h>
#include<cmath>
/*
*RCGA  class
*/
void RCGA::initialRCGA(int _popSize,int _geneNum,int _maxGeneration,double _crossoverRate,double _mutationRate, double _topLimit,double _downLimit,double _goal)
{
	popSize = _popSize;
	geneNum = _geneNum;
	maxGeneration = _maxGeneration;
	crossoverRate = _crossoverRate;
	mutationRate = _mutationRate;
	topLimit = _topLimit;
	bottomLimit = _downLimit;
	goal = _goal;
	srand((unsigned)time(NULL));
	for(int i =0;i<popSize;i++)
	{
		bp.initial_wb_nw();
		Individual _individual(geneNum);
		//for(int ii=1;ii<bp.numl;ii++)
		//	for(int jj=0;jj<bp.lsize[ii];jj++)
		//	{
		//		for(int k=0;k<bp.lsize[ii-1]+1;k++)//bias is the last one
		//		{
		//			_individual.chromosome.push_back( bp.weight[ii][jj][k]);
		//		}
		//	}
		_individual.initial(topLimit,bottomLimit);
		population.push_back(_individual);
	}
	bestindividual = population[0];
}
void RCGA::evolution()
{
	//FILE *fp = fopen("ga_gd_result.txt","w");
	//FILE *fp = fopen("ga_pso_result.txt","w");
	int GEN =0;
	t = 0;
	while(GEN<maxGeneration){

		fitness();	
		updateBestIndividual();	
		if( fabs(bestindividual.fitness-goal)<0.0001)
				break;
		
		selectOperator();
		crossoverOperator();
		mutationOperator();
		GEN++;
		t++;
		printf("\t%5d \t%.10lf\n", GEN,abs(1/bestindividual.fitness-1));
		//fprintf(fp,"%lf\n",abs(1/bestindividual.fitness-1));
	}

}

void RCGA::result(double *_result)
{
	for(int i=0;i<geneNum;i++)
	{
		_result[i] = bestindividual.chromosome[i];
	}
}

void RCGA::gaOptions(double *_in,double *tgt)
{
	input = _in;
	target = tgt;
}
/*
*private operators function
*/
void RCGA::selectOperator()
{
	int i,j,pindex=0;
	double p,pc,sum;
	i=0;
	j=0;
	pindex=0;
	p=0;
	pc=0;
	sum=0.0;
	
	for(i=0;i<popSize;i++)//求适应度总值
	{
		current=population[i];
		sum+=current.fitness;
	}
	population[0].weight = population[0].fitness/sum;
	for(i=1;i<popSize; i++)//求每一个个体的赌轮概率
	{
		current1 = population[i-1];
		current=population[i]; 
		
		current.weight = current.fitness/sum+current1.weight;
		
		population[i] = current;
		//printf("fitness is:%lf    weight is :%lf\n",population[i].fitness,population[i].weight);
	}
	srand((unsigned)time(NULL));
	for (i=0;i<popSize;i++)//轮盘赌概率选择，结果会选出popSize个个体
	{
		p = 1.0*rand()/(RAND_MAX);  //随机生成概率
		pindex=0;  
		pc = population[0].weight; 
		while(p > pc && pindex < popSize) 
		{
			pindex++;
			if(pindex<popSize)
			   pc =  population[pindex].weight;	
			
		}
		if(pindex<popSize)
				nextPoputation.push_back(population[pindex]);
	}
	/*
	* the next population after excuting  the selcet operator
	*/
	if(popSize!= nextPoputation.size())
	{
		printf("\terror!! --%lf\n",nextPoputation.size());
		exit(0);
	}
	for(int i=0;i<popSize;i++)
	{
		population[i] = nextPoputation[i];
	}

	nextPoputation.clear();

}

void RCGA::mutationOperator()
{
	int i,j;
	double r1,r2,p,sigma;//sigma高斯变异参数
	double delta =0;
	double b =2.0;
	srand((unsigned)time(NULL));
	for (i=0;i<popSize;i++)
	{
		current=population[i];
		//生成均值为current.chromosome，方差为sigma的高斯分布数

		for(j=0; j<geneNum; j++)
		{   
		p = 1.0*rand()/(RAND_MAX-1);
		if(p<mutationRate)
	
			{
				double sign;
				sign=rand()%2;
				r1 =  1.0*rand()/(RAND_MAX-1);
		    	r2 = 1.0*rand()/(RAND_MAX-1);
				sigma=0.01*(topLimit-bottomLimit);
				//高斯变异
				if(sign) //1
				{
					delta = (current.chromosome[j]-bottomLimit)*(1-pow(r1,pow(1-t/maxGeneration,b)));
					current.chromosome[j] -= delta;
				/*		(current.chromosome[j] 
					+ sigma*sqrt(-2*log(r1)/0.4323)*sin(2*3.1415926*r2));*/
				}
				else//0
				{
					delta = (topLimit- current.chromosome[j])*(1-pow(r1,pow(1-t/maxGeneration,b)));
					current.chromosome[j] +=delta;
	/*					(current.chromosome[j] 
					- sigma*sqrt(-2*log(r1)/0.4323)*sin(2*3.1415926*r2));*/
				}
				/*
				*Reset the chromosome
				*/
				if (current.chromosome[j]>topLimit)
				{
					current.chromosome[j]=1.0*rand()/(RAND_MAX-1)*(topLimit-bottomLimit)+bottomLimit;
				}
				if (current.chromosome[j]<bottomLimit)
				{
					current.chromosome[j]=1.0*rand()/(RAND_MAX-1)*(topLimit-bottomLimit)+bottomLimit;
				}
			}
		}
		population[i] = current;
	}
}
void RCGA::crossoverOperator()
{
	int i,j;
	double alpha,beta;
	int point,temp;
	double p;

	srand((unsigned)time(NULL));	
	
	for (i=0;i<popSize-1;i+=2)
	{
		p=1.0*rand()/(RAND_MAX-1);
	
		if (p<crossoverRate)
		{   
		
			alpha = 0.6;//1.0*rand()/(RAND_MAX-1);//交叉点
			beta = 1.0*rand()/(RAND_MAX-1);
			int d = rand()%geneNum;
			tmp = current=population[i];
			current1=population[i+1];
			for(j=0;j<geneNum;j++)			
			{ 
				//交叉	
	    			current.chromosome[j]=(alpha)*current.chromosome[j]+
						(1-alpha)*current1.chromosome[j];	
					 current1.chromosome[j]=(alpha)*current1.chromosome[j]+
						(1-alpha)*tmp.chromosome[j];	
						/*	 current1.chromosome[j]=tmp.chromosome[j]+
						(alpha)*(tmp.chromosome[j]-current1.chromosome[j]);*/
				
				if (current.chromosome[j]>topLimit)  //判断是否超界.
				{
					current.chromosome[j]=1.0*rand()/(RAND_MAX-1)*(topLimit-bottomLimit)+bottomLimit;
				}
				else if (current.chromosome[j]<bottomLimit)
				{
					current.chromosome[j]=1.0*rand()/(RAND_MAX-1)*(topLimit-bottomLimit)+bottomLimit;
				}
				if (current1.chromosome[j]>topLimit)  //判断是否超界.
				{
					current1.chromosome[j]=1.0*rand()/(RAND_MAX-1)*(topLimit-bottomLimit)+bottomLimit;
				}
				else if (current1.chromosome[j]<bottomLimit)
				{
					current1.chromosome[j]=1.0*rand()/(RAND_MAX-1)*(topLimit-bottomLimit)+bottomLimit;
				}
			}
			population[i] = current;
			population[i+1] = current1;
			//i++;
		}
	}
}
void RCGA::updateBestIndividual()
{
	for(int i=0;i<popSize;i++)
	{
		if(bestindividual.fitness < population[i].fitness)
			bestindividual = population[i];
	}
	
}
/*
*
*/
void RCGA::fitness()
{
	int i,j;
	double sum=0;
	for(i=0;i<popSize;i++){
		sum = 0;
		bp.getWeightFromGA(i);
		for(int j=0;j<bp.inputN;j++){
			//printf("%lf %lf\n",(tgt+j*bp.lsize[bp.numl-1])[0],(tgt+j*bp.lsize[bp.numl-1])[1]);
			bp.ffwd((input+j*bp.lsize[0]));
			sum += bp.mse((target+j*bp.lsize[bp.numl-1]));
		}
	//	printf("%lf\n",sum);
		//sum /= (bp.inputN*bp.lsize[bp.numl-1]);
		population[i].fitness = 1/(sum+1);

	}
}