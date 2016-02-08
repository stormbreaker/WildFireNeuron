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
#include "Parse_Files.h"

/******************************************************************************
Function:	 Parse CSV
Author: 	 Stephanie Athow
Description: Parses a csv file to obtain year, burned acrage, and PDSI data
	for each month of that year. It stores all the data for one year in a 
	vector. Then a vector is used to store all years.
Parameters:
******************************************************************************/
void parse_csv( char* pdsi_file, vector< vector<float> >& all_data )
{
	//float data_val;							// convert from line read in to a float

	string line;							// line read in from file
	
	ifstream pdsi;	

	// Open PDSI file
	pdsi.open( pdsi_file );

	// Check if file opened
	if( !pdsi )
	{
		printf( "Data file could not be opened. Exiting. \n" );
		return;
	}

	// Read in information, ignore non-date lines
	while( getline( pdsi, line ) )
	{
		if( ( 0 == line.compare( 0, 1, "1" ) ) || ( 0 == line.compare( 0, 1, "2") ) )
		{
			string data;
			vector <float> year_vals;

			// parse line to break it up at commas
			year_vals = split_string( line, ',' );
			
			/*
			while( getline( line, data, ',' ) )
			{	
				data_val = stof( data );
				year_vals.push_back( data_val );
			}
			*/

			// add line to all known data values
			all_data.push_back( year_vals );
		}
		else 
			continue;
	}

	// Close file
	pdsi.close();
	
	return;
}

/******************************************************************************
Function:	 Parse Params
Author: 	 Stephanie Athow
Description: Parses the given parameter file
Parameters:
******************************************************************************/
void parse_param( char* param_file, Parameters param_vals )
{
	string line;
	ifstream params_in;

	params_in.open( param_file );

	// Check for file open
	if( !params_in )
	{
		printf( "Parameter file could not be opened. Exiting. \n" );
		return;
	}

	while( getline( params_in, line ) ) 
	{
		// ignore comments
		if( 0 == line.compare( 0, 1, "#" ) )
			continue;
		
		// store data
	}
	
	params_in.close();
	return;
}


/******************************************************************************
Function:	 Split Strings
Author: 	 Stephanie Athow
Description: Parses the given parameter file
Parameters:
******************************************************************************/
vector<float> split_string( string line, char delim )
{
	vector<float> data_values;				// holds data values to be returned
	stringstream line_stream( line );		// turn into stream to use getline
	string tok;								// holds return from getline
	float val;								// holds float val converted from getline

	while( getline( line_stream, tok, delim ) )
	{
		val = stof( tok );
		data_values.push_back( val );
	}

	return data_values;
}
