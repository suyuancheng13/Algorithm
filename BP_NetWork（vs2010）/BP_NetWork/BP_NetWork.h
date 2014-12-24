#pragma once
#include "BackPro.h"
class BP_NetWork{
public:
	/*
	*constuctor
	*parameter _layerNum: the number of layers
	*parameter _layerArray: the layer array, the first one array is the INPUT layer, the last one is 
						OUTPUT(target) layer
	*parameter _epoch: the iterator number of training
	*parameter _thresh: the target of the training
	*parameter _inputn: the input vector number, not the length of the input vector that contained in first 
	                                  element of the layersArray
	*parameter _inputVector: the input vector pointer, that point to the first element in the vector
	*parameter  _targetVector:the target vector pointer,corresponding to the input vector, that point to the first element
		*/
	 BP_NetWork(int _layersNum,int *_layersArray,int _epoch,double _thresh,int _inputn,double *_inputVector,double *_targetVector);
	 ~BP_NetWork()
	{
		//delete bp;
		//delete[] layersArray;
		//delete[] inputVector;
		//delete[] testVector;
		//delete[] targetVector;
	}
	 /*
	 * TODO: train the bp network to ensure the function
	 */
	void Train();
	/*
	*TODO: test the data use the bp network that trained by the Train function
	*parameter testVector: the test data point, that point to the first element int the array
	*paramete testn: the test array quantities not the length of the test vector
	*/
	void Simulate(double *testVector,int testn);
private:
	int layersNum;
	int *layersArray;
	int epoch;
	double thresh;
	int inputn;
	int testn;
	int targetn;
	double *inputVector;
	double *targetVector;
	CBackProp *bp ;

};