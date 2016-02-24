#ifndef RANDOMIZE_H
#define RANDOMIZE_H	

/******************************************************************************
Program:	Artificial Neural Networks - Wildfire Prediction
Authors:	Stephanie Athow, Marcus Haberling
Class:		CSC 447/547
Instructor:	Dr. John weiss
Due Date:	Feb 23, 2016
Description:
	Holds function declarations for randomizing
******************************************************************************/
#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <list>

using namespace std;


//vector<int> create_order( int number_of_records );

// Randomizes order of a vector of vectors, but not the contents of the second
// vector 
void randomize( vector< vector<double> > & data );

#endif
