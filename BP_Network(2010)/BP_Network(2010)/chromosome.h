#pragma once
#include<vector>
using namespace std;


class Individual
{
public:
	Individual(){}
	Individual(int _geneNum);
	void initial(double _topLimit,double _downLimit);

public:/*Variables*/
	vector<double>chromosome;
	double fitness;
	double weight;
private:
	int geneNum;
	double topLimit;
	double bottomLimit;
	
};