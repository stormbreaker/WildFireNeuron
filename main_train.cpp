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
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "Parse_Files.h"
#include "Neuron_Layer.h"
#include "Normalization.h"
#include "Randomize.h"

using namespace std;


typedef vector<double> Year;			// holds values for one year
typedef vector<Year> All_Data;		// Vector of vectors, contains all PDSI data

void output_data( const All_Data& data );
void removeYears(const All_Data& data, All_Data& data_wo_yr);

vector<vector<double>> genOutputVector(All_Data& data, Parameters& param_vals);
vector<vector<double>> genInputVector(All_Data& data, Parameters& param_vals);


int main(int argc, char *argv[])
{
	srand(time(NULL));
	char *param_file;					// Parameter file passed in from args
	vector<double> results;

//	char *data_file;	// TEMPORARY FOR TESTING

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
		// TEMPORARY FOR TESTING
		//data_file = argv[1];
	}

	// parse parameter file
	parse_param( param_file, &param_vals );

	// parse PDSI csv file
//	cout<< "Data file name: " << param_vals.data_file << endl;
	parse_csv( param_vals.data_file, data );

	// check correct reading of PDSI info
	//	output_data( data );

	// normalize data
	normalize_pdsi( data );
	normalize_burned_acres( data, &param_vals );

	All_Data inData;
	inData = genInputVector(data, param_vals);


	All_Data finalOutput;

	finalOutput = genOutputVector(data, param_vals);


	output_data(finalOutput);

	//removeYears(data, data_wo_yr);

	//processing of net
	    for ( int x  = 0; x < param_vals.adjustable_weight_layers + 1; x++ )
        {
            cout << "Layer " << x << "\t Nodes: " << param_vals.nodes_per_layer[x] << endl;
        }

		//creating
		Neuron_Layer net = Neuron_Layer(param_vals.nodes_per_layer[0], param_vals); //head layer
		

		cout << "made head layer" << endl;
		for (int i = 1; i < param_vals.adjustable_weight_layers + 1; i++)
		{
			/*Neuron_Layer* layerpt;
			Neuron_Layer layer = Neuron_Layer(param_vals.nodes_per_layer[i]);
			layerpt = &layer;
			*/
			net.Attach(param_vals.nodes_per_layer[i]);
		}
        //net.Attach(3); //Output Layer

		ifstream weightsin;
		weightsin.open(param_vals.weights_file);
		if (weightsin.is_open())
		{
			net.Load_network(weightsin);
		}


	
	//at this point the net should be constructed

	//everything I wrote here needs to be looped


	ofstream weights;
	weights.open(param_vals.weights_file);
	vector<int> run_order;
    double rms = 100;

	for (int i = 0; i < param_vals.epochs && rms > param_vals.threshold_error ; i++)
	{
		run_order = create_order( inData.size());
		//need to re-randomize

        rms = 0;
		for (int j = 0; j < inData.size(); j++) //the idea is here but the
		{
			int pos = run_order[j];
			results = net.Run_and_Condition(inData[pos], finalOutput[pos]);
		
			for( int k = 0; k < results.size(); k++)
			{
				rms += pow(results[k] - finalOutput[pos][k], 2.0);
			}
		
		}
		rms = sqrt(rms/(inData.size()*results.size()));
		if( (i+1)%10 == 0 || rms < param_vals.threshold_error )
		cout << "Epoch: " << i+1 <<"\tRMS: "<< rms << endl;

	}
	net.Save_network(weights);

	weights.close();
	return 0;
}

/*
	Author: Stephanie Athow
	testing file parsing for pdsi
*/
void output_data( const All_Data& data )
{
	ofstream debug;
	debug.open("debug.txt");
	for( All_Data::const_iterator row = data.begin(); row != data.end(); row++ )
	{
		for( Year::const_iterator col = row->begin(); col != row->end(); col++ )
			debug << *col << setw(5) << " ";
		debug << endl;
	}
	debug.close();
}

/*
	Author: Benjamin Kaiser
*/
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

/*
	Author: Benjamin Kaiser
*/
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
