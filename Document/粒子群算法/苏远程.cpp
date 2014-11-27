
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

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

double sphere(int);
double Rastringrin(int);
double Griewank(int);
double Rosenbrock(int);
double Noise(int);
double Ackley(int);

void fitness();
void limit_PSO();//动态区间的设置
void initial_PSO();
void update_gbest();
void update_speed();
void update_position();
void update_Interweight();//更新权重

void Diversity_p();//位置多样性
void Diversity_v();//速度多样性
void Diversity_c();//认知多样性

/****************************************************************************************************/
 /***************      多样性分析表达式
/****************************************************************************************************/
void Diversity_p()//位置多样性
{
	FILE *fp;
	double pj_sum=0,pj_ave=0,Dj[Dim]={0},D=0;
	for(int j=0;j<Dim;j++)
	{
		Dj[j]=0;pj_sum=0;pj_ave=0;
		for(int i=0;i<number;i++)
		{
			pj_sum+=particles[i].position[j];
		}
		pj_ave=pj_sum/number;// 求每一维的中心
		for(int i=0;i<number;i++)
		{
			Dj[j]+=abs(particles[i].position[j]-pj_ave);//每一粒子到中心位置的距离和
			//printf("%d,%lf",j,Dj[j]);
		}
		Dj[j]=Dj[j]/number;
		D+=Dj[j];
		
	}
	D=D/Dim;
	fopen_s(&fp,"Diversity_p.txt","a");
	//printf("位置多样性%lf\n",D);
	fprintf_s(fp,"%d,%.10lf\n",T,D);
	fclose(fp);
}
void Diversity_v()//速度多样性
{
	FILE *fp;
	double vj_sum=0,vj_ave=0,V[Dim]={0},Vs=0;
	for(int j=0;j<Dim;j++)
	{
		
		for(int i=0;i<number;i++)
		{
			vj_sum+=particles[i].v[j];
		}
		vj_ave=vj_sum/number;// 求每一维速度的中心
		for(int i=0;i<number;i++)
		{
			V[j]+=abs(particles[i].v[j]-vj_ave);//每一粒子到中心速度的距离和
		}
		V[j]=V[j]/number;
		//V+=Vj;
		
	}
	for(int i=0;i<Dim;i++)
	{
		Vs+=V[i];
	}
	Vs=Vs/Dim;
	//fp=fopen("Diversity_V.txt","a");
	//printf("速度多样性%lf\n",Vs);
	//fprintf(fp,"%lf\n",Vs);
}
void Diversity_c()//认知多样性
{
}
/****************************************************************************************************/
/****************************************************************************************************/
/*******************************************************************
以下是各种测试函数
**********************************************************************/
double sphere(int index)
{
	double sum=0;
	for(int i=0;i<Dim;i++)
	{
		sum+=(double)(particles[index].position[i])*(particles[index].position[i]);
	}
	return sum;
}
double Rastringrin(int index)
{
	double sum=0;
	for(int i=0;i<Dim;i++)
	{
		sum+=(double)((particles[index].position[i])*(particles[index].position[i])-10*cos((2*Pi)*(particles[index].position[i]))+10);
	}
	return sum;
}
double Griewank(int index)
{
	double sum=0,sum1=0,sum2=1;
	for(int i=0;i<Dim;i++)
	{
		sum1+=(particles[index].position[i])*(particles[index].position[i])/4000.0;
		sum2=sum2*cos(particles[index].position[i]/sqrt((double)(i+1)));//printf("%lf\n",cos(particles[index].position[i]/sqrt((double)i)));
	}
	sum=sum1-sum2+1;
	
	return sum;
}
double Rosenbrock(int index)
{
	double sum=0;
	for(int i=0;i<Dim-1;i++)
	{
		//sum+=100*pow((pow(particles[index].position[i],2)-particles[index].position[i+1]),2)+pow((particles[index].position[i]-1),2);
		//printf("%lf\n",particles[index].position[i]);
		sum+=100*pow((pow(particles[index].position[i+1],2)-particles[index].position[i]),1)+pow((particles[index].position[i]-1),2);
	}
	return sum;
}
double Noise(int index)
{
	double sum=0;
	for(int i=0;i<Dim;i++)
	{
		sum+=(i+1)*pow(particles[index].position[i],4);
	}
	sum+=rand()%100/100.0;
	return sum;
}
double Ackley(int index)
{
	double sum=0,sum1=0,sum2=0;
	for(int i=0;i<Dim;i++)
	{
		sum1+=pow(particles[index].position[i],2);//printf("%lf\n",sum1);

		sum2+=cos(2*Pi*particles[index].position[i]);
	}
	sum=-20*exp(-0.2*sqrt(sum1/Dim))-exp(sum2/Dim)+20+exp(1.0);
//	printf("fitness is %lf\n",sum);
	return sum;
}
///////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

