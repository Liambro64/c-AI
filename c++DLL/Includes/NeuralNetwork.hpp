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
	NeuralNetwork(int ins, int mid, int out, int maxSyns = -1, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100);
	NeuralNetwork(NeuralNetwork *network);
	~NeuralNetwork();
	NeuralNetwork *Clone();
	void DestroyNeurons();

	int getRandRange() { return randRange; }
	int setRandRange(int range) { randRange = range; return randRange; }
	int getRandChance() { return randChance; }
	int setRandChance(int chance) { randChance = chance; return randChance; }
	int getMidRepeats() { return midRepeats; }
	int setMidRepeats(int repeats) { midRepeats = repeats; return midRepeats; }
	int getInputs() { return inputs; }
	int setInputs(int ins) { inputs = ins; return inputs; }
	int getNeurons() { return neurons; }
	int setNeurons(int mid) { neurons = mid; return neurons; }
	int getOutputs() { return outputs; }
	int setOutputs(int out) { outputs = out; return outputs; }
	int getUsableNeurons() { return usableNeurons; }
	int setUsableNeurons(int usable) { usableNeurons = usable; return usableNeurons; }
	int getOutSyns() { return outSyns; }
	int setOutSyns(int out) { outSyns = out; return outSyns; }

	Neuron *GetInputs() { return Inputs.get(); }
	Neuron *GetNeurons() { return Neurons.get(); }
	Neuron *GetOutputs() { return Outputs.get(); }


	Neuron *getNeuron(int a, int b);
	int		CreateSynapses(int maxSyns = -1);
	void Init(int ins, int mid, int out, int maxSyns = -1, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100);
	void SRand();

	int	findNeuron(Neuron *n);
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
	void printSynapses(char *filter = "none");

	void Randomise();
	
};


#endif