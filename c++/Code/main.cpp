//FOR TESTING
# include "../Project.hpp"
int main(int argc, char **argv) {
    NeuralNetwork *network = new NeuralNetwork(16, 512, 6);
    printf("Neural Network made");
    delete network;
    return 0;
}