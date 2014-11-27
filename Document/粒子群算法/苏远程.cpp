
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

#define Dim 10//����ά��
#define number 20//���Ӹ���
#define Pi 3.1415926

double c1=2;//�����������ʶ
double c2=2;//��������Ⱥ�����ʶ

double w;//����Ȩ��
double wmax=0.9;//������Ȩ��
double wmin=0.4;//��С����Ȩ��

int T;//��ǰ��������
int Tmax=1000;//����������

float pmax;//����λ���Ͻ�
float pmin;//����λ���½�

double vmax;//�ٶ����ֵ


int glbindex;//���������Ӧ�ȱ��
double gbest[Dim];//ȫ�����Ž�
double glbest;//�����Ӧ��

double m;//����1��0֮��������
double n;//����1��0֮��������

typedef struct{
	double v[Dim];//���ӱ����ٶ�
	double position[Dim];//����λ��
	double pbest[Dim];//���ӱ������Ž�

	double fitness;//������Ӧ��
	double bestfitness;//���������Ӧ��

}particle;

particle particles[number];

double sphere(int);
double Rastringrin(int);
double Griewank(int);
double Rosenbrock(int);
double Noise(int);
double Ackley(int);

void fitness();
void limit_PSO();//��̬���������
void initial_PSO();
void update_gbest();
void update_speed();
void update_position();
void update_Interweight();//����Ȩ��

void Diversity_p();//λ�ö�����
void Diversity_v();//�ٶȶ�����
void Diversity_c();//��֪������

/****************************************************************************************************/
 /***************      �����Է������ʽ
/****************************************************************************************************/
void Diversity_p()//λ�ö�����
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
		pj_ave=pj_sum/number;// ��ÿһά������
		for(int i=0;i<number;i++)
		{
			Dj[j]+=abs(particles[i].position[j]-pj_ave);//ÿһ���ӵ�����λ�õľ����
			//printf("%d,%lf",j,Dj[j]);
		}
		Dj[j]=Dj[j]/number;
		D+=Dj[j];
		
	}
	D=D/Dim;
	fopen_s(&fp,"Diversity_p.txt","a");
	//printf("λ�ö�����%lf\n",D);
	fprintf_s(fp,"%d,%.10lf\n",T,D);
	fclose(fp);
}
void Diversity_v()//�ٶȶ�����
{
	FILE *fp;
	double vj_sum=0,vj_ave=0,V[Dim]={0},Vs=0;
	for(int j=0;j<Dim;j++)
	{
		
		for(int i=0;i<number;i++)
		{
			vj_sum+=particles[i].v[j];
		}
		vj_ave=vj_sum/number;// ��ÿһά�ٶȵ�����
		for(int i=0;i<number;i++)
		{
			V[j]+=abs(particles[i].v[j]-vj_ave);//ÿһ���ӵ������ٶȵľ����
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
	//printf("�ٶȶ�����%lf\n",Vs);
	//fprintf(fp,"%lf\n",Vs);
}
void Diversity_c()//��֪������
{
}
/****************************************************************************************************/
/****************************************************************************************************/
/*******************************************************************
�����Ǹ��ֲ��Ժ���
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

void fitness(){//����Ӧ��
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

void limit_PSO(){//�������ӵ��ٶȺ�λ����Ϣ
	float a=100,b=-100;
	pmax=a;pmin=b;vmax=pmax-pmin;
	
	return;
}

void initial_PSO(){//��ʼ������
	int i,j;
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{
			particles[i].v[j]=(1.0*rand()/RAND_MAX)*vmax;//(double((double)(rand()%(int)(16384)/(16383.0))))*vmax;//���ӵĸ�ά�ٶ�
			particles[i].position[j]=(1.0*rand()/RAND_MAX)*(pmax-pmin)+(pmin);//(rand()%100/99.0)*(pmax-pmin)+(pmin);//���ӵ�λ��
			//particles[i].position[j]=rand()%(int)(pmax-pmin)+pmin;
			particles[i].pbest[j]=particles[i].position[j];
			//printf("%d 's initial speed is%lf\n",i,particles[i].v[j]);
			//printf("%d 's initial postition is%lf\n",i,particles[i].position[j]);
		}
	}

	fitness();

	//Diversity_p();
	//Diversity_v();
	
	for(i=0;i<number;i++)//Ѱ����õ���Ӧ�ȵ�����
	{
		particles[i].bestfitness=particles[i].fitness;	
		for(j=0;j<Dim;j++)
				particles[i].pbest[j]=particles[i].position[j];//��ʼ��ʱ�ĸ������λ��
	}
	glbindex=0;//��ʼ��ʱ�����������Ӧ�ȱ��
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
			gbest[j]=particles[glbindex].position[j];//��ʼ��ʱȫ�����λ��
		}
	glbest=particles[glbindex].bestfitness;
	//printf("%d,/��ʼ��%.9f\n",glbindex,glbest);//��ʼ��������Ӧ��
}

void update_Interweight(){//�������ӵĹ���Ȩ��,���Եݼ���
w=wmax-T*(wmax-wmin)/Tmax;
	 //w=0.9;
}

void update_speed(){//�������ӵĵ��ٶ�
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
void update_position(){//�������ӵ�λ��
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
		if(particles[i].fitness<particles[i].bestfitness)//�������ӵ���ʷ�������λ��
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
void update_gbest(){//����ȫ�����ӵ�ȫ������λ��
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
			update_Interweight();//���¹���Ȩ��
			update_speed();//�����ٶ�
			update_position();//����λ�ü���������
			update_gbest();//����ȫ������
		
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