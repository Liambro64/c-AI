// FOR TESTING
#include "../Project.hpp"

#include <chrono>

bool debug = false;
bool DEBUG()
{
    // if true prints stages of creation/deletion
    // otherwise doesnt, is a variable so I can change
    // it at runtime.
    return debug;
}

const void print(char *format, ...)
{
    if (debug)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}
void InitNetwork(NeuralNetwork *net)
{

    int ins = 50;
    int packets = 3;
    std::vector<PacketConfig> pkt_configs;
    std::vector<std::string> Ids = {"packet1", "packet2", "packet3"};
    for (int i = 0; i < packets; ++i)
    {
        PacketConfig pkt_config;
        pkt_config.id = Ids[i];
        pkt_config.inputs = 20;
        pkt_config.neurons = 50;
        pkt_config.outputs = 10;
        pkt_config.commOutputs = 0;
        pkt_configs.push_back(pkt_config);
    }
    int outs = 10;
    NetworkConfig config;
    config.systemInputs = ins;
    config.systemOutputs = outs;
    config.packetConfigs = pkt_configs;
    config.globalRepeats = 1;
    config.globalRandRange = 50;
    config.globalRandChance = 100;
    config.globalChance = 100;
    net->Init(config);
}
void RunNetwork(NeuralNetwork *net, int ins)
{
    std::vector<int> inputs;
    inputs.resize(ins);
    for (int i = 0; i < ins; i++)
    {
        inputs[i] = rand() % 100;
    }
    auto out = net->Run(inputs.data());
    printf("Outputs: ");
    for (int i = 0; i < out.size(); i++)
    {
        printf("%i ", out[i]);
    }
    printf("\n");
}
void RunNetworks(std::vector<NeuralNetwork> *nets, int ins)
{
    std::vector<std::thread> threads;
    std::vector<int> inputs;
    inputs.resize(ins);
    for (int i = 0; i < ins; i++)
    {
        inputs[i] = rand() % 100;
    }
    for (int i = 0; i < nets->size(); i++)
    {
        threads.push_back(std::thread([i, &nets, inputs]()
                                      {
        auto out = (*nets)[i].Run((int *)inputs.data());
        // printf("Network %i Outputs: ", i);
        // for (int j = 0; j < out.size(); j++)
        // {
        //     printf("%i ", out[j]);
        // } 
        // printf("\n");
     }));
    }
    for (int i = 0; i < threads.size(); i++)
    {
        if (threads[i].joinable())
            threads[i].join();
    }
}
std::vector<NeuralNetwork> CreateNetworks(int networks)
{
    std::vector<std::thread> threads;
    std::vector<NeuralNetwork> nets;
    nets.resize(networks, NeuralNetwork());
    for (int i = 0; i < networks; i++)
    {
        int ins = 50;
        int packets = 3;
        std::vector<PacketConfig> pkt_configs;
        std::vector<std::string> Ids = {"packet1", "packet2", "packet3"};
        for (int i = 0; i < packets; ++i)
        {
            PacketConfig pkt_config;
            pkt_config.id = Ids[i];
            pkt_config.inputs = 20;
            pkt_config.neurons = 50;
            pkt_config.outputs = 10;
            pkt_config.commOutputs = 0;
            pkt_configs.push_back(pkt_config);
        }
        int outs = 10;
        NetworkConfig config;
        config.systemInputs = ins;
        config.systemOutputs = outs;
        config.packetConfigs = pkt_configs;
        config.globalRepeats = 4;
        config.globalRandRange = 50;
        config.globalRandChance = 100;
        config.globalChance = 100;
        threads.push_back(std::thread([i, &nets, config]()
                                      { nets[i].Init(config); }));
    }
    for (int i = 0; i < threads.size(); i++)
    {
        if (threads[i].joinable())
            threads[i].join();
    }
    return nets;
};
void CreateAndRunNetwork()
{

    int ins = 50;
    int packets = 3;
    std::vector<PacketConfig> pkt_configs;
    std::vector<std::string> Ids = {"packet1", "packet2", "packet3"};
    for (int i = 0; i < packets; ++i)
    {
        PacketConfig pkt_config;
        pkt_config.id = Ids[i];
        pkt_config.inputs = 20;
        pkt_config.neurons = 50;
        pkt_config.outputs = 10;
        pkt_config.commOutputs = 0;
        pkt_configs.push_back(pkt_config);
    }
    int outs = 10;
    NetworkConfig config;
    config.systemInputs = ins;
    config.systemOutputs = outs;
    config.packetConfigs = pkt_configs;
    config.globalRepeats = 4;
    config.globalRandRange = 50;
    config.globalRandChance = 100;
    config.globalChance = 100;
    NeuralNetwork *net = new NeuralNetwork();
    net->Init(config);
    std::vector<int> inputs;
    inputs.resize(ins);
    for (int i = 0; i < ins; i++)
    {
        inputs[i] = rand() % 100;
    }
    // auto out = net->Run(inputs.data());
    // printf("Outputs: ");
    // for (int i = 0; i < out.size(); i++)
    // {
    //     printf("%i ", out[i]);
    // }
    // printf("\n");
}

void CurrentLiamTest()
{
    int networks = 100;
    printf("Starting Building Networks\n");

    std::vector<std::thread> threads;
    threads.reserve(networks);
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    CreateAndRunNetwork();
    std::chrono::duration<double> elapsed_seconds = std::chrono::high_resolution_clock::now() - start;
    printf("Created and Ran Networks in %f seconds\n", elapsed_seconds.count());
}

int main(int argc, char **argv)
{
    // change debug value, only used for print statments
    debug = true;
    bool waitForEnd = false;
    // initialise random
    srand(time(NULL));
    CurrentLiamTest();
    printf("Done!%s\n", waitForEnd ? " Press enter to end." : "");
    if (waitForEnd)
        getchar();
    return 0;
}
