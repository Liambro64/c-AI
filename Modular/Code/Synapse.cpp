#include "../Project.hpp"

//the synapse is a pretty basic implementation,
//and I feel shouldnt take much explaining.
Synapse::Synapse() {
	strength = 0;
	from = 0;
	to = 0;
}
Synapse::Synapse(int randRange, Neuron *frm, Neuron *too) {
	strength = (rand() % (randRange *2)) - randRange;
	from = frm;
	to = too;
}
Synapse::Synapse(Neuron *frm, Neuron *too, int str) {
	strength = str;
	from = frm;
	to = too;
}
void Synapse::Fire() {
	to->add(strength);
}

void Synapse::Randomise(int chance, int range) {
	if ((rand() % chance) == 1)
		strength += (rand() % (range * 2)) - range;
}
fdll int GetSynapseStrength(Synapse *s) {
	if (s != nullptr)
		return s->getStrength();
	return 0;
}
fdll Neuron *GetSynapseFrom(Synapse *s) {
	if (s != nullptr)
		return s->getFrom();
	return nullptr;
}
fdll Neuron *GetSynapseTo(Synapse *s) {
	if (s != nullptr)
		return s->getTo();
	return nullptr;
}