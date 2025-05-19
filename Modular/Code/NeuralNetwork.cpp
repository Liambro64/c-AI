#include "../Project.hpp"
#include <numeric>
#include <algorithm>
#include <stdarg.h>
#include <iostream> // For potential error messages

NeuralNetwork::NeuralNetwork() : inputs(0), outputs(0)
{
    // Default constructor creates an empty network, Init should be called later
}

// Constructor now takes NetworkConfig
NeuralNetwork::NeuralNetwork(const NetworkConfig &config)
{
    Init(config); // Delegate initialization to Init
}

NeuralNetwork::NeuralNetwork(NeuralNetwork *network)
{
    // Deep copy constructor
    this->inputs = network->inputs;
    this->outputs = network->outputs;
    this->packetCount = network->packetCount;
    this->midRepeats = network->midRepeats;
    this->randRange = network->randRange;
    this->randChance = network->randChance;
    this->Chance = network->Chance;
    // Global params like randRange, etc. are less relevant now, packets have their own.
    // If NetworkConfig had global overrides, they'd be copied here.
    packets.resize(network->packets.size());
    Inputs.resize(network->Inputs.size());
    Outputs.resize(network->Outputs.size());
    for (int i = 0; i < network->packets.size(); i++)
    {
        network->packets[i].CloneTo(&packets[i]);
    }
    for (int i = 0; i < network->Inputs.size(); i++)
    {
        network->Inputs[i].CloneTo(&Inputs[i]);
    }
    for (int i = 0; i < network->Outputs.size(); i++)
    {
        network->Outputs[i].CloneTo(&Outputs[i]);
    }
    
    for (int i = 0; i < network->Inputs.size(); i++)
    {
        for (int j = 0; j < network->Inputs[i].getNumSyns(); j++)
        {
            Neuron *targetOriginal = network->Inputs[i].getSynapses()[j].getTo();
            int synStrength = targetOriginal->getSynapses()->getStrength();
            int Packetindex = 0;
            int targetIdxOriginal = 0;
            int targetLayer = 0; // always gonna be to the input layer of the packet
            for (int i = 0; i < network->packets.size(); i++)
            {
                if (network->packets[i].findNeuron(targetOriginal) != -1)
                {
                    Packetindex = i;
                    targetIdxOriginal = network->packets[i].findNeuron(targetOriginal);
                    break;
                }
            }

            Neuron *targetClone = packets[Packetindex].getNeuron(targetLayer, targetIdxOriginal);
            Inputs[i].MakeSynapse(synStrength, targetClone);
        }
    }
    for (int i = 0; i < packetCount; i++)
    {
        for (int j = 0; j < packets[i].getOutputs(); j++)
        {
            Packet *pkt = &packets[i];
            for (int k = 0; k < ((*pkt->GetOutputs())[j].getNumSyns()); k++)
            {
                Neuron *frm = &((*pkt->GetOutputs())[j]);
                Neuron *targetOriginal = frm->getSynapses()[k].getTo();
                int synStrength = frm->getSynapses()->getStrength();
                int targetIdxOriginal = targetOriginal - Outputs.data();
                (*packets[i].GetOutputs())[j].MakeSynapse(synStrength, &Outputs[targetIdxOriginal]);
            }
        }
    }
}

NeuralNetwork::~NeuralNetwork()
{
    // unique_ptr will handle deallocation of Packets
    packets.clear();
}

