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
	//f("Creating Neuron With Basic Constructor\n");
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

Neuron::~Neuron()
{
	// Ensure unique_ptr automatically cleans up memory
	syns.reset();
}


//intital randomisation
void Neuron::InitRandomise(int range) {
	this->bias = (rand() % (range * 2)) - range;
	this->oprtr = (rand() % 2) == 0;
}

//add a single synapse. this should not be used
//as it has to move all the synapses to a new
//pointer and location in memory for one.
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
//a lot more efficient than adding a single synapse,
//but pretty much the same implementation.
Synapse *Neuron::addSynapses(Synapse *Syns, int size)
{
	if (numSyns < 0)
		numSyns = 0;
	std::unique_ptr<Synapse[]> ret(new Synapse[numSyns + size]);
	int i = 0;
	if (numSyns != 0)
	{
		for (i = 0; i < numSyns; i++)
			ret[i] = syns[i];
	}
	for (int j = 0; j < size; j++)
		ret[i + j] = Syns[j];
	numSyns += size;
	syns.reset(ret.release());
	return syns.get();
}

//make a single synapse, again, probably not good to use
Synapse *Neuron::MakeSynapse(Neuron *to, int randRange)
{
	return addSynapse(new Synapse(randRange, this, to));
}
//just better than adding a single synapse.
Synapse *Neuron::MakeSynapses(Neuron **tos, int amount, int randRange)
{
	std::shared_ptr<Synapse[]> nSyns(new Synapse[amount]);
	for (int i = 0; i < amount; i++) {
		nSyns[i] = Synapse(randRange, this, tos[i]);
	}
	return addSynapses(nSyns.get(), amount);
}

//for running the network
bool Neuron::valueGreaterThanBias()
{
	return oprtr ? bias > val : bias < val;
}
//fire the neuron's synapses unconditionally
Synapse *Neuron::FireNow()
{
	for (int i = 0; i < numSyns; i++)
	{
		syns[i].Fire();
	}
	return syns.get();
}
//why doesnt this fire the synapses?
//because im working on cuda functions that allow
//my gpu to do the addition work and copy the gpu
//memory (almost) straight into the neurons.
Synapse *Neuron::Fire()
{
	return syns.get();
}
//randomise for learning
void Neuron::Randomise(int chance, int range, bool synapses, Neuron *layer, int size, int synapseChance)
{
	int r1 = rand() % chance;
	int r2 = rand() % chance;
	if (r1 == 1)
		bias += (rand() % (range * 2)) - range;
	if (r2 == 1)
		oprtr = (rand() % 2) == 0;
	if (!synapses)
		return;
	RandomiseSynapses(synapseChance, chance, layer, size);
	for (int i = 0; i > numSyns; i++)
		syns[i].Randomise(chance, range);
}

/// @brief Randomise the synapses of this neuron.
/// @details This function will randomise the synapses of this neuron, including deleting, adding and randomising of values.
/// @param changeChance the chance of a change event.
/// @param chance the chance of a randomisation event.
/// @param range 
/// @return how many synapses were added
int Neuron::RandomiseSynapses(int changeChance, int chance, Neuron *layer, int size)
{
	int s = 0;
	if (rand() % chance != 1 || numSyns == 0)
		return 0;
	for (int i = 0; i < numSyns; i++)
	{
		int val = rand() % changeChance;
		if (val == 0)
		{
			syns[i].~Synapse();
			numSyns--;
			for (int j = i; j < numSyns - i; j++)
				syns[j] = syns[j + 1];
			i--;
			s--;
		}
	}
	for (int i = 0; i < size; i++) {
		int val = rand() % changeChance;
		if (val == 0 && layer + i != this)
		{
			MakeSynapse(layer + i, numSyns);
			s++;
		}
	}
	return s;
}
