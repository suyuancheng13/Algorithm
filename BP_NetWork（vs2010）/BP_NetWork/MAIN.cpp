#include"BP_NetWork.h"
int main(int argc, char* argv[])
{


	// prepare XOR traing data
	int i=0;
	double data[][4]={
				0,	0,	0,	0,
				0,	0,	1,	1,
				0,	1,	0,	1,
				0,	1,	1,	0,
				1,	0,	0,	1,
				1,	0,	1,	0,
				1,	1,	0,	0,
				1,	1,	1,	1 };

				double p[4][2] = {-1,0,-1,5,2,0,2,5};//{-1 ,-1, 2 ,2,0 ,5 ,0 ,5};
				double t[4]={-1,-1,1,1};
	// prepare test data
	double target[8][2]  = {  0,0,
											0,1,
											0,1,
											0,0,
											0,1,
											0,0,
											0,0,
											0,1};
	double testData[][3]={
								0,      0,      0,
                                0,      0,      1,
                                0,      1,      0,
                                0,      1,      1,
                                1,      0,      0,
                                1,      0,      1,
                                1,      1,      0,
                                1,      1,      1};

	
	// defining a net with 4 layers having 3,3,3, and 1 neuron respectively,
	// the first layer is input layer i.e. simply holder for the input parameters
	// and has to be the same size as the no of input parameters, in out example 3
	int numLayers = 3, lSz[3] = {2,12,1};

	
	// Learing rate - beta
	// momentum - alpha
	// Threshhold - thresh (value of target mse, training stops once it is achieved)
	double beta = 0.3, alpha = 0.1, Thresh =  0.000001;

	
	// maximum no of iterations during training
	long num_iter = 1;
	int inputn = 4;
//	long i;
	
	BP_NetWork *bpnet = new BP_NetWork(numLayers,lSz,10000,Thresh,4,p[0],t);
	bpnet->Train();
	bpnet->Simulate(p[0],4);
	
	return 0;
}