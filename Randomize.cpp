/******************************************************************************
Program:	Artificial Neural Networks - Wildfire Prediction
Authors:	Stephanie Athow, Marcus Haberling
Class:		CSC 447/547
Instructor:	Dr. John Weiss
Due Date:	Feb 23, 2016
Description:
	Holds the function definitions for randomizing 
******************************************************************************/
#include "Randomize.h"

/*******************************************************************************
Function:	Randomize
Author:		Marcus Haberline
Description: 
	
Parameters:
	
Returns:
	none
*******************************************************************************/

vector<int> create_order( int number_of_records )
{
    vector<int> in_order;
    vector<int> to_return;
    for( int x = 0; x < number_of_records; x++ )
    {
         in_order.push_back(x);
    }
    for( int x = number_of_records; x > 0; x -- )
    {
        int pos = rand()%x;
        to_return.push_back(in_order.at(pos));
        int added = in_order.at(pos);
        in_order.at(pos) = in_order.at(x-1);
        in_order.at(x-1) = added;
    }
	return to_return;
}


/*******************************************************************************
Function:	Randomize
Author:		Stephanie Athow
Description: 
	Randomize the order of the vectors in a vector of vectors, but does	not
	randomize the contents the second vector
Parameters:
	in/out:	data	A vector of vectors where one vector contains the year,
					burned acres, and 12 months of PDSI data
Returns:
	none
*******************************************************************************/
void randomize( vector< vector<double> > & data )
{
	// seeding for random number generator
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	// randomize first vector (shuffle the data based on years, but not the data for the year)
	shuffle( data.begin(), data.end(), default_random_engine( seed ) ); 
	
}

