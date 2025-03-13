#include "../Project.hpp"
NeuralNetwork::NeuralNetwork() {

}
NeuralNetwork::NeuralNetwork(int ins, int mid, int outs, int maxSyns, int outSyns, int repeats, int RandRange, int RandChance)
{
	// setting
	if (outSyns <= 0)
		outSyns = neurons/5;
	this->outSyns = (outSyns <= 0 ? neurons/2 : outSyns);
	randRange = RandRange;
	randChance = RandChance;
	midRepeats = repeats;
	inputs = ins;
	neurons = mid;
	outputs = outs;


	//create the input layer
	Inputs = (std::unique_ptr<Neuron[]>)(new Neuron[inputs]);
	if (DEBUG())
	printf("Made %i Inputs\n", inputs);
	for (int i = 0; i < inputs; i++)
		Inputs[i].InitRandomise(randRange);
	if (DEBUG())
	printf("Randomised Inputs\n", inputs);

	//create the middle layer, normally this would be multiple smaller
	//layers, but this is one big interconnected "ball" of neurons,
	//which have a random amount of connections to other neurons.
	//why? because I want to see if it works. I will make a basic
	//implementation and test both and test the efficiency.
	Neurons = (std::unique_ptr<Neuron[]>)(new Neuron[neurons]);
	if (DEBUG())
	printf("Made %i Neurons\n", neurons);
	for (int i = 0; i < neurons; i++)
		Neurons[i].InitRandomise(randRange);
	if (DEBUG())
	printf("Randomised Neurons\n", neurons);

	//create the output layer
	Outputs = (std::unique_ptr<Neuron[]>)(new Neuron[outputs]);
	if (DEBUG())
	printf("Made %i Outputs\n", outputs);
	for (int i = 0; i < outputs; i++)	
		Outputs[i].InitRandomise(randRange);
		
	if (DEBUG())
	printf("Randomised Outputs\n", neurons);
	CreateSynapses(maxSyns);
	if (DEBUG())
	printf("Created Network\n");

}

NeuralNetwork::NeuralNetwork(NeuralNetwork *n) {
	//implement based on network n and randomise
}

void NeuralNetwork::Init(int ins, int mid, int outs, int maxSyns, int outSyns, int repeats, int RandRange, int RandChance) {
	int NeuronsMade = 0;
	if (outSyns <= 0)
		outSyns = neurons/5;
	this->outSyns = (outSyns <= 0 ? neurons/2 : outSyns);
	randRange = RandRange;
	randChance = RandChance;
	midRepeats = repeats;
	inputs = ins;
	neurons = mid;
	outputs = outs;


	//create the input layer
	Inputs = (std::unique_ptr<Neuron[]>)(new Neuron[inputs]);
	for (int i = 0; i < inputs; i++)
		Inputs[i].InitRandomise(randRange);
	NeuronsMade += inputs;
	//create the middle layer, normally this would be multiple smaller
	//layers, but this is one big interconnected "ball" of neurons,
	//which have a random amount of connections to other neurons.
	//why? because I want to see if it works. I will make a basic
	//implementation and test both and test the efficiency.
	Neurons = (std::unique_ptr<Neuron[]>)(new Neuron[neurons]);
	for (int i = 0; i < neurons; i++)
		Neurons[i].InitRandomise(randRange);
	NeuronsMade += neurons;
	//create the output layer
	Outputs = (std::unique_ptr<Neuron[]>)(new Neuron[outputs]);
	for (int i = 0; i < outputs; i++)	
		Outputs[i].InitRandomise(randRange);
	NeuronsMade += outputs;
	if (DEBUG())
		printf("Made %i Neurons\n", NeuronsMade);
	CreateSynapses(maxSyns);
	if (DEBUG())
		printf("Made Network\n");
}
//destructor
NeuralNetwork::~NeuralNetwork() {
	Inputs.reset();
	Neurons.reset();
	Outputs.reset();
}

