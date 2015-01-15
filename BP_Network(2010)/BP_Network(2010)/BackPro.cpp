#include "Backpro.h"
#include <time.h>
#include <stdlib.h>


//	initializes and allocates memory on heap
CBackProp::CBackProp(int nl,int *sz,int _iterator,double _thresh,int inputn,double b,double a):beta(b),alpha(a),isInitial(true),iterator(_iterator),	Thresh(_thresh),inputN(inputn)
{
	
	//	set no. of layers and their sizes
	numl=nl;
	lsize=new int[numl];
	int i=0;
	int dim =0 ;
	Dim =0;
	for(int i=0;i<numl;i++){
		lsize[i]=sz[i];
	}

	//	allocate memory for output of each neuron
	out = new double*[numl];

	for( i=0;i<numl;i++){
		out[i]=new double[lsize[i]];
	}

	//	allocate memory for weights
	/*
	* i present the layer no.
	* j present the neuro in the layer
	* the third dimension present the weight for each neuro that in front of it
	*/
	weight = new double**[numl];

	for(i=1;i<numl;i++){
		weight[i]=new double*[lsize[i]];
	}
	for(i=1;i<numl;i++){
		for(int j=0;j<lsize[i];j++){
			weight[i][j]=new double[lsize[i-1]+1];
		}
	}

	//	seed and assign random weights
	srand((unsigned)(time(NULL)));
	for(i=1;i<numl;i++)
		for(int j=0;j<lsize[i];j++)
			for(int k=0;k<lsize[i-1]+1;k++)//bias is the last one
				{
					weight[i][j][k]= 0;//(double)(rand())/(RAND_MAX/2) - 1;//32767
						dim++;
						Dim++;
			}

				/*
				initial the delta weight
				*/
		prevDwt = new double**[numl];
		for(i=1;i<numl;i++){
			prevDwt[i]=new double*[lsize[i]];
		}
		for(i=1;i<numl;i++){
			for(int j=0;j<lsize[i];j++){
				prevDwt[i][j]=new double[lsize[i-1]+1];
			}
		}
		for(i=1;i<numl;i++)
			for(int j=0;j<lsize[i];j++)
				for(int k=0;k<lsize[i-1]+1;k++)
					prevDwt[i][j][k]=(double)0.0;
	
		/*initial the delta for gradient ascent
		*/
		delta = new double*[numl];
		for(i=1;i<numl;i++){
			delta[i]=new double[lsize[i]];
	}
		for(int i=1;i<numl;i++)
			for(int j=0;j<lsize[i];j++)
				delta[i][j] = 0.0;
	/*
	* initial the engine of pso
	*/

	psoEngine = new PSO(dim,60,*this);
	gaEngine = new RCGA(*this);
	

}
void CBackProp::initial_wb_nw()
{
	srand((unsigned)(time(NULL)));

	double wMag = 0.7*pow(lsize[1],1.0/lsize[0]);
	double **w = new double *[lsize[1]];
	double *W = new double[lsize[1]];
	double *bias = new double[lsize[1]];
	for(int i=0;i<lsize[1];i++)
	{
		w[i] = new double[lsize[0]];
	}
	/*
	rand with uniformation
	*/
	for(int i=0;i<lsize[1];i++)
		{
			for(int j=0;j<lsize[0];j++)
		{
			w[i][j] = 2.0*rand()/RAND_MAX-1;
		}
	}
	/*
	uniformat the rand weights
	*/
	double ss = 0;
	for(int i=0;i<lsize[1];i++)
	{
		ss = 0;
		for(int j=0;j<lsize[0];j++)
		{ 
			ss += pow(w[i][j],2);
		}
		W[i] = sqrt(1/ss);
	}

	for(int i=0;i<lsize[1];i++)
	{
		for(int j=0;j<lsize[0];j++)
		{ 
			w[i][j] *= wMag*W[1];
		}
	}
	/*
	compute the bias
	*/
	double interval = 2.0/lsize[1];
	double incrument = -1;
	for(int i=0;i<lsize[1];i++)
	{
		bias[i] =  wMag*incrument;
		incrument += interval;
	}
	for(int i=0;i<lsize[1];i++)
	{
		if(w[i][0]>0)
	      bias[i] *=1;
		else if(w[i][0]== 0)
			bias[i] =0;
		else
			bias[i] *=-1;
	}
	for(int i=1;i<numl;i++)//layers NO.
		for(int j=0;j<lsize[i];j++)//the neurons in layers
			for(int k=0;k<lsize[i-1]+1;k++)//bias is the last one
				{
					if( i == 1)
					{
						if(k == lsize[i-1])
							weight[i][j][k] = bias[j];
						else 
							weight[i][j][k] = w[j][k];
					}
					else
						weight[i][j][k]= 1.0*rand()/RAND_MAX;//(double)(rand())/(RAND_MAX/2) - 1;//32767
				//	printf("\t%lf",weight[i][j][k]);
			}
	delete [] bias;
	delete [] W;
  for(int i=0;i<lsize[1];i++)
		delete[] w[i];
  delete[] w;
}


