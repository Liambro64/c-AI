using System;
using System.Collections.Generic;
using System.Drawing;
using System.Runtime.InteropServices;

public class NeuralNetwork
{
    private IntPtr _networkPtr;
    public int inputs
    {
        get { return GetInputsSize(_networkPtr); }
    }
    public int neurons
    {
        get { return GetNeuronsSize(_networkPtr); }
    }
    public int outputs
    {
        get { return GetOutputsSize(_networkPtr); }
    }
    public Neuron[] Neurons {
        get => GetNeurons();
    }
    public Neuron[] Inputs {
        get => GetInputs();
    }
    public Neuron[] Outputs {
        get => GetOutputs();
    }
    ~NeuralNetwork()
    {
        DestroyNetwork(_networkPtr);
    }
    public int GetNeuronIndex(Neuron neuron)
    {
        return GetNeuronIndex(_networkPtr, neuron.ptr);
    }
    public int chance
    {
        get { return getChance(_networkPtr); }
        set { setChance(_networkPtr, value); }
    }


    public int usableNeurons
    {
        get { return GetUsableNeurons(_networkPtr); }
    }
    public int outSyns
    {
        get { return GetOutSyns(_networkPtr); }
    }
    public int randRange
    {
        get { return GetRandRange(_networkPtr); }
    }
    public int randChance
    {
        get { return GetRandChance(_networkPtr); }
    }
    public NeuralNetwork()
    {
        _networkPtr = CreateCleanNetwork();
    }
    public NeuralNetwork(int ins, int mid, int out_, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100)
    {
        _networkPtr = CreateNetwork(ins, mid, out_, maxSyns, outSyns, repeats, RandRange, RandChance, Chance);
    }
    NeuralNetwork(IntPtr networkPtr)
    {
        _networkPtr = networkPtr;
    }
    public void Init(int ins, int mid, int out_, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100)
    {
        Init(_networkPtr, ins, mid, out_, maxSyns, outSyns, repeats, RandRange, RandChance, Chance);
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
        for (int i = 0; i < inputs; i++)
        {
            Inputs[i] = new Neuron(inputsPtr + (i * Neuron.Size));
        }
        return Inputs;
    }
    public Neuron[] GetOutputs()
    {
        IntPtr outputsPtr = GetOutputs(_networkPtr);
        Neuron[] Outputs = new Neuron[outputs];
        for (int i = 0; i < outputs; i++)
        {
            Outputs[i] = new Neuron(outputsPtr + (i * Neuron.Size));
        }
        return Outputs;
    }
    public Neuron[] GetNeurons()
    {
        IntPtr neuronsPtr = GetNeurons(_networkPtr);
        Neuron[] Neurons = new Neuron[neurons];
        for (int i = 0; i < neurons; i++)
        {
            Neurons[i] = new Neuron(neuronsPtr + (i * Neuron.Size));
        }
        return Neurons;
    }
    public void srand()
    {
        sRand(_networkPtr);
    }

    public int GetIndex(Neuron to) {
        for (int i = 0; i < inputs; i++) {
            if (to.ptr == Inputs[i].ptr) {
                return i;
            }
        }
        for (int i = 0; i < neurons; i++) {
            if (to.ptr == Neurons[i].ptr) {
                return i + inputs;
            }
        }
        for (int i = 0; i < outputs; i++) {
            if (to.ptr == Outputs[i].ptr) {
                return i + inputs + neurons;
            }
        }
        return -1;
    }
    public NeuralNetwork Clone()
    {
        return new NeuralNetwork(CloneNetwork(_networkPtr));
    }
    public void Randomise()
    {
        RandomiseNetwork(_networkPtr);
    }

    [DllImport("NeuralNetwork")]
    private static extern IntPtr CreateNetwork(int ins, int mid, int out_, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100);

    [DllImport("NeuralNetwork")]
    private static extern IntPtr CreateCleanNetwork();

