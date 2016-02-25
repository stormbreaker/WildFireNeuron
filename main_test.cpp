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

#include "Parse_Files.h"
#include "Neuron_Layer.h"

using namespace std;


typedef vector<double> Year;			// holds values for one year
typedef vector<Year> All_Data;		// Vector of vectors, contains all PDSI data

double accuracy( int correct, int total );
void output_data( const All_Data& data );
void removeYears( const All_Data& data, All_Data& data_wo_yr );
vector<vector<double>> genOutputVector( All_Data& data, Parameters& param_vals );


int main(int argc, char *argv[])
{
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

	//output_data(test);

//	printf( "after normalization: \n" );
//	output_data( data );

	// randomize data
	randomize( data );
//	printf("after randomization: \n" );
	//output_data( data );

	cout << "randomized" << endl;
	All_Data finalOutput;
	cout << "declared final outvec" << endl;
	finalOutput = genOutputVector(data, param_vals);
	cout << "assigned final" << endl;

	output_data(finalOutput);

	//removeYears(data, data_wo_yr);
	cout << "removed years" << endl;

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

	cout << "BUILT NET" << endl;
	
	//at this point the net should be constructed

	//everything I wrote here needs to be looped


	ofstream weights;
	weights.open(param_vals.weights_file);
	for (int i = 0; i < param_vals.epochs; i++)
	{
		//need to re-randomize
		cout << "entered first foor loop" << endl;

		for (int j = 0; j < inData.size(); j++) //the idea is here but the
		{
			cout << "running condition" << endl;
			results = net.Run_and_Condition(inData[j], finalOutput[j]);
			cout << "got results" << endl;
		}
		//we need to save the weight
	}
	cout << "after for loop" << endl;
	net.Save_network(weights);

	weights.close();
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
