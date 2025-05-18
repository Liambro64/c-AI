#include "../Project.hpp"
#include <iostream> // For error messages



Packet::Packet() : packetId("default_id"), inputs(0), neurons(0), outputs(0), commOutputs(0) // Initialize new members
{
}
Packet::Packet(const std::string &id, int ins, int mid, int outs, int commOuts, int maxSyns, int outSyns, int commOutSynsParam, int repeats, int RandRange, int RandChance, int Chance) : packetId(id) 
{
	Init(id, ins, mid, outs, commOuts, maxSyns, outSyns, commOutSynsParam, repeats, RandRange, RandChance, Chance);
}
Packet::Packet(const Packet &pkt) {
	
}
Packet::Packet(Packet *packet)
{
	// copy constructor
	this->packetId = packet->packetId + "_clone"; 
	this->inputs = packet->inputs;
	this->neurons = packet->neurons;
	this->outputs = packet->outputs;
	this->commOutputs = packet->commOutputs; // Copy commOutputs size
	this->randRange = packet->randRange;
	this->randChance = packet->randChance;
	this->midRepeats = packet->midRepeats;
	this->outSyns = packet->outSyns;
	this->commOutSyns = packet->commOutSyns; // Copy commOutSyns
	this->Chance = packet->Chance;

	this->Inputs.reserve(inputs);
	this->Neurons.reserve(neurons);
	this->Outputs.reserve(outputs);
	this->CommOutputs.reserve(commOutputs); // Allocate CommOutputs

	for (int i = 0; i < inputs; i++)
	{
		Inputs[i].setBias(packet->Inputs[i].getBias());
		Inputs[i].setOperator(packet->Inputs[i].getOperator()); // Assuming Neuron::setOperator takes bool or int correctly
		// Deep copy synapses for Inputs
		Synapse* oldSyns = packet->Inputs[i].getSynapses();
		for (int j = 0; j < packet->Inputs[i].getNumSyns(); j++) {
			Neuron* targetOriginal = oldSyns[j].getTo();
			int targetIdxOriginal = packet->findNeuron(targetOriginal);
			Neuron* targetClone = this->getNeuron(targetIdxOriginal / (packet->inputs + packet->neurons + packet->outputs + packet->commOutputs), targetIdxOriginal % (packet->inputs + packet->neurons + packet->outputs + packet->commOutputs)); // This mapping is complex
			// Simplified: find relative index in target layer and use it in clone
            // This needs a robust way to map original target neurons to cloned target neurons.
            // The findNeuron and getNeuron logic needs to be very solid for this.
            // For now, assuming a simplified mapping or placeholder for deep synapse copy.
            // Inputs[i].addSynapse(Synapse(Inputs.get() + i, targetClone, oldSyns[j].getStrength()));
		}
	}
	for (int i = 0; i < neurons; i++)
	{
		Neurons[i].setBias(packet->Neurons[i].getBias());
		Neurons[i].setOperator(packet->Neurons[i].getOperator());
		// Deep copy synapses for Neurons (to Neurons, Outputs, CommOutputs)
	}
	for (int i = 0; i < outputs; i++) {
        Outputs[i].setBias(packet->Outputs[i].getBias());
        Outputs[i].setOperator(packet->Outputs[i].getOperator());
        // Outputs usually don't have outgoing synapses within the packet
    }
	for (int i = 0; i < commOutputs; i++) { // Copy CommOutputs neurons
        CommOutputs[i].setBias(packet->CommOutputs[i].getBias());
        CommOutputs[i].setOperator(packet->CommOutputs[i].getOperator());
        // CommOutputs usually don't have outgoing synapses *within* the packet
    }
	// TODO: Implement robust deep copy of synapses for all layers, mapping to *cloned* target neurons.
	// This is non-trivial and requires a map from old neuron addresses to new neuron addresses.
	// Randomise(Chance + (Chance / 4), Chance, Chance - (Chance / 4)); // Maybe not randomize a clone immediately
}

