// preProcessing.h
#ifndef PRE_PROCESSING_H
#define PRE_PROCESSING_H

#include <iostream>
#include <vector>
#include <cmath>
#include <tuple> // Include to retun different data types
#include <iomanip>

////////--------Print Array 1D function
void print1DArray(const std::vector<double>& arr) {
    int columnWidth = 9;
    for (const auto& row : arr) {
        std::cout << std::setw(columnWidth) << std::fixed << std::setprecision(4) << row <<"\t";//std::endl;
    }
    std::cout <<"\n"<< std::endl;
}//--------Print Array 1D function END


////////--------Print Array 2D function
void print2DArray(const std::vector<std::vector<double>>& arr) {
    int columnWidth = 9;
    for (const auto& row : arr) {
        for (const auto& element : row) {
            std::cout << std::setw(columnWidth) << std::fixed << std::setprecision(2) << element << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}//--------Print Array 2D function END


////////--------Element Stiffness Matrix Function
std::vector<std::vector<double>> createElementStiffMatrix(const double h, const double b) {
    // Print message at the start
    std::cout << "|----> Creating Element Stiffness Matrix..............." << std::endl;

    std::vector<std::vector<double>> ke(4, std::vector<double>(4, 0.0)); /*element stiffness matrix*/

    //Assigning the values to elements
    ke[0][0] = ke[2][2] = 6.0*(2.0*b/pow(h,3));
    ke[0][2] = ke[2][0] = -6.0*(2.0*b/pow(h,3));
    
    ke[0][1] = ke[0][3] = ke[1][0] = ke[3][0] = -3.0*h*(2.0*b/pow(h,3));
    ke[1][2] = ke[2][1] = ke[2][3] = ke[3][2] = 3.0*h*(2.0*b/pow(h,3));
    
    ke[1][3] = ke[3][1] = 1.0*h*h*(2.0*b/pow(h,3));
    ke[1][1] = ke[3][3] = 2.0*h*h*(2.0*b/pow(h,3));

    //print2DArray(ke);

    // Print message at the end
    std::cout << "|----> Creating Element Stiffness Matrix...........DONE" << std::endl;
    return ke;
}//--------Element Stiffness Matrix Function END



////////--------Global Stiffness Matrix - Diagonal Arrays Function
std::vector<std::vector<double>> createGlobalStiffnessMatrix(const double h, const double b, const double N) {
    // Print message at the start
    std::cout << "Creating Global Stiffness Matrix................" << std::endl;
    auto ke = createElementStiffMatrix(h,b);
    
    std::vector<std::vector<double>> kg(2*N+2, std::vector<double>(2*N+2, 0.0)); /*global stiffness matrix*/

    for(int counter=0;counter<2*N;counter+=2){
        for(int i=counter; i<counter+4; i++){
            for(int j=counter; j<counter+4; j++){
                kg[i][j] += ke[i-counter][j-counter];
            }
        }
    }
    
    // Print message at the end
    std::cout << "Creating Global Stiffness Matrix............DONE\n" << std::endl;

    return kg;
}//--------Global Stiffness Matrix - Diagonal Arrays Function END


////////--------Function to reduce a 2D vector to upper triangular matrix
void reduceToUpperTriangular(std::vector<std::vector<double>>& matrix, std::vector<double>& loads) {
    //Print messeage for start
    std::cout << "Reducing Global Stiffness to Upper Triangular matrix................" << std::endl;
    int rows = matrix.size();
    int cols = matrix[0].size();
    double diagonalElement = 0;

    for (int i = 2; i < rows; ++i) {
        // Make the diagonal element 1
        diagonalElement = matrix[i][i];
        //std::cout << "\nDiagonal Element = " << diagonalElement << std::endl;
        if (diagonalElement == 0){
          std::cout << "Zero detected on diagonal element ("<<i+1<<","<<i+1<<")" << std::endl;
          std::cout << "..............Calculation Aborted !!!!!" << std::endl;
          break;  
        }
        for (int j = i; j < cols; ++j) {
            matrix[i][j] /= diagonalElement;
            /*if (i == cols-1){
              std::cout << "\nLast element reached on diagonal ("<<i+1<<","<<j+1<<")" << std::endl;
              std::cout << "..............Calculation Aborted !!!!!" << std::endl;
              break;  
            }*/
        }
        /*std::cout << "**************************** i = " <<i<< std::endl;
        print2DArray(matrix);
        std::cout << "****************************------" << std::endl;
        print1DArray(loads);*/
        loads[i] /= diagonalElement;
        // Eliminate non-zero elements below the diagonal
        for (int k = i + 1; k < rows; ++k) {
            double factor = matrix[k][i];
            //std::cout << "\nFactor = " << factor << std::endl;
            for (int j = i; j < cols; ++j) {
                matrix[k][j] -= factor * matrix[i][j];
            }
            loads[k] -= factor * loads[i];
        }
        /*std::cout << "**************************** i = " <<i<< std::endl;
        print2DArray(matrix);
        std::cout << "****************************------" << std::endl;
        print1DArray(loads);*/ 
    }
    //Print messeage for end
    std::cout << "Reducing Global Stiffness to Upper Triangular matrix............DONE\n" << std::endl;
    
}//--------Function to reduce a 2D vector to upper triangular matrix END

////////-------- Backward substitution algorithm
/*This algorithm only works for a reduced matrix
where first node or 2 top elements of array are not
considered.

Modification required for simply supported beam.*/
std::vector<double> backwardSubstitution(const std::vector<std::vector<double>>& A, const std::vector<double>& B) {
    int n = A.size();
    std::vector<double> X(n, 0.0);
    for (int i = n - 1; i >= 2; --i) {
        X[i] = B[i];
        for (int j = i + 1; j < n; ++j) {
            X[i] -= A[i][j] * X[j];
        }
        X[i] /= A[i][i];
    }
    return X;
}//-------- Backward substitution algorithm END

////////--------Create Load Vector
void createLoadVector(std::vector<double>& loads,const double F, const double M, const double N, const int node_num) {
    if(node_num>N){
        std::cout << "Invalid node number......Node number > Total nodes"<< std::endl;
        return;
    }

    std::cout << "Nodes are considered in order 0,1,2,..." << std::endl;
    std::cout << "Forces and moments are in form F,-M....\n" << std::endl;
    
    std::cout << "Max Nodes: 0 to " << N <<std::endl;
    std::cout << "Force at node " << node_num<<" : "<<F<<"........SET"<< std::endl;
    std::cout << "Moment at node " << node_num<<" : "<<-M<<"......SET\n"<< std::endl;

    loads[2*node_num] += F;
    loads[2*node_num+1] += -M;

}//--------Create Load Vector END


////////--------Create UDL Vector
void createUDLVector(std::vector<double>& loads, const double h, const double f, const double N, int node_start, int node_end) {
    if(node_start>N || node_end>N){
        std::cout << "Invalid node number......Node number > Total nodes"<< std::endl;
        return;
    }

    std::cout << "Nodes are considered in order 0,1,2,..." << std::endl;
    std::cout << "Forces and moments are in form F,-M....\n" << std::endl;
    
    std::cout << "Max Nodes: 0 to " << N <<std::endl;
    std::cout << "UDL of "<<f<<", from node" << node_start <<" to node"<< node_end<<"........SET\n"<< std::endl;
    
    loads[2*node_start] += 6*f*h/12;
    loads[2*node_end] += 6*f*h/12;
    loads[2*node_start+1] += -h*f*h/12;
    loads[2*node_end+1] += h*f*h/12;

    for (int node_num=node_start+1;node_num<node_end;node_num++){
        loads[2*node_num] += 2*6*f*h/12;
    }    
}//--------Create UDL Vector END

////////--------Convert displacement to slope and deflection
void displacementToSlopeDeflection(std::vector<double>& dsplacmnt, std::vector<double>& deflection, std::vector<double>& slope){
    for(int i=0; i<dsplacmnt.size();i+=2){
        deflection[i/2] = dsplacmnt[i];
        slope[i/2] = dsplacmnt[i+1];
    }
}//--------Convert displacement to slope and deflection END

#endif // PRE_PROCESSING_H