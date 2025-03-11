#include "../Project.hpp"

Neuron::Neuron(int Bias, bool Oprtr)
{
	this->bias = bias;
	this->oprtr = Oprtr;
	this->numSyns = 0;
	this->val = 0;
	this->syns = nullptr;
}

Neuron::Neuron(int randRange)
{
	this->bias = (rand() % (randRange * 2)) - randRange;
	this->oprtr = (rand() % 2) == 0;
	this->numSyns = 0;
	this->val = 0;
	this->syns = nullptr;
}
void Neuron::addSynapse(Synapse *syn)
{
	Synapse **ret = syns;
	syns = (Synapse **)calloc(numSyns + 1, sizeof(Synapse **));
	if (numSyns != 0)
	{
		for (int i = 0; i < numSyns; i++)
			*(syns + i) = *(ret + i);
		free(ret);
	}
	*(syns + numSyns) = syn;
	numSyns++;
}

void Neuron::addSynapses(Synapse **Syns, int size)
{
	Synapse **ret = syns;
	syns = (Synapse **)calloc(numSyns + size, sizeof(Synapse **));
	if (numSyns != 0)
	{
		for (int i = 0; i < numSyns; i++)
			*(syns + i) = *(ret + i);
		free(ret);
	}
	for (int i = 0; i < size; i++)
		*(syns + i) = *(Syns + i);
	numSyns += size;
}
bool Neuron::valueGreaterThanBias()
{
	return oprtr ? bias > val : bias < val;
}
void Neuron::Fire()
{
	if (valueGreaterThanBias())
		for (int i = 0; i > numSyns; i++)
		{
			(*(syns + i))->Fire();
		}
}
void Neuron::Randomise(int chance, int range)
{
	int r1 = rand() % chance;
	int r2 = rand() % chance;
	if (r1 == 1)
		bias += (rand() % (range * 2)) - range;
	if (r2 == 1)
		oprtr = (rand() % 2) == 0;
	
	for (int i = 0; i > numSyns; i++)
	{
		(*(syns + i))->Fire();
	}
}