Packet *Packet::Clone()
{
	Packet *clone = new Packet(); 
	clone->packetId = this->packetId + "_cloned_via_clone";
	clone->inputs = inputs;
	clone->neurons = neurons;
	clone->outputs = outputs;
	clone->commOutputs = commOutputs; // Clone commOutputs size
	clone->randRange = randRange;
	clone->randChance = randChance;
	clone->midRepeats = midRepeats;
	clone->outSyns = outSyns;
	clone->commOutSyns = commOutSyns; // Clone commOutSyns
	clone->Chance = Chance;

	clone->Inputs.reserve(inputs);
	clone->Neurons.reserve(neurons);
	clone->Outputs.reserve(outputs);
	clone->CommOutputs.reserve(commOutputs); // Allocate CommOutputs for clone

	// Copy neuron properties (bias, operator)
	for (int i = 0; i < inputs; i++) {
		clone->Inputs[i].setBias(Inputs[i].getBias());
		clone->Inputs[i].setOperator(Inputs[i].getOperator());
	}
	for (int i = 0; i < neurons; i++) {
		clone->Neurons[i].setBias(Neurons[i].getBias());
		clone->Neurons[i].setOperator(Neurons[i].getOperator());
	}
	for (int i = 0; i < outputs; i++) {
		clone->Outputs[i].setBias(Outputs[i].getBias());
		clone->Outputs[i].setOperator(Outputs[i].getOperator());
	}
	for (int i = 0; i < commOutputs; i++) { // Clone CommOutputs neurons
        clone->CommOutputs[i].setBias(CommOutputs[i].getBias());
        clone->CommOutputs[i].setOperator(CommOutputs[i].getOperator());
    }

	// TODO: Implement robust deep copy of synapses, similar to the copy constructor.
    // This requires mapping original neuron pointers to their cloned counterparts.
    // A common strategy is to build a map: std::map<Neuron*, Neuron*> oldToNewNeuronMap;
    // Then iterate through all synapses of the original packet, get the original target neuron,
    // find its clone in the map, and create a new synapse in the cloned packet pointing to the cloned target.

	// For now, if CreateSynapses is called, it will create new random synapses, not clone.
	// clone->CreateSynapses(maxSyns); // This would re-randomize synapses.

	return clone; 
}
void Packet::Init(Packet *packet) {
	
	// copy constructor
	this->packetId = packet->packetId + "_clone"; 
	this->inputs = packet->inputs;
	this->neurons = packet->neurons;
	this->outputs = packet->outputs;
	this->commOutputs = packet->commOutputs; // Copy commOutputs size
	this->randRange = packet->randRange;
	this->randChance = packet->randChance;
	this->midRepeats = packet->midRepeats;
	this->outSyns = packet->outSyns;
	this->commOutSyns = packet->commOutSyns; // Copy commOutSyns
	this->Chance = packet->Chance;

	this->Inputs.reserve(inputs);
	this->Neurons.reserve(neurons);
	this->Outputs.reserve(outputs);
	this->CommOutputs.reserve(commOutputs); // Allocate CommOutputs

	for (int i = 0; i < inputs; i++)
	{
		Inputs[i].setBias(packet->Inputs[i].getBias());
		Inputs[i].setOperator(packet->Inputs[i].getOperator()); // Assuming Neuron::setOperator takes bool or int correctly
		// Deep copy synapses for Inputs
		Synapse* oldSyns = packet->Inputs[i].getSynapses();
		for (int j = 0; j < packet->Inputs[i].getNumSyns(); j++) {
			Neuron* targetOriginal = oldSyns[j].getTo();
			int targetIdxOriginal = packet->findNeuron(targetOriginal);
			Neuron* targetClone = this->getNeuron(targetIdxOriginal / (packet->inputs + packet->neurons + packet->outputs + packet->commOutputs), targetIdxOriginal % (packet->inputs + packet->neurons + packet->outputs + packet->commOutputs)); // This mapping is complex
			// Simplified: find relative index in target layer and use it in clone
            // This needs a robust way to map original target neurons to cloned target neurons.
            // The findNeuron and getNeuron logic needs to be very solid for this.
            // For now, assuming a simplified mapping or placeholder for deep synapse copy.
            // Inputs[i].addSynapse(Synapse(Inputs.get() + i, targetClone, oldSyns[j].getStrength()));
		}
	}
	for (int i = 0; i < neurons; i++)
	{
		Neurons[i].setBias(packet->Neurons[i].getBias());
		Neurons[i].setOperator(packet->Neurons[i].getOperator());
		// Deep copy synapses for Neurons (to Neurons, Outputs, CommOutputs)
	}
	for (int i = 0; i < outputs; i++) {
        Outputs[i].setBias(packet->Outputs[i].getBias());
        Outputs[i].setOperator(packet->Outputs[i].getOperator());
        // Outputs usually don't have outgoing synapses within the packet
    }
	for (int i = 0; i < commOutputs; i++) { // Copy CommOutputs neurons
        CommOutputs[i].setBias(packet->CommOutputs[i].getBias());
        CommOutputs[i].setOperator(packet->CommOutputs[i].getOperator());
        // CommOutputs usually don't have outgoing synapses *within* the packet
    }
}
void Packet::Init(const std::string &id, int ins, int mid, int outs, int commOuts, int maxSyns, int outSynsParam, int commOutSynsParam, int repeats, int RandRange, int RandChance, int ChanceParam) 
{
	this->packetId = id; 
	int NeuronsMade = 0;
	
	this->inputs = ins;
	this->neurons = mid;
	this->outputs = outs;
	this->commOutputs = commOuts; // Initialize commOutputs size

	if (outSynsParam <= 0 && neurons > 0 && outputs > 0) // Avoid division by zero
		this->outSyns = neurons / (outputs > 0 ? outputs : 1) / 2; // Default for Neurons->Outputs synapses
	else
		this->outSyns = outSynsParam;
	if (this->outSyns == 0 && neurons > 0 && outputs > 0) this->outSyns = 1;


	if (commOutSynsParam <= 0 && neurons > 0 && commOutputs > 0) // Avoid division by zero
		this->commOutSyns = neurons / (commOutputs > 0 ? commOutputs : 1) / 2; // Default for Neurons->CommOutputs synapses
	else
		this->commOutSyns = commOutSynsParam;
    if (this->commOutSyns == 0 && neurons > 0 && commOutputs > 0) this->commOutSyns = 1;


	this->randRange = RandRange;
	this->randChance = RandChance;
	this->midRepeats = repeats;
	this->Chance = ChanceParam;

	Inputs.resize(inputs);
	for (int i = 0; i < inputs; i++)
		Inputs[i].InitRandomise(randRange);
	NeuronsMade += inputs;

	Neurons.resize(neurons);
	for (int i = 0; i < neurons; i++)
		Neurons[i].InitRandomise(randRange);
	NeuronsMade += neurons;

	Outputs.resize(outputs);
	for (int i = 0; i < outputs; i++)
		Outputs[i].InitRandomise(randRange);
	NeuronsMade += outputs;

	CommOutputs.resize(commOutputs); // Initialize CommOutputs layer
	for (int i = 0; i < commOutputs; i++)
		CommOutputs[i].InitRandomise(randRange);
	NeuronsMade += commOutputs;
	
	// Adjust firingNeurons array size if it's used for all potentially firing internal neurons
	firingNeurons.resize(inputs + neurons /* + commOutputs if they also fire based on internal state before IPC */);
	
	CreateSynapses(maxSyns);
}
// destructor
Packet::~Packet()
{

}
void Packet::SRand()
{
	srand(time(0));
}
int Packet::findNeuron(Neuron *n)
{
	int base = 0;
	for (int i = 0; i < inputs; i++)
		if (Inputs.data() + i == n)
			return base + i;
	base += inputs;
	for (int i = 0; i < neurons; i++)
		if (Neurons.data() + i == n)
			return base + i;
	base += neurons;
	for (int i = 0; i < outputs; i++)
		if (Outputs.data() + i == n)
			return base + i;
	base += outputs;
	for (int i = 0; i < commOutputs; i++) // Search in CommOutputs
		if (CommOutputs.data() + i == n)
			return base + i;
	return -1; // Not found
}

