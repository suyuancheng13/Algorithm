#include"BP_NetWork.h"

// NeuralNet.cpp : Defines the entry point for the console application.
//

using namespace std;

BP_NetWork::BP_NetWork(int _layersNum,int *_layersArray,int _epoch,double _thresh,int _inputn,double *_inputVector,double *_targetVector):
	layersNum(_layersNum),epoch(_epoch),thresh(_thresh),inputn(_inputn)
	{
		double beta = 0.3, alpha = 0.1;
		/*
		construct the layers array
		*/
		layersArray = _layersArray;
	
		/*
		construc the input vector
		*/
		inputVector = _inputVector;
		
		/*
		construct the target vector
		*/
		targetVector = _targetVector;
		targetn = layersArray[layersNum-1];
		bp = new CBackProp(layersNum, layersArray, epoch,thresh,inputn,beta, alpha);
	}
	void BP_NetWork::Train()
	{
		cout<< endl <<  "Now training the network...." << endl;
		bp->bpgt_pso(inputVector, targetVector);
		cout<<  endl <<  "MSE:  " << bp->psoEngine->glbest//bp->mse(&data[i%8][3]) 
				<< "... Training..." << endl;
	}
	void BP_NetWork::Simulate(double *_testVector,int testn)
	{
		cout<< "Now using the trained network to make predctions on test data...." << endl << endl;	
		for ( int i = 0 ; i < testn ; i++ )
		{
			bp->ffwd((_testVector+i*layersArray[0]));
			for(int k =0;k<layersArray[0];k++)
				printf("\t%.4lf",(_testVector+i*layersArray[0])[k]);
			for(int j=0;j<targetn;j++)
				printf("\t%.4lf",bp->Out(j));//,bp->Out(1));
			printf("\n");
			//cout << testData[i][0]<< "  " << testData[i][1]<< "  "  << testData[i][2]<< "  " << bp->Out(0) << endl;
		}
	}