CBackProp::~CBackProp()
{
	//	free out
	for(int i=0;i<numl;i++)
		delete[] out[i];
	delete[] out;
	int i=0;
	//	free delta
	for(i=1;i<numl;i++)
		delete[] delta[i];
	delete[] delta;
	//	free weight
	for(i=1;i<numl;i++)
		for(int j=0;j<lsize[i]+1;j++)
			delete[] weight[i][j];
	for(i=1;i<numl;i++)
		delete[] weight[i];
	delete[] weight;

		//	free prevDwt
	for(i=1;i<numl;i++)
		for(int j=0;j<lsize[i];j++)
			delete[] prevDwt[i][j];
	for(i=1;i<numl;i++)
		delete[] prevDwt[i];
	delete[] prevDwt;
	//	free layer info
	delete[] lsize;
	//delete psoEngine;
	delete gaEngine;
}

//	sigmoid function
double CBackProp::sigmoid(double in)
{
		return (double)(2/(1+exp(-2*in))-1);//tansig sigmoid function
}
double CBackProp::purelin(double in)
{
	return in;
}
//	mean square errorsqrt
double CBackProp::mse(double *tgt) const
{
	double mse=0;
	for(int i=0;i<lsize[numl-1];i++){
		mse+=((tgt[i]-out[numl-1][i])*(tgt[i]-out[numl-1][i]));
	}
	return mse/(inputN*lsize[numl-1]);
}


//	returns i'th output of the net
double CBackProp::Out(int i) const
{
	return out[numl-1][i];
}

// feed forward one set of input
void CBackProp::ffwd(double *in)
{
	double sum;
	int i=0;
	
	//	assign content to input layer
	for(int i=0;i<lsize[0];i++)
	{	
		out[0][i]=in[i];  // output_from_neuron(i,j) Jth neuron in Ith Layer
	
	}

	//	assign output(activation) value 
	//	to each neuron usng sigmoid func
	for(i=1;i<numl;i++){				// For each layer
		for(int j=0;j<lsize[i];j++){		// For each neuron in current layer
			sum=0.0;
			for(int k=0;k<lsize[i-1];k++){		// For input from each neuron in preceeding layer
				sum+= out[i-1][k]*weight[i][j][k];	// Apply weight to inputs and add to sum
			}
		
			sum+=weight[i][j][lsize[i-1]];		// Apply bias
			if(i<numl-1)
				out[i][j]=sigmoid(sum);				// Apply sigmoid function
			else
				out[i][j]=purelin(sum);				//The output sigmoid function is purelin 
																	//to ensure output every function result
		}
	}
}
void CBackProp::maxMin(double *matrix, int rows,int coloums,double *max,double *min)
{
	int index = coloums;
	for(int i =0;i<rows;i++)
		//for(int j=0;j<coloums;j++)
		{
		
			if(*(matrix+index)>*max)
			{
				*max = *(matrix+index);
			}
			if(*(matrix+index)<*min)
			{
				*min = *(matrix+index);
			}
			index+= lsize[0];
		
		}

}
/*
normlize then normalized matrix by columns
x' = x*sqrt(1/sum(pow(xi,2))) (i = 0,1,,,columns)

*/
void CBackProp::uniformation(double *matrix,int rows,int coloums)
{
	int index =0;
	double max= -LONG_MAX;
	double min = LONG_MAX;
	
	for(int i =0;i<coloums;i++)
	{
		max= -LONG_MAX;
		min = LONG_MAX;
	   maxMin(matrix,rows,i,&max,&min);
	   index =i;
		for(int j=0;j<rows;j++)
		{
			*(matrix+index) = 2*(*(matrix+index)-min)/(max-min)-1;
				index+=lsize[0];
		}

		
	}
}