NeuralNetwork *NeuralNetwork::Clone()
{
    return new NeuralNetwork(this); // Use the copy constructor
}
void NeuralNetwork::CloneTo(NeuralNetwork *newNetwork)
{
    newNetwork->inputs = this->inputs;
    newNetwork->outputs = this->outputs;
    newNetwork->packetCount = this->packetCount;
    newNetwork->midRepeats = this->midRepeats;
    newNetwork->randRange = this->randRange;
    newNetwork->randChance = this->randChance;
    newNetwork->Chance = this->Chance;
    newNetwork->packets.resize(this->packets.size());
    for (int i = 0; i < this->packets.size(); i++)
    {
        this->packets[i].CloneTo(&newNetwork->packets[i]);
    }
    newNetwork->Inputs.clear();
    newNetwork->Outputs.clear();
    newNetwork->Inputs.resize(this->Inputs.size());
    newNetwork->Outputs.resize(this->Inputs.size());
    for (int i = 0; i < this->Inputs.size(); i++)
    {
        this->Inputs[i].CloneTo(&newNetwork->Inputs[i]);
    }
    for (int i = 0; i < this->Outputs.size(); i++)
    {
        this->Outputs[i].CloneTo(&newNetwork->Outputs[i]);
    }
    for (int i = 0; i < this->Inputs.size(); i++)
    {
        for (int j = 0; j < this->Inputs[i].getNumSyns(); j++)
        {
            Neuron *targetOriginal = this->Inputs[i].getSynapses()[j].getTo();
            int synStrength = targetOriginal->getSynapses()->getStrength();
            int Packetindex = 0;
            int targetIdxOriginal = 0;
            int targetLayer = 0; // always gonna be to the input layer of the packet
            for (int i = 0; i < this->packets.size(); i++)
            {
                if (this->packets[i].findNeuron(targetOriginal) != -1)
                {
                    Packetindex = i;
                    targetIdxOriginal = this->packets[i].findNeuron(targetOriginal);
                    break;
                }
            }

            Neuron *targetClone = newNetwork->packets[Packetindex].getNeuron(targetLayer, targetIdxOriginal);
            newNetwork->Inputs[i].MakeSynapse(synStrength, targetClone);
        }
    }
    for (int i = 0; i < packetCount; i++)
    {
        for (int j = 0; j < packets[i].getOutputs(); j++)
        {
            Packet *pkt = &packets[i];
            for (int k = 0; k < ((*pkt->GetOutputs())[j].getNumSyns()); k++)
            {
                Neuron *frm = &((*pkt->GetOutputs())[j]);
                Neuron *targetOriginal = frm->getSynapses()[k].getTo();
                int synStrength = frm->getSynapses()->getStrength();
                int targetIdxOriginal = targetOriginal - Outputs.data();
                (*newNetwork->packets[i].GetOutputs())[j].MakeSynapse(synStrength, &newNetwork->Outputs[targetIdxOriginal]);
            }
        }
    }
}
bool NeuralNetwork::operator==(const NeuralNetwork &other)
{
    if (this == &other)
        return true; // Same object

    if (inputs != other.inputs || outputs != other.outputs || packetCount != other.packetCount)
        return false;

    for (int i = 0; i < packets.size(); ++i)
    {
        if (!((Packet)packets[i]).operator==(other.packets[i])) return false;
    }
	// for (int i = 0; i < inputs; i++)
	// {
	// 	for (int j = 0; j < Inputs[i].getNumSyns(); j++)
	// 	{
	// 		int strength = Inputs[i].getSynapses()[j].getStrength();
	// 		int otherStrength = (*((Packet)other).GetInputs())[i].getSynapses()[j].getStrength();
	// 		int toIndex = getNeuronIndex(Inputs[i].getSynapses()[j].getTo());
	// 		int otherToIndex = ((Packet)other).getNeuronIndex((*((Packet)other).GetInputs())[i].getSynapses()[j].getTo());
	// 		val &= (strength == otherStrength && toIndex == otherToIndex);
	// 		if (val == false)
	// 			return false;
	// 	}
	// 	int bias = Inputs[i].getBias();
	// 	int otherBias = (*((Packet)other).GetInputs())[i].getBias();
	// 	bool oprtr = Inputs[i].getOperator();
	// 	bool otherOprtr = (*((Packet)other).GetInputs())[i].getOperator();
	// 	val &= (bias == otherBias && oprtr == otherOprtr);
	// 	if (val == false)
	// 		return false;
	// }
    return true;
}
void NeuralNetwork::AddPacket(Packet *packet)
{
    if (packet)
    {
        packets.push_back(packet);
    }
}