    [DllImport("NeuralNetwork")]
    private static extern void DestroyNetwork(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern void Init(IntPtr net, int ins, int mid, int out_, int maxSyns = 15, int outSyns = 0, int repeats = 4, int RandRange = 5, int RandChance = 100, int Chance = 100);

    [DllImport("NeuralNetwork")]
    private static extern IntPtr Run(IntPtr net, int[] input, int repeats = -1);

    [DllImport("NeuralNetwork")]
    private static extern IntPtr GetNeurons(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern IntPtr GetInputs(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern IntPtr GetOutputs(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern int GetInputsSize(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern int GetOutputsSize(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern int GetNeuronsSize(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern int GetUsableNeurons(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern int GetOutSyns(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern int GetRandRange(IntPtr net);

    [DllImport("NeuralNetwork")]
    private static extern int GetRandChance(IntPtr net);
    [DllImport("NeuralNetwork")]
    private static extern int getChance(IntPtr net);
    [DllImport("NeuralNetwork")]
    private static extern int setChance(IntPtr net, int range);

    [DllImport("NeuralNetwork")]
    private static extern int GetNeuronIndex(IntPtr net, IntPtr n);
    [DllImport("NeuralNetwork")]
    private static extern void sRand(IntPtr net);
    [DllImport("NeuralNetwork")]
    private static extern IntPtr CloneNetwork(IntPtr net);
    [DllImport("NeuralNetwork")]
    private static extern void RandomiseNetwork(IntPtr net);
}

public class Neuron
{
    private IntPtr _neuronPtr;
    public const int Size = 24;
    public IntPtr ptr {
        get { return _neuronPtr; }
    }
    public int bias
    {
        get { return GetNeuronBias(_neuronPtr); }
    }
    public int val
    {
        get { return GetNeuronVal(_neuronPtr); }
    }
    public bool operator_
    {
        get { return GetNeuronOperator(_neuronPtr); }
    }
    public int numSyns
    {
        get { return GetNeuronNumSyns(_neuronPtr); }
    }
    public Synapse[] synapses
    {
        get
        {
            IntPtr synapsesPtr = GetNeuronSynapses(_neuronPtr);
            Synapse[] synapses = new Synapse[numSyns];
            for (int i = 0; i < numSyns; i++)
            {
                synapses[i] = new Synapse(synapsesPtr + (i * Synapse.Size));
            }
            return synapses;
        }
    }
    public Neuron(IntPtr neuronPtr)
    {
        _neuronPtr = neuronPtr;
    }
    public void set(IntPtr neuron)
    {
        _neuronPtr = neuron;
    }
    [DllImport("NeuralNetwork")]
    public static extern IntPtr GetNeuronSynapses(IntPtr Neuron);

    [DllImport("NeuralNetwork")]
    public static extern int GetNeuronNumSyns(IntPtr Neuron);

    [DllImport("NeuralNetwork")]
    public static extern int GetNeuronBias(IntPtr neuron);

    [DllImport("NeuralNetwork")]
    public static extern int GetNeuronVal(IntPtr neuron);

    [DllImport("NeuralNetwork")]
    public static extern bool GetNeuronOperator(IntPtr neuron);
}

public class Synapse
{
    private IntPtr _synapsePtr;
    public const int Size = 24;
    public Synapse(IntPtr synapsePtr)
    {
        _synapsePtr = synapsePtr;
    }
    public int strength
    {
        get { return GetSynapseStrength(_synapsePtr); }
    }
    public Neuron from
    {
        get { return new Neuron(GetSynapseFrom(_synapsePtr)); }
    }
    public Neuron to
    {
        get { return new Neuron(GetSynapseTo(_synapsePtr)); }
    }
    public void set(IntPtr value)
    {
        _synapsePtr = value;
    }

    [DllImport("NeuralNetwork")]
    public static extern int GetSynapseStrength(IntPtr synapse);

    [DllImport("NeuralNetwork")]
    public static extern IntPtr GetSynapseFrom(IntPtr synapse);

    [DllImport("NeuralNetwork")]
    public static extern IntPtr GetSynapseTo(IntPtr synapse);
}
