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
	Synapse(Neuron *frm, Neuron *too, int str);
	~Synapse() {
		strength = 0;
		from = 0;
		to = 0;
	};
	Synapse Clone(Neuron *frm, Neuron *too) {
		return Synapse(strength, frm, too);
	}

	//accessors
	int getStrength()	{ return strength; 	}
	Neuron *getFrom()	{ return from; 		}
	Neuron *getTo()		{ return to; 		}

	//use functions
	void Fire();
	void FireNow(int val);
	void Randomise(int chance, int range);
};

#endif