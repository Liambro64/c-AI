#pragma once
#ifndef PACKET_HPP
# define PACKET_HPP
# include "../Project.hpp"

//my implementation
class Packet {
private:
	std::unique_ptr<Neuron[]> Inputs	{};
	std::unique_ptr<Neuron[]> Neurons	{};
	std::unique_ptr<Neuron[]> Outputs	{};
	std::unique_ptr<Neuron[]> CommOutputs {}; // Added Communication Outputs
	std::unique_ptr<bool[]>firingNeurons{}; // This might need to include CommOutputs if they can fire based on internal state
	std::string packetId; 
public:
	int randRange 		{};
	int randChance		{};
	int midRepeats		{};
	int inputs			{};
	int neurons			{};
	int outputs			{};
	int commOutputs		{}; // Size of CommOutputs layer
	int usableNeurons	{}; // May need adjustment if CommOutputs are considered "usable" in some contexts
	int outSyns			{}; // Synapses from Neurons to Outputs
	int commOutSyns		{}; // Synapses from Neurons to CommOutputs
	int Chance 			{};


	Packet();
	Packet(const std::string& id, int ins, int mid, int outs, int commOuts, int maxSyns = -1, int outSyns = 0, int commOutSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100); // Added commOuts and commOutSyns
	Packet(Packet *packet); 
	~Packet();
	Packet *Clone();
	void DestroyNeurons();

	int getRandRange() { return randRange; }
	int getChance() { return Chance; }
	int setChance(int chance) { Chance = chance; return Chance; }
	int setRandRange(int range) { randRange = range; return randRange; }
	int getRandChance() { return randChance; }
	int setRandChance(int chance) { randChance = chance; return randChance; }
	int getMidRepeats() { return midRepeats; }
	int setMidRepeats(int repeats) { midRepeats = repeats; return midRepeats; }
	int getInputs() { return inputs; }
	int setInputs(int ins) { inputs = ins; return inputs; }
	int getNeurons() { return neurons; }
	int setNeurons(int mid) { neurons = mid; return neurons; }
	int getOutputs() { return outputs; }
	int setOutputs(int out) { outputs = out; return outputs; }
	int getCommOutputsCount() { return commOutputs; } // Getter for commOutputs size
	int setCommOutputsCount(int co) { commOutputs = co; return commOutputs; } // Setter for commOutputs size
	int getUsableNeurons() { return usableNeurons; }
	int setUsableNeurons(int usable) { usableNeurons = usable; return usableNeurons; }
	int getOutSyns() { return outSyns; }
	int setOutSyns(int out) { outSyns = out; return outSyns; }
	int getCommOutSyns() { return commOutSyns; } // Getter for commOutSyns
	int setCommOutSyns(int coSyns) { commOutSyns = coSyns; return commOutSyns; } // Setter for commOutSyns

	Neuron *GetInputs() { return Inputs.get(); }
	Neuron *GetNeurons() { return Neurons.get(); }
	Neuron *GetOutputs() { return Outputs.get(); }
	Neuron *GetCommOutputs() { return CommOutputs.get(); } // Getter for CommOutputs neurons
	std::string GetId() const { return packetId; } 


	Neuron *getNeuron(int a, int b); // Layer index 'a' will need to account for CommOutputs
	int		CreateSynapses(int maxSyns = -1);
	void Init(const std::string& id, int ins, int mid, int outs, int commOuts, int maxSyns = -1, int outSyns = 0, int commOutSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100); // Added commOuts and commOutSyns
	void SRand();

	int	findNeuron(Neuron *n); // Will need to search CommOutputs
	bool *Run();
	bool *RunCPU();
	bool *RunGPU();
	bool hasNeuron(Neuron **ns, int size, Neuron *n);
	int *RunCPU(int *, int = 3);
	void RunOnceCPU(int *);
	void RunOnceGPU(int *);
	int *Run(int *);
	void getFiringNeurons();
	void getFiringNeuronsAndDecay();
	void getFiringNeuronsFromPossibleNeurons(bool *);
	std::vector<std::vector<int>> getSynStrengths();
	std::vector<int> strengthSum(std::vector<std::vector<int>>);
	int getNeuronIndex(Neuron *n); // Will need to account for CommOutputs
	bool *LayerCheck(Neuron *layer, int length);
	void printSynapses(char *filter = "none");

	void Randomise(int Chance1, int chance2, int chance3);
	void EzRandomise();
};


#endif