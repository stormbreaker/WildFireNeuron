/******************************************************************************
Program:     Artifical Neural Networks - Wildfire Prediction
Authors:     Stephanie Athow, Ben Kaiser, Marcus 
Class:       CSC 447/547 (Artificial Inelligence) 
Instructor:  Dr. John Weiss
Due Date:    Feb 21, 2016
Description: 

Date                Comment
----                ------------------------------
Feb 2, 2016         Started file structure, file parsing
Feb 3, 2016			Finished file parsing, untested
Feb 5, 2016			Finished network training code
Feb 6, 2016			Finished code for neural network file I/O
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>

#include "Parse_Files.h"
#include "Neuron_Layer.h"

using namespace std;

int main(int argc, char *argv[])
{
	char *param_file;					// Parameter file passed in from args

	char *data_file;	// TEMPORARY FOR TESTING

	Parameters param_vals;				// Values from parameter file

	vector<vector<float> > all_data;	// Vector of vectors, contain PDSI data

	// check for correct number of arguments
	if ( argc !=2 )
	{
		printf( "Incorrect usage. Program expects one file name. Exiting. \n" );
		return 1;
	}
	
	// get name of parameter file
	else
	{
		param_file = argv[1];
		// TEMPORARY FOR TESTING
		data_file = argv[1];
	}

	// parse parameter file
//	parse_param( param_file, param_vals );	

	// parse PDSI csv file
	parse_csv( data_file, all_data );

	/*
	// check correct reading of PDSI info
	for ( vector<int> &vec : all_data )
	{
		for( int x : vec )
			cout << x << ' ' << endl;
	}
*/

	return 0;
}
