#include"PSO.h"
#include"BackPro.h"

PSO::PSO(int _Dim,int _Number,CBackProp &_bp):Dim(_Dim),number(_Number),bp(_bp)
{
	
	 c1=2;//�����������ʶ
	 c2=2;//��������Ⱥ�����ʶ

	 w =0 ;//����Ȩ��
	 wmax=0.9;//������Ȩ��
	 wmin=0.4;//��С����Ȩ��

	 T=0;//��ǰ��������
	 Tmax=0;//����������

	 pmax=0;//����λ���Ͻ�
	 pmin=0;//����λ���½�

	 vmax=0;//�ٶ����ֵ


	 glbindex = -1;//���������Ӧ�ȱ��
	 
	 gbest = new double[Dim];//ȫ�����Ž�
	
	 glbest = 200000;//�����Ӧ��

	 m = 0;//����1��0֮��������
	 n = 0;//����1��0֮��������

	 /*
	 *particles
	 */
	 particles = (Particle*)malloc(number*sizeof(Particle));
	 for(int i =0 ;i<number;i++)
	 {
		 Particle p(Dim);
		 particles[i] = p;
	}
		 	 /*
			 write the result out
			 */
	// out = fopen("result.txt","a");
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

void PSO::fitness(){//����Ӧ��
	int i,j;
	double sum=0;
	for(i=0;i<number;i++){
		sum = 0;
		bp.getWeightFromPSO(i);
		for(int j=0;j<bp.inputN;j++){
			//printf("%lf %lf\n",(tgt+j*bp.lsize[bp.numl-1])[0],(tgt+j*bp.lsize[bp.numl-1])[1]);
			bp.ffwd((in+j*bp.lsize[0]));
			sum += bp.mse((tgt+j*bp.lsize[bp.numl-1]));
		}
		particles[i].fitness =sum;

	}

}

void PSO::limit_PSO(){//�������ӵ��ٶȺ�λ����Ϣ
	float a= 5,b=-5;
	pmax=a;pmin=b;vmax= 2.0;//pmax-pmin;
	return;
}

void PSO::initial_PSO(double *_in,double *_tgt){//��ʼ������
	int i,j;
	Tmax =  bp.iterator;
	tgt = _tgt;
	in = _in;
	srand((unsigned)(time(NULL)));
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{
			particles[i].v[j]=vmax*1.0*rand()/RAND_MAX;//���ӵĸ�ά�ٶ�
			particles[i].position[j]= 1.0*rand()/RAND_MAX*(2)+(-1);//���ӵ�λ��
			particles[i].pbest[j]=particles[i].position[j];
			/*printf("%d 's initial speed is%lf\n",i,particles[i].v[j]);
			printf("%d 's initial postition is%lf\n",i,particles[i].position[j]);
			*/
		}
	}
}
void PSO::initialBest(bool initial){
	
	int i =0,j=0;
	fitness();
	if(initial)
    {
        glbindex=0;//��ʼ��ʱ�����������Ӧ�ȱ��

	}

	for(i=0;i<number;i++)//Ѱ����õ���Ӧ�ȵ�����
	{
		particles[i].bestfitness=particles[i].fitness;	
		for(j=0;j<Dim;j++)
				particles[i].pbest[j]=particles[i].position[j];//��ʼ��ʱ�ĸ������λ��
	}

	if(!initial)
	{
			glbest =particles[glbindex].bestfitness;
			
			return;
	}

	double s=particles[glbindex].bestfitness;
	for(int i=0;i<number;i++)
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
	
}

void PSO::update_Interweight(){//�������ӵĹ���Ȩ��,���Եݼ���
	w=wmax-T*(wmax-wmin)/Tmax;
}

void PSO::update_speed(){//�������ӵĵ��ٶ�
	int i,j;
	srand((unsigned)(time(NULL)));
	m=(double((double)(rand()%(int)(16384)/(16383.0))));
	n=(double((double)(rand()%(int)(16384)/(16383.0))));
	
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{ 
			//particles[i].v[j]=w*particles[i].v[j]+c1*m*(particles[i].pbest[j]-particles[i].position[j])+c2*n*(gbest[j]-particles[i].position[j]);
		 particles[i].v[j]=w*particles[i].v[j]+c1*1.0*rand()/RAND_MAX*(particles[i].pbest[j]-particles[i].position[j])
				+c2*1.0*rand()/RAND_MAX*(gbest[j]-particles[i].position[j]);
			if(particles[i].v[j]>vmax)
				particles[i].v[j]=vmax;
			if(particles[i].v[j]<-vmax)
				particles[i].v[j]=-vmax;
			//printf("%d,speed %.10lf\n",i,particles[i].v[j]);
		}
	}
	
	return;
}
void PSO::update_position(){//�������ӵ�λ��
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
void PSO::update_gbest(){//����ȫ�����ӵ�ȫ������λ��
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
	
	//fprintf(out,"%lf ",glbest);
  printf("\n%d,%.9f\n",glbindex,glbest);
	
     
	return;
}
