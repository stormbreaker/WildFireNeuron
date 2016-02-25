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

#include "Randomize.h"
#include "Parse_Files.h"
#include "Neuron_Layer.h"

using namespace std;


typedef vector<double> Year;			// holds values for one year
typedef vector<Year> All_Data;		// Vector of vectors, contains all PDSI data

void output_data( const All_Data& data );

vector<vector<double>> genOutputVector(All_Data& data, Parameters& param_vals);
vector<vector<double>> genInputVector(All_Data& data, Parameters& param_vals);


int main(int argc, char *argv[])
{
	char *param_file;					// Parameter file passed in from args

	//	char *data_file;	// TEMPORARY FOR TESTING

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
		// TEMPORARY FOR TESTING
		//data_file = argv[1];
	}

	// parse parameter file
	parse_param( param_file, &param_vals );	

	// parse PDSI csv file
	cout<< "Data file name: " << param_vals.data_file << endl;
	parse_csv( param_vals.data_file, data );

	// check correct reading of PDSI info
	//	output_data( data );

	//pull one out, train, test

	All_Data inData;
	inData = genInputVector(data, param_vals);

	All_Data finalOutput;
	finalOutput = genOutputVector(data, param_vals);
	
	vector<int> inputVectorList;

	vector<double> testVector;
	vector<double> testOVector;

	for (int indextoSkip = 0; indextoSkip < inputVectorList.size(); indextoSkip++)
	{
		inputVectorList = create_order(finalOutput.size());
		

		//assign the testing vector as the vector from inData with the index which has the same 
		// value as the value in inputVectorList which is indexed by indextoSkip
		testVector = inData[inputVectorList[indextoSkip]];
		testOVector = finalOutput[inputVectorList[indextoSkip]];


		Neuron_Layer net = Neuron_Layer(param_vals.nodes_per_layer[0], param_vals); //head layer
		
		cout << "made head layer" << endl;

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

		for (int i = 0; i < inData.size(); i++)
		{
			cout << "entered first foor loop" << endl;
			if (i != indextoSkip)
			{
				cout << "running condition" << endl;
				results = net.Run_and_Condition(inData[j], finalOutput[j]);
				cout << "got results" << endl;
			}
		}
		
		results = net.Run(testVector);

		net.Save_network(weights);

		weights.close();

	}	
	
	
	return 0;
}

/*
	testing file parsing for pdsi
*/
void output_data( const All_Data& data )
{
	for( All_Data::const_iterator row = data.begin(); row != data.end(); ++row )
	{
		for( Year::const_iterator col = row->begin(); col != row->end(); ++col )
			cout << *col << " ";
		cout << endl;
	}
}

/*
	Author: Benjamin Kaiser
*/

vector<vector<double>> genOutputVector(All_Data& data, Parameters& param_vals)
{
	vector<vector<double>> output;
	vector<double> outputSingle;
	for (int i = 0; i < data.size(); i++)
	{
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
		cout << outputSingle;
		cin.ignore();
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