void printCustom(Neuron *from, Neuron **tos, Packet *net, int size)
{
	printf("Neuron %i to Neurons: ", net->findNeuron(from));
	for (int i = 0; i < size; i++)
	{
		printf("%i%s", net->findNeuron(*(tos + i)), i == size - 1 ? "" : ", ");
	}
	printf("\nSyns: %i\n", size);
}
void Packet::printSynapses(char *filter)
{
	// Print Input Synapses
	for (int i = 0; i < inputs; i++)
	{
		if (Inputs[i].getNumSyns() == 0) {
			// printf("Input Neuron %i in Packet %s has no synapses\n", i, packetId.c_str());
			continue;
		}
		printf("Packet %s: Input Neuron %i to (%i) Neurons: ", packetId.c_str(), i, Inputs[i].getNumSyns());
		for (int j = 0; j < Inputs[i].getNumSyns(); j++)
		{
			Neuron* targetNeuron = Inputs[i].getSynapses()[j].getTo();
			int n_idx = findNeuron(targetNeuron);
			const char *layer_name = "Unknown";
			int relative_idx = -1;

			if (n_idx >= inputs && n_idx < inputs + neurons) {
				layer_name = "Middle";
				relative_idx = n_idx - inputs;
			} else { // Add other layers if Inputs can connect to them
				layer_name = "Other";
				relative_idx = n_idx;
			}
			printf("%s Neuron %i%s", layer_name, relative_idx, j == Inputs[i].getNumSyns() - 1 ? "\n" : ", ");
		}
	}
	// Print Middle Neuron Synapses
	for (int i = 0; i < neurons; i++)
	{
		if (Neurons[i].getNumSyns() == 0) {
			// printf("Middle Neuron %i in Packet %s has no synapses\n", i, packetId.c_str());
			continue;
		}
		printf("Packet %s: Middle Neuron %i to (%i) Neurons: ", packetId.c_str(), i, Neurons[i].getNumSyns());
		for (int j = 0; j < Neurons[i].getNumSyns(); j++)
		{
			Neuron* targetNeuron = Neurons[i].getSynapses()[j].getTo();
			int n_idx = findNeuron(targetNeuron);
			const char *layer_name = "Unknown";
			int relative_idx = -1;

			if (n_idx >= inputs && n_idx < inputs + neurons) {
				layer_name = "Middle";
				relative_idx = n_idx - inputs;
			} else if (n_idx >= inputs + neurons && n_idx < inputs + neurons + outputs) {
				layer_name = "Output";
				relative_idx = n_idx - (inputs + neurons);
			} else if (n_idx >= inputs + neurons + outputs && n_idx < inputs + neurons + outputs + commOutputs) {
				layer_name = "CommOutput";
				relative_idx = n_idx - (inputs + neurons + outputs);
			}
			else {
				layer_name = "Other"; // Could be an inter-packet synapse target
				relative_idx = n_idx; // Print absolute if not in known local layers
			}
			printf("%s Neuron %i%s", layer_name, relative_idx, j == Neurons[i].getNumSyns() - 1 ? "\n" : ", ");
		}
	}
    // CommOutputs typically don't have synapses *within* the packet, their synapses go *out* to other packets.
    // Output neurons also typically don't have synapses within the packet.
}
int Packet::CreateSynapses(int maxSyns)
{
	int numCreated = 0;

	// Connections from Inputs to Neurons
	if (inputs > 0 && neurons > 0) {
		for (int i = 0; i < inputs; i++)
		{
			int rt = sqrtl(neurons);
			if (rt == 0) rt = 1;
			int synapseAmount = rt + (rand() % (rt * 2));
			if (synapseAmount == 0 && neurons > 0) synapseAmount = 1;
			if (synapseAmount > neurons) synapseAmount = neurons;

			std::vector<Neuron*> ns_vec;
			ns_vec.reserve(synapseAmount);
			for (int j = 0; j < synapseAmount; j++)
			{
				Neuron *b = getNeuron(1, rand() % neurons); // Layer 1 is Neurons
				bool exists = false;
				for(Neuron* existing_n : ns_vec) if(existing_n == b) exists = true;
				while (exists && ns_vec.size() < (size_t)neurons) // Ensure not to loop infinitely if all neurons already picked
				{
					b = getNeuron(1, rand() % neurons);
					exists = false;
					for(Neuron* existing_n : ns_vec) if(existing_n == b) exists = true;
				}
				if (!exists) ns_vec.push_back(b);
			}
			if (!ns_vec.empty()) {
				Inputs[i].MakeSynapses(ns_vec.data(), ns_vec.size(), randRange);
				numCreated += ns_vec.size();
			}
		}
	}

	// Connections between Neurons (recurrent/sphere)
	if (neurons > 0) {
		for (int i = 0; i < neurons; i++)
		{
			int synapseAmountTarget = (maxSyns == -1 ? rand() % (int)sqrtl(neurons) : rand() % maxSyns);
			if (synapseAmountTarget == 0 && neurons > 1) synapseAmountTarget = 1; // Ensure some connectivity if possible
			if (synapseAmountTarget > neurons) synapseAmountTarget = neurons;
			
			std::vector<Neuron*> ns_vec;
			ns_vec.reserve(synapseAmountTarget);
			for (int j = 0; j < synapseAmountTarget; j++)
			{
				Neuron *n = getNeuron(1, rand() % neurons); // Target within Neurons layer
				bool exists = false;
				for(Neuron* existing_n : ns_vec) if(existing_n == n) exists = true;
				if (n == &Neurons[i]) exists = true; // Don't connect to self in this simple setup

				while (exists && ns_vec.size() < (size_t)neurons -1) // -1 to account for self
				{
					n = getNeuron(1, rand() % neurons);
					exists = false;
					for(Neuron* existing_n : ns_vec) if(existing_n == n) exists = true;
					if (n == &Neurons[i]) exists = true;
				}
				if (!exists) ns_vec.push_back(n);
			}
			if (!ns_vec.empty()) {
				Neurons[i].MakeSynapses(ns_vec.data(), ns_vec.size(), randRange);
				numCreated += ns_vec.size();
			}
		}
	}
	if (outSyns == 0)
		outSyns = neurons / outputs;
	// Connections from Neurons to Outputs
	if (neurons > 0 && outputs > 0 && outSyns > 0) {
		std::vector<Neuron*> source_neurons_for_output;
		source_neurons_for_output.reserve(outSyns);
		for(int i=0; i<outSyns; ++i) {
			Neuron* n = getNeuron(1, rand() % neurons);
			bool exists = false;
			for(Neuron* existing_n : source_neurons_for_output) if(existing_n == n) exists = true;
			while(exists && source_neurons_for_output.size() < (size_t)neurons) {
				n = getNeuron(1, rand() % neurons);
				exists = false;
				for(Neuron* existing_n : source_neurons_for_output) if(existing_n == n) exists = true;
			}
			if(!exists) source_neurons_for_output.push_back(n);
		}

		for (Neuron* source_neuron : source_neurons_for_output) {
			int numSynsToOut = outputs / 2 + (rand() % (outputs - outputs / 2 + 1));
			if (numSynsToOut == 0 && outputs > 0) numSynsToOut = 1;
			if (numSynsToOut > outputs) numSynsToOut = outputs;

			std::vector<Neuron*> target_output_neurons;
			target_output_neurons.reserve(numSynsToOut);
			for (int j = 0; j < numSynsToOut; j++) {
				Neuron* target_n = getNeuron(2, rand() % outputs); // Layer 2 is Outputs
				bool exists = false;
				for(Neuron* existing_n : target_output_neurons) if(existing_n == target_n) exists = true;
				while(exists && target_output_neurons.size() < (size_t)outputs) {
					target_n = getNeuron(2, rand() % outputs);
					exists = false;
					for(Neuron* existing_n : target_output_neurons) if(existing_n == target_n) exists = true;
				}
				if(!exists) target_output_neurons.push_back(target_n);
			}
			if(!target_output_neurons.empty()) {
				source_neuron->MakeSynapses(target_output_neurons.data(), target_output_neurons.size(), randRange);
				numCreated += target_output_neurons.size();
			}
		}
	}

	// Connections from Neurons to CommOutputs
	if (neurons > 0 && commOutputs > 0 && commOutSyns > 0) {
		std::vector<Neuron*> source_neurons_for_comm;
		source_neurons_for_comm.reserve(commOutSyns);
         for(int i=0; i<commOutSyns; ++i) { // Select commOutSyns source neurons from Middle layer
			Neuron* n = getNeuron(1, rand() % neurons); // Layer 1 is Neurons
			bool exists = false;
			for(Neuron* existing_n : source_neurons_for_comm) if(existing_n == n) exists = true;
			while(exists && source_neurons_for_comm.size() < (size_t)neurons) {
				n = getNeuron(1, rand() % neurons);
				exists = false;
				for(Neuron* existing_n : source_neurons_for_comm) if(existing_n == n) exists = true;
			}
			if(!exists) source_neurons_for_comm.push_back(n);
		}

		for (Neuron* source_neuron : source_neurons_for_comm) {
			int numSynsToComm = commOutputs / 2 + (rand() % (commOutputs - commOutputs / 2 + 1));
			if (numSynsToComm == 0 && commOutputs > 0) numSynsToComm = 1;
            if (numSynsToComm > commOutputs) numSynsToComm = commOutputs;

			std::vector<Neuron*> target_comm_neurons;
			target_comm_neurons.reserve(numSynsToComm);
			for (int j = 0; j < numSynsToComm; j++) {
				Neuron* target_n = getNeuron(3, rand() % commOutputs); // Layer 3 is CommOutputs
				bool exists = false;
				for(Neuron* existing_n : target_comm_neurons) if(existing_n == target_n) exists = true;
				while(exists && target_comm_neurons.size() < (size_t)commOutputs) {
					target_n = getNeuron(3, rand() % commOutputs);
					exists = false;
					for(Neuron* existing_n : target_comm_neurons) if(existing_n == target_n) exists = true;
				}
				if(!exists) target_comm_neurons.push_back(target_n);
			}
			if(!target_comm_neurons.empty()) {
				source_neuron->MakeSynapses(target_comm_neurons.data(), target_comm_neurons.size(), randRange);
				numCreated += target_comm_neurons.size();
			}
		}
	}
	return numCreated;
}
void Packet::Run() {
	RunOnceGPU(nullptr);
	for (int i = 0; i < midRepeats - 1; i++)
	{
		RunOnceGPU(nullptr);
	}
	for (int i = 0; i < outputs; i++) {
		Outputs[i].FireNow();
	}
}
// check if neuron n is in neuron ns
bool Packet::hasNeuron(Neuron **ns, int size, Neuron *n)
{
	for (int i = 0; i < size; i++)
		if (*(ns + i) == n)
			return true;
	return false;
}
// get a neuron from somewhere within the network
Neuron *Packet::getNeuron(int a, int b) // Layer type 'a', index 'b'
{
	if (a == 0 && b < inputs) // Inputs
		return &(Inputs[b]);
	else if (a == 1 && b < neurons) // Neurons (middle)
		return &(Neurons[b]);
	else if (a == 2 && b < outputs) // Outputs
		return &(Outputs[b]);
	else if (a == 3 && b < commOutputs) // CommOutputs
		return &(CommOutputs[b]);
	return nullptr;
}

