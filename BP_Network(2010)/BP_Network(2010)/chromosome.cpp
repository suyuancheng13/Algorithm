#include"chromosome.h"
#include"BackPro.h"
#include<cmath>
Individual::Individual(int _geneNum):geneNum(_geneNum),fitness(
		0),weight(0)
	{
		
}
void Individual::initial(double _topLimit,double _downLimit)
{
	topLimit = _topLimit;
	bottomLimit = _downLimit;
	/*
	*initial the chromosome
	*/
	for(int i=0;i<geneNum;i++)
	{
		double gen = 1.0*rand()/(RAND_MAX-1)*(topLimit-bottomLimit)+bottomLimit;
		chromosome.push_back(gen);
	}

}