#include "../Project.hpp"


Synapse::Synapse(int randRange, Neuron *frm, Neuron *too) {
	strength = (rand() % (randRange *2)) - randRange;
	from = frm;
	to = too;
}
Synapse::~Synapse() {
	std::cout << "Destroyed Synapse" << std::endl;
}

void Synapse::Fire() {
	to->add(strength);
}

void Synapse::Randomise(int chance, int range) {
	if ((rand() % chance) == 1)
		strength += (rand() % (range * 2)) - range;
}