// set the values of firingNeurons to the neuron's ValGreaterThanBias
void Packet::getFiringNeurons()
{
	int i = 0;
	for (i = 0; i < inputs; i++)
		firingNeurons[i] = Inputs[i].valueGreaterThanBias();
	for (int j = 0; j < neurons; j++)
		firingNeurons[j + i] = Neurons[j].valueGreaterThanBias();
}

void Packet::getFiringNeuronsAndDecay() 
{
	int i = 0;
	for (i = 0; i < inputs; i++)
		firingNeurons[i] = Inputs[i].canFire();
	for (int j = 0; j < neurons; j++)
		firingNeurons[j + i] = Neurons[j].canFire();
}

std::vector<int> Packet::strengthSum(std::vector<std::vector<int>> Strengths)
{
	std::vector<int> ret(neurons + outputs, 0);
	for (size_t i = 0; i < Strengths.size(); ++i) // Iterate up to Strengths.size()
	{
		if (Strengths[i].empty())
			continue; // Skip if no strengths for this neuron

		// Assuming Strengths[i][0] is the count, and strengths start from [1]
		// Or, if Strengths[i] only contains strengths (count is implicit by .size())
		// For this fix, let's assume Strengths[i] contains [count, s1, s2, ...]
		int count = Strengths[i][0];
		for (int j = 0; j < count; ++j)
		{
			ret[i] += Strengths[i][j + 1]; // Summing correctly
		}
	}
	return ret;
}

