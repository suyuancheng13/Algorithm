#pragma once

class Particle
{
public:
	Particle(int Dim);
	/*~Particle()
	{
		delete[] v;
		delete[] position;
		delete[] pbest;
	}*/

	double *v;//粒子本身速度
	double *position;//粒子位置
	double *pbest;//粒子本身最优解

	double fitness;//粒子适应度
	double bestfitness;//粒子最好适应度
};