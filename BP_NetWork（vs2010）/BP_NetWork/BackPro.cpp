#include "Backpro.h"
#include <time.h>
#include <stdlib.h>


//	initializes and allocates memory on heap
CBackProp::CBackProp(int nl,int *sz,double b,double a):beta(b),alpha(a),isInitial(true)
{
	
	//	set no of layers and their sizes
	numl=nl;
	lsize=new int[numl];
	int i=0;
	int dim =0 ;
	for(int i=0;i<numl;i++){
		lsize[i]=sz[i];
	}

	//	allocate memory for output of each neuron
	out = new double*[numl];

	for( i=0;i<numl;i++){
		out[i]=new double[lsize[i]];
	}

	//	allocate memory for delta
	delta = new double*[numl];

	for(i=1;i<numl;i++){
		delta[i]=new double[lsize[i]];
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

	//	allocate memory for previous weights
	prevDwt = new double**[numl];

	for(i=1;i<numl;i++){
		prevDwt[i]=new double*[lsize[i]];

	}
	for(i=1;i<numl;i++){
		for(int j=0;j<lsize[i];j++){
			prevDwt[i][j]=new double[lsize[i-1]+1];
		}
	}

	//	seed and assign random weights
	srand((unsigned)(time(NULL)));
	for(i=1;i<numl;i++)
		for(int j=0;j<lsize[i];j++)
			for(int k=0;k<lsize[i-1]+1;k++)//bias is the last one
				{
					weight[i][j][k]= 0;//(double)(rand())/(RAND_MAX/2) - 1;//32767
					//if(k < lsize[i-1])
					//printf("%lf",weight[i][j][k]);
						dim++;
			}

	//	initialize previous weights to 0 for first iteration
	for(i=1;i<numl;i++)
		for(int j=0;j<lsize[i];j++)
			for(int k=0;k<lsize[i-1]+1;k++)
				prevDwt[i][j][k]=(double)0.0;

// Note that the following variables are unused,
//
// delta[0]
// weight[0]
// prevDwt[0]

//  I did this intentionaly to maintains consistancy in numbering the layers.
//  Since for a net having n layers, input layer is refered to as 0th layer,
//  first hidden layer as 1st layer and the nth layer as output layer. And 
//  first (0th) layer just stores the inputs hence there is no delta or weigth
//  values corresponding to it.

	/*
	* initial the engine of pso
	*/

	psoEngine = new PSO(dim,30,*this);
	

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
		for(int j=0;j<lsize[i];j++)
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
	delete psoEngine;
}

//	sigmoid function
double CBackProp::sigmoid(double in)
{
		return (double)(2/(1+exp(-2*in))-1);
}

//	mean square error
double CBackProp::mse(double *tgt) const
{
	double mse=0;
	for(int i=0;i<lsize[numl-1];i++){
		mse+=(tgt[i]-out[numl-1][i])*(tgt[i]-out[numl-1][i]);
		//printf("target:%lf\n",tgt[i]);
	}
	return mse/2;
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
	{	out[0][i]=in[i];  // output_from_neuron(i,j) Jth neuron in Ith Layer
	
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
			out[i][j]=sigmoid(sum);				// Apply sigmoid function
		
		}
	}
	//printf("\noutput is :%lf",out[lsize[numl-1]][0]);
}


//	backpropogate errors from output
//	layer uptill the first hidden layer
void CBackProp::bpgt(double *in,double *tgt)
{
	double sum;
	int i=0;
	//	update output values for each neuron
	ffwd(in);

	//	find delta for output layer
	for(int i=0;i<lsize[numl-1];i++){
		delta[numl-1][i]=out[numl-1][i]*
		(1-out[numl-1][i])*(tgt[i]-out[numl-1][i]);
	}

	//	find delta for hidden layers	
	for(i=numl-2;i>0;i--){
		for(int j=0;j<lsize[i];j++){
			sum=0.0;
			for(int k=0;k<lsize[i+1];k++){
				sum+=delta[i+1][k]*weight[i+1][k][j];
			}
			delta[i][j]=out[i][j]*(1-out[i][j])*sum;
		}
	}

	//	apply momentum ( does nothing if alpha=0 )
	for(i=1;i<numl;i++){
		for(int j=0;j<lsize[i];j++){
			for(int k=0;k<lsize[i-1];k++){
				weight[i][j][k]+=alpha*prevDwt[i][j][k];
			}
			weight[i][j][lsize[i-1]]+=alpha*prevDwt[i][j][lsize[i-1]];
		}
	}

	//	adjust weights usng steepest descent	
	for(i=1;i<numl;i++){
		for(int j=0;j<lsize[i];j++){
			for(int k=0;k<lsize[i-1];k++){
				prevDwt[i][j][k]=beta*delta[i][j]*out[i-1][k];
				weight[i][j][k]+=prevDwt[i][j][k];
			}
			prevDwt[i][j][lsize[i-1]]=beta*delta[i][j];
			weight[i][j][lsize[i-1]]+=prevDwt[i][j][lsize[i-1]];
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
			{for(int k=0;k<lsize[i-1]+1;k++)//bias is the last one
				{
					weight[i][j][k]= psoEngine->particles[index].position[dim++];//(double)(rand())/(RAND_MAX/2) - 1;//32767
					//printf("%lf,",weight[i][j][k]);
			}
			//printf("\n");
		}
		//printf("///////////////");
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
	
	for(;psoEngine->T++<psoEngine->Tmax;)
	{
		//getWeightFromPSO();
		//ffwd(in);

		if(psoEngine->glbest<0.00001)
		{
			//printf("%d\n",psoEngine->glbindex);
				printf("\nachived in pso\n");
				break;
		}
		psoEngine->update_Interweight();
		psoEngine->update_speed();
		psoEngine->update_position();
		psoEngine->update_gbest();
		
	}
	psoEngine->T=0;
//	psoEngine->Print();
	//printf("%d\n",psoEngine->glbindex);
	int dim = 0;
	for(int i=1;i<numl;i++)
		for(int j=0;j<lsize[i];j++)
			{for(int k=0;k<lsize[i-1]+1;k++)//bias is the last one
				{
					weight[i][j][k]= psoEngine->gbest[dim++];//(double)(rand())/(RAND_MAX/2) - 1;//32767
					//printf("\t%lf,",weight[i][j][k]);
			}
			//printf("\n");
		}

}