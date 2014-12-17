#include"Particle.h"
Particle::Particle(int Dim):fitness(0),bestfitness(0)
{
	v = new double[Dim];
	position = new double[Dim];
	pbest = new double[Dim];	 
}