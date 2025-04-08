#include "../Project.hpp"
NeuralNetwork::NeuralNetwork()
{
}
NeuralNetwork::NeuralNetwork(int ins, int mid, int outs, int maxSyns, int outSyns, int repeats, int RandRange, int RandChance)
{
	// setting
	if (outSyns <= 0)
		outSyns = neurons / 5;
	this->outSyns = (outSyns <= 0 ? neurons / 2 : outSyns);
	randRange = RandRange;
	randChance = RandChance;
	midRepeats = repeats;
	inputs = ins;
	neurons = mid;
	outputs = outs;

	firingNeurons = (std::unique_ptr<bool[]>)new bool[neurons];
	// create the input layer
	Inputs = (std::unique_ptr<Neuron[]>)(new Neuron[inputs]);
	("Made %i Inputs\n", inputs);
	for (int i = 0; i < inputs; i++)
		Inputs[i].InitRandomise(randRange);

	// create the middle layer, normally this would be multiple smaller
	// layers, but this is one big interconnected "ball" of neurons,
	// which have a random amount of connections to other neurons.
	// why? because I want to see if it works. I will make a basic
	// implementation and test both and test the efficiency.
	Neurons = (std::unique_ptr<Neuron[]>)(new Neuron[neurons]);
	for (int i = 0; i < neurons; i++)
		Neurons[i].InitRandomise(randRange);

	// create the output layer
	Outputs = (std::unique_ptr<Neuron[]>)(new Neuron[outputs]);
	for (int i = 0; i < outputs; i++)
		Outputs[i].InitRandomise(randRange);

	CreateSynapses(maxSyns);
}

NeuralNetwork::NeuralNetwork(NeuralNetwork *n)
{
	// implement based on network n and randomise
}

void NeuralNetwork::Init(int ins, int mid, int outs, int maxSyns, int outSyns, int repeats, int RandRange, int RandChance)
{
	int NeuronsMade = 0;
	if (outSyns <= 0)
		outSyns = neurons / 5;
	this->outSyns = (outSyns <= 0 ? neurons / 2 : outSyns);
	randRange = RandRange;
	randChance = RandChance;
	midRepeats = repeats;
	inputs = ins;
	neurons = mid;
	outputs = outs;

	// create the input layer
	Inputs = (std::unique_ptr<Neuron[]>)(new Neuron[inputs]);
	for (int i = 0; i < inputs; i++)
		Inputs[i].InitRandomise(randRange);
	NeuronsMade += inputs;
	// create the middle layer, normally this would be multiple smaller
	// layers, but this is one big interconnected "ball" of neurons,
	// which have a random amount of connections to other neurons.
	// why? because I want to see if it works. I will make a basic
	// implementation and test both and test the efficiency.
	Neurons = (std::unique_ptr<Neuron[]>)(new Neuron[neurons]);
	for (int i = 0; i < neurons; i++)
		Neurons[i].InitRandomise(randRange);
	NeuronsMade += neurons;
	// create the output layer
	Outputs = (std::unique_ptr<Neuron[]>)(new Neuron[outputs]);
	for (int i = 0; i < outputs; i++)
		Outputs[i].InitRandomise(randRange);
	NeuronsMade += outputs;
	firingNeurons = (std::unique_ptr<bool[]>)(new bool[inputs + neurons]);
	CreateSynapses(maxSyns);
}
// destructor
NeuralNetwork::~NeuralNetwork()
{
	if (Inputs)
		Inputs.reset();
	if (Neurons)
		Neurons.reset();
	if (Outputs)
		Outputs.reset();
	if (firingNeurons)
		firingNeurons.reset();
}

