
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>


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



void fitness();
void limit_PSO();//��̬���������
void initial_PSO();
void update_gbest();
void update_speed();
void update_position();
void update_Interweight();//����Ȩ��



void fitness(){//����Ӧ��
	int i,j;double sum=0;
	for(i=0;i<number;i++){
		particles[i].fitness=sum;
	}
}

void limit_PSO(){//�������ӵ��ٶȺ�λ����Ϣ
	float a=5.12,b=-5.12;
	pmax=a;pmin=b;vmax=2;
	
	return;
}

void initial_PSO(){//��ʼ������
	int i,j;
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{
			particles[i].v[j]=(double((double)(rand()%(int)(16384)/(16383.0))));//���ӵĸ�ά�ٶ�
			particles[i].position[j]=(rand()%100/99.0)*(pmax-pmin)+(pmin);//���ӵ�λ��
			particles[i].pbest[j]=particles[i].position[j];
			//printf("%d 's initial speed is%lf\n",i,particles[i].v[j]);
			//printf("%d 's initial postition is%lf\n",i,particles[i].position[j]);
		}
	}

	fitness();

	glbindex=0;//��ʼ��ʱ�����������Ӧ�ȱ��
	for(i=0;i<number;i++)//Ѱ����õ���Ӧ�ȵ�����
	{
		particles[i].bestfitness=particles[i].fitness;	
		for(j=0;j<Dim;j++)
				particles[i].pbest[j]=particles[i].position[j];//��ʼ��ʱ�ĸ������λ��
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
			gbest[j]=particles[glbindex].position[j];//��ʼ��ʱȫ�����λ��
		}
	glbest=particles[glbindex].bestfitness;
	//printf("%d,/��ʼ��%.9f\n",glbindex,glbest);//��ʼ��������Ӧ��
}

void update_Interweight(){//�������ӵĹ���Ȩ��,���Եݼ���
	w=wmax-T*(wmax-wmin)/Tmax;
}

void update_speed(){//�������ӵĵ��ٶ�
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
void update_position(){//�������ӵ�λ��
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
			update_Interweight();//���¹���Ȩ��
			update_speed();//�����ٶ�
			update_position();//����λ�ü���������
			update_gbest();//����ȫ������
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