void Packet::RunOnceCPU(int *a)
{
	getFiringNeuronsAndDecay(); 
	if (a != nullptr)
	{
		for (int i = 0; i < inputs; i++)
		{
			Inputs[i].set(a[i]); 
		}
	}

	// The getSynStrengths and summation logic needs to be aware of CommOutputs as potential targets
	auto synStrengths = getSynStrengths(); 
	// totalUNs now includes Neurons, Outputs, and CommOutputs if they can be targets of intra-packet synapses
	auto totalUNs = neurons + outputs + commOutputs; 
	
	std::vector<int> totals(totalUNs, 0);
    for (int i = 0; i < totalUNs; ++i) {
        if (synStrengths[i].empty() || synStrengths[i][0] == 0) 
			continue; 

        int count = synStrengths[i][0]; 
        for (int k = 0; k < count; ++k) {
            totals[i] += synStrengths[i][k + 1]; 
        }
    }

	for (int i = 0; i < totalUNs; i++)
	{
		Neuron *n = nullptr;
		if (i < neurons) { // Target is a middle Neuron
			n = getNeuron(1, i); 
		} else if (i < neurons + outputs) { // Target is an Output Neuron
			n = getNeuron(2, i - neurons); 
		} else { // Target is a CommOutput Neuron
			n = getNeuron(3, i - (neurons + outputs));
		}
		
		if (n != nullptr)
		{
			n->add(totals[i]);
		}
	}
	// Input neurons typically don't decay this way, their value is set externally.

	if (a != nullptr) { 
		for (int i = 0; i < inputs; i++)
		{
			Inputs[i].set(0); 
		}
	}
}

