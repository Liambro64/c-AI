#pragma once
#ifndef NEURON_HPP
# define NEURON_HPP
# include "../Project.hpp"

class Synapse;

class Neuron
{
private:

	int			val		{};
    int			bias	{};
	int			numSyns	{};
	//if true b > v, if false b < v
	bool		oprtr	{};
    Synapse*	syns	{};

public:
	Neuron(int Bias, bool Oprtr);
    Neuron(int randRange);
	~Neuron();

    void print();

	//basic functions
    int const getBias() const { return bias; }
    Synapse * const getSynapses() const { return syns; }
    bool const getOperator() const { return oprtr; }

	Synapse *addSynapse(Synapse* syn);
	Synapse *addSynapses(Synapse*	Syns, int size);
	bool valueGreaterThanBias() ;
	
	//network functions
	Synapse *Fire();
	void	FireNow();
	Synapse *MakeSynapses(Neuron **tos, int amount, int randRange);
	Synapse *MakeSynapse(Neuron *to, int randRange);
	void Randomise(int chance, int range);

	//operators
	void const add(int a) {
		val += a;
	}
	void const set(int a) {
		val = a;
	}
};

#endif