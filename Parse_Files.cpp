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
void parse_csv( char* pdsi_file, vector< vector<double> >& all_data )
{
	//float data_val;							// convert from line read in to a float

	string line;							// line read in from file
	
	ifstream pdsi;	

//	cout << "parse csv pdsi file name: " << pdsi_file << endl;

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
			vector <double> year_vals;

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
void parse_param( char* param_file, Parameters *param_vals )
{
	int pos;								// used in substring splitting
	int lines_stored = 0;					// used to count the number of lines stored and to "index" which line
	int beginpos;							// used to get the start position for getting number of nodes in a layer

	string line;							//temp for an entire line
	
	string sub_line;						// used to temp store file names (weights and csv)
	string sub_line2;						// used to process when finding all the weights
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

		//printf( "in getline loop. \n" );
		//printf( "line_count is: %d \n", lines_stored );
	
		// store data
		switch ( lines_stored )
		{
			case 0:
				//cout << "Line is:" << line << endl;	
				pos = line.find( " " );
				//cout << "pos is: " << pos << endl;
				sub_line = line.substr( 0, pos );
				//cout << "sub_line is: " << sub_line << endl;
				strcpy( param_vals->weights_file, sub_line.c_str() );
				//printf( "case 0 \n" );
				break;

			case 1:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->epochs = stoi( sub_line );
				break;

			case 2:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->learn_rate = stod( sub_line );
				break;

			case 3:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->momentum = stod( sub_line );
				break;

			case 4:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->threshold_error = stod( sub_line );
				break;

			case 5:
				//cout << "Case 5, line is: " << line << endl;
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				//cout << "Case 5, sub_line is: " << sub_line << endl;
				param_vals->adjustable_weight_layers = stoi( sub_line );
				break;

			case 6:
				sub_line2 = line;
				for( int i = 0; i < param_vals->adjustable_weight_layers; i++ )
				{
					pos = sub_line2.find( " " );
					sub_line2 = sub_line2.substr(pos + 1, sub_line2.length());
					sub_line = sub_line2.substr( 0, pos );
					param_vals->nodes_per_layer[i] = stoi( sub_line );
				}
				break;

			case 7:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				strcpy( param_vals->data_file, sub_line.c_str() );
				//cout << "parsing - data file name is: " << param_vals.data_file << endl;
				break;

			case 8:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->years_burned_acres = stoi( sub_line );
				break;

			case 9:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->pdsi_months = stoi( sub_line );
				break;

			case 10:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->end_month = stoi( sub_line );
				break;

			case 11:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->classes = stoi( sub_line );
				break;

			case 12:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->threshold_low = stoi( sub_line );
				break;

			case 13:
				pos = line.find( " " );
				sub_line = line.substr( 0, pos );
				param_vals->threshold_medium = stoi( sub_line );
				break;

			default:
				printf( "something went wrong. \n" );
		}
		lines_stored++; 
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
vector<double> split_string( string line, char delim )
{
	vector<double> data_values;				// holds data values to be returned
	stringstream line_stream( line );		// turn into stream to use getline
	string tok;								// holds return from getline
	double val;								// holds float val converted from getline

	while( getline( line_stream, tok, delim ) )
	{
		val = stof( tok );
		data_values.push_back( val );
	}

	return data_values;
}
