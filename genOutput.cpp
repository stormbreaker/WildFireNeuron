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
