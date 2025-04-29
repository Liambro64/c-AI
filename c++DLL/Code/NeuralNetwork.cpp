#include "../Project.hpp"
NeuralNetwork::NeuralNetwork()
{
}
NeuralNetwork::NeuralNetwork(int ins, int mid, int outs, int maxSyns, int outSyns, int repeats, int RandRange, int RandChance, int Chance)
{
	Init(ins, mid, outs, maxSyns, outSyns, repeats, RandRange, RandChance, Chance);
}
NeuralNetwork::NeuralNetwork(NeuralNetwork *network)
{
	// copy constructor
	this->inputs = network->inputs;
	this->neurons = network->neurons;
	this->outputs = network->outputs;
	this->randRange = network->randRange;
	this->randChance = network->randChance;
	this->midRepeats = network->midRepeats;
	this->outSyns = network->outSyns;
	this->Inputs = std::make_unique<Neuron[]>(inputs);
	this->Neurons = std::make_unique<Neuron[]>(neurons);
	this->Outputs = std::make_unique<Neuron[]>(outputs);
	for (int i = 0; i < inputs; i++)
	{
		Inputs[i].setBias(network->Inputs[i].getBias());
		Inputs[i].setOperator(network->Inputs[i].getOperator());
		for (int j = 0; j < network->Inputs[i].getNumSyns(); j++)
		{
			Inputs[i].addSynapse(new Synapse(network->Inputs[i].getSynapses()[j].getStrength(), Inputs.get() + i, Neurons.get() + (findNeuron(network->Inputs[i].getSynapses()[j].getTo()) - inputs)));
		}
	}
	for (int i = 0; i < neurons; i++)
	{
		Neurons[i].setBias(network->Neurons[i].getBias());
		Neurons[i].setOperator(network->Neurons[i].getOperator());
		for (int j = 0; j < Neurons[i].getNumSyns(); j++)
		{
			int to = findNeuron(network->Neurons[i].getSynapses()[j].getTo()) - inputs;
			int layer = to < neurons ? 0 : 1;
			if (layer == 1)
				to -= neurons;
			Neuron *too = (layer == 0 ? Neurons.get() + to : Outputs.get() + to);
			Neurons[i].addSynapse(new Synapse(network->Neurons[i].getSynapses()[j].getStrength(), Neurons.get() + i, too));
		}
	}
	Randomise(Chance + (Chance / 4), Chance, Chance - (Chance / 4));
}
NeuralNetwork *NeuralNetwork::Clone()
{
	NeuralNetwork clone =  NeuralNetwork();
	clone.inputs = inputs;
	clone.neurons = neurons;
	clone.outputs = outputs;
	clone.randRange = randRange;
	clone.randChance = randChance;
	clone.midRepeats = midRepeats;
	clone.outSyns = outSyns;
	clone.Inputs = std::make_unique<Neuron[]>(inputs);
	clone.Neurons = std::make_unique<Neuron[]>(neurons);
	clone.Outputs = std::make_unique<Neuron[]>(outputs);
	for (int i = 0; i < inputs; i++)
	{
		clone.Inputs[i].setBias(Inputs[i].getBias());
		clone.Inputs[i].setOperator(Inputs[i].getOperator());
		for (int j = 0; j < Inputs[i].getNumSyns(); j++)
		{
			clone.Inputs[i].addSynapse(new Synapse(clone.Inputs.get() + i, clone.Neurons.get() + (findNeuron(Inputs[i].getSynapses()[j].getTo()) - inputs), Inputs[i].getSynapses()[j].getStrength()));
		}
	}
	for (int i = 0; i < neurons; i++)
	{
		clone.Neurons[i].setBias(Neurons[i].getBias());
		clone.Neurons[i].setOperator(Neurons[i].getOperator());
		for (int j = 0; j < Neurons[i].getNumSyns(); j++)
		{
			int to = findNeuron(Neurons[i].getSynapses()[j].getTo()) - inputs;
			int layer = to < neurons ? 0 : 1;
			if (layer == 1)
				to -= neurons;
			Neuron *too = (layer == 0 ? clone.Neurons.get() + to : clone.Outputs.get() + to);
			clone.Neurons[i].addSynapse(new Synapse(clone.Neurons.get() + i, too, Neurons[i].getSynapses()[j].getStrength()));
		}
	}
	return &clone;
}
void NeuralNetwork::Init(int ins, int mid, int outs, int maxSyns, int outSyns, int repeats, int RandRange, int RandChance, int Chance)
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

	this->Chance = Chance;

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
void NeuralNetwork::SRand()
{
	srand(time(0));
}
int NeuralNetwork::findNeuron(Neuron *n)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < inputs; i++)
		if (Inputs.get() + i == n)
			return i;
	for (j = 0; j < neurons; j++)
		if (Neurons.get() + j == n)
			return j + i;
	for (int k = 0; k < outputs; k++)
		if (Outputs.get() + k == n)
			return k + i + j;
}

