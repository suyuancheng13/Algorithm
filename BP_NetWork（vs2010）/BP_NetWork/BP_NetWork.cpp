#include"BP_NetWork.h"

// NeuralNet.cpp : Defines the entry point for the console application.
//

using namespace std;

//BP_NetWork::BP_NetWork(int _layersNum,int *_layersArray,int _epoch,int _thresh,int _inputn,double *_inputVector,double *_targetVector,int _targetn):
//	layersNum(_layersNum),epoch(_epoch),thresh(_thresh),inputn(_inputn),targetn(_targetn)
//	{
//		double beta = 0.3, alpha = 0.1;
//		/*
//		construct the layers array
//		*/
//		layersArray = _layersArray;
//	
//		/*
//		construc the input vector
//		*/
//		inputVector = _inputVector;
//		
//		/*
//		construct the target vector
//		*/
//		targetVector = _targetVector;
//		
//		bp = new CBackProp(layersNum, layersArray, epoch,thresh,inputn,beta, alpha);
//	}
//	void BP_NetWork::Train()
//	{
//		cout<< endl <<  "Now training the network...." << endl;
//		bp->bpgt_pso(inputVector, targetVector);
//		cout<<  endl <<  "MSE:  " << bp->psoEngine->glbest//bp->mse(&data[i%8][3]) 
//				<< "... Training..." << endl;
//	}
//	void BP_NetWork::Simulate(double *_testVector,int testn)
//	{
//		cout<< "Now using the trained network to make predctions on test data...." << endl << endl;	
//		for ( int i = 0 ; i < testn ; i++ )
//		{
//			bp->ffwd((_testVector+i));
//			for(int k =0;k<layersArray[0];k++)
//				printf("\t%.4lf",(*(_testVector+i)+k));
//			for(int j=0;j<targetn;j++)
//				printf("\t%.4lf",bp->Out(j));//,bp->Out(1));
//			printf("\n");
//			//cout << testData[i][0]<< "  " << testData[i][1]<< "  "  << testData[i][2]<< "  " << bp->Out(0) << endl;
//		}
//	}


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
	int numLayers = 3, lSz[3] = {2,12,1};

	// Learing rate - beta
	// momentum - alpha
	// Threshhold - thresh (value of target mse, training stops once it is achieved)
	double beta = 0.3, alpha = 0.1, Thresh =  0.000001;

	
	// maximum no of iterations during training
	long num_iter = 1;
	int inputn = 4;
	
	// Creating the net
	CBackProp *bp = new CBackProp(numLayers, lSz, 10000,Thresh,inputn,beta, alpha);

	cout<< endl <<  "Now training the network...." << endl;	
	for ( i=0; i<num_iter ; i++)
	{
		/*
		  二维数组其实相当于(*a)[]这样一个指针，不是一个指针或者二级指针
		*/
		//bp->bpgt_pso(testData[0], target[0]);
		bp->bpgt_pso(p[0], t);
			cout<<  endl <<  "MSE:  " << bp->psoEngine->glbest//bp->mse(&data[i%8][3]) 
				<< "... Training..." << endl;
	
	}
	cout<< "Now using the trained network to make predctions on test data...." << endl << endl;	
	for ( i = 0 ; i < inputn ; i++ )
	{
		bp->ffwd(p[i]);
		printf("%lf \t%lf\t%.4lf\n",p[i][0],p[i][1],bp->Out(0));//,bp->Out(1));
		//cout << testData[i][0]<< "  " << testData[i][1]<< "  "  << testData[i][2]<< "  " << bp->Out(0) << endl;
	}

	return 0;
}