// create the synapses for the neurons, the "middle" neurons have a max of maxSyns
int NeuralNetwork::CreateSynapses(int maxSyns)
{
	int numCreated = 0;

	// creating random connections from input neurons to
	// the middle neurons.
	for (int i = 0; i < inputs; i++)
	{
		int rt = sqrtl(neurons);
		int synapseAmount = rt + (rand() % (rt * 2));
		Neuron **ns = (Neuron **)calloc(synapseAmount, sizeof(Neuron *));
		for (int j = 0; j < synapseAmount; j++)
		{
			Neuron *b = getNeuron(1, rand() % neurons);
			while (!hasNeuron(ns, j, b) && j > 0)
			{
				b = getNeuron(1, rand() % neurons);
			}
			*(ns + j) = b;
		}
		Inputs[i].MakeSynapses(ns, synapseAmount, randRange);
		free(ns);
		numCreated += synapseAmount;
	}
	// adding synapses between neurons, also making
	// sure they dont double up E.G Neuron A has
	// connection to Neuron B in >1 synapses,
	// but "backtracing" is allowed, E.G A and B have
	// 1 synapse from a->b and 1 from b->a
	for (int i = 0; i < neurons; i++)
	{
		int synapseAmount = rand() % maxSyns;
		Neuron **ns = (Neuron **)calloc(synapseAmount, sizeof(Neuron *));
		for (int j = 0; j < synapseAmount; j++)
		{
			Neuron *n = getNeuron(1, rand() % neurons);
			while (j > 0 && !hasNeuron(ns, j, n))
			{
				n = getNeuron(1, rand() % neurons);
			}
			*(ns + j) = n;
		}
		Neurons[i].MakeSynapses(ns, synapseAmount, randRange);
		free(ns);
		numCreated += synapseAmount;
	}
	// This adds up to (outputs) synapses to
	// outSyns neurons from the middle layer,
	// also stops synapse doubling
	Neuron **nss = (Neuron **)calloc(outSyns, sizeof(Neuron *));
	for (int i = 0; i < outSyns; i++)
	{
		int numSyns = rand() % outputs;
		Neuron **ns = (Neuron **)calloc(numSyns, sizeof(Neuron *));
		for (int j = 0; j < numSyns; j++)
		{
			Neuron *n = getNeuron(2, (rand() % outputs));
			while (j > 0 && !hasNeuron(ns, j, n))
				n = getNeuron(2, (rand() % outputs));
			*(ns + i) = n;
		}
		Neuron *n = getNeuron(1, rand() % neurons);
		while (i > 0 && !hasNeuron(nss, i, n))
			n = getNeuron(1, rand() % neurons);
		n->MakeSynapses(ns, numSyns, randRange);
		free(ns);
		*(nss + i) = n;
		numCreated += numSyns;
	}
	tryFree(nss);
	return numCreated;
}

// check if neuron n is in neuron ns
bool NeuralNetwork::hasNeuron(Neuron **ns, int size, Neuron *n)
{
	for (int i = 0; i < size; i++)
		if (*(ns + i) == n)
			return true;
	return false;
}
// get a neuron from somewhere within the network
Neuron *NeuralNetwork::getNeuron(int a, int b)
{
	if (a == 0)
		return &(Inputs[b]);
	else if (a == 1)
		return &(Neurons[b]);
	else if (a == 2)
		return &(Outputs[b]);
	return nullptr;
}

// set the values of firingNeurons to the neuron's ValGreaterThanBias
void NeuralNetwork::getFiringNeurons()
{
	int i = 0;
	for (i = 0; i < inputs; i++)
		firingNeurons[i] = Inputs[i].valueGreaterThanBias();
	for (int j = 0; j < neurons; j++)
		firingNeurons[j + i] = Neurons[j].valueGreaterThanBias();
}

std::vector<int> NeuralNetwork::strengthSum(std::vector<std::vector<int>> Strengths)
{
	std::vector<int> ret(neurons + outputs, 0); // Use std::vector for safer memory management
	for (int i = 0; i < neurons+outputs; i++)
	{
		int len = Strengths[i][0];
		for (int j = 0; j < len; j++)
		{
			ret[i] = Strengths[i][j + 1];
		}
	}
	return ret;
}

// running this type of network "requires" the middle
// layer to be run multiple times, but I want to
// implement a "tracking" way of doing this
// where instead I keep track of the synapses
// that fire, and then only check neurons
// that are connected to the firing synapses.
void NeuralNetwork::RunOnceCPU(int *a)
{
	// gets the bool value of the neurons
	getFiringNeurons();
	if (a != nullptr)
	{
		for (int i = 0; i < inputs; i++)
		{
			Inputs[i].set(a[i]);
		}
	}
	// gets the strength of the synapses sorted
	// by their "to" value, this is for gpu
	// parallel computing.
	auto synStrengths = getSynStrengths();
	auto totalUNs = neurons + outputs;
	std::vector<int> totals = strengthSum(synStrengths); // Use std::vector for safer memory management

	// Ensure totals array is properly populated
	for (int i = 0; i < totalUNs; i++)
	{
		for (int strength : synStrengths[i])
		{
			totals[i] += strength;
		}
	}

	for (int i = 0; i < totalUNs; i++)
	{
		Neuron *n = getNeuron(1 + (i / neurons), i % neurons);
		if (n != nullptr)
		{
			n->add(totals[i]);
		}
	}

	for (int i = 0; i < inputs; i++)
	{
		Inputs[i].set(0);
	}
}

int *NeuralNetwork::RunCPU(int *input, int repeats)
{
	// Ensure proper memory allocation for outputs
	int *ret = (int *)calloc(outputs, sizeof(int));
	if (!ret)
	{
		return nullptr;
	}

	// Run the network twice
	RunOnceCPU(input);
	for (int i = 0; i < repeats - 1; i++)
	{
		RunOnceCPU(nullptr);
	}

	// Populate the output values
	for (int i = 0; i < outputs; i++)
	{
		ret[i] = Outputs[i].getVal();
	}

	return ret;
}

bool *NeuralNetwork::LayerCheck(Neuron *layer, int length)
{
	bool *b = (bool *)calloc(length, sizeof(bool));
	for (int i = 0; i < length; i++)
		b[i] = layer[i].valueGreaterThanBias();
	return b;
}