Packet *NeuralNetwork::GetPacket(const std::string &packetId)
{
    auto it = std::find_if(packets.begin(), packets.end(),
                           [&](Packet p)
                           {
                               return &p != nullptr && p.GetId() == packetId;
                           });
    if (it != packets.end())
    {
        return it.base(); // Return the raw pointer
    }
    return nullptr;
}

void NeuralNetwork::Init(const NetworkConfig &config)
{
    this->inputs = config.systemInputs;
    this->outputs = config.systemOutputs;
    this->packets.clear(); // Clear any existing packets
    this->randChance = config.globalRandChance;
    this->randRange = config.globalRandRange;
    this->midRepeats = config.globalRepeats;
    this->Chance = config.globalChance;
    this->packetCount = config.packetConfigs.size();
    packets.resize(packetCount);
    // 1. Create Packets based on configuration
    for (int i = 0; i < packetCount; i++)
    {
        auto pkt_config = config.packetConfigs[i];
        this->packets[i].Init(pkt_config.id,
                              pkt_config.inputs,
                              pkt_config.neurons,
                              pkt_config.outputs,
                              pkt_config.commOutputs,
                              -1,                    // maxSyns (use packet default or add to config)
                              0,                     // outSyns (use packet default or add to config)
                              0,                     // commOutSyns (use packet default or add to config)
                              pkt_config.repeats,    // Use packet-specific repeats
                              pkt_config.randRange,  // Use packet-specific randRange
                              pkt_config.randChance, // Use packet-specific randChance
                              pkt_config.SynChance   // Use packet-specific Chance
        );
    }

    // 2. Create Inter-Packet Synapses based on configuration
    CreateNetwork(config);
}

void NeuralNetwork::CreateNetwork(const NetworkConfig &config)
{
    Inputs.clear();
    Inputs.reserve(inputs);
    for (int i = 0; i < this->inputs; i++)
    {
        Inputs.push_back(Neuron(randRange));
    }
    Outputs.clear();
    Outputs.reserve(outputs);
    for (int i = 0; i < outputs; i++)
    {
        Outputs.push_back(Neuron());
    }
    int pktInCount = 0;
    std::vector<int> pktIns;
    std::vector<int> pktOuts;
    int pktOutCount = 0;
    pktIns.reserve(packets.size());
    pktOuts.reserve(packets.size());
    for (int i = 0; i < packets.size(); i++)
    {
        pktInCount += packets[i].getInputs();
        pktIns[i] = packets[i].getInputs();
        pktOutCount += packets[i].getOutputs();
        pktOuts[i] = packets[i].getOutputs();
    }
    int rt = (int)sqrtl(pktInCount);
    int rt4 = (rt / 4) + 1;
    for (int i = 0; i < inputs; i++)
    {
        std::vector<Neuron *> neurons;
        int syns = rt4 + (rand() % (rt - rt4));
        if (syns > 0)
        {
            for (int j = 0; j < syns; j++)
            {
                Neuron *n = nullptr;
                while (n == nullptr)
                {
                    int r = rand() % pktInCount;
                    int ins = packets[0].getInputs();
                    for (int k = 0; k < packets.size(); k++)
                    {
                        if (r < ins)
                        {
                            n = packets[k].getNeuron(0, r);
                            break;
                        }
                        r -= ins;
                        if (k < packets.size() - 1)
                            ins = packets[k + 1].getInputs();
                    }
                    if (std::find(neurons.begin(), neurons.end(), n) == neurons.end())
                        neurons.push_back(n);
                    else
                        n = nullptr; // Avoid duplicates
                }
            }
            Inputs[i].MakeSynapses(neurons.data(), neurons.size(), randRange);
        }
    }
    for (int i = 0; i < outputs; i++)
    {
        std::vector<Neuron *> neurons;
        int rt = (int)sqrtl(pktOutCount);
        int rt4 = (rt / 4) + 1;
        int syns = rt4 + (rand() % (rt - rt4));
        if (syns > 0)
        {
            for (int j = 0; j < syns; j++)
            {
                Neuron *n = nullptr;
                while (n == nullptr)
                {
                    int r = rand() % pktOutCount;
                    int outs = packets[0].getOutputs();
                    for (int k = 0; k < packets.size(); k++)
                    {
                        if (r < outs)
                        {
                            n = packets[k].getNeuron(2, r);
                            break;
                        }
                        r -= outs;
                        if (k < packets.size() - 1)
                            outs = packets[k + 1].getOutputs();
                    }
                    if (std::find(neurons.begin(), neurons.end(), n) == neurons.end())
                        neurons.push_back(n);
                    else
                        n = nullptr; // Avoid duplicates
                }
            }
        }
        for (int j = 0; j < neurons.size(); j++)
        {
            neurons[j]->MakeSynapse(&Outputs[i], randRange);
        }
    }
}

