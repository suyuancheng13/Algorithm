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
    double test[][2] = {-0.98, 0.0001, -1.010 ,4.89};
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

	

	int numLayers = 3, lSz[3] = {2,12,1};


	double Thresh =  0.000001;

	int inputn = 4;

	/*
     bp network train and test
     */
	BP_NetWork *bpnet = new BP_NetWork(numLayers,lSz,10000,Thresh,4,p[0],t);
	bpnet->Train();
	bpnet->Simulate(test[0],2);
	
	return 0;
}