gcc=gcc
srcf=Code/
objf=Objs/
objs=Neuron.o Synapse.o NeuralNetwork.o
main=main.cpp


all: $(objs)
	$(gcc) $(objf)*.o -c $(srcf)$(main) -o a.out
$(objs): %.o: $(srcf)%.cpp
	$(gcc) -c $^ -o $(objf)$@
