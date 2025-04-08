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

dll: $(objs)
	$(dgcc) -shared -o $(bin)NeuralNetwork.dll $(objf)*.o

debugdll: $(dobjs)
	$(dgcc) -shared -o $(bin)DBG_NeuralNetwork.dll $(objf)*.o

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