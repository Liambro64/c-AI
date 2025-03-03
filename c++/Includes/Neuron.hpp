#pragma once
#ifndef NEURON_HPP
# define NEURON_HPP
# include "../Project.hpp"


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
    int getBias() const { return bias; }
    Synapse **getSynapses() const { return syns; }
	void addSynapse(Synapse* syn);
	void addSynapses(Synapse** syns, int size);
    bool getOperator() const { return oprtr; }
	bool valueGreaterThanBias() ;
	
	//network functions
	void Fire();
	void Randomise(int chance, int range);

	//operators
	Neuron add(int a) {
		val += a;
	}
};

#endif