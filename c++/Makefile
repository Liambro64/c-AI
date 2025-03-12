gcc=g++ -g
rm=rm
srcf=Code/
objf=Objs/
objs=Neuron.o Synapse.o NeuralNetwork.o
main=main.cpp


all: $(objs)
	$(gcc) $(objf)*.o $(srcf)$(main) -o a.exe

$(objs): %.o: $(srcf)%.cpp
	$(gcc) -c $^ -o $(objf)$@

fclean:
	$(rm) ./a.out