/******************************************************************************
Program:	 Artificial Neural Networks - Wildfire Prediction
Author:		 Stephanie Athow
Class:		 CSC 447/547 (Artificial Inelligence) 
Instructor:	 Dr. John Weiss
Due Date:	 Feb 24, 2016
Description: 
	Contains function definitions for file parsing the parameter and csv files
******************************************************************************/
#include "Parse_Files.h"

/******************************************************************************
Function:	 Parse CSV
Author: 	 Stephanie Athow
Description: Parses a csv file to obtain year, burned acrage, and PDSI data
	for each month of that year. It stores all the data for one year in a 
	vector. Then a vector is used to store all years.
Parameters:
	in: 	pdsi_file 		name of the csv file to parse
	in/out: all_data		a vector of vectors to contain all data
Returns:
	None
******************************************************************************/
void parse_csv( char* pdsi_file, vector< vector<double> >& all_data )
{
	string line;					// line read in from file
	ifstream pdsi;					// file stream for csv file


	// Open PDSI file
	pdsi.open( pdsi_file );

	// Check if file opened
	if( !pdsi )
	{
		printf( "CSV file could not be opened. Exiting. \n" );
		return;
	}

	// Read in information, ignore non-date lines
	while( getline( pdsi, line ) )
	{
		if( ( 0 == line.compare( 0, 1, "1" ) ) || ( 0 == line.compare( 0, 1, "2") ) )
		{
			string data;
			vector <double> year_vals;

			// parse line to break it up at commas
			year_vals = split_string( line, ',' );
			
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
Function:	 Parse Param
Author: 	 Stephanie Athow
Description: Parses the given parameter file
Parameters:
	in:		param_file		name of the parameter file to parse
	in/out:	param_vals		parameter values read in from the file, used to set
							up the neural net
******************************************************************************/
void parse_param( char* param_file, Parameters *param_vals )
{
	int pos;					// used in substring splitting
	int lines_stored = 0;		// used to count the number of lines stored and to "index" which line

	string line;				//temp for an entire line
	
	string sub_line;			// used to temp store file names (weights and csv)
	string sub_line2;			// used to process when finding all the weights
	string sub_try;
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

		// ignore blank lines
		if( line.empty() )
			continue;

		// store data
		switch ( lines_stored )
		{
			// weights file name
			case 0:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				strcpy( param_vals->weights_file, sub_line.c_str() );
				break;

			// number of epochs
			case 1:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->epochs = stoi( sub_line );
				break;

			// learning rate
			case 2:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->learn_rate = stod( sub_line );
				break;

			// momentum rate
			case 3:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->momentum = stod( sub_line );
				break;

			// error threshold
			case 4:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->threshold_error = stod( sub_line );
				break;

			// number of weight layers
			case 5:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->adjustable_weight_layers = stoi( sub_line );
				break;

			// read in number of nodes per layer
			case 6:
				sub_line2 = line;
				cout << "line is: " << line << endl;
				for( int i = 0; i < param_vals->adjustable_weight_layers +1; i++ )
				{
					pos = sub_line2.find( " " );
					sub_try = sub_line2.substr( 0, pos );
					param_vals->nodes_per_layer[i] = stoi( sub_try );
					sub_line2 = sub_line2.substr(pos + 1, sub_line2.length());
				}
				break;

			// csv file name
			case 7:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				strcpy( param_vals->data_file, sub_line.c_str() );
				break;

			// number of years of burned acre data
			case 8:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->years_burned_acres = stoi( sub_line );
				break;

			// number of months of PDSI data
			case 9:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->pdsi_months = stoi( sub_line );
				break;

			// final month of PDSI data
			case 10:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->end_month = stoi( sub_line );
				break;

			// number of classifications
			case 11:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->classes = stoi( sub_line );
				break;

			// threshold for low fire danger
			case 12:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->threshold_low = stoi( sub_line );
				break;

			// threshold for medium fire danger 
			case 13:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->threshold_medium = stoi( sub_line );
				break;

			// something broke
			default:
				printf( "Error: file parsing incomplete. \n" );
		}
		lines_stored++; 
	}
	
	params_in.close();
	return;
}


/******************************************************************************
Function:	 Split Strings
Author: 	 Stephanie Athow
Description: 
	Parses a csv file, it will break up a comma sperated line
Parameters:
	in: line		line to break up
	in: delim		character to delineate a break
Returns:
	data_values		a vector of doubles that contains the year, burned acres
					and 12 months of PDSI values
******************************************************************************/
vector<double> split_string( string line, char delim )
{
	vector<double> data_values;				// holds data values to be returned
	stringstream line_stream( line );		// turn into stream to use getline
	string tok;								// holds return from getline
	double val;								// holds double val converted from getline

	while( getline( line_stream, tok, delim ) )
	{
		val = stod( tok );
		data_values.push_back( val );
	}

	return data_values;
}
