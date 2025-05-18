#pragma once
#ifndef NEURON_HPP
# define NEURON_HPP
# include "../Project.hpp"
# include <vector> // Include for std::vector

class Synapse;

class Neuron
{
private:

	int			val		{};
    int			bias	{};
	// numSyns will now be syns.size()
	//if true b > v, if false b < v
	bool		oprtr	{};
    std::vector<Synapse> syns	{}; // Changed to std::vector

public:
	Neuron();
	Neuron(int Bias, bool Oprtr);
    Neuron(int randRange);
    Neuron(int randRange, int index);
	Neuron Clone();
	~Neuron();

	//basic functions
    int const getBias() const { return bias; }
	void const setBias(int a) { bias = a; }
    // Returning a pointer to the vector's data for compatibility with existing getSynapses() users.
    // Be cautious if the vector reallocates. For read-only access, this is generally okay.
    Synapse * const getSynapses() { return syns.data(); } 
	int const getNumSyns() const {return syns.size();} // Changed to return syns.size()
	int const getVal() const {return val; }
    bool const getOperator() const { return oprtr; }
	void const setOperator(int a) { oprtr = a; } // Should be bool, not int, if oprtr is bool

	// Synapse management functions will change
	void addSynapse(const Synapse& syn); // Pass by const reference
	void addSynapses(const std::vector<Synapse>& newSyns); // Pass by const reference
	bool valueGreaterThanBias() ;
	bool canFire();
	
	//network functions
	Synapse *Fire(); // Return type might need review if direct pointer access is critical elsewhere
	Synapse	*FireNow();
	void MakeSynapses(Neuron **tos, int amount, int randRange); // Implementation will change
	void MakeSynapse(Neuron *to, int randRange); // Implementation will change
	void InitRandomise(int range);
	void Randomise(int chance, int range, bool synapses,  std::vector<Neuron> *layer, int synapseChance);
	int	 RandomiseSynapses(int ChangeChance, int chance, int randRange, std::vector<Neuron> *layer);
	void ApplyBiasDecay(); // New method for decay

	//operators
	void const add(int a) {
		val += a;
	}
	void const set(int a) {
		val = a;
	}
};

#endif