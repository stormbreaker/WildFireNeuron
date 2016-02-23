#include "Layer.h"

#include "Weight.h"
#include "Perceptron.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

Layer::Layer(int nodes)
{
    prev_layer = NULL;
    next_layer = NULL;
    num_nodes = nodes;
    perceptrons.resize(num_nodes);
}

Layer::~Layer()
{
    results.clear();
    input_weights.clear();
    if(next_layer != NULL )
    {
        next_layer -> ~Layer();
    }
}

int Layer::Num_nodes()
{
    return num_nodes;
}

bool Layer::Is_head()
{
    return (prev_layer == NULL);
}

bool Layer::Is_tail()
{
    return (next_layer == NULL);
}

void Layer::Attach( Layer* new_layer )
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
        for( int x = 0; x < num_nodes; x++ )
        {
            cout << "Adding Neuron: " << x << endl;
            new_weights.clear();    
            new_weights.resize(num_nodes);
            next_layer -> input_weights.push_back(newWeights);
        }
        cout << "Done adding neurons!!" << endl;
    }
    return;

}

double Layer::Activate(vector<double> values, vector<Weight> weights)
{
    double weighted_sum = 0;
    for( int x = 0; x < values.size(); x++ )
    {
        weighted_sum += values.at(x) * weights.at(x).Curr();
    }
    return (1/(1+ exp(-1 *weighted_sum)))/2;
}

void Layer::Learn(vector<double> expected)
{
    if(Is_head())
    {
        return;
    }
    if(Is_tail())
    {
        vector<double> error;
        error.resize(num_nodes);
        for( int x = 0; x < num_nodes; x++ )
        {
            for( int y = 0; y <  prev_layer -> num_nodes; y++)
            {
                double prev = input_weights.at(x).at(y).Curr()
                double curr = prev + learning
            }
        }
    }
}



