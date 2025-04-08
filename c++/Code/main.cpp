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

void MultipleTimeTest(int ins, int middles, int outs, int amount)
{

    auto start = std::chrono::high_resolution_clock::now();
    std::unique_ptr<NeuralNetwork[]> networks(new NeuralNetwork[amount]);
    for (int i = 0; i < amount; i++)
        networks[i].Init(ins, middles, outs);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    printf("Made %i Neural Network%s in %f seconds\n", amount, amount > 1 ? "s" : "", diff);
    printf("Average time per network: %f\n", diff / amount);
    networks.reset();
}

void SingleBigTimeTest()
{
    auto start = std::chrono::high_resolution_clock::now();
    std::unique_ptr<NeuralNetwork> network(new NeuralNetwork());
    network.get()->Init(1000, 10000, 1000);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    printf("Made 1 Neural Network of size (1000, 10000, 1000) in %f seconds\n", diff);
    network.reset();
}

void TimeTest(int ins, int mids, int outs)
{

    auto start = std::chrono::high_resolution_clock::now();
    std::unique_ptr<NeuralNetwork> network(new NeuralNetwork);
    network->Init(ins, mids, outs);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    printf("Made network of size %i %i %i in %f seconds\n", ins, mids, outs, diff);
    network.reset();
}

void RunTest(int ins, int mids, int outs, int *inputs, int repeats = 3)
{
    std::unique_ptr<NeuralNetwork> network(new NeuralNetwork);
    int *outputs;
    if (debug)
    {
        auto start = std::chrono::high_resolution_clock::now();
        network->Init(ins, mids, outs);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        print("Made network of size %i %i %i in %f seconds\n", ins, mids, outs, diff);
        start = std::chrono::high_resolution_clock::now();
        outputs = network->RunCPU(inputs, repeats);
        end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        print("Ran network in %f seconds\n", diff);
    }
    else
    {
        network->Init(ins, mids, outs);
        outputs = network->RunCPU(inputs);
    }
    print("Outputs: ");
    for (int i = 0; i < outs; i++)
        print("%i%s", outputs[i], (i != outs - 1) ? ", " : "\n");
    free(outputs);
    network.reset();
}

void CurrentLiamTest()
{

    // test time for making 1 networks of 5, 25, 5
    int inSize = 5;
    int outSize = 5;
    int midSize = inSize * outSize;

    int max = 25;

    int inputs[inSize];
    for (int i = 0; i < inSize; i++)
        inputs[i] = (rand() % (max * 2)) - max;

    //TimeTest(inSize, midSize, outSize);
    RunTest(inSize, midSize, outSize, inputs, 3000);
    // just look at the name and the function lol
    // SingleBigTimeTest();
}

int main(int argc, char **argv)
{
    // change debug value, only used for print statments
    debug = true;
    bool waitForEnd = false;
    // initialise random
    srand(5);
    CurrentLiamTest();
    printf("Done!%s\n", waitForEnd ? " Press enter to end." : "");
    if (waitForEnd)
        getchar();
    return 0;
}
