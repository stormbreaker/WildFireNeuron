/******************************************************************************
Program:
Author:
Class:
Instructor:
Due Date:
Description:
******************************************************************************/
#include "Randomize.h"

void randomize( vector< vector<double> > & data )
{
	// seeding for random number generator
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	// randomize first vector (shuffle the data based on years, but not the data for the year)
	shuffle( data.begin(), data.end(), default_random_engine( seed ) ); 
	
}


