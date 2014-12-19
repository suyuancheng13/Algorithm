#pragma once
#include "BackPro.h"
class BP_NetWork{
public:
	 BP_NetWork(int _layersNum,int *_layersArray,int _epoch,int _thresh,int _inputn,double *_inputVector,double *_targetVector,int _targetn);
	 ~BP_NetWork()
	{
		//delete bp;
		//delete[] layersArray;
		//delete[] inputVector;
		//delete[] testVector;
		//delete[] targetVector;
	}
	void Train();
	void Simulate(double *testVector,int testn);
private:
	int layersNum;
	int *layersArray;
	int epoch;
	int thresh;
	int inputn;
	int testn;
	int targetn;
	double *inputVector;
	double *targetVector;
	CBackProp *bp ;

};