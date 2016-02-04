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
/*
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "main.cpp"
*/

/******************************************************************************
						Function Prototypes
******************************************************************************/
void parse_csv( ifstream& PDSI, vector< vector<float> >& all_data );
void parse_param( ifstream& PARAMS );

/******************************************************************************
Function:	 Parse CSV
Author: 	 Stephanie Athow
Description: Parses a csv file to obtain year, burned acrage, and PDSI data
	for each month of that year. It stores all the data for one year in a 
	vector. Then a vector is used to store all years.
Parameters:
******************************************************************************/
void parse_csv( ifstream& PDSI, vector< vector<float> >& all_data )
{
	float data_val;							// convert from line read in to a float

	string line;							// line read in from file
	
	// Open PDSI file
	PDSI.open();

	// Check if file opened
	if( !PDSI )
	{
		printf( "Data file could not be opened. Exiting. \n" );
		return;
	}

	// Read in information, ignore non-date lines
	while( getline( PDSI, line ) )
	{
		if( ( 0 == line.compare( 0, 1, "1" ) ) || ( 0 == line.compare( 0, 1, "2") ) )
		{
			string data;
			vector <float> year_vals;

			// parse line to break it up at commas
			while( getline( line, data, ',' ) )
			{	
				data_val = stof( data );
				year_vals.push_back( data_val );
			}
			
			// add line to all known data values
			all_data.push_back( year_vals );
		}
		else 
			continue;
	}

	// Close file
	PDSI.close();
	
	return;
}

/******************************************************************************
Function:	 Parse Params
Author: 	 Stephanie Athow
Description: Parses the given parameter file
Parameters:
******************************************************************************/
void parse_param( ifstream& PARAMS )
{
	string line;

	PARAMS.open();

	// Check for file open
	if( !PARAMS )
	{
		printf( "Parameter file could not be opened. Exiting. \n" );
		return;
	}

	while( getline( PARAMS, line ) ) 
	{
		// ignore comments
		if( 0 == line.compare( 0, 1, "#" ) )
			continue;
		
		// store data
	}

	return;
}

