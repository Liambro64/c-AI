#include "../Project.hpp"
#include <numeric> // For std::iota if needed later
#include <algorithm> // For std::find_if
#include <stdarg.h>

NeuralNetwork::NeuralNetwork() : inputs(0), outputs(0) {
    // Initialize default values if necessary
    randRange = 5;
    randChance = 100;
    midRepeats = 4;
    Chance = 100;
}

// A more detailed constructor might take configurations for packets
NeuralNetwork::NeuralNetwork(int systemInputs, int systemOutputs, const std::vector<std::string, std::vector<int>>& packetIds)
    : inputs(systemInputs), outputs(systemOutputs) {
    randRange = 5; // Default values
    randChance = 100;
    midRepeats = 4;
    Chance = 100;
	//todo: implement packet creation based on packetIds
	// Create inter packet synapses 
}

NeuralNetwork::NeuralNetwork(NeuralNetwork *network) {
    // Deep copy constructor
    this->inputs = network->inputs;
    this->outputs = network->outputs;
    this->randRange = network->randRange;
    this->randChance = network->randChance;
    this->midRepeats = network->midRepeats;
    this->Chance = network->Chance;

    for (const auto& packet_ptr : network->packets) {
        if (packet_ptr) {
            this->packets.push_back(std::unique_ptr<Packet>(packet_ptr->Clone()));
        }
    }
}

NeuralNetwork::~NeuralNetwork() {
    // unique_ptr will handle deallocation of Packets
    packets.clear();
}

NeuralNetwork *NeuralNetwork::Clone() {
    return new NeuralNetwork(this); // Use the copy constructor
}

void NeuralNetwork::AddPacket(std::unique_ptr<Packet> packet) {
    if (packet) {
        packets.push_back(std::move(packet));
    }
}

Packet* NeuralNetwork::GetPacket(const std::string& packetId) {
    auto it = std::find_if(packets.begin(), packets.end(), 
                           [&](const std::unique_ptr<Packet>& p) {
                               return p && p->GetId() == packetId;
                           });
    if (it != packets.end()) {
        return it->get();
    }
    return nullptr;
}

void NeuralNetwork::Init(int systemInputs, int systemOutputs /*, packet configurations */) {
    this->inputs = systemInputs;
    this->outputs = systemOutputs;
    this->packets.clear(); // Clear any existing packets

    // TODO: Implement logic to create and configure packets based on system needs
    // This is highly dependent on how you want to structure the network of packets.
    // Example: Create a simple "vision" packet and a "motor" packet.
    // This requires defining how system inputs map to packet inputs and packet outputs to system outputs.

    // Placeholder:
    // auto vision_packet = std::make_unique<Packet>("vision_pkt", systemInputs, 50, 10, -1, 0, midRepeats, randRange, randChance, Chance);
    // AddPacket(std::move(vision_packet));
    // auto motor_packet = std::make_unique<Packet>("motor_pkt", 10, 30, systemOutputs, -1, 0, midRepeats, randRange, randChance, Chance);
    // AddPacket(std::move(motor_packet));
}

void NeuralNetwork::SRand() {
    srand(time(0)); // Seed globally once
}

