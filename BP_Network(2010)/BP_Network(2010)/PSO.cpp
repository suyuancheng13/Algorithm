#include"PSO.h"
#include"BackPro.h"

PSO::PSO(int _Dim,int _Number,CBackProp &_bp):Dim(_Dim),number(_Number),bp(_bp)
{
	
	 c1=2;
	 c2=2;

	 w =0 ;
	 wmax=0.9;
	 wmin=0.4;

	 T=0;
	 Tmax=0;

	 pmax=0;
	 pmin=0;

	 vmax=0;


	 glbindex = -1;
	 gBest = new double[Dim];
	 gbest[0] = new double[Dim];
	 gbest[1] = new double[Dim];
	 gbest[2] = new double[Dim];
	 glbest = 200000;

	 m = 0;
	 n = 0;

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
	delete[] gbest[0];
	delete[] gbest[1];
	delete[] gbest[2];
	for(int i =0 ;i<number;i++)
	{
		delete[] particles[i].v;
		delete[] particles[i].position ;
		delete[] particles[i].pbest ;
	}
	free(particles);
}

void PSO::fitness(){
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

void PSO::limit_PSO(){
	float a= 5,b=-5;
	pmax=a;pmin=b;vmax= 2.0;//pmax-pmin;
	return;
}

void PSO::initial_PSO(double *_in,double *_tgt){
	int i,j;
	Tmax =  bp.iterator;
	tgt = _tgt;
	in = _in;
	srand((unsigned)(time(NULL)));
	for(i=0;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{
			particles[i].v[j]=vmax*1.0*rand()/RAND_MAX;
			particles[i].position[j]= 1.0*rand()/RAND_MAX*(2)+(-1);
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
			glbindex=0;
			glbindex1 = number/3;
			glbindex2 = 2*number/3;
	}

	for(i=0;i<number;i++)
	{
		particles[i].bestfitness=particles[i].fitness;	
		for(j=0;j<Dim;j++)
				particles[i].pbest[j]=particles[i].position[j];
	}

	if(!initial)
	{
			glbest = glbest0=particles[glbindex].bestfitness;
			glbest1=particles[glbindex1].bestfitness;
			glbest2=particles[glbindex2].bestfitness;
			return;
	}

	double s=particles[glbindex].bestfitness;
	for(int i=0;i<number/3;i++)
	{
		if(particles[i].bestfitness<s)
		{
			s=particles[i].bestfitness;
			glbindex=i;	
		}
	}
		for(j=0;j<Dim;j++)
		{
			gbest[0][j]=particles[glbindex].position[j];
		}
	glbest0=particles[glbindex].bestfitness;

	double s1=particles[glbindex1].bestfitness;
	for(int i=number/3;i<2*number/3;i++)
	{
		if(particles[i].bestfitness<s)
		{
			s1=particles[i].bestfitness;
			glbindex1=i;	
		}
	}
		for(j=0;j<Dim;j++)
		{
			gbest[1][j]=particles[glbindex1].position[j];
		}
	glbest1=particles[glbindex1].bestfitness;
	
	double s2=particles[glbindex2].bestfitness;
	for(int i=2*number/3;i<number;i++)
	{
		if(particles[i].bestfitness<s)
		{
			s2=particles[i].bestfitness;
			glbindex2=i;	
		}
	}
		for(j=0;j<Dim;j++)
		{
			gbest[2][j]=particles[glbindex2].position[j];
		}
	glbest2=particles[glbindex2].bestfitness;

	glbest  = glbest0;
	GLBINDEX =glbindex;
	if(glbest > glbest1)
	{	glbest = glbest1;
	GLBINDEX =glbindex1;
	}
	if(glbest > glbest2)
	{
		glbest = glbest2;
		GLBINDEX =glbindex2;
	}
	for(int i=0;i<Dim;i++)
	{
		gBest[i]= particles[GLBINDEX].position[j];
	}
	   
}

void PSO::update_Interweight(){
	w=wmax-T*(wmax-wmin)/Tmax;
}

void PSO::update_speed(){
	int i,j;
	srand((unsigned)(time(NULL)));
	m=(double((double)(rand()%(int)(16384)/(16383.0))));
	n=(double((double)(rand()%(int)(16384)/(16383.0))));
	
	for(i=0;i<number/3;i++)
	{
		for(j=0;j<Dim;j++)
		{ 
			//particles[i].v[j]=w*particles[i].v[j]+c1*m*(particles[i].pbest[j]-particles[i].position[j])+c2*n*(gbest[j]-particles[i].position[j]);
		 particles[i].v[j]=w*particles[i].v[j]+c1*1.0*rand()/RAND_MAX*(particles[i].pbest[j]-particles[i].position[j])
				+c2*1.0*rand()/RAND_MAX*(gbest[0][j]-particles[i].position[j]);
			if(particles[i].v[j]>vmax)
				particles[i].v[j]=vmax;
			if(particles[i].v[j]<-vmax)
				particles[i].v[j]=-vmax;
			//printf("%d,speed %.10lf\n",i,particles[i].v[j]);
		}
	}
		/*
		��Ⱥ2
		*/
	for(i=number/3;i<2*number/3;i++)
	{
		for(j=0;j<Dim;j++)
		{ 
			//particles[i].v[j]=w*particles[i].v[j]+c1*m*(particles[i].pbest[j]-particles[i].position[j])+c2*n*(gbest[j]-particles[i].position[j]);
		 particles[i].v[j]=w*particles[i].v[j]+c1*1.0*rand()/RAND_MAX*(particles[i].pbest[j]-particles[i].position[j])
				+c2*1.0*rand()/RAND_MAX*(gbest[1][j]-particles[i].position[j]);
			if(particles[i].v[j]>vmax)
				particles[i].v[j]=vmax;
			if(particles[i].v[j]<-vmax)
				particles[i].v[j]=-vmax;
			//printf("%d,speed %.10lf\n",i,particles[i].v[j]);
		}
	}
		/*
		swarm 3
		*/
		for(i=2*number/3;i<number;i++)
	{
		for(j=0;j<Dim;j++)
		{ 
			//particles[i].v[j]=w*particles[i].v[j]+c1*m*(particles[i].pbest[j]-particles[i].position[j])+c2*n*(gbest[j]-particles[i].position[j]);
		 particles[i].v[j]=w*particles[i].v[j]+c1*1.0*rand()/RAND_MAX*(particles[i].pbest[j]-particles[i].position[j])
				+c2*1.0*rand()/RAND_MAX*(gbest[2][j]-particles[i].position[j]);
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
		if(particles[i].fitness<particles[i].bestfitness)//�������ӵ���ʷ�������λ��?
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

	for(i=0;i<number/3;i++)
	{
		if(particles[i].bestfitness<glbest0)
		{
				glbindex=i;
				glbest0=particles[glbindex].bestfitness;
				for(int j=0;j<Dim;j++)
				{
					gbest[0][j]=particles[glbindex].pbest[j];
				}
			
		}
	}
		/*
		swarm2
		*/
		for(i=number/3;i<2*number/3;i++)
	{
		if(particles[i].bestfitness<glbest1)
		{
				glbindex1=i;
				glbest1=particles[glbindex1].bestfitness;
				for(int j=0;j<Dim;j++)
				{
					gbest[1][j]=particles[glbindex1].pbest[j];
				}
			
		}
		}
		/*
		swarm 3
		*/
		for(i=2*number/2;i<number;i++)
	{
		if(particles[i].bestfitness<glbest2)
		{
				glbindex2=i;
				glbest2=particles[glbindex2].bestfitness;
				for(int j=0;j<Dim;j++)
				{
					gbest[2][j]=particles[glbindex2].pbest[j];
				}
			
		}
		}
	
	/*
	*ȷ��ȫ�����?
	*/
	if(glbest >glbest0)
	{
		glbest  = glbest0;
		GLBINDEX = glbindex;
	}
	if(glbest > glbest1)
	{
		glbest = glbest1;
		GLBINDEX = glbindex1;
	}
	if(glbest > glbest2)
	{
		glbest = glbest2;
		GLBINDEX = glbindex2;
	}
	//fprintf(out,"%lf ",glbest);
  printf("\n%d,%.9f\n",GLBINDEX,glbest);
	for(int i=0;i<Dim;i++)
	{
		gBest[i]= particles[GLBINDEX].pbest[i];
	}
     
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