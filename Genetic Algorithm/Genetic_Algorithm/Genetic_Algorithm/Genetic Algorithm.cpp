// Genetic Algorithm.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"
#include"RCGA.h"

int main()
{
	double re[2];
	RCGA rcga;
	rcga.initialRCGA(40,1,5000,0.8,0.1,3,0,3.0);
	rcga.evolution();
	//rcga.result(re);

	return 0;
}

