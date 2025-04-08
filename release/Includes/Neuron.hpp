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
    std::unique_ptr<Synapse[]> syns	{};

public:
	Neuron();
	Neuron(int Bias, bool Oprtr);
    Neuron(int randRange);
    Neuron(int randRange, int index);
	~Neuron();

	//basic functions
    int const getBias() const { return bias; }
    Synapse * const getSynapses() const { return syns.get(); }
	int const getNumSyns() const {return numSyns;} 
	int const getVal() const {return val; }
    bool const getOperator() const { return oprtr; }

	Synapse *addSynapse(Synapse* syn);
	Synapse *addSynapses(Synapse* Syns, int size);
	bool valueGreaterThanBias() ;
	
	//network functions
	Synapse *Fire();
	Synapse	*FireNow();
	Synapse *MakeSynapses(Neuron **tos, int amount, int randRange);
	Synapse *MakeSynapse(Neuron *to, int randRange);
	void InitRandomise(int range);
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