#include "../Project.hpp"

Neuron::Neuron() {
	bias = 0;
	oprtr = 0;
	numSyns = 0;
	val = 0;
	syns = 0;
}

Neuron::Neuron(int Bias, bool Oprtr)
{
	//printf("Creating Neuron With Basic Constructor\n");
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
	if (numSyns == 0)
		return;
	syns.reset();
}

void Neuron::InitRandomise(int range) {
	
	this->bias = (rand() % (range * 2)) - range;
	this->oprtr = (rand() % 2) == 0;
}

Synapse *Neuron::addSynapse(Synapse *syn)
{
	std::unique_ptr<Synapse[]> ret(new Synapse[numSyns + 1]);
	int i = 0;
	if (numSyns != 0)
	{
		for (i = 0; i < numSyns; i++)
			ret[i] = syns[i];
	}
	ret[i] = *(syn);
	numSyns++;
	syns.reset(ret.release());
	
	return syns.get();
}

Synapse *Neuron::addSynapses(Synapse *Syns, int size)
{
	std::unique_ptr<Synapse[]> ret(new Synapse[numSyns + size]);
	int i = 0;
	if (numSyns != 0)
	{
		for (i = 0; i < numSyns; i++)
			ret[i] = syns[i];
	}
	for (int j = 0; j < size; i++, j++)
		ret[i] = Syns[i];
	numSyns += size;
	syns.reset(ret.release());
	return syns.get();
}

Synapse *Neuron::MakeSynapse(Neuron *to, int randRange)
{
	return addSynapse(new Synapse(randRange, this, to));
}
Synapse *Neuron::MakeSynapses(Neuron **tos, int amount, int randRange)
{
	std::unique_ptr<Synapse[]> nSyns(new Synapse[amount]);
	for (int i = 0; i < amount; i++) {
		nSyns[i] = Synapse(randRange, this, tos[i]);
	}
	return addSynapses(nSyns.get(), amount);
}

bool Neuron::valueGreaterThanBias()
{
	return oprtr ? bias > val : bias < val;
}
void Neuron::FireNow()
{
	for (int i = 0; i < numSyns; i++)
	{
		syns[i].Fire();
	}
}
Synapse *Neuron::Fire()
{
	if (valueGreaterThanBias())
		return syns.get();
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
		syns[i].Randomise(chance, range);
}