//FOR TESTING
# include "../Project.hpp"

#include <chrono>

bool DEBUG() {
    //if true prints stages of creation/deletion
    //otherwise doesnt
    return true;
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

void SingleBigTimeTest() {
    auto start = std::chrono::high_resolution_clock::now();
    std::unique_ptr<NeuralNetwork> network(new NeuralNetwork());
    network.get()->Init(1000, 10000, 1000);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    printf("Made 1 Neural Network of size (1000, 10000, 1000) in %f seconds\n", diff);
    network.reset();
}

int main(int argc, char **argv) {
    //initialise random
    srand(time(0));
    //test time for making 25 networks of 100, 1000, 100
    TimeTest(100, 1000, 100, 25);
    //just look at the name and the function lol
    SingleBigTimeTest();
    return 0;
}
