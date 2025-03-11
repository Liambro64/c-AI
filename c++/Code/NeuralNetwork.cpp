#include "../Project.hpp"

NeuralNetwork::NeuralNetwork(int ins, int mid, int outs, int RandRange, int RandChance)
{
	//would do in main, but here is fine for now
	srand(time(0));
	// setting
	randRange = RandRange;
	randChance = RandChance;
	inputs = ins;
	neurons = mid;
	outputs = outs;
	// doing
	Inputs = (Neuron *)calloc(inputs, sizeof(Neuron));
	for (int i = 0; i < inputs; i++)
	{
		*(Inputs + i) = *new Neuron(randRange);
	}
	Neurons = (Neuron *)calloc(neurons, sizeof(Neuron));
	for (int i = 0; i < neurons; i++)
	{
		*(Neurons + i) = *new Neuron(randRange);
	}
	Outputs = (Neuron *)calloc(outputs, sizeof(Neuron));
	for (int i = 0; i < outputs; i++)
	{
		*(Outputs + i) = *new Neuron(randRange);
	}
	usableNeurons = neurons + outputs;
	allUsableNeurons = (Neuron **)calloc(2, sizeof(Neuron *));
	*(allUsableNeurons) = Neurons;
	*(allUsableNeurons + 1) = Outputs;
	for (int i = 0; i < inputs; i++)
	{
		int amount = rand() % (usableNeurons / 2);
		for (int j = 0; j < amount; j++)
		{
			int r = rand() % usableNeurons;
			Synapse *syn = new Synapse(randRange, (Inputs + i), getNeuron(r));
			(Inputs + i)->addSynapse(syn);
		}
	}
}
NeuralNetwork::NeuralNetwork(NeuralNetwork *n)
{
	// setting
	randRange = n->randRange;
	randChance = n->randChance;
	inputs = n->inputs;
	neurons = n->neurons;
	outputs = n->outputs;
	// doing
	Inputs = (Neuron *)calloc(inputs, sizeof(Neuron));
	for (int i = 0; i < inputs; i++)
	{
		*(Inputs + i) = *new Neuron(randRange);
	}
	Neurons = (Neuron *)calloc(neurons, sizeof(Neuron));
	for (int i = 0; i < neurons; i++)
	{
		*(Neurons + i) = *new Neuron(randRange);
	}
	Outputs = (Neuron *)calloc(outputs, sizeof(Neuron));
	for (int i = 0; i < outputs; i++)
	{
		*(Outputs + i) = *new Neuron(randRange);
	}
	allUsableNeurons = (Neuron **)calloc(2, sizeof(Neuron *));
	*(allUsableNeurons) = Neurons;
	*(allUsableNeurons + 1) = Outputs;
}

Neuron *NeuralNetwork::getNeuron(int i)
{
	int a = i / neurons;
	int b = i % neurons;
	return ((*(allUsableNeurons + a)) + b);
}

bool *NeuralNetwork::Run()
{
	//call fire on neurons, return output neuron's valueGreaterThanBias
	bool *vgtb = (bool *)calloc(outputs, sizeof(bool));
	for (int i = 0; i < neurons; i++) {
		(Neurons+i)->Fire();
	}
}
int *NeuralNetwork::Run(int *) {
	//call fire on neurons, return output neuron's val
}

void NeuralNetwork::Randomise() {
	//r
}