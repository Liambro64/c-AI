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
Neuron::~Neuron() {
	if (syns != nullptr) {
		for (int i = 0; i < numSyns; i++)
			delete (syns + i);
		free(syns);
	}
	std::cout << "Destroyed Neuron" << std::endl;
}
Synapse *Neuron::addSynapse(Synapse *syn)
{
	Synapse *ret = syns;
	syns = (Synapse *)calloc(numSyns + 1, sizeof(Synapse));
	if (numSyns != 0)
	{
		for (int i = 0; i < numSyns; i++)
			*(syns + i) = *(ret + i);
		free(ret);
	}
	*(syns + numSyns) = *syn;
	numSyns++;
	return syns;
}

Synapse *Neuron::addSynapses(Synapse *Syns, int size)
{
	Synapse *ret = syns;
	syns = (Synapse *)calloc(numSyns + size, sizeof(Synapse));
	if (numSyns != 0)
	{
		for (int i = 0; i < numSyns; i++)
			*(syns + numSyns + i) = *(ret + i);
		free(ret);
	}
	for (int i = 0; i < size; i++)
		*(syns + i) = *(Syns + i);
	numSyns += size;
	return syns;
}

Synapse *Neuron::addSynapses(Synapse *Syns, int size)
{
	Synapse *ret = syns;
	syns = (Synapse *)calloc(numSyns + size, sizeof(Synapse));
	if (numSyns != 0)
	{
		for (int i = 0; i < numSyns; i++)
			*(syns + i) = *(ret + i);
		free(ret);
	}
	for (int i = 0; i < size; i++)
		*(syns + numSyns + i) = *(Syns + i);
	numSyns += size;
	return syns;
}
Synapse *Neuron::MakeSynapse(Neuron *to, int randRange)
{
	return addSynapse(new Synapse(randRange, this, to));
}
Synapse *Neuron::MakeSynapses(Neuron **tos, int amount, int randRange)
{
	Synapse *nSyns = (Synapse *)calloc(amount, sizeof(Synapse));
	for (int i = 0; i < amount; i++)
	{
		*(nSyns + i) = *(new Synapse(randRange, this, tos[i]));
	}
	return addSynapses(nSyns, amount);
}

bool Neuron::valueGreaterThanBias()
{
	return oprtr ? bias > val : bias < val;
}
void Neuron::FireNow()
{
	for (int i = 0; i < numSyns; i++)
	{
		(syns + i)->Fire();
	}
}
Synapse *Neuron::Fire()
{
	if (valueGreaterThanBias())
		return syns;
	return nullptr;
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
		(syns + i)->Fire();
	}
}