#include"PSO.h"
#include"BackPro.h"

PSO::PSO(int _Dim,int _Number,CBackProp &_bp):Dim(_Dim),number(_Number),bp(_bp)
{
	
	 c1=2.0;
	 c2=2.0;

	 w =0 ;
	 wmax=0.9;
	 wmin=0.4;

	 T=0;
	 Tmax=0;

	 pmax=0;
	 pmin=0;

	 vmax=0;


	 glbindex = -1;
	gbest = new double[Dim];
	
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
	   //  out = fopen("ga_pso_result.txt","a");
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

void PSO::fitness(){
	int i,j;
	double sum=0;
	for(i=0;i<number;i++){
		sum = 0;
		bp.getWeightFromPSO(i);
		for(int j=0;j<bp.inputN;j++){
			//printf("%lf %lf\n",(tgt+j*bp.lsize[bp.numl-1])[0],(tgt+j*bp.lsize[bp.numl-1])[1]);
			bp.ffwd((in+j*bp.lsize[0]));
			sum += bp.mse(tgt+j*bp.lsize[bp.numl-1]);
		}
		particles[i].fitness =sum;

	}

}

void PSO::limit_PSO(){
	float a= 5,b=-5;
	pmax=a;pmin=b;vmax= 1.50;//pmax-pmin;
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
		bp.initial_wb_nw();
		for(j=0;j<Dim;j++)
		{
			particles[i].v[j]=vmax*1.0*rand()/RAND_MAX;

			for(int ii=1;ii<bp.numl;ii++)
				for(int jj=0;jj<bp.lsize[ii];jj++)
				{
					for(int k=0;k<bp.lsize[ii-1]+1;k++)//bias is the last one
					{
						particles[i].position[j] = bp.weight[ii][jj][k];
					}
				}
			
			//particles[i].position[j]= 1.0*rand()/RAND_MAX*(pmax-pmin)+(pmin);
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

	}

	for(i=0;i<number;i++)
	{
		particles[i].bestfitness=particles[i].fitness;	
		for(j=0;j<Dim;j++)
				particles[i].pbest[j]=particles[i].position[j];
	}

	if(!initial)
	{
			glbest = particles[glbindex].bestfitness;
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
			gbest[j]=particles[glbindex].position[j];
		}
	glbest=particles[glbindex].bestfitness;
	   
}

void PSO::update_Interweight(){
	w=wmax-T*(wmax-wmin)/Tmax;
}

void PSO::update_speed(){
	int i,j;
	srand((unsigned)(time(NULL)));

	
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
void PSO::update_position(){
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
		if(particles[i].fitness<particles[i].bestfitness)
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
void PSO::update_gbest(){
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
  printf("\n%d,%.9f\n",glbindex,glbest);
   //fprintf(out,"%.10lf\n",glbest);
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