#ifndef Neuron_Layer_H
#define Neuron_Layer_H

#include <vector>
#include <fstream>

using namespace std;

class Neuron_Layer
{
  public:
    Neuron_Layer(int nodes);
   ~Neuron_Layer();

    int Num_nodes();
    bool Is_head();
    bool Is_tail();

    void Attach(Neuron_Layer* new_layer );

    bool Save_network(ofstream network_out, int layer = 1);

    bool Load_network(ifstream network_in);

    vector<double> Run(vector<double> &input);
    vector<double> Run_and_Condition(vector<double> &input, vector<double> &expected);

  //protected:
    static double Log_sigmoid(double t);
	
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
