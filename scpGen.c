#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <time.h>

using namespace std;

#define VERBOSE 1

void generateProblem(int numItems, int numSets, char* filename);

int main (int  argc, char *argv[])
{
	if (argc < 4 )
	{
	        cout << "Usage: ./solver numItems numSets outFile seed" << endl;
	        exit(-1);
	}

	srand ( atoi(argv[4]) );
	generateProblem(atoi(argv[1]), atoi(argv[2]), argv[3]);
	
	return 0;
} /* END main */

void
generateProblem(int numItems, int numSets, char* filename){
	// leave an identity matrix which allows us to have any singe item for a high price
	numSets = numSets - numItems;
	
	// initialize problem matrix
	vector< int > costs(numSets);
	vector< vector<int> > problem_matrix(numItems);
	for( int ii = 0; ii < numItems; ii++ ){ 
		problem_matrix[ii].resize(numSets,0);
	}
	
	// assign the costs of the sets uniformally at random
	fprintf( stderr, "assigning the costs...\n" );
	for( int ss = 0; ss < numSets; ss++ ){  }
	
	// fill in the remaining problem
	int costUniform = rand()%2;
	int densityConstant = rand()%2;		// is the density of the row or column constant or variable
	int selectUniform = rand()%2;		// each cell has a uniform chance of being selected or is the
										//   selection centered around some cell with a gaussian 
										//   distribution
	int vecSize = numItems;
	int vecRepetitions = numSets;
	
	int vecDensity = 1 + vecSize*(0.1+0.3*((rand()%10000)/10000.0));
	vector<int> vecProblem;
	
	vector<int> order(vecSize);
	for( int ii = 0; ii < vecSize; ii++ ){
		order[ii] = ii;
	}
	
	fprintf( stderr, " densityConstant: %d \n selectUniform: %d \n", densityConstant, selectUniform ); 
	
	fprintf( stderr, "generating problem...\n" );
	for( int rr = 0; rr < vecRepetitions; rr++ ){
		vecProblem.clear();
		vecProblem.resize(vecSize,0);
		
		if( densityConstant == 0 ){ vecDensity = 1 + vecSize*(0.1+0.3*((rand()%10000)/10000.0)); }
		
		// fill in the problem vector
		if( selectUniform == 1 ){
			// each cell has equal probability of being chosen
			random_shuffle( order.begin(), order.end() );
			
			for( int cc = 0; cc < vecDensity; cc++ ){
				vecProblem[ order[cc] ] = 1;
			}
		}
		else{
			// each cell has a higher probability of being selected if it is closer to the start
			//   exponential decay with the first element has 80% chance of being chosen and the last 
			//   element has a 10% chance of being chosen
			float a = 0.8;
			float b = 1.38629 / vecSize;
			int count = 0;
			while ( count < vecDensity ){
				for( int ii = 0; ii < vecSize; ii++ ){
					if( vecProblem[ ii ] != 1 ){
						float prob = rand()%10000/10000.0;
						if( prob <= a*exp(-b*ii) ){
							vecProblem[ ii ] = 1;
							count++;
						}
						if( count >= vecDensity ){ break; }
					}
				}
			}
		}
		
		// insert the vector into the problem matrix
		for( int ii = 0; ii < vecSize; ii++ ){ problem_matrix[ii][rr] = vecProblem[ii]; }
		
		
		// define the cost of the set
		if( costUniform == 0 ){
			costs[rr] = 1 + (rand()%1000) * (0.4*vecSize-vecDensity);
		}
		else{
			costs[rr] = 1 + (rand()%1000);
		}
	}
	
	// shuffle the rows and columns of the problem to make it more random 
	fprintf( stderr, "shuffling the problem...\n");
	//random_shuffle( problem_matrix.begin(), problem_matrix.end() );
	
	//*
	vector<int> rowOrder(numItems);
	for( int ii = 0; ii < numItems; ii++ ){ rowOrder[ii] = ii; }
	random_shuffle( rowOrder.begin(), rowOrder.end() );
	
	vector< vector<int> > temp_matrix = problem_matrix;
	
	for( int ii = 0; ii < numItems; ii++ ){
		for( int ss = 0; ss < numSets; ss++ ){
			problem_matrix[ii][ss] = temp_matrix[ rowOrder[ii] ][ss];
		}
	}
	
	vector<int> colOrder(numSets);
	for( int ss = 0; ss < numSets; ss++ ){ colOrder[ss] = ss; }
	random_shuffle( colOrder.begin(), colOrder.end() );
	
	for( int ii = 0; ii < numItems; ii++ ){
		vector<int> old = problem_matrix[ii];
		for( int ss = 0; ss < numSets; ss++ ){
			problem_matrix[ii][ss] = old[ colOrder[ss] ];
		}
	}
	//*/
	
	// save problem to file
	fprintf( stderr, "saving the problem...\n" );
	FILE * pFile;
	
	pFile = fopen (filename,"w");
	
	fprintf( pFile, "%d \n", numItems );
	fprintf( pFile, "%d \n", (numSets+numItems) );
	
	for( int ii = 0; ii < numItems; ii++ ){
		fprintf( pFile, "10000000 1 %d \n", ii );
	}
	
	for( int ss = 0; ss < numSets; ss++ ){
		fprintf( pFile, "%d ", costs[ss] );
		int card = 0;
		for( int ii = 0; ii < numItems; ii++ ){
			if( problem_matrix[ii][ss] == 1 ){
				card++;
			}
		}
		fprintf( pFile, "%d ", card );
		for( int ii = 0; ii < numItems; ii++ ){
			if( problem_matrix[ii][ss] == 1 ){
				fprintf( pFile, "%d ", ii );
			}
		}
		fprintf( pFile, "\n" );
	}
	fclose(pFile);
}