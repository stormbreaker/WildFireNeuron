/******************************************************************************
Program:     Artifical Neural Networks - Wildfire Prediction
Authors:     Stephanie Athow, Ben Kaiser, Marcus
Class:       CSC 447/547 (Artificial Inelligence)
Instructor:  Dr. John Weiss
Due Date:    Feb 21, 2016
Description:

Date                Comment
----                ------------------------------
Feb 2, 2016         Started file structure, file parsing
Feb 3, 2016			Finished file parsing, untested
Feb 5, 2016			Finished network training code
Feb 6, 2016			Finished code for neural network file I/O
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "Parse_Files.h"
#include "Neuron_Layer.h"
#include "Normalization.h"
#include "Randomize.h"

using namespace std;


typedef vector<double> Year;		// holds values for one year
typedef vector<Year> All_Data;		// Vector of vectors, contains all PDSI data

double accuracy( int correct, int total );
bool compare_vectors( vector<int> expected, vector<double> predicted );
void output_data( const All_Data& data );
void removeYears( const All_Data& data, All_Data& data_wo_yr );
vector<vector<double>> genOutputVector( All_Data& data, Parameters& param_vals );
vector<vector<double>> genInputVector(All_Data& data, Parameters& param_vals);


int main(int argc, char *argv[])
{
	char *param_file;					// Parameter file passed in from args
	vector<double> results;

	All_Data data;
	All_Data data_wo_yr;

	Parameters param_vals;				// Values from parameter file

	// check for correct number of arguments
	if ( argc !=2 )
	{
		printf( "Incorrect usage. Program expects one file name. Exiting. \n" );
		return 1;
	}

	// get name of parameter file
	else
	{
		param_file = argv[1];
	}

	// parse parameter file
	parse_param( param_file, &param_vals );

	// parse PDSI csv file
	parse_csv( param_vals.data_file, data );

	// normalize data
	normalize_pdsi( data );
	normalize_burned_acres( data, &param_vals );

	All_Data inputData;
	inputData = genInputVector( data, param_vals );

	All_Data expectedOutput;
	expectedOutput = genOutputVector(data, param_vals);

	//create ANN
	Neuron_Layer net = Neuron_Layer(param_vals.nodes_per_layer[0], param_vals); //head layer
		
	for (int i = 1; i < param_vals.adjustable_weight_layers + 1; i++)
	{
		net.Attach(param_vals.nodes_per_layer[i]);
	}

	// open weights file
	ifstream weightsin;
	weightsin.open(param_vals.weights_file);

	// if open, load weights
	if (weightsin.is_open())
	{
		net.Load_network(weightsin);
	}
	else
	{
		cout << "Could not open weights file. Exiting." << endl;
		return -1;
	}

	int yearOffset = max( param_vals.years_burned_acres, (param_vals.pdsi_months/12) );

	cout << "input data size is: " << inputData.size() << endl;

	// run input vectors through net
	for (int j = 0; j < inputData.size(); j++ )
	{
	//	cout << "J is: " << j << " data is: " << inputData[j][0] << endl;

		vector<int> expected;

		results = net.Run(inputData[j]);

		for( int i = 0; i < results.size(); i++ )
		{
			expected.push_back( (int) expectedOutput[j+yearOffset][i] );
		}
//		cout << "size of expected is: " << expected.size();

		for( int z = 0; z < results.size(); z++ )
		{

			if( results[z] > 0.5 )
				results[z] = 1;
			else
				results[z] = 0;
		}
		cout << "Year: " << data[j+yearOffset][0] << "\t Expected: " << expectedOutput[j+yearOffset][0] << expectedOutput[j+yearOffset][1] << expectedOutput[j+yearOffset][2];

		cout << " \t Predicted: " << results[0] << results[1] << results[2];

		bool match;
		match = compare_vectors( expected, results );

		if( !match )
			cout << "*" << endl;
		else
			cout << endl;

	}

	weightsin.close();
	return 0;

}

/******************************************************************************
Function:	 accuracy
Author: 	 Stephanie Athow
Description: 
	calculate accuracy of number of correct predictions
******************************************************************************/
double accuracy( int correct, int total )
{
	double accuracy = (double) correct / (double) total;
	
	return accuracy;
}

