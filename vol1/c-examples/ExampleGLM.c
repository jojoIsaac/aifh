/*
 * Artificial Intelligence for Humans
 * Volume 1: Fundamental Algorithms
 * C/C++ Version
 * http://www.aifh.org
 * http://www.jeffheaton.com
 *
 * Code repository:
 * https://github.com/jeffheaton/aifh

 * Copyright 2013 by Jeff Heaton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For more information on Heaton Research copyrights, licenses
 * and trademarks visit:
 * http://www.heatonresearch.com/copyright
 */
#include "aifh-vol1-examples.h"


void ExampleGLM(int argIndex, int argc, char **argv) {	
	char filename[FILENAME_MAX];
	NORM_DATA *norm;
	DATA_SET *data;
	REGRESSION_MODEL *reg;
	double *ideal, actual, *input, error;
	unsigned int i, iteration;

	LocateFile("breast-cancer-wisconsin.csv",filename,FILENAME_MAX);
	norm = NormCreate();
	NormDefIgnore(norm);
	NormDefPass(norm);
	NormDefPass(norm);
	NormDefPass(norm);
	NormDefPass(norm);
	NormDefPass(norm);
	NormDefPass(norm);
	NormDefPass(norm);
	NormDefPass(norm);
	NormDefPass(norm);
	NormDefReplace(norm,4,1,0);

	NormAnalyze(norm,filename);
	data = NormProcess(norm,filename,9,1);
	
	printf("Actual input count: %i\n", data->inputCount);
	printf("Actual output count: %i\n", data->idealCount);

	reg = RegressionCreate(data->inputCount,LinkLOGIT);
	
	iteration = 0;
    do {
		iteration++;
		error = RegressionReweightLeastSquares(reg,data);
		printf("Iteration #%i, Error: %.8f\n",iteration,error);
	} while (iteration < 1000 && error > 0.01);

	/* Display results */
	for(i=0;i<data->recordCount;i++) {
		ideal = DataGetIdeal(data,i);
		input = DataGetInput(data,i);
		actual = RegressionCalculate(reg,input);
		printf("Ideal: %.2f, Actual: %.2f\n",ideal[0],actual);
	}

	NormDelete(norm);
	DataDelete(data);
	RegressionDelete(reg);
}