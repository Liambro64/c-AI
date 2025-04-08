using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

NeuralNetwork net = new(5, 25, 5);
Console.WriteLine($"Inputs: {net.inputs}");
Console.WriteLine($"Neurons: {net.neurons}");
Console.WriteLine($"Outputs: {net.outputs}");
public class NeuralNetwork
{
    private IntPtr _networkPtr;
    public int inputs {
        get { return GetInputsSize(_networkPtr); }
    }
    public int neurons {
        get { return GetNeuronsSize(_networkPtr); }
    }
    public int outputs {
        get { return GetOutputsSize(_networkPtr); }
    }
    public int usableNeurons {
        get { return GetUsableNeurons(_networkPtr); }
    }
    public int outSyns {
        get { return GetOutSyns(_networkPtr); }
    }
    public int randRange {
        get { return GetRandRange(_networkPtr); }
    }
    public int randChance {
        get { return GetRandChance(_networkPtr); }
    }
    public NeuralNetwork()
    {
        _networkPtr = CreateCleanNetwork();
    }
    public NeuralNetwork(int ins, int mid, int out_, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100)
    {
        _networkPtr = CreateNetwork(ins, mid, out_, maxSyns, outSyns, repeats, RandRange, RandChance);
    }
    public void Init(int ins, int mid, int out_, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100)
    {
        Init(_networkPtr, ins, mid, out_, maxSyns, outSyns, repeats, RandRange, RandChance);
    }
    public int[] Run(int[] input, int repeats = -1)
    {
        IntPtr resultPtr = Run(_networkPtr, input, repeats);
        int[] result = new int[outputs];
        Marshal.Copy(resultPtr, result, 0, outputs);
        return result;
    }
    public Neuron[] GetInputs()
    {
        IntPtr inputsPtr = GetInputs(_networkPtr);
        Neuron[] Inputs = new Neuron[inputs];
        for (int i = 0; i < inputs; i++) {
            Inputs[i] = new Neuron(inputsPtr + (i * Marshal.SizeOf(typeof(Neuron))));
        }
        return Inputs;
    }
    public Neuron[] GetOutputs()
    {
        IntPtr outputsPtr = GetOutputs(_networkPtr);
        Neuron[] Inputs = new Neuron[inputs];
        for (int i = 0; i < inputs; i++) {
            Inputs[i] = new Neuron(outputsPtr + (i * Marshal.SizeOf(typeof(Neuron))));
        }
        return Inputs;
    }
    public Neuron[] GetNeurons()
    {
        IntPtr neuronsPtr = GetNeurons(_networkPtr);
        Neuron[] Inputs = new Neuron[inputs];
        for (int i = 0; i < inputs; i++) {
            Inputs[i] = new Neuron(neuronsPtr + (i * Marshal.SizeOf(typeof(Neuron))));
        }
        return Inputs;
    }


    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr CreateNetwork(int ins, int mid, int out_, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr CreateCleanNetwork();

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern void DestroyNetwork(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern void Init(IntPtr net, int ins, int mid, int out_, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr Run(IntPtr net, int[] input, int repeats = -1);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr GetNeurons(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr GetInputs(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr GetOutputs(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern int GetInputsSize(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern int GetOutputsSize(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern int GetNeuronsSize(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern int GetUsableNeurons(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern int GetOutSyns(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern int GetRandRange(IntPtr net);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern int GetRandChance(IntPtr net);
}

public class Neuron {
    private IntPtr _neuronPtr;
    public int bias {
        get { return GetNeuronBias(_neuronPtr); }
    }
    public int val {
        get { return GetNeuronVal(_neuronPtr); }
    }
    public bool operator_ {
        get { return GetNeuronOperator(_neuronPtr); }
    }
    public int numSyns {
        get { return GetNeuronNumSyns(_neuronPtr); }
    }
    public Synapse[] synapses {
        get {
            IntPtr synapsesPtr = GetNeuronSynapses(_neuronPtr);
            Synapse[] synapses = new Synapse[numSyns];
            for (int i = 0; i < numSyns; i++) {
                synapses[i] = new Synapse(synapsesPtr + (i * Marshal.SizeOf(typeof(Synapse))));
            }
            return synapses;
        }
    }
    public Neuron(IntPtr neuronPtr)
    {
        _neuronPtr = neuronPtr;
    }
    public void set(IntPtr neuron) {
        _neuronPtr = neuron;
    }
    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr GetNeuronSynapses(IntPtr Neuron);
    
    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetNeuronNumSyns(IntPtr Neuron);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetNeuronBias(IntPtr neuron);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetNeuronVal(IntPtr neuron);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool GetNeuronOperator(IntPtr neuron);
}

public class Synapse {
    private IntPtr _synapsePtr;

    public Synapse(IntPtr synapsePtr)
    {
        _synapsePtr = synapsePtr;
    }
    public int strength {
        get { return GetSynapseStrength(_synapsePtr); }
    }
    public Neuron from {
        get { return new Neuron(GetSynapseFrom(_synapsePtr)); }
    }
    public Neuron to {
        get { return new Neuron(GetSynapseTo(_synapsePtr)); }
    }
    public void set(IntPtr value) {
         _synapsePtr = value;
    }

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetSynapseStrength(IntPtr synapse);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr GetSynapseFrom(IntPtr synapse);

    [DllImport("NeuralNetwork.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr GetSynapseTo(IntPtr synapse);
}