// UNUSED, AI ADDED IT FOR NO REASON AND I AM YET TO IMPLEMENT PROPERLY
void NeuralNetwork::CreateInterPacketSynapses(const NetworkConfig &config)
{
    // Iterate through each defined inter-packet connection
    for (const auto &conn_config : config.interPacketConnections)
    {
        Packet *sourcePacket = GetPacket(conn_config.sourcePacketId);
        Packet *targetPacket = GetPacket(conn_config.targetPacketId);

        if (!sourcePacket)
        {
            std::cerr << "Warning: Source packet '" << conn_config.sourcePacketId << "' not found for connection." << std::endl;
            continue;
        }
        if (!targetPacket)
        {
            std::cerr << "Warning: Target packet '" << conn_config.targetPacketId << "' not found for connection." << std::endl;
            continue;
        }
        if (sourcePacket->getCommOutputsCount() == 0)
        {
            std::cerr << "Warning: Source packet '" << conn_config.sourcePacketId << "' has no CommOutputs to connect from." << std::endl;
            continue;
        }

        Neuron *sourceCommOutputs = sourcePacket->GetCommOutputs()->data();
        int numSourceCommOutputs = sourcePacket->getCommOutputsCount();

        Neuron *targetLayer = nullptr;
        int numTargetNeurons = 0;

        if (conn_config.targetLayerType == 0)
        { // Target is Inputs layer
            targetLayer = targetPacket->GetInputs()->data();
            numTargetNeurons = targetPacket->getInputs();
        }
        else if (conn_config.targetLayerType == 1)
        { // Target is Neurons (middle) layer
            targetLayer = targetPacket->GetNeurons()->data();
            numTargetNeurons = targetPacket->getNeurons();
        }
        else
        {
            std::cerr << "Warning: Invalid targetLayerType (" << conn_config.targetLayerType << ") for connection from '" << conn_config.sourcePacketId << "' to '" << conn_config.targetPacketId << "'." << std::endl;
            continue;
        }

        if (numTargetNeurons == 0)
        {
            std::cerr << "Warning: Target packet '" << conn_config.targetPacketId << "' has no neurons in the specified target layer (" << conn_config.targetLayerType << ") to connect to." << std::endl;
            continue;
        }

        // Create numSynapses connections from random CommOutputs in source to random neurons in target layer
        int synapsesToCreate = conn_config.numSynapses;
        if (synapsesToCreate <= 0)
        {                                                                                     // Default number of synapses if not specified or zero
            synapsesToCreate = std::max(1, std::min(numSourceCommOutputs, numTargetNeurons)); // Connect at least 1 if possible
        }
        // Ensure we don't try to create more synapses than possible unique connections
        // (numSourceCommOutputs * numTargetNeurons)
        if (synapsesToCreate > numSourceCommOutputs * numTargetNeurons)
        {
            synapsesToCreate = numSourceCommOutputs * numTargetNeurons;
        }

        // Simple random connection strategy: Pick random source and random target for each synapse
        for (int i = 0; i < synapsesToCreate; ++i)
        {
            Neuron *sourceNeuron = &sourceCommOutputs[rand() % numSourceCommOutputs];
            Neuron *targetNeuron = &targetLayer[rand() % numTargetNeurons];

            // TODO: Add check to avoid duplicate synapses between the exact same pair of neurons
            // This requires iterating through sourceNeuron's existing synapses.

            sourceNeuron->MakeSynapse(targetNeuron, config.globalRandRange); // Use global randRange for inter-packet syns
        }
        std::cout << "Created " << synapsesToCreate << " inter-packet synapses from '" << sourcePacket->GetId() << "' (CommOutputs) to '" << targetPacket->GetId() << "' (Layer " << conn_config.targetLayerType << ")." << std::endl;
    }
}