bool find_in(std::vector<Neuron> &vec, Neuron *n)
{	
	auto it = std::find_if(vec.begin(), vec.end(),
		[&](const Neuron &neuron) { return &neuron == n; });
	if (it != vec.end())
    	return true;
	return false; // Not found
}

int find_where(std::vector<Neuron> &vec, Neuron *n)
{
	auto it = std::find_if(vec.begin(), vec.end(),
		[&](const Neuron &neuron) { return &neuron == n; });
	if (it != vec.end())
		return std::distance(vec.begin(), it);
	return -1; // Not found
}
int Packet::getNeuronIndex(Neuron *n)
{

	if (find_in(Inputs, n))
		return find_where(Inputs, n);
	if (find_in(Neurons, n))
		return find_where(Neurons, n) + inputs;
	if (find_in(Outputs, n))
		return find_where(Outputs, n) + inputs + neurons;
	return -1;
}
std::vector<std::vector<int>> Packet::getSynStrengths()
{
	int totalUNs = neurons + outputs;
	std::vector<std::vector<int>> synStrengths(totalUNs); // Use vector of vectors for safer memory management
	std::vector<int> indexAmount;			  // Track the number of synapses for each neuron
	std::vector<int> iter;

	indexAmount.resize(totalUNs, 1);
	iter.resize(totalUNs, 1);
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
		synStrengths[i].resize(indexAmount[i]); // +1 for the length
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
		int val = n->getVal()/n->getNumSyns();
		for (int j = 0; j < n->getNumSyns(); j++)
		{
			int toIndex = getNeuronIndex(syns[j].getTo());
			if (toIndex >= 0 && toIndex < totalUNs)
			{
				synStrengths[toIndex][iter[toIndex]++] = syns[j].getStrength() + val;
			}
		}
	}

	return synStrengths;
}
void Packet::RunOnceGPU(int *a)
{
	getFiringNeuronsAndDecay();

	auto synStrengthsHost = getSynStrengths(); 
	int totalTargetableUNs = neurons + outputs + commOutputs; 

	if (totalTargetableUNs == 0) // No targetable neurons, nothing to do
		return;

	// --- Prepare data for GPU ---
	// 1. Flatten synStrengthsHost into a single array and calculate total number of strength entries
	std::vector<int> h_flat_strengths;
	std::vector<int*> h_from_pointers(totalTargetableUNs); // Host array of pointers
	size_t total_strength_entries = 0;

	for (const auto& strengths_for_neuron : synStrengthsHost) {
		total_strength_entries += strengths_for_neuron.size();
	}
	h_flat_strengths.reserve(total_strength_entries);

	size_t current_flat_offset = 0;
	for (int i = 0; i < totalTargetableUNs; ++i) {
		if (!synStrengthsHost[i].empty()) {
			// The address h_from_pointers[i] will point to on the GPU
			// will be relative to the start of d_flat_strengths.
			// So, h_from_pointers[i] stores the *data* that d_from_pointers[i] will point to.
			// This requires careful handling of pointer arithmetic on the GPU side or
			// passing an array of offsets if d_from_pointers stores offsets.
			// For the current kernel `int **from`, `d_from_pointers` needs to be an array of actual device pointers.

			// Copy data for current neuron into the flat vector
			h_flat_strengths.insert(h_flat_strengths.end(), synStrengthsHost[i].begin(), synStrengthsHost[i].end());
			// h_from_pointers[i] will be set after d_flat_strengths is allocated and pointers are calculated
		} else {
			// If a neuron has no incoming synapses, its pointer can be null or point to an empty segment.
            // For simplicity with the current kernel, we might need to ensure it points to a "count = 0" entry.
            // If synStrengthsHost[i] is truly empty, we need to handle this.
            // Let's assume getSynStrengths always provides at least a count element [0] if no strengths.
            if (synStrengthsHost[i].empty()) { // Should not happen if getSynStrengths guarantees [count, ...]
                 // Add a dummy [0] entry if it's completely empty
                 h_flat_strengths.push_back(0);
            } else {
                 // Already handled by the loop above
            }
		}
	}
    
	// --- GPU Memory Allocation ---
	int *d_totals = nullptr;
	int *d_flat_strengths_all = nullptr; // This will hold all data from h_flat_strengths
	int **d_from_pointers_array = nullptr; // This will be an array of pointers on the device

	cudaError_t err;
	err = cudaMalloc((void**)&d_totals, totalTargetableUNs * sizeof(int));
	if (err != cudaSuccess) { std::cerr << "CUDA Malloc d_totals failed: " << cudaGetErrorString(err) << std::endl; return; }
	
	err = cudaMalloc((void**)&d_flat_strengths_all, h_flat_strengths.size() * sizeof(int));
	if (err != cudaSuccess) { std::cerr << "CUDA Malloc d_flat_strengths_all failed: " << cudaGetErrorString(err) << std::endl; cudaFree(d_totals); return; }

	err = cudaMalloc((void**)&d_from_pointers_array, totalTargetableUNs * sizeof(int*));
	if (err != cudaSuccess) { std::cerr << "CUDA Malloc d_from_pointers_array failed: " << cudaGetErrorString(err) << std::endl; cudaFree(d_totals); cudaFree(d_flat_strengths_all); return; }

	// --- Copy flat strengths to GPU ---
	err = cudaMemcpy(d_flat_strengths_all, h_flat_strengths.data(), h_flat_strengths.size() * sizeof(int), cudaMemcpyHostToDevice);
	if (err != cudaSuccess) { std::cerr << "CUDA Memcpy h_flat_strengths to d_flat_strengths_all failed: " << cudaGetErrorString(err) << std::endl; /* free memory */ return; }

	// --- Prepare the array of device pointers (d_from_pointers_array) ---
	// h_from_pointers will store the actual device pointers that d_from_pointers_array will point to.
	std::vector<int*> h_device_pointers_for_d_from_array(totalTargetableUNs);
	current_flat_offset = 0;
	for(int i=0; i < totalTargetableUNs; ++i) {
		if (!synStrengthsHost[i].empty()) {
			h_device_pointers_for_d_from_array[i] = d_flat_strengths_all + current_flat_offset;
			current_flat_offset += synStrengthsHost[i].size();
		} else {
            // If getSynStrengths guarantees a [0] for empty, this offset logic needs to account for it.
            // Assuming h_flat_strengths included a [0] for originally empty synStrengthsHost[i]
            h_device_pointers_for_d_from_array[i] = d_flat_strengths_all + current_flat_offset;
            current_flat_offset += 1; // Account for the [0] entry
		}
	}
	
	err = cudaMemcpy(d_from_pointers_array, h_device_pointers_for_d_from_array.data(), totalTargetableUNs * sizeof(int*), cudaMemcpyHostToDevice);
	if (err != cudaSuccess) { std::cerr << "CUDA Memcpy h_device_pointers_for_d_from_array to d_from_pointers_array failed: " << cudaGetErrorString(err) << std::endl; /* free memory */ return; }

	// --- Launch Kernel ---
	int threadsPerBlock = 256;
	int numBlocks = (totalTargetableUNs + threadsPerBlock - 1) / threadsPerBlock;
	Wrapper::VSWrapper(d_totals, d_from_pointers_array, totalTargetableUNs);


	// --- Copy results back to host ---
	std::vector<int> totals(totalTargetableUNs);
	err = cudaMemcpy(totals.data(), d_totals, totalTargetableUNs * sizeof(int), cudaMemcpyDeviceToHost);
	if (err != cudaSuccess) { std::cerr << "CUDA Memcpy d_totals to totals failed: " << cudaGetErrorString(err) << std::endl; /* free memory */ return; }

	// --- Free GPU Memory ---
	cudaFree(d_totals);
	cudaFree(d_flat_strengths_all);
	cudaFree(d_from_pointers_array);

	// --- Apply summed strengths to neurons (same as CPU version) ---
	for (int i = 0; i < totalTargetableUNs; i++)
	{
		Neuron *n = nullptr;
		if (i < neurons) { 
			n = getNeuron(1, i); 
		} else if (i < neurons + outputs) { 
			n = getNeuron(2, i - neurons); 
		} else { 
			n = getNeuron(3, i - (neurons + outputs));
		}
		
		if (n != nullptr)
		{
			n->add(totals[i]);
		}
	}

	if (a != nullptr) { 
		for (int i = 0; i < inputs; i++)
		{
			Inputs[i].set(0); 
		}
	}
}


