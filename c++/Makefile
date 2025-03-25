gcc=g++
dgcc=g++ -gdwarf-2
rm=del
srcf=Code/
objf=Objs/
objs=Neuron.o Synapse.o NeuralNetwork.o HelpFuncs.o
dobjs=HelpFuncs.o Neuron.o Synapse.o NeuralNetwork.o
main=main.cpp
name=a
ext=.out


all: $(objs)
	$(gcc) $(objf)*.o $(srcf)$(main) -o $(name)$(ext)

debug: dbgObjs
	$(dgcc) $(objf)*.o $(srcf)$(main) -o dbug$(ext)

dbgObjs:
	$(dgcc) -c $(srcf)Neuron.cpp		-o $(objf)Neuron.o
	$(dgcc) -c $(srcf)Synapse.cpp		-o $(objf)Synapse.o
	$(dgcc) -c $(srcf)NeuralNetwork.cpp	-o $(objf)NeuralNetwork.o
	$(dgcc) -c $(srcf)HelpFuncs.cpp		-o $(objf)HelpFuncs.o

$(objs): %.o: $(srcf)%.cpp
	$(gcc) -c $^ -o $(objf)$@

fclean:
	$(rm) a.exe
	$(rm) "Objs/*.o"