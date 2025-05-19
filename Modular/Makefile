gcc=g++
nvcc=nvcc
dbg=-g
rm=rm -rf
srcf=Code
objf=Objs
nvcx=-Xcompiler
objs=HelperFuncs.o Neuron.o Synapse.o Packet.o NeuralNetwork.o 
dobjs=HelpFuncs.o Neuron.o Synapse.o NeuralNetwork.o 
cuFile=CUDAFuncs.cu
cuObj=CUDAFuncs.o
main=main.cpp
bin=Bin
name=NeuralNetwork.dll
dllBuild = null
extension = .exe
link=blah
dbglink=blah
copy=cp
ifeq ($(OS),Windows_NT)
    dllBuild = -fPIC
	extension = .exe
	dbg = -gdwarf-2
	rm = del
	srcf=Code\\
	objf=Objs\\
	bin=Bin\\
	copy=copy
	link=-L$(bin) -l$(name)
	dbglink=-L$(bin) -lDebug$(name)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		dllBuild = -fPIC
		extension = .out
		dbg = -g
		rm = rm -rf
		srcf=Code/
		objf=Objs/
		bin=Bin/
		copy=cp
		link=$(bin)$(name)
		dbglink=$(bin)Debug$(name)
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



dll: $(objs) CudaObj
	$(nvcc) -o $(bin)$(name) $(addprefix $(objf),$(objs)) $(objf)$(cuObj) $(nvcx) -shared

debugdll: dbgObjs
	$(nvcc) -o $(bin)Debug$(name) $(addprefix $(objf),$(objs)) $(objf)$(cuObj) $(nvcx) -shared $(nvcx) $(dbg)

dbgObjs: 
	$(nvcc) -c $(srcf)Neuron.cpp		-o $(objf)Neuron.o			$(nvcx) $(dbg) $(nvcx) $(dllBuild)
	$(nvcc) -c $(srcf)Synapse.cpp		-o $(objf)Synapse.o			$(nvcx) $(dbg) $(nvcx) $(dllBuild)
	$(nvcc) -c $(srcf)Packet.cpp		-o $(objf)Packet.o			$(nvcx) $(dbg) $(nvcx) $(dllBuild)
	$(nvcc) -c $(srcf)NeuralNetwork.cpp	-o $(objf)NeuralNetwork.o	$(nvcx) $(dbg) $(nvcx) $(dllBuild)
	$(nvcc) -c $(srcf)HelperFuncs.cpp	-o $(objf)HelperFuncs.o		$(nvcx) $(dbg) $(nvcx) $(dllBuild)
	$(nvcc) -c $(srcf)CUDAFuncs.cu		-o $(objf)CUDAFuncs.o		$(nvcx) $(dbg) $(nvcx) $(dllBuild)

$(objs): %.o: $(srcf)%.cpp
	$(nvcc) -c $^ -o $(objf)$@ $(nvcx) $(dllBuild)
CudaObj: $(srcf)$(cuFile)
	$(nvcc) -c $(srcf)$(cuFile) -o $(objf)$(cuObj) $(nvcx) $(dllBuild)
test: dll
	$(gcc) -o $(bin)test$(extension) $(srcf)$(main) $(link)
debugtest: debugdll
	$(gcc) -o $(bin)testdebug$(extension) $(srcf)$(main) $(dbg) $(dbglink) 

runtest : test
	$(bin)test$(extension)

makenpush : dll
	$(copy) $(bin)$(name) "C:\\Users\\Cooke\\Coding Projects\\UnityCameraIntegration\\C++AI Visualiser\\Assets\\Scripts\\NetworkWrapper\\NeuralNetwork.dll"

fclean:
	$(rm) $(objf)*.o
	$(rm) $(bin)$(name)
	$(rm) $(bin)*$(extension)