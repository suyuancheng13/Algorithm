
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>


#define Dim 10//粒子维数
#define number 20//粒子个数

#define Pi 3.1415926

double c1=2;//粒子自身的认识
double c2=2;//粒子整个群体的认识

double w;//惯性权重
double wmax=0.9;//最大惯性权重
double wmin=0.4;//最小惯性权重

int T;//当前迭代次数
int Tmax=1000;//最大迭代次数

float pmax;//粒子位置上界
float pmin;//粒子位置下界

double vmax;//速度最大值


int glbindex;//粒子最好适应度编号
double gbest[Dim];//全局最优解
double glbest;//最好适应度

double m;//介于1，0之间的随机数
double n;//介于1，0之间的随机数

typedef struct{
	double v[Dim];//粒子本身速度
	double position[Dim];//粒子位置
	double pbest[Dim];//粒子本身最优解

	double fitness;//粒子适应度
	double bestfitness;//粒子最好适应度

}particle;
particle particles[number];



void fitness();
void limit_PSO();//动态区间的设置
void initial_PSO();
void update_gbest();
void update_speed();
void update_position();
void update_Interweight();//更新权重



void fitness(){//求适应度
	int i,j;double sum=0;
	for(i=0;i<number;i++){
		particles[i].fitness=sum;
	}
}

void limit_PSO(){//输入粒子的速度和位置信息
	float a=5.12,b=-5.12;
	pmax=a;pmin=b;vmax=2;
	
	return;
}

void initial_PSO(){//初始化粒子
	int i,j;
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{
			particles[i].v[j]=(double((double)(rand()%(int)(16384)/(16383.0))));//粒子的各维速度
			particles[i].position[j]=(rand()%100/99.0)*(pmax-pmin)+(pmin);//粒子的位置
			particles[i].pbest[j]=particles[i].position[j];
			//printf("%d 's initial speed is%lf\n",i,particles[i].v[j]);
			//printf("%d 's initial postition is%lf\n",i,particles[i].position[j]);
		}
	}

	fitness();

	glbindex=0;//初始化时的粒子最好适应度编号
	for(i=0;i<number;i++)//寻找最好的适应度的粒子
	{
		particles[i].bestfitness=particles[i].fitness;	
		for(j=0;j<Dim;j++)
				particles[i].pbest[j]=particles[i].position[j];//初始化时的个体最好位置
	}

	double s=particles[glbindex].bestfitness;
	for(int i=0;i<number;i++)
	{
		if(particles[i].bestfitness<s)
		{
			s=particles[i].bestfitness;glbindex=i;	
		}
	}

	for(j=0;j<Dim;j++)
		{
			gbest[j]=particles[glbindex].position[j];//初始化时全局最好位置
		}
	glbest=particles[glbindex].bestfitness;
	//printf("%d,/初始化%.9f\n",glbindex,glbest);//初始化粒子适应度
}

void update_Interweight(){//更新粒子的惯性权重,线性递减。
	w=wmax-T*(wmax-wmin)/Tmax;
}

void update_speed(){//更新粒子的的速度
	int i,j;
	m=(double((double)(rand()%(int)(16384)/(16383.0))));
	n=(double((double)(rand()%(int)(16384)/(16383.0))));
	
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{ 
			particles[i].v[j]=w*particles[i].v[j]+c1*m*(particles[i].pbest[j]-particles[i].position[j])+c2*n*(gbest[j]-particles[i].position[j]);
		    if(particles[i].v[j]>vmax)
				particles[i].v[j]=vmax;
			//printf("%d,speed %.10lf\n",i,particles[i].v[j]);
		}
	
	}
	return;
}
void update_position(){//更新粒子的位置
    int i,j;
	for(i=0;i<number;i++){                                          
		for(j=0;j<Dim;j++)
		{	
			particles[i].position[j]=particles[i].position[j]+particles[i].v[j];
			if(particles[i].position[j]>pmax)
				particles[i].position[j]=2*pmax-particles[i].position[j];
			if(particles[i].position[j]<pmin)
				particles[i].position[j]=2*pmin-particles[i].position[j];
			//printf("%d,position  %.10lf\n",i,particles[i].position[j]);
		}
	}
	
	fitness();
	for(i=0;i<number;i++)
	{
		if(particles[i].fitness<particles[i].bestfitness)//更新粒子的历史个体最好位置
		{
			particles[i].bestfitness=particles[i].fitness;
			for(j=0;j<Dim;j++)
			{
				particles[i].pbest[j]=particles[i].position[j];
			}
	   }
	}
	return;
}
void update_gbest(){//更新全体粒子的全局最优位置
	int i;

	for(i=0;i<number;i++)
	{
		if(particles[i].bestfitness<glbest)
		{
				glbindex=i;glbest=particles[glbindex].bestfitness;
				for(int j=0;j<Dim;j++)
				{
					gbest[j]=particles[glbindex].pbest[j];
				}
			
		}
	}
	

  printf("%d,%.9f\n",glbindex,glbest);
     
	return;
}

int ttmain(){
      double sum=0;
	time_t a=0,b=0;
	time(&b);
	
//	for(int i=0;i<50;i++)
	{
		srand((unsigned)time(NULL));
		limit_PSO();
		initial_PSO();
		for(T=0;T<=Tmax;T++)
		{
			update_Interweight();//更新惯性权重
			update_speed();//更新速度
			update_position();//更新位置及个体最优
			update_gbest();//更新全局最优
		}
		sum+=glbest; 
		//printf("%d,%.9f\n",glbindex,glbest);
	//printf("%d,%.10lf\n",i,glbest);
	}
	time(&a);
	//printf("%lf\n",difftime(a,b)/500);
	//printf("%.10lf",sum/500);
	return 1;
	
}