//create the synapses for the neurons, the "middle" neurons have a max of maxSyns
int NeuralNetwork::CreateSynapses(int maxSyns) {
	int numCreated = 0;

	//creating random connections from input neurons to
	//the middle neurons.
	for (int i = 0; i < inputs; i++) {
		int rt = sqrtl(neurons);
		int synapseAmount = rt + (rand() % (rt * 2));
		Neuron **ns = (Neuron **)calloc(synapseAmount, sizeof(Neuron *));
		for (int j = 0; j < synapseAmount; j++) {
			Neuron *b = getNeuron(1, rand() % neurons);
			while (!hasNeuron(ns, j, b) && j > 0) {
				b = getNeuron(1, rand() % neurons);
			}
			*(ns + j) = b;
		}
		Inputs[i].MakeSynapses(ns, synapseAmount, randRange);
		free(ns);
		numCreated += synapseAmount;
	}
	//adding synapses between neurons, also making
	//sure they dont double up E.G Neuron A has
	//connection to Neuron B in >1 synapses,
	//but "backtracing" is allowed, E.G A and B have
	//1 synapse from a->b and 1 from b->a
	for (int i = 0; i < neurons; i++) {
		int synapseAmount = rand() % maxSyns;
		Neuron **ns = (Neuron **)calloc(synapseAmount, sizeof(Neuron *));
		for (int j = 0; j < synapseAmount; j++) {
			Neuron *n = getNeuron(1, rand() % neurons);
			while ( j > 0 && !hasNeuron(ns, j, n)) {
				n = getNeuron(1, rand() % neurons);
			}
			*(ns + j) = n;
		}
		Neurons[i].MakeSynapses(ns, synapseAmount, randRange);
		free(ns);
		numCreated += synapseAmount;
	}
	//This adds up to (outputs) synapses to
	//outSyns neurons from the middle layer,
	//also stops synapse doubling
	Neuron **nss = (Neuron **)calloc(outSyns, sizeof(Neuron *));
	for (int i = 0; i < outSyns; i++) {
		int numSyns = rand() % outputs;
		Neuron **ns = (Neuron **)calloc(numSyns, sizeof(Neuron *));
		for (int j = 0; j < numSyns; j++){
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
	if (DEBUG())
		printf("Created %i Synapses\n", numCreated);
	return numCreated;
}

//check if neuron n is in neuron ns
bool NeuralNetwork::hasNeuron(Neuron **ns, int size, Neuron *n) {
	for (int i = 0; i < size; i++)
		if (*(ns + i) == n)
			return true;
	return false;
}
//get a neuron from somewhere within the network
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

//get all the synapses that fire, for later running
Synapse **NeuralNetwork::getFiringSynapses(int *lengthOut) {
	*lengthOut = 0;
	Synapse **Syns = (Synapse **)calloc(neurons, sizeof(Synapse *));
	for (int i = 0; i < neurons; i++) {
		Synapse *a = Neurons[i].Fire();
		if (a != nullptr)
			*(Syns + ((*lengthOut)++)) = a;
	}
	return Syns;
}

//running this type of network "requires" the middle
//layer to be run multiple times, but I want to
//implement a "tracking" way of doing this
//where instead I keep track of the synapses
//that fire, and then only check neurons
//that are connected to the firing synapses.
bool *NeuralNetwork::Run()
{
	//call fire on neurons, return output neuron's valueGreaterThanBias
	bool *vgtb = (bool *)calloc(outputs, sizeof(bool));
	for (int i = 0; i < inputs; i++) {
		Inputs[i].FireNow();
	}
	int *vals = (int *)calloc(neurons, sizeof(int));
	for (int i = 0; i < midRepeats; i++) {
		
	}
}

int *NeuralNetwork::Run(int *input) {
	for (int i = 0; i < inputs; i++) {
		Inputs[i].set(*(input + i));
	}
	//call fire on neurons, return output neuron's val
}


//randomise, for networks based off other networks.
//this should include adding/deleting synapses,
//randomising synapse strength, and neuron bias.
void NeuralNetwork::Randomise() {
	//code here <3
}