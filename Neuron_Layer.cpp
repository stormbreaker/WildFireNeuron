#include "Neuron_Layer.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

/******************************************************************************
Method: Neuron_Layer

Description: Creates a new layer of a neural network with a supplied number of
			 nuerons. It initializes the internal vectors for storring results
			 and delta values.

Parameters: nodes - Amount of nodes (or neurons) that the network layer will 
                    have.
******************************************************************************/
Neuron_Layer::Neuron_Layer(int nodes)
{
    prev_layer = NULL;
    next_layer = NULL;
    num_nodes = nodes;
    results.resize(num_nodes);
	delta.resize(num_nodes);
}

/******************************************************************************
Method: ~Neuron_Layer

Description: Delalocates the current Neuron Layer. It will recursively delete
			 each following layer in the network. As a result, only one layer
			 of a network should be stored on the stack or errors will occur
			 during dealocation.
******************************************************************************/
Neuron_Layer::~Neuron_Layer()
{
    results.clear();
    input_weights.clear();
    if(next_layer != NULL )
    {
        next_layer -> ~Neuron_Layer();
    }
}

/******************************************************************************
Method: ~Neuron_Layer

Description: Returns the number of nodes in the layer.
******************************************************************************/
int Neuron_Layer::Num_nodes()
{
    return num_nodes;
}

/******************************************************************************
Method: Is_head

Description: Returns true if this is the heal layer in the neural net.
******************************************************************************/
bool Neuron_Layer::Is_head()
{
    return (prev_layer == NULL);
}

/******************************************************************************
Method: Is_tail

Description: Returns true if this is the tail layer in the neural net.
******************************************************************************/
bool Neuron_Layer::Is_tail()
{
    return (next_layer == NULL);
}

/******************************************************************************
Method: Run

Description: Runs a set of inputs through the network. It recursively calls
			 the Run meathod of the next layer after completion. This function
			 only behaves correctly when called from the Head layer of the
			 network.

Paramaters: input - vector of values to be processed through the network. Input
					is not checked to be the same size as the number of nodes
					in the head layer of the network, but needs to be fo the
					program to work correctly
******************************************************************************/
vector<double> Neuron_Layer::Run(vector<double> &input)
{
	if (Is_head())
	{
		results = input;
		if (Is_tail())
			return results;
		else
			return next_layer->Run(input);
	}
	for (int x = 0; x < num_nodes; x++)
	{
		int sum = 0;
		for (int y = 0; y < prev_layer->num_nodes; y++)
		{
			sum += prev_layer->results.at(y) * input_weights.at(x).at(y);
		}
		results.at(x) = Log_sigmoid(sum);
	}
	if (Is_tail())
	{
		return results;
	}
	return next_layer->Run(input);
}

/******************************************************************************
Method: Run_and_Condition

Description: Works exactly kile the Run meathod. The only difference being it
			 runs the learn meathod after the network is finished running. 

Paramaters: input - vector of values to be processed through the network. Input
    		        is not checked to be the same size as the number of nodes
			        in the head layer of the network, but needs to be fo the
			        program to work correctly

			expected - vector of desired values used for training. This
			           needs to be the same as the number of nodes in the tail
					   layer.
******************************************************************************/
vector<double> Neuron_Layer::Run_and_Condition(vector<double> &input, vector<double> &expected)
{
	if (Is_head())
	{
		results = input;
		if (Is_tail())
			return results;
		else
			return next_layer->Run_and_Condition(input, expected);
	}
	for (int x = 0; x < num_nodes; x++)
	{
		int sum = 0;
		for (int y = 0; y < prev_layer->num_nodes; y++)
		{
			sum += prev_layer->results.at(y) * input_weights.at(x).at(y);
		}
		results.at(x) = Log_sigmoid(sum);
	}
	if (Is_tail())
	{
		Learn(expected);
		return results;
	}
	return next_layer->Run_and_Condition(input, expected);
}

/******************************************************************************
Method: Attach

Description: Attahces a layer to the end of the network. This function will
			 also initializes the vectors of weights to match the number of
			 nodes in the row before it. The new layer will always be the tail
			 layer of the network.

Paramaters: new_layer - pointer to a new network layer to attach to the net.
******************************************************************************/
void Neuron_Layer::Attach( Neuron_Layer* new_layer )
{
    if(!Is_tail())
    {
        next_layer -> Attach(new_layer);
    }
    else
    {
        next_layer = new_layer;
        next_layer -> prev_layer = this;
        next_layer -> next_layer = NULL;
        next_layer -> input_weights.clear();
		vector<double> to_add;
        for( int x = 0; x < next_layer->num_nodes; x++ )
        {
			cout << "Adding Neuron: ";
            to_add.clear();
            to_add.resize(num_nodes);
			for( int y = 0; y < to_add.size(); y++ )
			{
				to_add.at(y) = (rand()%1000)/5000.0;
				cout << to_add.at(y) << " ";
			}
			cout << endl;
            next_layer -> input_weights.push_back(to_add);
        }
        cout << "Done adding neurons!!" << endl;
    }
    return;

}

/******************************************************************************
Method: Learn

Description: Trains the network using expected values. After the layer is
			 trained it recursively calls learn on pervious layer of the 
			 network and continues this until the head layer is reached. 

Paramaters: expected - expected output values for the network.
******************************************************************************/
void Neuron_Layer::Learn(vector<double> &expected)
{
    if(Is_head()) //The head layer learns nothing because it only stores input
    {
        return;
    }
    if(Is_tail()) // The tail learns off the expected value
	{
		for (int x = 0; x < num_nodes; x++)
		{
			double curr_delta = results.at(x)*(1 - results.at(x))*(expected.at(x) - results.at(x));
			delta.at(x) = curr_delta;
			for (int y = 0; y < prev_layer->num_nodes; y++)
			{
				input_weights.at(x).at(y) = input_weights.at(x).at(y) + prev_layer->results.at(y)*(1)*curr_delta;
			}
		}
    }
	else 
	{
		for (int x = 0; x < num_nodes; x++)
		{
			double next_delta = 0;
			for (int y = 0; y < next_layer -> num_nodes; y++)
			{ 
				next_delta += next_layer->input_weights.at(y).at(x) * next_layer->delta.at(y);
			}
			double curr_delta = results.at(x)*(1 - results.at(x))*next_delta;
			delta.at(x) = curr_delta;
			for (int y = 0; y < prev_layer->num_nodes; y++)
			{
				input_weights.at(x).at(y) = input_weights.at(x).at(y) + prev_layer->results.at(y)*(1)*curr_delta;
			}
		}
	}
	prev_layer->Learn(expected);
	return;
}

double Neuron_Layer::Log_sigmoid(double t)
{
	return 1 / (1 + exp(-1 * t));
}
