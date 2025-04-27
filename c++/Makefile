gcc=g++
dgcc=g++ -g
mdgcc=g++ -gdwarf-2
rm=del
srcf=Code/
objf=Objs/
objs=Neuron.o Synapse.o NeuralNetwork.o HelpFuncs.o
dobjs=HelpFuncs.o Neuron.o Synapse.o NeuralNetwork.o
main=main.cpp
bin=Bin/

all: $(objs)
	$(gcc) $(objf)*.o $(srcf)$(main) -o $(bin)a.out

dll: $(objs)
	$(gcc) -shared -o $(bin)NeuralNetwork.dll $(objf)*.o

debug: dbgObjs
	$(dgcc) $(objf)*.o $(srcf)$(main) -o $(bin)debug.out
dbgObjs: 
	$(dgcc) -c $(srcf)Neuron.cpp		-o $(objf)Neuron.o
	$(dgcc) -c $(srcf)Synapse.cpp		-o $(objf)Synapse.o
	$(dgcc) -c $(srcf)NeuralNetwork.cpp	-o $(objf)NeuralNetwork.o
	$(dgcc) -c $(srcf)HelpFuncs.cpp		-o $(objf)HelpFuncs.o

memDebug: memdbgObjs
	$(mdgcc) $(objf)*.o $(srcf)$(main) -o $(bin)memDebug.out

memDbgObjs:
	$(mdgcc) -c $(srcf)Neuron.cpp		-o $(objf)Neuron.o
	$(mdgcc) -c $(srcf)Synapse.cpp		-o $(objf)Synapse.o
	$(mdgcc) -c $(srcf)NeuralNetwork.cpp	-o $(objf)NeuralNetwork.o
	$(mdgcc) -c $(srcf)HelpFuncs.cpp		-o $(objf)HelpFuncs.o

$(objs): %.o: $(srcf)%.cpp
	$(gcc) -c $^ -o $(objf)$@

fclean:
	$(rm) $(bin)*
	$(rm) "Objs/*.o"