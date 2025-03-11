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
    Synapse**	syns	{};

public:
	Neuron(int Bias, bool Oprtr);
    Neuron(int randRange);

    void print() const;

	//basic functions
    int const getBias() const { return bias; }
    Synapse ** const getSynapses() const { return syns; }
    bool const getOperator() const { return oprtr; }

	void addSynapse(Synapse* syn);
	void addSynapses(Synapse** syns, int size);
	bool valueGreaterThanBias() ;
	
	//network functions
	void Fire();
	void Randomise(int chance, int range);

	//operators
	const void add(int a) {
		val += a;
	}
};

#endif