#pragma once
#ifndef NEURALNETWORK_HPP
# define NEURALNETWORK_HPP
# include "../Project.hpp"
# include "Packet.hpp" 
# include <vector>
# include <string>
# include <map>
# include <utility> // For std::pair

// Configuration structures for defining the network topology
struct PacketConfig {
    std::string id;
    int inputs;
    int neurons;
    int outputs;
    int commOutputs;
    // Add other packet-specific parameters here if needed (e.g., repeats, randRange, etc.)
    int repeats = 4;
    int randRange = 5;
    int randChance = 100;
    int SynChance = 100;
};

struct InterPacketConnectionConfig {
    std::string sourcePacketId;
    std::string targetPacketId;
    int numSynapses; // Number of synapses from source's CommOutputs to target's Inputs/Neurons
    // Optional: Specify target layer (0 for Inputs, 1 for Neurons, or a ratio)
    // For simplicity, let's assume target is always Neurons for now, or a mix.
    // Let's add a target layer type: 0=Inputs, 1=Neurons
    int targetLayerType = 1; // Default to connecting to the middle (Neurons) layer
};

struct NetworkConfig {
    int systemInputs;
    int systemOutputs;
    std::vector<PacketConfig> packetConfigs;
    std::vector<InterPacketConnectionConfig> interPacketConnections;
    // Add global network parameters here if they override packet defaults
    int globalRepeats = 4; // Default repeats for packets if not specified in PacketConfig
    int globalRandRange = 5;
    int globalRandChance = 100;
    int globalChance = 100;
};


class NeuralNetwork {
private:
    std::vector<Packet> packets;
	std::vector<Neuron> Inputs;
	std::vector<Neuron> Outputs;
    // We might need mappings for system I/O to specific packet I/O later

public:
    // Removed global randRange, randChance, midRepeats, Chance as they are now in NetworkConfig
	// -- reply --
	// I think the network should retain these parameters for synapse creation between packets, inputs->packets and packets->outputs.
    int inputs			{}; // Total system inputs
	int packetCount		{}; // Total packets in the network
	int outputs			{}; // Total system outputs
	int midRepeats		{}; // Total repeats for the network
	int randRange		{}; // Global random range for synapse strength
	int randChance		{}; // Global random chance for synapse creation
	int Chance			{}; // Global chance for synapse creation

	// Constructor


	NeuralNetwork();
    // Constructor now takes NetworkConfig
	NeuralNetwork(const NetworkConfig& config); 
	NeuralNetwork(NeuralNetwork *network); // For cloning
	void CreateNetwork(const NetworkConfig& config);
	~NeuralNetwork();
	NeuralNetwork *Clone();
	void CloneTo(NeuralNetwork *);
    bool operator==(const NeuralNetwork& other);

    // Methods to add and get packets
    void AddPacket(Packet *packet);
    Packet* GetPacket(const std::string& packetId);
    const std::vector<Packet>& GetAllPackets() const { return packets; }

    // Init now takes NetworkConfig and builds the network
	void Init(const NetworkConfig& config);
    // New method to create synapses between packets based on config
    void CreateInterPacketSynapses(const NetworkConfig& config);

	void SRand(); 

    // Run methods will orchestrate packet execution
	std::vector<int> Run(int *systemInputValues); 

    // Randomisation will apply to all contained packets
	void RandomiseNetwork(); 
};


#endif