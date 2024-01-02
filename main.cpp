#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include "preProcessing.h"
#include "userInputsGUI.h"
#include "postProcessing.h"


int main() {
    //*********Setting Up Input Variables using FLTK***********//
    // Structure to store input values
    InputValues inputValues;

    // Declaring variables
    double L, E, I, N;

    // Get input values from the user
    getInputValues(inputValues);

    // Set input values inside the parameters
    L = inputValues.L;
    N = inputValues.N;
    E = inputValues.E;
    I = inputValues.I;

    //*********Setting Up Input Variables using FLTK END***********//
    double h=L/N, b=E*I;
    std::cout << "Input parameters are not real values,simplified values used.\n-------------------------Please CHECK HERE\n" <<std::endl;

    auto kg = createGlobalStiffnessMatrix(h,b,N);
    //std::cout << "Global Stiffness Matrix: " << std::endl;
    //print2DArray(kg);
    
    /*Use -ve for downward direction*/
    double f; /*Unifromdistributed load*/
    double F1,M1; /*Point loads and moments*/
    int N1; /*Point load location*/
    int S1, E1; /*Start & End UDL load location*/
    std::vector<double> loads(2*N+2,0.0);
    
    //*********get and set Load type using FLTK***********//
    getLoadType();
    //*********get and set Load type using FLTK END***********//
    
    switch (selectLoadType) {
        case 1:
            LoadValues loadValues;
            setInputLoads(loadValues, N);
            N1 = loadValues.N1;
            F1 = loadValues.F1;
            M1 = loadValues.M1;
            // Code for selectLoadType == 1
            createLoadVector(loads,F1,M1,N,N1); /*createLoadVector(load_vector,force,moment,Total nodes,loading node);*/
            std::cout <<"Current configuration set for.....Point Load"<<std::endl;
            std::cout <<"at the end of..............a Cantilever Beam\n"<< std::endl;
            break;
        case 2:
            uniDistLoadValues load;
            setUDLInputLoads(load, N);
            f = load.F1;
            S1 = load.S1;
            E1 = load.E1;
            // Code for selectLoadType == 2
            createUDLVector(loads,h,f,N,S1,E1); /*createUDLVector(load_vector,element_size,force,Total nodes,starting_node,ending_node);*/
            std::cout <<"Current configuration set for.....UDL"<<std::endl;
            std::cout <<"over................a Cantilever Beam\n"<< std::endl;
            break;
        default:
            // Code for any other value of selectLoadType
            std::cerr << "Error: Invalid load type selected.\n";
            break;
    }

    //std::cout << "Loads Matrix:" << std::endl;
    //print1DArray(loads);

    //std::cout << "Original Matrix:" << std::endl;
    //print2DArray(kg);
    reduceToUpperTriangular(kg,loads);
    //std::cout << "\nUpper Triangular Matrix:" << std::endl;
    //print2DArray(kg);
    //std::cout << "Loads Matrix:" << std::endl;
    //print1DArray(loads);

    std::vector<double> dx = backwardSubstitution(kg, loads);
    //std::cout << "Solution vector dx:" << std::endl;
    //print1DArray(dx);

    std::vector<double> deflection(dx.size()/2,0.0), slope(dx.size()/2,0.0);
    displacementToSlopeDeflection(dx,deflection,slope);

    //std::cout << "Deflection:" << std::endl;
    //print1DArray(deflection);
    //std::cout << "Slope:" << std::endl;
    //print1DArray(slope);

    //---------- Plotting the data
    std::vector<double> X = linspace(0,L,N+1);
    std::vector<double> y0(deflection.size(),0.0);
    std::vector<double> y0u(deflection.size(),0.01);
    std::vector<double> y0d(deflection.size(),-0.01);

    auto [yd, yu] = scaleVectors(deflection);
    
    plt::figure(1);
    plt::plot(X,y0,".b", {{"label", "Initial Position"}});
    plt::plot(X,y0u,"--b",X,y0d,"--b");
    plt::plot(X,deflection,".r", {{"label", "Final Position"}});
    plt::plot(X,yd,"-k",X,yu,"-k");

    plt::legend("upper right");
    plt::title("Initial vs Deflected Poistion of Beam");
    plt::ylabel("Deflection (m)");
    plt::xlabel("Length of Beam (m)");
    plt::axis("equal");
    plt::grid(true);

    plt::savefig("Deflection.pdf");
    plt::show();


    return 0;
}