void NeuralNetwork::SRand()
{
    srand(time(0)); // Seed globally once
}

std::vector<int> NeuralNetwork::Run(int *systemInputValues)
{

    // this should simply push the inputs through their synapses,
    // because synapses can hold any neuron anywhere.
    for (int i = 0; i < inputs; i++)
    {
        Inputs[i].set(systemInputValues[i]);
        Inputs[i].FireNow();
    }

    std::vector<std::thread> threads;
    // then it would run the packets (maybe even multiple times, each packet would keep track)
    for (int i = 0; i < packets.size(); i++)
    {
        threads.push_back(std::thread([this, i]()
                                      { packets[i].Run(); }));
    }
    // wait for all threads to finish
    for (int i = 0; i < threads.size(); i++)
    {
        std::thread *thread = &threads[i];
        if (thread->joinable())
            thread->join();
    }
    // running the packets should be done in parallel so research multi-threading (done)
    // run its own RunGPU function (later we could check for cuda support and run GPU if available)
    // and return the outputs as an array of ints.
    std::vector<int> outputValues;
    outputValues.resize(outputs);
    for (int i = 0; i < outputs; i++)
    {
        outputValues[i] = Outputs[i].getVal();
    }
    return outputValues;
}

void NeuralNetwork::RandomiseNetwork()
{
    for (int i = 0; i < inputs; i++)
    {
        Inputs[i].Randomise(randChance, randRange, false, NULL, 0);
        for (int j = 0; j < packets.size(); j++)
        {
            Inputs[i].RandomiseSynapses(Chance, randChance, randRange, packets[j].GetInputs());
        }
    }
    for (int i = 0; i < packets.size(); i++)
    {
        packets[i].Randomise(Chance);
        for (int j = 0; j < packets[i].getOutputs(); j++)
        {
            (*packets[i].GetOutputs())[j].RandomiseSynapses(Chance, randChance, randRange, &Outputs);
        }
    }
}
fdll NeuralNetwork *CreateNeuralNetwork(const NetworkConfig &config)
{
    return new NeuralNetwork(config);
}
fdll void DestroyNeuralNetwork(NeuralNetwork *network)
{
    delete network;
}
fdll void RunNeuralNetwork(NeuralNetwork *network, int *systemInputValues, int *outputValues)
{
    std::vector<int> outputs = network->Run(systemInputValues);
    std::copy(outputs.begin(), outputs.end(), outputValues);
}
fdll Packet *GetPacketFromNetwork(NeuralNetwork *network, const char *packetId)
{
    return network->GetPacket(packetId);
}
fdll Packet *GetPacketFromNetworkByIndex(NeuralNetwork *network, int index)
{
    if (index < 0 || index >= network->GetAllPackets().size())
    {
        return nullptr; // Out of bounds
    }
    return (Packet *)&network->GetAllPackets()[index];
}
fdll Packet *GetPackets(NeuralNetwork *network)
{
    return (Packet *)((network->GetAllPackets()).data());
}
fdll int GetPacketCount(NeuralNetwork *network)
{
    return network->GetAllPackets().size();
}
fdll void RandomiseNetwork(NeuralNetwork *network)
{
    network->RandomiseNetwork();
}