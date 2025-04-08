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
name=NeuralNetwork.dll

dll: $(objs)
	$(gcc) -shared -o $(bin)$(name) $(addprefix $(objf),$(objs))

debugdll: $(dobjs)
	$(dgcc) -shared -o $(bin)DBG_$(name) $(addprefix $(objf),$(dobjs))

dbgObjs: 
	$(dgcc) -c $(srcf)Neuron.cpp		-o $(objf)Neuron.o
	$(dgcc) -c $(srcf)Synapse.cpp		-o $(objf)Synapse.o
	$(dgcc) -c $(srcf)NeuralNetwork.cpp	-o $(objf)NeuralNetwork.o
	$(dgcc) -c $(srcf)HelpFuncs.cpp		-o $(objf)HelpFuncs.o

$(objs): %.o: $(srcf)%.cpp
	$(gcc) -c -DBUILD_DLL $^ -o $(objf)$@

test:
	$(gcc) -o $(bin)a.exe $(srcf)$(main) -L$(bin) -lNeuralNetwork -Wl,--enable-auto-import

rbtest: dll
	$(gcc) -o $(bin)a.exe $(srcf)$(main) -L$(bin) -lNeuralNetwork -Wl,--enable-auto-import

fclean:
	$(rm) a.exe
	$(rm) "Objs/*.o"
	del $(bin)*.dll