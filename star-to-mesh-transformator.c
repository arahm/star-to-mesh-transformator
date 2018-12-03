// star-to-mesh-transformator: C++ code by Alexander D. Rahm (May 2018),
// for computing the total conductivity of a network of Ohmian resistors.
// Covered by the GNU General Public License v3.0.
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <tuple>
using namespace std;

std::vector<double> DeleteEntry(std::vector<double> row, int vertexToEliminate) {
    std::vector<double> ShortenedRow(row.size()-1);
    for (int i=0; i<vertexToEliminate; i++){
	ShortenedRow[i]  = row[i];
    }

    for (int i=vertexToEliminate+1; i<row.size(); i++){
	ShortenedRow[i-1]  = row[i];
    }       
    return ShortenedRow;
} 

std::vector<std::vector<double>> StarToMeshTransform(std::vector<std::vector<double>> conductivities, int vertexToEliminate) {

    std::vector<std::vector<double>> transformedConductivities(conductivities.size()-1);
    
    for (int i=0; i<vertexToEliminate; i++){

	transformedConductivities[i]  = DeleteEntry(conductivities[i], vertexToEliminate);
    }
    for (int i=vertexToEliminate+1; i<conductivities.size(); i++){
	transformedConductivities[i-1]  = DeleteEntry(conductivities[i], vertexToEliminate);
    }    
    
    std::list<int> Rays;
    double SumOverRays;
    SumOverRays = 0;
    
    for (int j=0; j < conductivities.size(); j++){
	if (conductivities[vertexToEliminate][j] > 0){
		Rays.push_back(j);
		SumOverRays = SumOverRays +conductivities[vertexToEliminate][j];
	}
    }

    std::list<int> FirstTerminals = Rays;
    while(FirstTerminals.size() > 1){
    	int a = FirstTerminals.front();
    	FirstTerminals.pop_front();
    	std::list<int> SecondTerminals = FirstTerminals;
    	while(SecondTerminals.size() > 0){
    	  int b = SecondTerminals.front();
    	  // Add the conductivites of the mesh to the existing conductivities in the circuit
    	  // (the resistors of the mesh that replaces the star are put in parallel to the existing resistors):
    	  
    	  if(a < vertexToEliminate && b < vertexToEliminate){
		// indices stay as they are	
    		transformedConductivities[a][b] = conductivities[a][b] +
    		  conductivities[a][vertexToEliminate]*conductivities[b][vertexToEliminate]/SumOverRays;
    		transformedConductivities[b][a] = transformedConductivities[a][b];  
	  }
	  if(a > vertexToEliminate && b < vertexToEliminate){
		// indices stay as they are	
    		transformedConductivities[a-1][b] = conductivities[a][b] +
    		  conductivities[a][vertexToEliminate]*conductivities[b][vertexToEliminate]/SumOverRays;
    		transformedConductivities[b][a-1] = transformedConductivities[a-1][b];    		  
	  }
    	  if(a < vertexToEliminate && b > vertexToEliminate){
		// indices stay as they are	
    		transformedConductivities[a][b-1] = conductivities[a][b] +
    		  conductivities[a][vertexToEliminate]*conductivities[b][vertexToEliminate]/SumOverRays;
    		transformedConductivities[b-1][a] = transformedConductivities[a][b-1];    		  
	  }	  
	  if(a > vertexToEliminate && b > vertexToEliminate){
		// indices stay as they are	
    		transformedConductivities[a-1][b-1] = conductivities[a][b] +
    		  conductivities[a][vertexToEliminate]*conductivities[b][vertexToEliminate]/SumOverRays;
      		transformedConductivities[b-1][a-1] = transformedConductivities[a-1][b-1];
	  }	  
	  SecondTerminals.pop_front();
	}	
    }    
    return transformedConductivities;
} 


void writeOutConductivity(char name[20]){
  char resistorsfilename[50];   
//////////////////////////////////////////////////////////////////////
// Construct the vertex-vertex distance matrix from the node contacts.
//////////////////////////////////////////////////////////////////////
  std::vector<std::vector<double>> conductivities(N_r);
  for (int i=0; i<N_r; i++){
      std::vector<double> ithconductivity(N_r); 
      for (int j=i+1; j<N_r; j++){
 	"The user needs to provide a function distanceBetweenNodes here,";
	"which reads in nodes which may for instance be specified by there coordinates.";
	"Alternatively, the Ohmian resistors could be specified directly.";
	double distance = distanceBetweenNodes(nodes[i], nodes[j]); 
	if (distance < "Insert a threshold here"){
	  if (distance == 0){
	  	ithconductivity[j] = 999999; // For nodes that much in touch, 
	  				     // there is almost infinite conductivity.
	  }
	  else{
	  	ithconductivity[j] = 1/(distance*distance);
	  }
        }
        else{
        	ithconductivity[j] = 0;
        }
      }
      conductivities[i] = ithconductivity;
  }
  for (int i=0; i<N_r; i++){
    for (int j=0; j<i; j++){
	 conductivities[i][j] = conductivities[j][i];     
    }
  }
//////////////////////////////////////////////////////////////////////////////////////
// Prepare writing out the conductivity matrix
//////////////////////////////////////////////////////////////////////////////////////
 
  strcpy(resistorsfilename, "resistors");
  ofstream resistorsoutFile(resistorsfilename, ios::out);

/////////////////////////////////////////////////////////////////////////////////////////
// Write out the conductivity matrix
  
  for (int i=0; i<N_r; i++){
    for (int j=i+1; j<N_r; j++){
      if( conductivities[i][j] > 0){
	 resistorsoutFile << "Conductivity from vertex " << i << " to " << j
	  << " is " << conductivities[i][j] << " Ohm^-1. Resistance: " << 1/conductivities[i][j] << " Ohm." << endl;   
      }  
    }
  }
  for (int i=1; i<N_r-1; i++){
     conductivities = StarToMeshTransform(conductivities, 1);
  }   
  int i=0; int j= conductivities.size()-1; 
  resistorsoutFile << "Total conductivity from vertex 0 to vertex " << N_r-1 << " through the electrical circuit of Ohmian resistors is " << conductivities[i][j] << " Ohm^-1. ";
  if ( conductivities[i][j] > 0){
  	resistorsoutFile << "Resistance: " << 1/conductivities[i][j] << " Ohm." << endl; 
  }else{	  
	resistorsoutFile << "Infinite resistance." << endl;     
  }
  resistorsoutFile << "Norming by the distance " << sorted_distances[0][N_r-1] <<
  " between these two vertices, we obtain the conductivity of " << conductivities[i][j]/sorted_distances[0][N_r-1] << " (Ohm*micrometres)^-1." << endl; 
//////////////////////////////////////////////////
 resistorsoutFile.close();
}

