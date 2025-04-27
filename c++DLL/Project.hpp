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
# define uniqueptr unique_ptr
# define rn std::chrono::high_resolution_clock::now
# define e extern
# define c "C"
# ifdef _WIN32
#  define dll __declspec(dllexport)
# else
#  define dll __attribute__((visibility("default")))
# endif
# define fdll e c dll
bool tryFree(void *ptr);
const void print(char *format, ...);

#endif