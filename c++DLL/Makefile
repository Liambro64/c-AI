gcc=g++
dgcc=g++ -g
mdgcc=g++ -gdwarf-2
rm=rm -rf
srcf=Code
objf=Objs
objs=Neuron.o Synapse.o NeuralNetwork.o HelpFuncs.o
dobjs=HelpFuncs.o Neuron.o Synapse.o NeuralNetwork.o
main=main.cpp
bin=Bin
name=NeuralNetwork.dll
dllBuild = null
extension = .exe
link=blah
ifeq ($(OS),Windows_NT)
    dllBuild = -DBUILD_DLL
	extension = .exe
	rm = del
	srcf=Code\\
	objf=Objs\\
	bin=Bin\\
	link=-L$(bin) -lNeuralNetwork
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		dllBuild = -fPIC
		extension = .out
		rm = rm -rf
		srcf=Code/
		objf=Objs/
		bin=Bin/
		link=$(bin)$(name)
    endif
    ifeq ($(UNAME_S),Darwin)
		dllBuild = -fPIC
		extension = .out
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
		dllBuild = -fPIC
		extension = .out
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
		dllBuild = -fPIC
		extension = .out
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
		dllBuild = -fPIC
		extension = .out
    endif
endif



dll: $(objs)
	$(gcc) -shared -o $(bin)$(name) $(addprefix $(objf),$(objs))

dllWin: winObjs
	$(gcc) -shared -o $(bin)$(name) $(addprefix $(objf),$(objs))

debugdll: $(dobjs)
	$(dgcc) -shared -o $(bin)DBG_$(name) $(addprefix $(objf),$(dobjs))

winObjs: 
	$(gcc) -c -DBUILD_DLL $(srcf)Neuron.cpp			-o $(objf)Neuron.o
	$(gcc) -c -DBUILD_DLL $(srcf)Synapse.cpp			-o $(objf)Synapse.o
	$(gcc) -c -DBUILD_DLL $(srcf)NeuralNetwork.cpp	-o $(objf)NeuralNetwork.o
	$(gcc) -c -DBUILD_DLL $(srcf)HelpFuncs.cpp		-o $(objf)HelpFuncs.o

dbgObjs: 
	$(dgcc) -c $(srcf)Neuron.cpp		-o $(objf)Neuron.o
	$(dgcc) -c $(srcf)Synapse.cpp		-o $(objf)Synapse.o
	$(dgcc) -c $(srcf)NeuralNetwork.cpp	-o $(objf)NeuralNetwork.o
	$(dgcc) -c $(srcf)HelpFuncs.cpp		-o $(objf)HelpFuncs.o

$(objs): %.o: $(srcf)%.cpp
	$(gcc) -c $(dllBuild) $^ -o $(objf)$@

test: dll
	$(gcc) -o $(bin)test$(extension) $(srcf)$(main) $(link)

runtest : test
	$(bin)test$(extension)

fclean:
	$(rm) $(objf)*.o
	$(rm) $(bin)$(name)
	$(rm) $(bin)*$(extension)