#include "Neuron_Layer.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

/******************************************************************************
Method: Neuron_Layer

Author: Marcus Haberling

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

Author: Marcus Haberling

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
Method: Save_network

Author: Marcus Haberling

Description: Save all the weights of the network in a human readable file. 
             If everything goes OK the function returns true. If the ofstream
             is not open or not good, or the network is empty( having only one
             layer ) it returns false.

Parameters: network_out - ofstream to write the network to.
******************************************************************************/
bool Neuron_Layer::Save_network(ofstream& network_out)
{
    if (Is_head() && Is_tail())
    {
        return false;
    }
    if (Is_head())
    {
        return next_layer->Save_network(network_out); 
    }
    if (!network_out.good() || !network_out.is_open())
    {
        return false;
    }

    network_out << input_weights.size() << " " << input_weights.at(0).size() << endl;
    for (unsigned int x = 0; x < input_weights.size(); x++)
    {
        for (unsigned int y = 0; y < input_weights.at(0).size(); y++)
            network_out << input_weights.at(x).at(y) << " ";
        network_out << endl;
    }
    network_out << endl;
    if (Is_tail())
    {
        return true;
    }
    else 
    {
        return next_layer->Save_network(network_out);
    }

}

/******************************************************************************
Method: Load_network

Author: Marcus Haberling

Description: Loads all of the weights from a human readable file. This should
             be used fith files that were created with the Save_network 
             command. Returns false if it encounters an error with the file
             stream, but it does not check for errors inside the files 
             formatting. If it runs out of values prematurally, it will throw
             an exception.

Parameters: network_in - ifstream to load network from.
******************************************************************************/
bool Neuron_Layer::Load_network(ifstream &netowork_in)
{
    if (Is_head())
    {
        if (!Is_tail())//clear network
            next_layer -> ~Neuron_Layer();
        if (!netowork_in.good() || !netowork_in.is_open())
            return false;

        int next_size = 0;
        int this_size = 0;
        netowork_in >> next_size >> this_size;
        results.resize(this_size);
        Attach(new Neuron_Layer(next_size));
        return next_layer->Load_network(netowork_in);
    }

    if (!netowork_in.good() || !netowork_in.is_open())
        return false;
    
    input_weights.resize(num_nodes);
    for (int x = 0; x < num_nodes; x++)
    {
        input_weights.at(x).resize(prev_layer->num_nodes);
    }
    double in_weight = 0;
    for (int x = 0; x < num_nodes; x++)
    {
        for (int y = 0; y < prev_layer->num_nodes; y++)
        {
            netowork_in >> in_weight;
            input_weights.at(x).at(y) = in_weight;
        }
    }
    int next_size = 0;
    int this_size = 0;
    if (netowork_in >> next_size)
    {
        netowork_in >> this_size;
        Attach(new Neuron_Layer(next_size));
        return next_layer->Load_network(netowork_in);
    }
    return true;
}
/******************************************************************************
Method: Num_nodes

Author: Marcus Haberling

Description: Returns the number of nodes in the layer.
******************************************************************************/
int Neuron_Layer::Num_nodes()
{
    return num_nodes;
}

/******************************************************************************
Method: Is_head

Author: Marcus Haberling

Description: Returns true if this is the heal layer in the neural net.
******************************************************************************/
bool Neuron_Layer::Is_head()
{
    return (prev_layer == NULL);
}

/******************************************************************************
Method: Is_tail

Author: Marcus Haberling

Description: Returns true if this is the tail layer in the neural net.
******************************************************************************/
bool Neuron_Layer::Is_tail()
{
    return (next_layer == NULL);
}

/******************************************************************************
Method: Run

Author: Marcus Haberling

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
        double sum = 0;
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

Author: Marcus Haberling

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
        double sum = 0;
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

Author: Marcus Haberling

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
            to_add.clear();
            to_add.resize(num_nodes);
            for( unsigned int y = 0; y < to_add.size(); y++ )
            {
                to_add.at(y) = (rand()%1000)/5000.0;
            }
            next_layer -> input_weights.push_back(to_add);
        }
    }
    return;

}

/******************************************************************************
Method: Learn

Author: Marcus Haberling

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

/******************************************************************************
Method: Log_sigmoid

Author: Marcus Haberling

Description: The log_sigmoid function to use as an activation function

Paramaters: t - the sum of results*weights to use in the activation function.
******************************************************************************/
double Neuron_Layer::Log_sigmoid(double t)
{
    return 1 / (1 + exp(-1 * t));
}