// ...existing code...

// randomise, for networks based off other networks.
// this should include adding/deleting synapses,
// randomising synapse strength, and neuron bias.
void Packet::Randomise(int SynapseChance)
{
	// code here <3
	for (int i = 0; i < inputs; i++)
	{
		Inputs[i].Randomise(randChance, randRange, true, &Neurons, SynapseChance);
	}
	for (int i = 0; i < neurons; i++)
	{
		Neurons[i].Randomise(randChance, randRange, true, &Neurons, SynapseChance);
		Neurons[i].RandomiseSynapses(SynapseChance, randChance, randRange, &Outputs);
	}
	for (int i = 0; i < outputs; i++)
	{
		Outputs[i].Randomise(randChance, randRange, false, NULL, 0);
	}
}

// Updated fdll functions to be Packet-centric
// Note: The original CreateNetwork in NeuralNetwork.cpp was for the old NeuralNetwork class.
// This CreatePacket is specifically for creating a single Packet instance.
fdll Packet *CreatePacket(const char* id, int ins, int mid, int out, int commOuts, int maxSyns = 15, int outSyns = 0, int commOutSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100)
{
	return new Packet(std::string(id), ins, mid, out, commOuts, maxSyns, outSyns, commOutSyns, repeats, RandRange, RandChance, Chance);
}

