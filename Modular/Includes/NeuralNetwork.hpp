#pragma once
#ifndef NEURALNETWORK_HPP
# define NEURALNETWORK_HPP
# include "../Project.hpp"
# include "Packet.hpp" // Include Packet header
# include <vector>
# include <string>
# include <map>

// class NeuralNetwork_Basic {
// 	//basic implementation
// }; // This can be removed if not used

class NeuralNetwork {
private:
    // Removed direct neuron arrays (Inputs, Neurons, Outputs, firingNeurons)
    std::vector<std::unique_ptr<Packet>> packets; // Manages multiple Packet objects
    // We might need a mapping from system input/output indices to packet input/output neurons
    // For now, let's assume a simple setup or handle mapping in RunCPU

public:
    // randRange, randChance, midRepeats, Chance might be global settings or per-packet
    // For now, let's keep them as potential global defaults for new packets.
	int randRange 		{};
	int randChance		{};
	int midRepeats		{}; // Default repeats for packets if not specified per packet
	int inputs			{}; // Total system inputs
	int outputs			{}; // Total system outputs
	int Chance 			{};

    // Removed neuron-specific getters like GetInputs, GetNeurons, GetOutputs from NeuralNetwork itself

	NeuralNetwork();
    // Constructor might take overall system I/O counts and packet configurations
	// Each packet vector also includes its own input/middle/communication/output counts (the int vector) and should be on size 4
	NeuralNetwork(int systemInputs, int systemOutputs, const std::vector<std::string, std::vector<int>>& packetIds /*, other config */);
	NeuralNetwork(NeuralNetwork *network); // For cloning
	~NeuralNetwork();
	NeuralNetwork *Clone();
	// void DestroyNeurons(); // This was tied to NN-specific neurons, now handled by Packet destructors

	int getRandRange() const { return randRange; }
	int getChance() const { return Chance; }
	int setChance(int chance) { Chance = chance; return Chance; }
	int setRandRange(int range) { randRange = range; return randRange; }
	int getRandChance() const { return randChance; }
	int setRandChance(int chance) { randChance = chance; return randChance; }
	int getMidRepeats() const { return midRepeats; }
	int setMidRepeats(int repeats) { midRepeats = repeats; return midRepeats; }
	
    int getSystemInputs() const { return inputs; }
    int getSystemOutputs() const { return outputs; }

    // Methods to add and get packets
    void AddPacket(std::unique_ptr<Packet> packet);
    Packet* GetPacket(const std::string& packetId);
    const std::vector<std::unique_ptr<Packet>>& GetAllPackets() const { return packets; }

    // Init will now set up the overall network structure, possibly creating default packets
	void Init(int systemInputs, int systemOutputs /*, packet configurations */);
	void SRand(); // Could call SRand on all packets or be a global seed

    // findNeuron, getNeuron, CreateSynapses, printSynapses were NN-specific,
    // similar functionality would now be per-packet or need careful redefinition.
	
    // Run methods will orchestrate packet execution
	int *RunCPU(int *systemInputValues, int repeats = -1); // Repeats could be global or per-packet
	// void RunOnceCPU(int *); // This logic will be part of the new RunCPU

    // Randomisation will apply to all contained packets
	void RandomiseNetwork(int chance1, int chance2, int chance3); // Renamed for clarity
	void EzRandomiseNetwork(); // Renamed for clarity
};


#endif