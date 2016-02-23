/******************************************************************************
Program:
Author:
Class:
Instructor:
Due Date:
Description:
******************************************************************************/
#include "Normalization.h"

void normalize_pdsi( vector< vector<double> >& data )
{
	int row_size = 0;					// number of rows in 2d vector
	int col_size = 0;					// number of cols in 2d vector

	double max;						// maximum PDSI val for that year
	double min;						// minimum PDSI val for that year

	double denom; 					// (max - min) for normalization

	// get number of vectors to loop over
	row_size = data.size();

	// loop through all year's PDSI values
	for (int i = 0 ; i < row_size; i++ )
	{
		// find min and max PDSI values
		auto result = minmax_element( data[i].begin()+2, data[i].end() );
	
		min = *result.first;
		max = *result.second;

		denom = max - min;

		// loop through the PDSI values to normalize 
		// ( curr_val - min ) / denomenator

		col_size = data[i].size();			// get number of cols in vector

		for( int j = 2; j < col_size; j++ )
		{
			data[i].at(j) = ( ( data.at(i).at(j) - min) / denom );
		}
	}

}

void normalize_burned_acres( vector< vector<double> >& data, Parameters *param_vals )
{
	int i;								// iterator for "for loops"
	int row_size;						// number of rows in 2d vector
	
	double max;							// maximum number of burned acres
	double min;							// minimum number of burned acres

	double thresh_low;					// normalize the low threshold wrt data 
	double thresh_med;					// normalize the medium threshold wrt data

	double denom;						// (max - min) for normalization

	vector<double> burned_acres;			// temporarily holds burned acre values to normalize

	// get number of vectors to loop over
	row_size = data.size();

	// get low and medium thresholds
	thresh_low = param_vals -> threshold_low;
	thresh_med = param_vals -> threshold_medium;

	// loop through and get all burned acre values
	for( i = 0; i < row_size; i++ )
	{
		burned_acres.push_back( data.at(i).at(1) );
	}
	
	// find min/max of burned acres
	auto result = minmax_element( burned_acres.begin(), burned_acres.end() );
	
	min = *result.first;
	max = *result.second;

	denom = max - min;

	// loop through burned acre values to normalize
	for ( i = 0; i < row_size; i++ )
	{
		data.at(i).at(1) = ( ( data.at(i).at(1) - min ) / denom );
	}

	// normalize low and med thresh
	param_vals -> norm_threshold_low = ( ( thresh_low - min ) / denom );
	param_vals -> norm_threshold_med = ( ( thresh_med - min ) / denom );
}





