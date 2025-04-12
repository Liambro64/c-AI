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


	firingNeurons = (std::unique_ptr<bool[]>)new bool[neurons];
	//create the input layer
	Inputs = (std::unique_ptr<Neuron[]>)(new Neuron[inputs]);
	if (DEBUG())
	printf("Made %i Inputs\n", inputs);
	for (int i = 0; i < inputs; i++)
		Inputs[i].InitRandomise(randRange);
	if (DEBUG())
	printf("Randomised Inputs\n");


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
	printf("Randomised Neurons\n");

	//create the output layer
	Outputs = (std::unique_ptr<Neuron[]>)(new Neuron[outputs]);
	if (DEBUG())
	printf("Made %i Outputs\n", outputs);
	for (int i = 0; i < outputs; i++)	
		Outputs[i].InitRandomise(randRange);
		
	if (DEBUG())
	printf("Randomised Outputs\n");
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
	firingNeurons.reset();
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
	if(DEBUG())
		printf("Made Input Synapses");
	
	//adding synapses between neurons, also making
	//sure they dont double up E.G Neuron A hasx
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
	if(DEBUG())
		printf("Made Neuron Synapses");
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

//set the values of firingNeurons to the neuron's ValGreaterThanBias
void NeuralNetwork::getFiringNeurons() {
	int i = 0;
	for (i = 0; i < inputs; i++)
		firingNeurons[i] = Inputs[i].valueGreaterThanBias();
	for (int j = 0; j < neurons; j++)
		firingNeurons[j + i] = Neurons[j].valueGreaterThanBias();
}

//running this type of network "requires" the middle
//layer to be run multiple times, but I want to
//implement a "tracking" way of doing this
//where instead I keep track of the synapses
//that fire, and then only check neurons
//that are connected to the firing synapses.
int	*NeuralNetwork::RunCPU(int *a) {
	//gets the bool value of the neurons
	getFiringNeurons();
	//gets the strength of the synapses sorted
	//by their "to" value, this is for gpu
	//parallel computing.
	int **syns = getSynStrengths();
	

}

bool *NeuralNetwork::Run()
{
	//gets the bool value of the neurons
	getFiringNeurons();
	//gets the strength of the synapses sorted
	//by their "to" value, this is for gpu
	//parallel computing.
	std::unique_ptr<int *[]> synStrengths(getSynStrengths());
	//here, I need to add CUDA functions, and after this
	//I will be unable to test networks running in parallel on my laptop
	//which does not have a gpu.
}
int NeuralNetwork::getNeuronIndex(Neuron *n) {
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
int **NeuralNetwork::getSynStrengths() {
	int totalUNs = neurons+outputs;
	int **synStrengths  = (int **)calloc(totalUNs, sizeof(int *));
	int *indexAmout = (int *)calloc(totalUNs, sizeof(int));
	int *iter = (int *)calloc(totalUNs, sizeof(int));
	for (int i = 0; i < totalUNs; i++) {
		if (firingNeurons[i] == false)
			continue;
		Neuron *n = getNeuron(i / inputs >= 1 ? 1 : 0,  i / inputs >= 1 ? i - inputs : i);
		Synapse *syns = n->getSynapses();
		for (int j = 0; j < n->getNumSyns(); j++)
			indexAmout[getNeuronIndex(syns[j].getTo())]++;
	}
	for (int i = 0; i < totalUNs; i++) {
		synStrengths[i] = (int *)calloc(indexAmout[i] + 1, sizeof(int));
		if (synStrengths[i][0] == 0)
			synStrengths[i][0] = indexAmout[i];
		Neuron *n = getNeuron(i / inputs >= 1 ? 1 : 0,  i / inputs >= 1 ? i - inputs : i);
		Synapse *syns = n->getSynapses();
		for (int j = 0; j < n->getNumSyns(); j++) {
			int toIndex = getNeuronIndex(syns[i].getTo());
			synStrengths[toIndex][(iter[toIndex]++) + 1] = syns[j].getStrength();
		}
	}
	free(indexAmout);
	free(iter);
	return synStrengths;
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