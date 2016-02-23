#ifndef	LAYER_H
#define LAYER_H

#include "Weight.h"
#include <vector>
#include <fstream>

using namespace std;

class Layer
{
  public:
    Layer(int nodes);
   ~Layer();

    int Num_nodes();
    bool Is_head();
    bool Is_tail();

    void Attach( Layer* new_layer );

    bool Save_network(ofstream network_out, int layer = 1);

    bool Load_network(ifstream network_in);

    vector<double> Run();
    vector<double> Run_and_Condition(vector<double> expected);

    
  //protected:
    static double Activate(vector<double> values, vector<Weight> weights);

    void Learn(vector<double> expected);
    int num_nodes;
    Layer * prev_layer;
    Layer * next_layer;
    vector<vector<Weight> > input_weights;
    vector<double> results;
    
};

#endif