/******************************************************************************
Function:	 compare vectors
Author: 	 Stephanie Athow
Description: 
	compare two vectors to see if they are the same.
Returns:
	true 	vectors are the same
	false 	vectors are different
******************************************************************************/
bool compare_vectors( vector<int> expected, vector<double> predicted )
{
	if( expected.size() != predicted.size() )
		return false; 

	for( int i = 0; i < expected.size() +1; i++ )
	{
		if( expected[i] != (int) predicted[i] )
			return false;
	}

	return true;
}

/******************************************************************************
Function:	 Output Data
Author: 	 Stephanie Athow
Description: 
	print out data for debugging purposes
******************************************************************************/
void output_data( const All_Data& data )
{
	for( All_Data::const_iterator row = data.begin(); row != data.end(); ++row )
	{
		for( Year::const_iterator col = row->begin(); col != row->end(); ++col )
			cout << *col << " ";
		cout << endl;
	}
}

void removeYears(const All_Data& data, All_Data& data_wo_yr)
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		for (unsigned int j = 1; j < data[i].size(); j++)
		{
			data_wo_yr[i][j-1] = data[i][j];
		}
	}
}

/******************************************************************************
Function:	 Generate Output Vector
Author: 	 Ben Kaiser
Description: 
	Generate a vector with expected output values
******************************************************************************/
vector<vector<double>> genOutputVector(All_Data& data, Parameters& param_vals)
{
	vector<vector<double>> output;
	vector<double> outputSingle;
	for (int i = 0; i < data.size(); i++)
	{
		outputSingle.clear();
		if (data[i][1] < param_vals.norm_threshold_low)
		{
			outputSingle.push_back(1);
			outputSingle.push_back(0);
			outputSingle.push_back(0);
		}
		else if(data[i][1] >= param_vals.norm_threshold_low && data[i][1] <= param_vals.norm_threshold_med)
		{
			outputSingle.push_back(0);
			outputSingle.push_back(1);
			outputSingle.push_back(0);
		}
		else
		{
			outputSingle.push_back(0);
			outputSingle.push_back(0);
			outputSingle.push_back(1);
		}
		output.push_back(outputSingle);
	}
	return output;		
}

/*
	Author: Benjamin Kaiser
*/
vector<vector<double>> genInputVector(All_Data& data, Parameters& param_vals)
{
	int dataStartYear = data[0][0];
	int currYear = dataStartYear;

	vector<double> temp;
	vector<vector<double>> finalData;

	int monthCounter;

	int yearsOffset = max(param_vals.years_burned_acres, (param_vals.pdsi_months/12));

	while (currYear - yearsOffset < dataStartYear)
	{
		currYear++;
	};
	
	vector<double> inputVector;

	for (int j = yearsOffset; j < data.size(); j++)
	{
		monthCounter = 0;
		inputVector.clear();

		//get burned acres to put in input vector
		for (int i = 0; i < param_vals.years_burned_acres; i++)
		{
			inputVector.push_back(data[j][1]);
		}

		//get pdsi data (this logic should be double-checked)
		int end_month = param_vals.end_month + 1;
	
		int k = j;


		while (monthCounter < param_vals.pdsi_months)
		{

			for (int i = end_month; i > 1; i--)
			{
				temp.emplace_back(data[k][i]);
				monthCounter++;
			}
			end_month = 13;

			k--;

		}
		reverse(temp.begin(), temp.end()); // i got the months backwards so now we need to flip them around

		for (int i = 0; i < temp.size(); i++) //concatenate the pdsi into the input vector
		{
			inputVector.push_back(temp[i]);
		}

		finalData.push_back(inputVector);

	}
	return finalData;
}
