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


typedef vector<float> Year;			// holds values for one year
typedef vector<Year> All_Data;		// Vector of vectors, contains all PDSI data

void output_data( const All_Data& data );


int main(int argc, char *argv[])
{
	char *param_file;					// Parameter file passed in from args

//	char *data_file;	// TEMPORARY FOR TESTING

	All_Data data;
	
	Parameters param_vals;				// Values from parameter file

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
		//data_file = argv[1];
	}

	// parse parameter file
	parse_param( param_file, &param_vals );	

	// parse PDSI csv file
	cout<< "Data file name: " << param_vals.data_file << endl;
	parse_csv( param_vals.data_file, data );

	// check correct reading of PDSI info
//	output_data( data );	
	

	return 0;
}

/*
	testing file parsing for pdsi
*/
void output_data( const All_Data& data )
{
	for( All_Data::const_iterator row = data.begin(); row != data.end(); ++row )
	{
		for( Year::const_iterator col = row->begin(); col != row->end(); ++col )
			cout << *col << " ";
		cout << endl;
	}
}













