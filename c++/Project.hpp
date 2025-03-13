#pragma once
#ifndef PROJECT_HPP
# define PROJECT_HPP
# include <stdlib.h>
# include <stdio.h>
# include <iostream>
# include <time.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>
# include <memory>

# include "Includes/Neuron.hpp"
# include "Includes/Synapse.hpp"
# include "Includes/NeuralNetwork.hpp"

void print(char *);
bool tryFree(void *ptr);

bool DEBUG();

#endif