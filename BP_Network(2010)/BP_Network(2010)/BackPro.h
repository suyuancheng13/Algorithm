
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


public:
	//	output of each neuron
	double **out;
	double **delta;
//	vector of weights for each neuron
	double ***weight;
	double ***prevDwt;
//	no of layers in net	including input layer
	int numl;

// input vector numbers

	int inputN;

//	vector of numl elements for size of each layer
	int *lsize;


private:
	double beta;
	int Dim;
//	momentum parameter
	double alpha;

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

	//initial the weight and bias with Nguyen Widrow algorithm
	void initial_wb_nw();
	/*
	*using gredicent to adjust the weight
	*/
	void bpgt_gd(double *in,double *tgt);
	/*
	*using pso to adjust the weight
	*/
	void bpgt_pso(double *in,double *tgt);
	/*
	*using ga to adjut the weight
	*/
	void bpgt_ga(double *in,double *tgt);

   /*
   feed forwards activations for one set of inputs
   */
	void ffwd(double *in);

//	returns mean square error of the net
	double mse(double *tgt) const;	
	
//	returns i'th output of the net
	double Out(int i) const;

	//copy the weight from particle
	void  getWeightFromPSO(int index);

	//copy the weight from chromosome of GA
	void  getWeightFromGA(int index);

	void uniformation(double *matrix,int rows,int coloums);
	

private:
	/*
	get the max and min elements
	*/
	void maxMin(double *matrix, int rows,int coloums,double *max,double *min);
};

#endif