fdll Packet *CreateCleanPacket(const char *id) 
{
	// Creates a packet with no neurons, intended to be initialized later via InitPacket.
	// Or, you could give it default small sizes.
	return new Packet(std::string(id), 0, 0, 0, 0); 
}

// Renamed from CloneNetwork to ClonePacket for clarity
fdll Packet *ClonePacket(Packet *pkt)
{
	if (pkt != nullptr)
	{
		// Packet::Clone() handles the deep copy logic (with TODOs for full synapse cloning)
		return pkt->Clone(); 
	}
	return nullptr;
}

// Renamed from RandomiseNetwork to RandomisePacket
fdll void RandomisePacket(Packet *pkt, int chance)
{
	if (pkt != nullptr)
	{
		// EzRandomise uses the packet's internal Chance, randChance, randRange
		pkt->Randomise(chance); 
	}
}

fdll int GetPacketChance(Packet *pkt) // Renamed
{
	if (pkt != nullptr)
	{
		return pkt->getChance();
	}
	return 0;
}
fdll int SetPacketChance(Packet *pkt, int chance) // Renamed
{
	if (pkt != nullptr)
	{
		return pkt->setChance(chance);
	}
	return 0;
}

// Renamed from DestroyNetwork to DestroyPacket
fdll void DestroyPacket(Packet *pkt)
{
	if (pkt != nullptr)
	{
		delete pkt;
	}
}

// Renamed from Init to InitPacket for clarity in C API
fdll void InitPacket(Packet *pkt, const char *id, int ins, int mid, int out, int commOuts, int maxSyns = 15, int outSyns = 0, int commOutSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100) 
{
	if (pkt != nullptr)
	{
		pkt->Init(std::string(id), ins, mid, out, commOuts, maxSyns, outSyns, commOutSyns, repeats, RandRange, RandChance, Chance);
	}
}

fdll Neuron *GetPacketNeurons(Packet *pkt) // Renamed
{
	if (pkt != nullptr)
	{
		return pkt->GetNeurons()->data(); 
	}
	return nullptr;
}
fdll Neuron *GetPacketInputs(Packet *pkt) // Renamed
{
	if (pkt != nullptr)
	{
		return pkt->GetInputs()->data();
	}
	return nullptr;
}
fdll Neuron *GetPacketOutputs(Packet *pkt) // Renamed
{
	if (pkt != nullptr)
	{
		return pkt->GetOutputs()->data();
	}
	return nullptr;
}
fdll Neuron *GetPacketCommOutputs(Packet *pkt) // Added for CommOutputs
{
    if (pkt != nullptr)
    {
        return pkt->GetCommOutputs()->data();
    }
    return nullptr;
}

fdll int GetPacketInputsSize(Packet *pkt) // Renamed
{
	if (pkt != nullptr)
	{
		return pkt->getInputs();
	}
	return 0;
}
fdll int GetPacketOutputsSize(Packet *pkt) // Renamed
{
	if (pkt != nullptr)
	{
		return pkt->getOutputs();
	}
	return 0;
}
fdll int GetPacketNeuronsSize(Packet *pkt) // Renamed
{
	if (pkt != nullptr)
	{
		return pkt->getNeurons();
	}
	return 0;
}
fdll int GetPacketCommOutputsSize(Packet *pkt) // Added for CommOutputs
{
    if (pkt != nullptr)
    {
        return pkt->getCommOutputsCount();
    }
    return 0;
}
fdll void SRandPacket(Packet *pkt) // Renamed, though SRand is often global
{
	if (pkt != nullptr)
	{
		// srand(time(0)) is global, Packet::SRand() just calls it.
		// Calling it per packet is redundant if it's just srand(time(0)).
		// If Packet::SRand() had packet-specific seeding, this would be more meaningful.
		pkt->SRand(); 
	}
}