#ifndef NORMALIZATION_H
#define NORMALIZATION_H

/******************************************************************************
Program:	Artificial Neural Net - Wildfire Prediction
Author:		Stephanie Athow
Class:		CSC 447/547
Instructor: Dr. John Weiss
Due Date:	Feb 23 2016
Description:
	This contains the function prototypes for normalizing the PDSI and burned
	acre data sets.

******************************************************************************/
#include <algorithm>
#include <vector>
#include <stdio.h>

#include "Parse_Files.h"

using namespace std;

/******************************************************************************
								Function Prototypes
******************************************************************************/
// Normalize the PDSI data
void normalize_pdsi( vector< vector<double> >& data );

// Normalize the burned acre data
void normalize_burned_acres( vector< vector<double> >& data, Parameters *param_vals );

#endif
