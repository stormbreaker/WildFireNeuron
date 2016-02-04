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
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>

#include "Parse_Files.cpp"

using namespace std;

int main(int argc, char argv[])
{
	ifstream PARAMS;					// Parameter file passed in from args
	ifstream PDSI;						// PDSI file read in from param file

	vector<vector<float> > all_data;	// Vector of vectors, contain PDSI data

	// parse parameter file
	parse_param( PARAMS );	

	// parse PDSI csv file
	parse_csv( PDSI, all_data );


	return 0;
}
