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

	double *v;//���ӱ����ٶ�
	double *position;//����λ��
	double *pbest;//���ӱ������Ž�

	double fitness;//������Ӧ��
	double bestfitness;//���������Ӧ��
};