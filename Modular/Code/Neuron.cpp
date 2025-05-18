#include "../Project.hpp"
#include <algorithm> // For std::remove_if

Neuron::Neuron() {
	bias = 0;
	oprtr = false; // Default to false
	val = 0;
	// syns is an empty vector by default
}

Neuron::Neuron(int Bias, bool Oprtr)
{
	this->bias = Bias;
	this->oprtr = Oprtr;
	this->val = 0;
	// syns is an empty vector by default
}

Neuron::Neuron(int randRange)
{
	this->bias = (rand() % (randRange * 2)) - randRange;
	this->oprtr = (rand() % 2) == 0;
	this->val = 0;
	// syns is an empty vector by default
}

Neuron::~Neuron()
{
	// std::vector cleans itself up
}


//intital randomisation
void Neuron::InitRandomise(int range) {
	this->bias = (rand() % (range * 2)) - range;
	this->oprtr = (rand() % 2) == 0;
}

// Add a single synapse.
void Neuron::addSynapse(const Synapse& syn) // Pass by const reference
{
	syns.push_back(syn);
}

// Add multiple synapses.
void Neuron::addSynapses(const std::vector<Synapse>& newSyns) // Pass by const reference
{
	syns.insert(syns.end(), newSyns.begin(), newSyns.end());
}

//make a single synapse
void Neuron::MakeSynapse(Neuron *to, int randRange)
{
	syns.emplace_back(randRange, this, to); // Use emplace_back for efficiency
}

//make multiple synapses
void Neuron::MakeSynapses(Neuron **tos, int amount, int randRange)
{
	syns.reserve(syns.size() + amount); // Pre-allocate memory
	for (int i = 0; i < amount; i++) {
		if (tos[i] != nullptr) { // Ensure target neuron is not null
			syns.emplace_back(randRange, this, tos[i]);
		}
	}
}

//for running the network
bool Neuron::valueGreaterThanBias()
{
	return oprtr ? bias > val : bias < val;
}

//fire the neuron's synapses unconditionally
Synapse *Neuron::FireNow()
{
	for (size_t i = 0; i < syns.size(); i++)
	{
		syns[i].FireNow(val);
	}
	return syns.data(); // Return pointer to underlying data, be cautious
}

//randomise for learning
void Neuron::Randomise(int chance, int range, bool randomiseSynapsesFlag, std::vector<Neuron> *layer, int synapseChance)
{
	int r1 = rand() % chance;
	int r2 = rand() % chance;
	if (r1 == 1)
		bias += (rand() % (range * 2)) - range;
	if (r2 == 1)
		oprtr = (rand() % 2) == 0;
	if (!randomiseSynapsesFlag)
		return;
	
	RandomiseSynapses(synapseChance, chance, range, layer); 
	
	for (size_t i = 0; i < syns.size(); i++) 
		syns[i].Randomise(chance, range);
}

bool Neuron::canFire()
{
	bool r = valueGreaterThanBias();
	if (r)
	{
		ApplyBiasDecay();
	}
	return r;
}

/// @brief Randomise the synapses of this neuron.
/// @details This function will randomise the synapses of this neuron, including deleting, adding and randomising of values.
/// @param changeChance the chance of a change event.
/// @param randChance The chance of a randomisation event (e.g. overall modification).
/// @param randRange The range for randomizing new synapse strengths.
/// @return how many synapses were added/removed (net change)
int Neuron::RandomiseSynapses(int changeChance, int randChance, int randRange, std::vector<Neuron> *layer)
{
	int netChange = 0;
	int layerSize = layer ? layer->size() : 0; // Check if layer is not null and get its size
	if (rand() % randChance != 1) // Overall chance to modify synapses
		return 0;

	// Attempt to delete existing synapses
	if (!syns.empty()) {
		// Using erase-remove idiom for efficient deletion
		auto original_size = syns.size();
		syns.erase(std::remove_if(syns.begin(), syns.end(),
			[&](const Synapse& s) {
				if (rand() % changeChance == 0) {
					netChange--;
					return true; // Mark for removal
				}
				return false;
			}), syns.end());
	}
	
	// Attempt to add new synapses from the provided layer
	if (layer != nullptr && layerSize > 0) {
		for (int i = 0; i < layerSize; i++) {
			if (rand() % changeChance == 0 && (&((*layer)[i]) != this)) // Chance to add a new synapse
			{
				// Avoid duplicate synapses to the same target neuron if desired (requires checking existing syns)
				// For simplicity, this example doesn't check for duplicates before adding.
				syns.push_back(Synapse(randRange, this, &(*layer)[i]));
				netChange++;
			}
		}
	}
	return netChange;
}

void Neuron::ApplyBiasDecay() {
	// Subtracts the bias from the current value.
	// This causes the neuron's activation to diminish if not sufficiently reinforced.
	this->val -= this->bias;
}

fdll int GetNeuronBias(Neuron *n)
{
	if (n != nullptr)
	{
		return n->getBias();
	}
	return 0;
}
fdll int GetNeuronVal(Neuron *n)
{
	if (n != nullptr)
	{
		return n->getVal();
	}
	return 0;
}
fdll int GetNeuronNumSyns(Neuron *n)
{
	if (n != nullptr)
	{
		return n->getNumSyns();
	}
	return 0;
}
fdll int GetNeuronOperator(Neuron *n)
{
	if (n != nullptr)
	{
		return n->getOperator();
	}
	return 0;
}
fdll Synapse *GetNeuronSynapses(Neuron *n)
{
	if (n != nullptr)
	{
		// Return a pointer to the beginning of the vector's data.
		// Note: If the vector is empty, data() may return nullptr or a valid pointer to a zero-sized buffer.
		// The behavior of data() on an empty vector can vary.
		// getNumSyns() should be checked by the caller if processing the array.
		return n->getSynapses(); 
	}
	return nullptr;
}