#include "Weight.h"

#include <cstdlib>

using namespace std;

Weight::Weight()
{
    Randomize();
}

Weight::~Weight()
{
    return;
}

double Weight::Curr()
{
    return current_weight;
}
double Weight::Prev()
{
    return previous_weight;
}

void Weight::Update(double new_weight)
{
    previous_weight = current_weight;
    current_weight = new_weight;
    return;
}

void Weight::Randomize()
{
    current_weight = ( rand() % 1000 ) / 1000.0;
    previous_weight = current_weight;
}  