/*
* get the weight from particles
*/
void CBackProp::getWeightFromPSO(int index)
{
	int dim = 0;
	for(int i=1;i<numl;i++)
		for(int j=0;j<lsize[i];j++)
			{
				for(int k=0;k<lsize[i-1]+1;k++)//bias is the last one
				{
					weight[i][j][k]= psoEngine->particles[index].position[dim++];
			}
		}
}
/*
* get the weight from GA
*/
void CBackProp::getWeightFromGA(int index)
{
	int dim = 0;
	for(int i=1;i<numl;i++)
		for(int j=0;j<lsize[i];j++)
			{for(int k=0;k<lsize[i-1]+1;k++)//bias is the last one
				{
					weight[i][j][k]= gaEngine->population[index].chromosome[dim++];
			}
		}
}
void CBackProp::bpgt_gd(double *in,double *tgt)
{

	double sum=0.0;
	double _mse = 0.0;
	//int i=0;
	double beta = 0.3;//learning rate
	//FILE *fp = fopen("ga_gd_result.txt","a");
	/*
	intitial weights and bias
	*/
	//initial_wb_nw();
	for(int t =0 ;t<iterator-160;t++)
	{
			/*
			* batch training
			*/
		_mse =0;
		for(int i=1;i<numl;i++)
			for(int j=0;j<lsize[i];j++)
				delta[i][j] = 0.0;
		for(int i=1;i<numl;i++)
			for(int j=0;j<lsize[i];j++)
				for(int k=0;k<lsize[i-1]+1;k++)
					prevDwt[i][j][k]=(double)0.0;

			for(int l=0;l<inputN;l++)
			{
				//	update output values for each neuron
				ffwd(in+l*lsize[0]);	

				_mse += mse(tgt+l*lsize[numl-1]);
				//	find delta for output layer
				for(int i=0;i<lsize[numl-1];i++){
					delta[numl-1][i]  =  ((tgt+l*lsize[numl-1])[i]-out[numl-1][i]);
				}

				//	find delta for hidden layers	
				for(int i=numl-2;i>0;i--){
					for(int j=0;j<lsize[i];j++){
						sum=0.0;
						for(int k=0;k<lsize[i+1];k++){
							sum+=delta[i+1][k]*weight[i+1][k][j];
						}
						delta[i][j] =(1-pow(out[i][j],2))*sum;
					}
				}	
				for(int i=1;i<numl;i++){
					for(int j=0;j<lsize[i];j++){
						for(int k=0;k<lsize[i-1];k++){
							prevDwt[i][j][k] += beta*delta[i][j]*out[i-1][k];
							//weight[i][j][k]+=prevDwt[i][j][k];
						}
						prevDwt[i][j][lsize[i-1]] +=beta*delta[i][j];//调整bias
						//weight[i][j][lsize[i-1]]+=prevDwt[i][j][lsize[i-1]];
					}
				}
			}
		//	fprintf(fp,"%lf\n",_mse);
			printf("\t%d \t%lf\n",t,_mse);
			if(_mse<Thresh)
			{
				break;
			}
			/*
			adjust weights usng steepest descent
			*/
			for(int i=1;i<numl;i++){
					for(int j=0;j<lsize[i];j++){
						for(int k=0;k<lsize[i-1];k++){
							weight[i][j][k]+=prevDwt[i][j][k]/inputN;
							//	printf(" \t%lf\n",prevDwt[i][j][k]/inputN);
						}
						weight[i][j][lsize[i-1]]+=prevDwt[i][j][lsize[i-1]]/inputN;
					}
			}
	}
}
/*
*using pso to adjust the weight
*/
void CBackProp::bpgt_pso(double *in,double *tgt )
{
	//	update output values for each neuron
	
	if(isInitial)
	{
		psoEngine->limit_PSO();
		psoEngine->initial_PSO(in,tgt);
		psoEngine->initialBest(isInitial);
		isInitial = false;
	}

	//psoEngine->in = in;
	//psoEngine->tgt = tgt;
	///*
	//对于每一组输入都要更新最优值，否则可能因为对前一组输入值更优导致无法更新
	//*/
	//psoEngine->initialBest(isInitial);
	
	for(;psoEngine->T++<psoEngine->Tmax-160;)
	{

		printf("Epoch:%d\n",psoEngine->T);
		if(psoEngine->glbest<Thresh)
		{
				printf("\nachived in pso\n");
				break;
		}
		psoEngine->update_Interweight();
		psoEngine->update_speed();
		psoEngine->update_position();
		psoEngine->update_gbest();
		
		
	}
	psoEngine->T=0;
	int dim = 0;
	for(int i=1;i<numl;i++)
		for(int j=0;j<lsize[i];j++)
			{
				for(int k=0;k<lsize[i-1]+1;k++)//bias is the last one
				{
					weight[i][j][k]= psoEngine->gbest[dim++];
				}
		}

}
void CBackProp::bpgt_ga(double *in, double *tgt)
{
		gaEngine->initialRCGA(60,Dim,160,0.8,0.05, 5.0,-5.0,0.0);
		gaEngine->gaOptions(in,tgt);
		gaEngine->evolution();

		int dim = 0;
		for(int i=1;i<numl;i++)
			for(int j=0;j<lsize[i];j++)
				{
					for(int k=0;k<lsize[i-1]+1;k++)//bias is the last one
					{
					weight[i][j][k]= gaEngine->bestindividual.chromosome[dim++];
					}
			}

}