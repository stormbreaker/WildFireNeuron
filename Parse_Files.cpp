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
#include <string>
#include "main.cpp"


/******************************************************************************
						Function Prototypes
******************************************************************************/
void parse_csv( ifstream& PDSI );

/******************************************************************************
Function:	 Parse CSV
Description: Parses a csv file to obtain year, burned acrage, and PDSI data
	for each month of that year. It stores all the data for one year in a 
	vector. Then a vector is used to store all years.
Parameters:
******************************************************************************/
void parse_csv( ifstream& PDSI )
{
	string line;
	

	PDSI.open();

	// Check if file opened
	if( !PDSI )
	{
		printf( "File could not be opened. Exiting. \n" );
		return;
	}

	// Read in information, ignore non-date lines
	while( getline( PDSI, line ) )
	{
		if( ( line.compare( 0, 1, "1" ) == 0 ) || ( line.compare( 0, 1, "2") == 0 ) )
		{

		}
		else 
			continue;
	}

	// Close file
	PDSI.close();
	
	return;
}