void printCustom(Neuron *from, Neuron **tos, NeuralNetwork *net, int size)
{
	printf("Neuron %i to Neurons: ", net->findNeuron(from));
	for (int i = 0; i < size; i++)
	{
		printf("%i%s", net->findNeuron(*(tos + i)), i == size - 1 ? "" : ", ");
	}
	printf("\nSyns: %i\n", size);
}
void NeuralNetwork::printSynapses(char *filter)
{
	for (int i = 0; i < inputs; i++)
	{
		if (Inputs[i].getNumSyns() == 0) {
			printf("Input Neuron %i has no synapses\n", i);
			continue;
		}
		printf("Input Neuron %i to (%i) Neurons: ", i, Inputs[i].getNumSyns());
		for (int j = 0; j < Inputs[i].getNumSyns(); j++)
		{
			int n = findNeuron(Inputs[i].getSynapses()[j].getTo());
			const char *a = (n < inputs ? "Input" : n < neurons + inputs ? "Middle"
																		 : "Output");
			int offset = n < inputs ? 0 : n < neurons + inputs ? inputs : neurons + inputs;
			printf("%s Neuron %i%s", a, findNeuron(Inputs[i].getSynapses()[j].getTo()) - offset, j == Inputs[i].getNumSyns() - 1 ? "\n" : ", ");
		}
	}
	for (int i = 0; i < neurons; i++)
	{
		if (Neurons[i].getNumSyns() == 0) {
			printf("Middle Neuron %i has no synapses\n", i);
			continue;
		}
		printf("Middle Neuron %i to Neurons: ", i);
		for (int j = 0; j < Neurons[i].getNumSyns(); j++)
		{
			int n = findNeuron(Neurons[i].getSynapses()[j].getTo());
			const char *a = (n < inputs ? "Input" : n < neurons + inputs ? "Middle"
																		 : "Output");
			printf("%s Neuron %i%s", a, findNeuron(Neurons[i].getSynapses()[j].getTo()), j == Neurons[i].getNumSyns() - 1 ? "\n" : ", ");
		}
	}
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
			while (hasNeuron(ns, j, b) && j > 0)
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
		int synapseAmount = (maxSyns == -1 ? rand() % (int)sqrtl(neurons) : rand() % maxSyns);
		Neuron **ns = (Neuron **)calloc(synapseAmount, sizeof(Neuron *));
		for (int j = 0; j < synapseAmount; j++)
		{
			Neuron *n = getNeuron(1, rand() % neurons);
			while (j > 0 && hasNeuron(ns, j, n))
			{
				n = getNeuron(1, rand() % neurons);
			}

			*(ns + j) = n;
		}
		Neurons[i].MakeSynapses(ns, synapseAmount, randRange);
		free(ns);
		numCreated += synapseAmount;
	}
	if (outSyns <= 0)
		outSyns = neurons / outputs;
	// This adds up to (outputs) synapses to
	// outSyns neurons from the middle layer,
	// also stops synapse doubling
	Neuron **nss = (Neuron **)calloc(outSyns, sizeof(Neuron *));
	for (int i = 0; i < outSyns; i++)
	{
		int numSyns = outputs / 2 + (rand() % (outputs - outputs / 2));
		Neuron **ns = (Neuron **)calloc(numSyns, sizeof(Neuron *));
		for (int j = 0; j < numSyns; j++)
		{
			Neuron *n = getNeuron(2, (rand() % outputs));
			while (j > 0 && hasNeuron(ns, j, n))
			{
				n = getNeuron(2, (rand() % outputs));
			}
			*(ns + j) = n;
		}
		Neuron *n = getNeuron(1, rand() % neurons);
		// printCustom(n, ns, this, numSyns);
		while (i > 0 && hasNeuron(nss, i, n))
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
	for (int i = 0; i < neurons + outputs; i++)
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
	std::vector<int> indexAmount(totalUNs, 0);			  // Track the number of synapses for each neuron
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
		synStrengths[i][0] = indexAmount[i];					   // Store the length at the first index
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

	Neuron *ns = Inputs.get();
	for (int i = 0; i < inputs; i++)
		if (ns + i == n)
			return i;
	ns = Neurons.get();
	for (int i = 0; i < neurons; i++)
		if (ns + i == n)
			return i + inputs;
	ns = Outputs.get();
	for (int i = 0; i < outputs; i++)
		if (ns + i == n)
			return i + neurons + inputs;
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
void NeuralNetwork::Randomise(int Chance1, int chance2, int chance3)
{
	// code here <3
	for (int i = 0; i < inputs; i++)
	{
		Inputs[i].Randomise(randChance, randRange, true, Neurons.get(), neurons, Chance1);
	}
	for (int i = 0; i < neurons; i++)
	{
		Neurons[i].Randomise(randChance, randRange, true, Neurons.get(), neurons, chance2);
		Neurons[i].RandomiseSynapses(chance3, randChance, Neurons.get(), neurons);
	}
	for (int i = 0; i < outputs; i++)
	{
		Outputs[i].Randomise(randChance, randRange, false, 0, 0, 0);
	}
}
void NeuralNetwork::EzRandomise()
{
	Randomise(Chance + (Chance / 4), Chance, Chance - (Chance / 4));
}
fdll NeuralNetwork *CreateNetwork(int ins, int mid, int out, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100)
{
	return new NeuralNetwork(ins, mid, out, maxSyns, outSyns, repeats, RandRange, RandChance);
}

fdll NeuralNetwork *CreateCleanNetwork()
{
	return new NeuralNetwork();
}

fdll NeuralNetwork *CloneNetwork(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return new NeuralNetwork(net);
	}
	return nullptr;
}
fdll void RandomiseNetwork(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		int Chance = net->getChance();
		net->Randomise(Chance + (Chance / 4), Chance, Chance - (Chance / 4));
	}
}
fdll int getChance(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getChance();
	}
	return 0;
}
fdll int setChance(NeuralNetwork *net, int chance)
{
	if (net != nullptr)
	{
		return net->setChance(chance);
	}
	return 0;
}
fdll void DestroyNetwork(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		delete net;
	}
}
fdll void Init(NeuralNetwork *net, int ins, int mid, int out, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100)
{
	if (net != nullptr)
	{
		net->Init(ins, mid, out, maxSyns, outSyns, repeats, RandRange, RandChance);
	}
}
fdll int *Run(NeuralNetwork *net, int *input, int repeats = -1)
{
	if (net != nullptr)
	{
		if (repeats = -1)
			repeats = net->midRepeats;
		return net->RunCPU(input, repeats);
	}
	return nullptr;
}

fdll Neuron *GetNeurons(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->GetNeurons();
	}
	return nullptr;
}
fdll Neuron *GetInputs(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->GetInputs();
	}
	return nullptr;
}
fdll Neuron *GetOutputs(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->GetOutputs();
	}
	return nullptr;
}
fdll int GetInputsSize(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getInputs();
	}
	return 0;
}
fdll int GetOutputsSize(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getOutputs();
	}
	return 0;
}
fdll int GetNeuronsSize(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getNeurons();
	}
	return 0;
}

fdll int GetUsableNeurons(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getUsableNeurons();
	}
	return 0;
}
fdll int GetOutSyns(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getOutSyns();
	}
	return 0;
}

fdll int GetRandRange(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getRandRange();
	}
	return 0;
}
fdll int GetRandChance(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		return net->getRandChance();
	}
	return 0;
}
fdll int GetNeuronIndex(NeuralNetwork *net, Neuron *n)
{
	if (net != nullptr && n != nullptr)
	{
		return net->getNeuronIndex(n);
	}
	return -1;
}
fdll void sRand(NeuralNetwork *net)
{
	if (net != nullptr)
	{
		net->SRand();
	}
}