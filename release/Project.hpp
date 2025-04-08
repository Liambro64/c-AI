#pragma once
#ifndef PROJECT_HPP
# define PROJECT_HPP
# include <stdlib.h>
# include <stdio.h>
# include <iostream>
# include <vector>
# include <time.h>
# include <math.h>
# include <string.h>
# include <stdarg.h>
# include <memory>

# include "Includes/Neuron.hpp"
# include "Includes/Synapse.hpp"
# include "Includes/NeuralNetwork.hpp"
# define uniqueptr std::unique_ptr
# define sharedptr std::shared_ptr
# define rn std::chrono::high_resolution_clock::now

bool tryFree(void *ptr);

#endif