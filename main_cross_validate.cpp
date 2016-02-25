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
#include <time.h>
#include <stdlib.h>

#include "Randomize.h"
#include "Parse_Files.h"
#include "Neuron_Layer.h"

using namespace std;


typedef vector<double> Year;			// holds values for one year
typedef vector<Year> All_Data;		// Vector of vectors, contains all PDSI data

void output_data( const All_Data& data );

double accuracy( int correct, int total );
vector<vector<double>> genOutputVector(All_Data& data, Parameters& param_vals);
vector<vector<double>> genInputVector(All_Data& data, Parameters& param_vals);
bool compare_vectors( vector<double> expected, vector<double> predicted );


int main(int argc, char *argv[])
{
	char *param_file;					// Parameter file passed in from args

	srand(time(NULL));
	bool match;
	double acc = 0;


	int correct = 0;					// number of correctly predicted outputs

	All_Data data;
	
	Parameters param_vals;				// Values from parameter file

	vector<double> results;

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
	cout<< "Data file name: " << param_vals.data_file << endl;
	parse_csv( param_vals.data_file, data );


	All_Data inData;
	inData = genInputVector(data, param_vals);

	All_Data finalOutput;
	finalOutput = genOutputVector(data, param_vals);
	
	vector<int> inputVectorList;

	vector<double> testVector;
	vector<double> testOVector;
	double rms = 1; //must be set to 1 to enter the if statement, which we want at first.

	inputVectorList = create_order(inData.size());

	for (int indextoSkip = 0; indextoSkip < inputVectorList.size(); indextoSkip++)
	{
		
		int indexToWork = inputVectorList[indextoSkip];

		//assign the testing vector as the vector from inData with the index which has the same 
		// value as the value in inputVectorList which is indexed by indextoSkip
		testVector = inData[indexToWork];
		testOVector = finalOutput[indexToWork];


		Neuron_Layer net = Neuron_Layer(param_vals.nodes_per_layer[0], param_vals); //head layer
		

		for (int i = 1; i < param_vals.adjustable_weight_layers + 1; i++)
		{
			net.Attach(param_vals.nodes_per_layer[i]);
		}
	
		ifstream weightsin;
		weightsin.open(param_vals.weights_file);

		//check to see if weights file was opened
		if (weightsin.is_open())
		{
			net.Load_network(weightsin);
			weightsin.close();
		}


		ofstream weights;
		weights.open(param_vals.weights_file);
		if (rms > param_vals.threshold_error)
		{
	
			for (int i = 0; i < inData.size(); i++)
			{
				int pos = inputVectorList[i];
				if (i != indextoSkip)
				{
					results = net.Run_and_Condition(inData[indexToWork], finalOutput[indexToWork]);
					for( int k = 0; k < results.size(); k++)
					{
						rms += pow(results[k] - finalOutput[pos][k], 2.0);
					}
				}
			}
			rms = sqrt(rms/(inData.size()*results.size()));
		}

		//cout <<"\tRMS: "<< rms << endl; //This could be uncommented to check the RMS if necessary
		
		results = net.Run(testVector);
	

		for( int z = 0; z < results.size(); z++ )
		{

			if( results[z] > 0.5 )
				results[z] = 1;
			else
				results[z] = 0;
		}

		cout << "Year: " << "\tExpected: " << testOVector[0] << testOVector[1] << testOVector[2];
		cout << " Predicted: " << results[0] << results[1] << results[2];

		match = compare_vectors(testOVector, results );

		if( !match )
			cout << "*" << endl;
		else
		{
			cout << endl;
			correct++;
		}

		net.Save_network(weights);



		

		weights.close();

	}
		acc = accuracy( correct, inData.size() );
		acc = acc*100;

		cout << "Accuracy is: " << setprecision(4)  << acc << "%" << endl;	
	
	
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
bool compare_vectors( vector<double> expected, vector<double> predicted )
{
	if( expected.size() != predicted.size() )
		return false; 
	for( int i = 0; i < expected.size() ; i++ )
	{
		if( expected[i] != (int) predicted[i] )
			return false;
	}

	return true;
}
/******************************************************************************
Function:	 output_data
Author: 	 Stephanie Athow
Description: 
	This function takes a 2D vector of doubles and prints it out.  This was
	used for error checking/debugging
Returns:
	nothing.  This is a void function strictly for output.  
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

/******************************************************************************
Function:	 genOutputVector
Author: 	 Benjamin Kaiser
Description: 
	This function takes a 2D vector of doubles and the parameter structure which
	was created for generating a complete vector of vectors.  These inner
	vectors are the individual expected output vectors for the net.  
	It uses the threshold values which have been previously normalized
	in the parameters struct to determin if it should generate a vector of
	<1,0,0>, <0,1,0>, or <0,0,1>.  These are low, medium, and high respectively
Returns:
	output:  a 2D vector of doubles.    
******************************************************************************/
vector<vector<double>> genOutputVector(All_Data& data, Parameters& param_vals)
{
	vector<vector<double>> output;
	vector<double> outputSingle;
	int yearsOffset = max(param_vals.years_burned_acres, (param_vals.pdsi_months/12));
	for (int i = yearsOffset; i < data.size(); i++)
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


/******************************************************************************
Function:	 genInputVector
Author: 	 Benjamin Kaiser
Description: 
	This function takes a 2D vector of doubles and the parameter structure which
	was created for generating a complete vector of vectors.  These inner
	vectors are the individual input vectors for the net.  They use the
	parameter struct and the data from that to determine how to construct the
	input vector.  This includes calculating the year offset and the number of
	years of burned acres as well as the number of months of PDSI data.  It
	then concatenates these into the final input vector.  
Returns:
	output:  a 2D vector of doubles.    
******************************************************************************/
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














