#include "../Project.hpp"

NeuralNetwork::NeuralNetwork(int ins, int mid, int outs, int outSyns, int repeats, int RandRange, int RandChance)
{
	//would do in main, but here is fine for now
	srand(time(0));
	if (outSyns <= 0)
		outSyns = neurons/2;
	this->outSyns = (outSyns <= 0 ? neurons/2 : outSyns);
	// setting
	randRange = RandRange;
	randChance = RandChance;
	midRepeats = repeats;
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
	for (int i = 0; i < inputs; i++) {
		int synapseAmount = (neurons/3) + (rand() % (neurons - (neurons/3)));
		Neuron **ns = (Neuron **)calloc(synapseAmount, sizeof(Neuron *));
		for (int j = 0; j < synapseAmount; j++) {
			Neuron *b = getNeuron(rand() % neurons);
			while (!hasNeuron(ns, j, b) && j > 0) {
				b = getNeuron(rand() % neurons);
			}
			*(ns + j) = b;
		}
		(Inputs + i)->MakeSynapses(ns, synapseAmount, randRange);
		free(ns);
	}
	for (int i = 0; i < neurons; i++) {
		int synapseAmount = rand() % usableNeurons;
		Neuron **ns = (Neuron **)calloc(synapseAmount, sizeof(Neuron *));
		for (int j = 0; j < synapseAmount; j++) {
			Neuron *n = getNeuron(rand() % neurons);
			while ( j > 0 && !hasNeuron(ns, j, n)) {
				n = getNeuron(rand() % neurons);
			}
			*(ns + j) = n;
		}
		(Neurons + i)->MakeSynapses(ns, synapseAmount, randRange);
		free(ns);
	}
	Neuron **nss = (Neuron **)calloc(outSyns, sizeof(Neuron *));
	for (int i = 0; i < outSyns; i++) {
		int numSyns = rand() % outputs;
		Neuron **ns = (Neuron **)calloc(numSyns, sizeof(Neuron *));
		for (int j = 0; j < numSyns; j++){
			Neuron *n = getNeuron(neurons + (rand() % outputs));
			while (j > 0 && !hasNeuron(ns, j, n))
				n = getNeuron(neurons + (rand() % outputs));
		}
		Neuron *n = getNeuron(rand() % neurons);
		while (i > 0 && !hasNeuron(nss, i, n))
			n = getNeuron(rand() % neurons);
		n->MakeSynapses(ns, numSyns, randRange);
		free(ns);
		*(nss + i) = n;
	}
	free(nss);
}
NeuralNetwork::NeuralNetwork(NeuralNetwork *n)
{
	// setting
	outSyns = n->outSyns;
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
NeuralNetwork::~NeuralNetwork() {
	DestroyNeurons();
	free(Inputs);
	free(Neurons);
	free(Outputs);
	free(allUsableNeurons);
	std::cout << "Destroyed Network" << std::endl;
}
void NeuralNetwork::DestroyNeurons() {
	int max __max(__max(inputs, neurons), outputs);
	for (int i = 0; i < max; i++) {
		if (i < inputs)
			delete (Inputs + i);
		if (i < neurons)
			delete (Neurons + i);
		if (i < outputs)
			delete (Outputs + i);
	}
}
bool NeuralNetwork::hasNeuron(Neuron **ns, int size, Neuron *n) {
	for (int i = 0; i < size; i++)
		if (*(ns + i) == n)
			return true;
	return false;
}
Neuron *NeuralNetwork::getNeuron(int i)
{
	int a = i / neurons;
	int b = i % neurons;
	return ((*(allUsableNeurons + a)) + b);
}
Synapse **NeuralNetwork::getFiringSynapses(int *lengthOut) {
	*lengthOut = 0;
	Synapse **Syns = (Synapse **)calloc(neurons, sizeof(Synapse *));
	for (int i = 0; i < neurons; i++) {
		Synapse *a = (Neurons+i)->Fire();
		if (a != nullptr)
			*(Syns + ((*lengthOut)++)) = a;
	}
	return Syns;
}
bool *NeuralNetwork::Run()
{
	//call fire on neurons, return output neuron's valueGreaterThanBias
	bool *vgtb = (bool *)calloc(outputs, sizeof(bool));
	for (int i = 0; i < neurons; i++) {
		(Neurons+i)->FireNow();
	}
	int *vals = (int *)calloc(neurons, sizeof(int));
	for (int i = 0; i < midRepeats; i++) {
		
	}
}

int *NeuralNetwork::Run(int *input) {
	for (int i = 0; i < inputs; i++) {
		(Inputs+i)->set(*(input + i));
	}
	//call fire on neurons, return output neuron's val
}

void NeuralNetwork::Randomise() {
	//r
}