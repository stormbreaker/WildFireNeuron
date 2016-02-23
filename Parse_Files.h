#ifndef PARSE_FILES_H
#define PARSE_FILES_H

/******************************************************************************
Program:	 Artifical Neural Networks - Wildfire Prediction
Author:		 Stephanie Athow
Class:		 CSC 447/547 (Artificial Inelligence) 
Instructor:	 Dr. John Weiss
Due Date:	 Feb 21, 2016
Description: 

Date				Comment
----				------------------------------
Feb 2, 2016			Started file,
******************************************************************************/
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <stdio.h>

using namespace std;

/******************************************************************************
									Structs
******************************************************************************/
struct Parameters{

	char data_file[100];			// training and testing data file
	char weights_file[100];		// name of ANN weight file

	int adjustable_weight_layers;	// layers of adjustable weights
	int classes;					// Number of classes for classification
	int epochs;						// Number of training epochs
	int end_month;					// End month of current year
	int pdsi_months;				// Months of PDSI data
	int threshold_low;				// below this is considered low
	int threshold_medium;			// above low and below this is medium
	int years_burned_acres;			// number of years of burned acreage

	int nodes_per_layer[ 10 ];	// How many nodes are in each layer  of ANN

	double learn_rate;			
	double momentum;
	double threshold_error;			// Threshold of ANN error - training cuttoff
									// or testing acceptance
	double norm_threshold_low;		// normalized threshold low
	double norm_threshold_med;		// normalized threshold medium

};


/******************************************************************************
							  Function Prototypes
******************************************************************************/
void parse_csv( char* pdsi_file, vector< vector<double> >& all_data );
void parse_param( char*  params_file, Parameters *param_vals );

vector<double> split_string( const string line, char delim );


#endif