int *NeuralNetwork::RunCPU(int *systemInputValues, int repeatsOverride) {
    if (packets.empty()) {
        // Or handle error appropriately
        return nullptr; 
    }

    int currentRepeats = (repeatsOverride == -1) ? this->midRepeats : repeatsOverride;

    // --- Stage 1: Distribute System Inputs to Packets ---
    // This is a placeholder and needs a defined mapping.
    // Example: First packet takes all system inputs.
    // More complex scenarios might involve splitting inputs or routing them based on type.
    if (!packets.empty() && packets[0]) {
        // Assuming the first packet is the primary input packet for now
        // And its input size matches systemInputs
        if (packets[0]->getInputs() > 0 && systemInputValues != nullptr) {
             // This is a simplified direct mapping.
             // A real system might need a more sophisticated input distribution.
             // For now, we'll assume the first packet can take the system inputs.
             // We are not running the packet yet, just setting its inputs if it's designed to receive them.
             // The actual running happens in a loop or defined order.
        }
    }
    
    // --- Stage 2: Run Packets (and handle inter-packet communication if designed) ---
    // For now, run packets sequentially. Inter-packet communication needs explicit design.
    // Example: Output of packet A becomes input of packet B.
    
    // This is a very simplified execution model.
    // A more robust model would define an execution graph or order for packets.
    std::map<std::string, std::vector<int>> packetOutputsTemp;

    for (const auto& packet : packets) {
        if (packet) {
            // How do we get input for this specific packet?
            // For now, let's assume systemInputValues are for the first packet,
            // and other packets might get inputs from previous packets (not implemented here yet)
            // or have no direct external input for this step.
            
            int* currentPacketInput = nullptr;
            if (packet->GetId() == "vision_pkt" && systemInputValues != nullptr && packet->getInputs() == this->inputs) { // Example condition
                currentPacketInput = systemInputValues;
            }
            // else if (packet->GetId() == "motor_pkt") {
            //    // Try to get input from vision_pkt's output
            //    if(packetOutputsTemp.count("vision_pkt")) {
            //        // This requires packetOutputsTemp["vision_pkt"] to be correctly sized for motor_pkt's input
            //        // currentPacketInput = packetOutputsTemp["vision_pkt"].data();
            //    }
            // }


            int* packetResultRaw = packet->RunCPU(currentPacketInput, currentRepeats);
            
            if (packetResultRaw) {
                std::vector<int> resultVec(packet->getOutputs());
                for(int i=0; i < packet->getOutputs(); ++i) resultVec[i] = packetResultRaw[i];
                packetOutputsTemp[packet->GetId()] = resultVec;
                free(packetResultRaw); // Free memory allocated by packet's RunCPU
            }
        }
    }

    // --- Stage 3: Gather System Outputs from Packets ---
    // This also needs a defined mapping.
    // Example: Last packet's outputs are the system outputs.
    int *systemOutputValues = (int *)calloc(this->outputs, sizeof(int));
    if (!systemOutputValues) return nullptr;

    if (!packets.empty()) {
        // Placeholder: Assume the last packet provides the system output,
        // or a specific packet is designated as the output packet.
        // Let's assume a packet named "motor_pkt" (if it exists and was run) provides output.
        Packet* outputPacket = GetPacket("motor_pkt"); // Example
        if (!outputPacket && !packets.empty()) outputPacket = packets.back().get(); // Fallback to last packet

        if (outputPacket && packetOutputsTemp.count(outputPacket->GetId())) {
             const auto& finalPacketOutputData = packetOutputsTemp[outputPacket->GetId()];
            for (int i = 0; i < this->outputs && i < finalPacketOutputData.size(); ++i) {
                systemOutputValues[i] = finalPacketOutputData[i];
            }
        }
    }
    
    return systemOutputValues;
}


void NeuralNetwork::RandomiseNetwork(int chance1, int chance2, int chance3) {
    for (const auto& packet : packets) {
        if (packet) {
            // Packets now have their own Chance, randChance, randRange.
            // We could pass these parameters, or let packet use its own.
            // For now, let packet use its own internal Randomise which uses its members.
            packet->Randomise(chance1, chance2, chance3); // Or packet->EzRandomise();
        }
    }
}

void NeuralNetwork::EzRandomiseNetwork() {
     for (const auto& packet : packets) {
        if (packet) {
            packet->EzRandomise();
        }
    }
}


// Removed old C-style interface functions that were specific to the old NeuralNetwork structure.
// New ones would be needed if this NeuralNetwork class is to be used via a C API.
// For example:
// fdll NeuralNetwork* CreateSystemNetwork(int systemInputs, int systemOutputs) {
//     return new NeuralNetwork(systemInputs, systemOutputs, {}); // Basic initialization
// }
// fdll void AddPacketToSystem(NeuralNetwork* systemNet, Packet* packet) {
//     if(systemNet && packet) {
//         // NeuralNetwork::AddPacket expects unique_ptr, so this needs careful handling
//         // systemNet->AddPacket(std::unique_ptr<Packet>(packet)); // This takes ownership, might be problematic if packet is managed elsewhere
//     }
// }
// fdll int* RunSystem(NeuralNetwork* systemNet, int* inputs) {
//     if(systemNet) return systemNet->RunCPU(inputs);
//     return nullptr;
// }
// fdll void DestroySystemNetwork(NeuralNetwork* systemNet) {
//     delete systemNet;
// }