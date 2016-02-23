#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <vector>

class Perceptron
{
  public:
    perceptron();
   ~perceptron();
    
    void Calculate();
    void Learn();
    void Learn(vector<double> & expected);
  //protected:
    
    vector<Weight> weights;
    vector<Perceptron*> inputs;
    vector<Perceptron*> outputs
    double result;
    double error; 
};

#endif
