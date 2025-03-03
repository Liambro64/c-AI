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
	int inputs			;
	int neurons			;
	int outputs			;
	int usableNeurons	;

	NeuralNetwork(int ins, int mid, int out, int RandRange = 5, int RandChance = 100);
	NeuralNetwork(NeuralNetwork *network);

	Neuron *getNeuron(int i);

	bool *Run();
	int *Run(int *);

	void Randomise();
	
};


#endif