void fitness(){//求适应度
	int i,j;double sum=0;
	for(i=0;i<number;i++){
		sum=0;
		sum=sphere(i);
		//sum=Rastringrin(i);
		//sum=Griewank(i);
		//sum=Rosenbrock(i);
		//sum=Noise(i);
		//sum=Ackley(i);
		particles[i].fitness=sum;
	}
}

void limit_PSO(){//输入粒子的速度和位置信息
	float a=100,b=-100;
	pmax=a;pmin=b;vmax=pmax-pmin;
	
	return;
}

void initial_PSO(){//初始化粒子
	int i,j;
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{
			particles[i].v[j]=(1.0*rand()/RAND_MAX)*vmax;//(double((double)(rand()%(int)(16384)/(16383.0))))*vmax;//粒子的各维速度
			particles[i].position[j]=(1.0*rand()/RAND_MAX)*(pmax-pmin)+(pmin);//(rand()%100/99.0)*(pmax-pmin)+(pmin);//粒子的位置
			//particles[i].position[j]=rand()%(int)(pmax-pmin)+pmin;
			particles[i].pbest[j]=particles[i].position[j];
			//printf("%d 's initial speed is%lf\n",i,particles[i].v[j]);
			//printf("%d 's initial postition is%lf\n",i,particles[i].position[j]);
		}
	}

	fitness();

	//Diversity_p();
	//Diversity_v();
	
	for(i=0;i<number;i++)//寻找最好的适应度的粒子
	{
		particles[i].bestfitness=particles[i].fitness;	
		for(j=0;j<Dim;j++)
				particles[i].pbest[j]=particles[i].position[j];//初始化时的个体最好位置
	}
	glbindex=0;//初始化时的粒子最好适应度编号
	double s=particles[glbindex].bestfitness;
	for( i=0;i<number;i++)
	{
		if(particles[i].bestfitness<s)
		{
			s=particles[i].bestfitness;
			glbindex=i;	
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
	 //w=0.9;
}

void update_speed(){//更新粒子的的速度
	int i,j;
	m=1.0*rand()/RAND_MAX;//(double((double)(rand()%(int)(16384)/(16383.0))));
	n=1.0*rand()/RAND_MAX;//(double((double)(rand()%(int)(16384)/(16383.0))));
	//printf("%lf",1.0*rand()/RAND_MAX);
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{ 
			particles[i].v[j]=w*particles[i].v[j]+c1*1.0*rand()/RAND_MAX*(particles[i].pbest[j]-particles[i].position[j])+c2*1.0*rand()/RAND_MAX*(gbest[j]-particles[i].position[j]);
		    if(particles[i].v[j]>vmax)
			    particles[i].v[j]=vmax;
	         if(particles[i].v[j]<-vmax)
				particles[i].v[j]=-vmax;
			//printf("%d,speed %.10lf\n",i,particles[i].v[j]);
		}
	
	}
	//Diversity_v();
	return;
}
void update_position(){//更新粒子的位置
    int i,j;
	for(i=0;i<number;i++){                                          
		for(j=0;j<Dim;j++)
		{	
			particles[i].position[j]=particles[i].position[j]+particles[i].v[j];
			if(particles[i].position[j]>pmax)
				particles[i].position[j]=2*pmax-particles[i].position[j];//(rand()%100/99.0)*(pmax-pmin)+(pmin);
			if(particles[i].position[j]<pmin)
				particles[i].position[j]=2*pmin-particles[i].position[j];//-((rand()%100/99.0)*(pmax-pmin)+(pmin));
			//printf("%d,position  %.10lf\n",i,particles[i].position[j]);
		}
	}
	
	fitness();
	//Diversity_p();
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
				glbindex=i;
				glbest=particles[glbindex].bestfitness;
				for(int j=0;j<Dim;j++)
				{
					gbest[j]=particles[glbindex].pbest[j];
				}
			
		}
	}
  //printf("%d,%.9f\n",glbindex,glbest);
	return;
}

int main(){
	FILE *fp=fopen("re.txt","a");
    double sum=0;
	time_t a=0,b=0;
	time(&b);
	limit_PSO();
	srand((unsigned)time(NULL));
	for(int i=0;i<1;i++)
	{ 
		//Sleep(1000);
		//srand((unsigned)time(NULL));
		initial_PSO();

		for(T=0;T<Tmax;T++)
		{
			update_Interweight();//更新惯性权重
			update_speed();//更新速度
			update_position();//更新位置及个体最优
			update_gbest();//更新全局最优
		
		}
		sum+=glbest; 
	printf("%d,%d,%.9f\n",i,glbindex,glbest);


	glbest=0;
	//printf("%d,%.10lf\n",i,glbest);
	}
	time(&a);
	//printf("%lf\n",difftime(a,b)/500);
	printf("%.10lf\n",sum/50);
/*	for(int i=0;i<Dim;i++)
	{
		printf("%d,%lf\n",i,gbest[i]);
	}*/


}