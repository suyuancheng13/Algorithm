#include"PSO.h"
#include"BackPro.h"

PSO::PSO(int _Dim,int _Number,CBackProp &_bp):Dim(_Dim),number(_Number),bp(_bp)
{
	
	 c1=2;//粒子自身的认识
	 c2=2;//粒子整个群体的认识

	 w =0 ;//惯性权重
	 wmax=0.9;//最大惯性权重
	 wmin=0.4;//最小惯性权重

	 T=0;//当前迭代次数
	 Tmax=100;//最大迭代次数

	 pmax=0;//粒子位置上界
	 pmin=0;//粒子位置下界

	 vmax=0;//速度最大值


	 glbindex = -1;//粒子最好适应度编号
	 gbest = new double[Dim];//全局最优解
	 glbest = 1000;//最好适应度

	 m = 0;//介于1，0之间的随机数
	 n = 0;//介于1，0之间的随机数

	 /*
	 *particles
	 */
	 particles = (Particle*)malloc(number*sizeof(Particle));
	 for(int i =0 ;i<number;i++)
	 {
		 Particle p(Dim);
		 particles[i] = p;
	}
		 	 
}
PSO::~PSO()
{
	delete[] gbest;
	for(int i =0 ;i<number;i++)
	{
		delete[] particles[i].v;
		delete[] particles[i].position ;
		delete[] particles[i].pbest ;
	}
	free(particles);
}

void PSO::fitness(){//求适应度
	int i,j;
	double sum=0;
	for(i=0;i<number;i++){
		bp.getWeightFromPSO(i);
		bp.ffwd(in);
		particles[i].fitness= bp.mse(tgt);
		//printf("\t%lf,",particles[i].fitness);
	}
	//printf("one time\n");
}

void PSO::limit_PSO(){//输入粒子的速度和位置信息
	float a=2,b=0;
	pmax=a;pmin=b;vmax=2;
	return;
}

void PSO::initial_PSO(double *_in,double *_tgt){//初始化粒子
	int i,j;
	tgt = _tgt;
	in = _in;
	srand((unsigned)(time(NULL)));
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{
			particles[i].v[j]=(double((double)(rand()%(int)(16384)/(16383.0))));//粒子的各维速度
			particles[i].position[j]=(rand()%100/99.0)*(pmax-pmin)+(pmin);//粒子的位置
			particles[i].pbest[j]=particles[i].position[j];
			/*printf("%d 's initial speed is%lf\n",i,particles[i].v[j]);
			printf("%d 's initial postition is%lf\n",i,particles[i].position[j]);
			*/
		}
	}
}
void PSO::initialBest(){
	
	int i =0,j=0;
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

void PSO::update_Interweight(){//更新粒子的惯性权重,线性递减。
	w=wmax-T*(wmax-wmin)/Tmax;
}

void PSO::update_speed(){//更新粒子的的速度
	int i,j;
	srand((unsigned)(time(NULL)));
	m=(double((double)(rand()%(int)(16384)/(16383.0))));
	n=(double((double)(rand()%(int)(16384)/(16383.0))));
	
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{ 
			particles[i].v[j]=w*particles[i].v[j]+c1*m*(particles[i].pbest[j]-particles[i].position[j])+c2*n*(gbest[j]-particles[i].position[j]);
		    if(particles[i].v[j]>vmax)
				particles[i].v[j]=vmax;
			if(particles[i].v[j]<-vmax)
				particles[i].v[j]=-vmax;
			//printf("%d,speed %.10lf\n",i,particles[i].v[j]);
		}
	
	}
	return;
}
void PSO::update_position(){//更新粒子的位置
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
void PSO::update_gbest(){//更新全体粒子的全局最优位置
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
	

 // printf("\n%d,%.9f\n",glbindex,glbest);
     
	return;
}
void PSO::Print()
{
	 int i,j;
	//for(i=0;i<number;i++){                                          
		for(j=0;j<Dim;j++)
		{	
		
			printf("\t%.10lf",gbest[j]);
		}
	
}