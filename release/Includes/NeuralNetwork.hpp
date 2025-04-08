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
	std::unique_ptr<Neuron[]> Inputs	{};
	std::unique_ptr<Neuron[]> Neurons	{};
	std::unique_ptr<Neuron[]> Outputs	{};
	std::unique_ptr<bool[]>firingNeurons{};
public:
	int randRange 		{};
	int randChance		{};
	int midRepeats		{};
	int inputs			{};
	int neurons			{};
	int outputs			{};
	int usableNeurons	{};
	int outSyns			{};


	NeuralNetwork();
	NeuralNetwork(int ins, int mid, int out, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100);
	NeuralNetwork(NeuralNetwork *network);
	~NeuralNetwork();
	void DestroyNeurons();

	Neuron *getNeuron(int a, int b);
	int		CreateSynapses(int maxSyns);
	void Init(int ins, int mid, int out, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100);

	bool *Run();
	bool *RunCPU();
	bool hasNeuron(Neuron **ns, int size, Neuron *n);
	int *RunCPU(int *, int = 3);
	void RunOnceCPU(int *);
	int *Run(int *);
	void getFiringNeurons();
	void getFiringNeuronsFromPossibleNeurons(bool *);
	std::vector<std::vector<int>> getSynStrengths();
	std::vector<int> strengthSum(std::vector<std::vector<int>>);
	int getNeuronIndex(Neuron *n);
	bool *LayerCheck(Neuron *layer, int length);

	void Randomise();
	
};


#endif