std::vector<std::vector<int>> NeuralNetwork::getSynStrengths()
{
	int totalUNs = neurons + outputs;
	std::vector<std::vector<int>> synStrengths(totalUNs); // Use vector of vectors for safer memory management
	std::vector<int> indexAmount(totalUNs, 0); // Track the number of synapses for each neuron
	std::vector<int> iter(totalUNs, 0);

	// Count the number of synapses for each neuron
	for (int i = 0; i < inputs + neurons; i++)
	{
		if (!firingNeurons[i])
			continue;

		Neuron *n = getNeuron(i < inputs ? 0 : 1, i < inputs ? i : i - inputs);
		if (!n)
			continue;

		Synapse *syns = n->getSynapses();
		for (int j = 0; j < n->getNumSyns(); j++)
		{
			int toIndex = getNeuronIndex(syns[j].getTo());
			if (toIndex >= 0 && toIndex < totalUNs)
			{
				indexAmount[toIndex]++;
			}
		}
	}

	// Resize vectors to hold the synapse strengths
	for (int i = 0; i < totalUNs; i++)
	{
		synStrengths[i] = std::vector<int>(indexAmount[i] + 1, 0); // +1 for the length
		synStrengths[i][0] = indexAmount[i]; // Store the length at the first index
	}

	// Populate the synapse strengths
	for (int i = 0; i < inputs + neurons; i++)
	{
		if (!firingNeurons[i])
			continue;

		Neuron *n = getNeuron(i < inputs ? 0 : 1, i < inputs ? i : i - inputs);
		if (!n)
			continue;

		Synapse *syns = n->getSynapses();
		for (int j = 0; j < n->getNumSyns(); j++)
		{
			int toIndex = getNeuronIndex(syns[j].getTo());
			if (toIndex >= 0 && toIndex < totalUNs)
			{
				synStrengths[toIndex][iter[toIndex]++] = syns[j].getStrength();
			}
		}
	}

	return synStrengths;
}

bool *NeuralNetwork::Run()
{
	// gets the bool value of the neurons
	getFiringNeurons();
	// gets the strength of the synapses sorted
	// by their "to" value, this is for gpu
	// parallel computing.
	std::vector<std::vector<int>> synStrengths(getSynStrengths());
	// here, I need to add CUDA functions, and after this
	// I will be unable to test networks running on my laptop
	// which does not have a gpu.
}
int NeuralNetwork::getNeuronIndex(Neuron *n)
{
	Neuron *ns = Neurons.get();
	for (int i = 0; i < neurons; i++)
		if (ns + i == n)
			return i;
	ns = Outputs.get();
	for (int i = 0; i < outputs; i++)
		if (ns + i == n)
			return i + neurons;
	return -1;
}
int *NeuralNetwork::Run(int *input)
{
	for (int i = 0; i < inputs; i++)
	{
		Inputs[i].set(*(input + i));
	}
	// call fire on neurons, return output neuron's val
}

// randomise, for networks based off other networks.
// this should include adding/deleting synapses,
// randomising synapse strength, and neuron bias.
void NeuralNetwork::Randomise()
{
	// code here <3
}

e c __declspec(dllexport) NeuralNetwork *CreateNetwork(int ins, int mid, int out, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100)
{
	return new NeuralNetwork(ins, mid, out, maxSyns, outSyns, repeats, RandRange, RandChance);
}

e c __declspec(dllexport) NeuralNetwork *CreateCleanNetwork()
{
	return new NeuralNetwork();
}

e c __declspec(dllexport) void DestroyNetwork(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		delete net;
	}
}
e c __declspec(dllexport) void Init(NeuralNetwork *net, int ins, int mid, int out, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100)
{
	if (net != nullptr)
	{
		net->Init(ins, mid, out, maxSyns, outSyns, repeats, RandRange, RandChance);
	}
}
e c __declspec(dllexport) int *Run(NeuralNetwork *net, int *input, int repeats = -1)
{
	if (net != nullptr)
	{
		if (repeats = -1)
			repeats = net->midRepeats;
		return net->RunCPU(input, repeats);
	}
	return nullptr;
}

e c __declspec(dllexport) Neuron *GetNeurons(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->GetNeurons();
	}
	return nullptr;
}
e c __declspec(dllexport) Neuron *GetInputs(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->GetInputs();
	}
	return nullptr;
}
e c __declspec(dllexport) Neuron *GetOutputs(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->GetOutputs();
	}
	return nullptr;
}
e c __declspec(dllexport) int GetInputsSize(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getInputs();
	}
	return 0;
}
e c __declspec(dllexport) int GetOutputsSize(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getOutputs();
	}
	return 0;
}
e c __declspec(dllexport) int GetNeuronsSize(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getNeurons();
	}
	return 0;
}

e c __declspec(dllexport) int GetUsableNeurons(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getUsableNeurons();
	}
	return 0;
}
e c __declspec(dllexport) int GetOutSyns(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getOutSyns();
	}
	return 0;
}

e c __declspec(dllexport) int GetRandRange(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getRandRange();
	}
	return 0;
}
e c __declspec(dllexport) int GetRandChance(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getRandChance();
	}
	return 0;
}