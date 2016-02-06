#ifndef Neuron_Layer_H
#define Neuron_Layer_H

#include <vector>
#include <fstream>

using namespace std;

class Neuron_Layer
{
  public:
	/*
	**Initialize layer with given node size
	*/
    Neuron_Layer(int nodes);

	/*
	**Deallocate entire network
	*/
   ~Neuron_Layer();

	/*
	**Number of nodes in this network layer
	*/
    int Num_nodes();

	/*
	**Is this the first layer in network?
	*/
    bool Is_head();

	/*
	**Is this the last layer in network?
	*/
    bool Is_tail();

	/*
	**Add a new layer to the end of the network
	*/
    void Attach(Neuron_Layer* new_layer );

	/*
	**Save network to text file
	*/
    bool Save_network(ofstream& network_out);

	/*
	**Load network from text file
	*/
    bool Load_network(ifstream& network_in);

	/*
	**Run a set of inputs through the network
	*/
    vector<double> Run(vector<double> &input);

	/*
	**Run inputs and train on expected results
	*/
    vector<double> Run_and_Condition(vector<double> &input, vector<double> &expected);

  protected:
	/*
	**Activation funtion
	*/
    static double Log_sigmoid(double t);
	
	/*
	**Learn from expected results
	*/
    void Learn(vector<double> &expected);
    
	/*
	**Number of nodes or neurons in this layer of the network.
	*/
	int num_nodes;
	
	/*
	**Pointer to last layer of the network (null if head layer)
	*/
	Neuron_Layer * prev_layer;

	/*
	**Pointer to the next layer of the network (null if tail layer)
	*/
	Neuron_Layer * next_layer;

	/*
	**Double vector of weights for this layers inputs
	*/
    vector< vector<double> > input_weights;

	/*
	**vector of results for the layers calculation
	*/
    vector<double> results;

	/*
	**vector of delta values for back propagation (learning)
	*/
	vector<double> delta;

};

#endif
