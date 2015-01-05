
//////////////////////////////////////////////
//	Fully connected multilayered feed		//
//	forward	artificial neural network using	//
//	Backpropogation	algorithm for training.	//
//////////////////////////////////////////////


#ifndef backprop_h
#define backprop_h

#include<assert.h>
#include<iostream>
#include<stdio.h>
#include<math.h>
#include"PSO.h"
#include"RCGA.h"

class CBackProp{

//	output of each neuron
public:
	double **out;

//	delta error value for each neuron
	double **delta;

//	vector of weights for each neuron
	double ***weight;

//	no of layers in net
//	including input layer
	int numl;

	// input vector numbers
	int inputN;
//	vector of numl elements for size 
//	of each layer
	int *lsize;

private:
//	learning rate
	double beta;
	int Dim;
//	momentum parameter
	double alpha;


//	storage for weight-change made
//	in previous epoch
	double ***prevDwt;

//	squashing function
	double sigmoid(double in);
	double purelin(double in);
// pso engine
	
	double *target;
	bool  isInitial;
	
	double Thresh;
public:
	PSO	*psoEngine;
	RCGA *gaEngine;
	int iterator;

public:
	~CBackProp();

//	initializes and allocates memory
	CBackProp(int nl,int *sz,int iterator,double Thresh,int inputn,double b,double a);

//	backpropogates error for one set of input
	void bpgt(double *in,double *tgt);
	/*
	*using pso to adjust the weight
	*/
	void bpgt_pso(double *in,double *tgt);
	/*
	*using ga to adjut the weight
	*/
	void bpgt_ga(double *in,double *tgt);

//	feed forwards activations for one set of inputs
	void ffwd(double *in);

//	returns mean square error of the net
	double mse(double *tgt) const;	
	
//	returns i'th output of the net
	double Out(int i) const;

	//copy the weight from particle
	void  getWeightFromPSO(int index);
	//copy the weight from chromosome of GA
	void  getWeightFromGA(int index);
};

#endif