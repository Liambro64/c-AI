//FOR TESTING
# include "../Project.hpp"

#include <chrono>

bool debug = false;
bool DEBUG() {
    //if true prints stages of creation/deletion
    //otherwise doesnt, is a variable so I can change
    //it at runtime.
    return debug;
}

void TimeTest(int ins, int middles, int outs, int amount) {

    auto start = std::chrono::high_resolution_clock::now();
    std::unique_ptr<NeuralNetwork[]> networks(new NeuralNetwork[amount]);
    for (int i = 0; i < amount; i++)
        networks[i].Init(ins, middles, outs);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    printf("Made %i Neural Network%s in %f seconds\n", amount, amount > 1 ? "s" : "", diff);
    printf("Average time per network: %f\n", diff/amount);
    networks.reset();
}

void SingleBigTimeTest(int in, int mid, int out) {
    auto start = std::chrono::high_resolution_clock::now();
    std::unique_ptr<NeuralNetwork> network(new NeuralNetwork(in, mid, out));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    printf("Made 1 Neural Network of size (1000, 10000, 1000) in %f seconds\n", diff);
    network.reset();
}

int main(int argc, char **argv) {
    //initialise random
    srand(time(0));
    debug = true;
    //test time for making 25 networks of 16, 64, 5
    //TimeTest(100, 1000, 50, 1);
    //just look at the name and the function lol
    SingleBigTimeTest(100, 1000, 100);
    return 0;
}
