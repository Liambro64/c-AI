#pragma once
#ifndef SYNAPSE_HPP
# define SYNAPSE_HPP
# include "../Project.hpp"

class Neuron;

class Synapse {

private:
	int strength;
	Neuron* from;
	Neuron* to;

public:
	Synapse();
	Synapse(int randRange, Neuron *frm, Neuron *too);
	~Synapse() {};

	//accessors
	int getStrength()	{ return strength; 	}
	Neuron *getFrom()	{ return from; 		}
	Neuron *getTo()		{ return to; 		}

	//use functions
	void Fire();
	void Randomise(int chance, int range);
};

#endif