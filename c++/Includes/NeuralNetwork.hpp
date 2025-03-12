#pragma once
#ifndef NEURALNETWORK_HPP
# define NEURALNETWORK_HPP
# include "../Project.hpp"

class NeuralNetwork_Basic {
	//basic implementation
};
//my implementation
class NeuralNetwork {
private:
	Neuron *Inputs	;
	Neuron *Neurons	;
	Neuron *Outputs	;

	Neuron **allUsableNeurons ;
public:
	int randRange 		;
	int randChance		;
	int midRepeats		;
	int inputs			;
	int neurons			;
	int outputs			;
	int usableNeurons	;
	int outSyns			;

	NeuralNetwork(int ins, int mid, int out, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100);
	NeuralNetwork(NeuralNetwork *network);
	~NeuralNetwork();
	void DestroyNeurons();

	Neuron *getNeuron(int i);

	bool *Run();
	bool hasNeuron(Neuron **ns, int size, Neuron *n);
	int *Run(int *);
	Synapse **getFiringSynapses(int *lengthOut);

	void Randomise();
	
};


#endif