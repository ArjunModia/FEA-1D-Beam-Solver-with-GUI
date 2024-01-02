// userInputsGUI.h
#ifndef USER_INPUTS_GUI_H
#define USER_INPUTS_GUI_H


//***********FLTK GUI Input Window********************//
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

/////-----Structure to store input values
struct InputValues {
    double L;
    double N;
    double E;
    double I;

    // Input boxes
    Fl_Input* inputL;
    Fl_Input* inputN;
    Fl_Input* inputE;
    Fl_Input* inputI;
};//-----Structure to store input values END

/////-----Function to get user input via GUI
void getInputValues(InputValues& inputValues) {
    // Create the main window
    Fl_Window window(400, 250, "Inputs");

    // Create input boxes for L, N, E, and I
    Fl_Input inputL(220, 30, 150, 30, "Length of beam, L (m) : ");
    Fl_Input inputN(220, 70, 150, 30, "Total number of elements, N : ");
    Fl_Input inputE(220, 110, 150, 30, "Young's Modulus, E (N/m^2) : ");
    Fl_Input inputI(220, 150, 150, 30, "Area Moment, I (m^2) : ");

    // Create the OK button
    Fl_Button okButton(220, 190, 150, 30, "OK");

    // Set up the window and show it
    window.end();
    // Set the window position to the center of the screen
    int screenWidth = Fl::w(); // Get screen width
    int screenHeight = Fl::h(); // Get screen height
    int windowWidth = window.w(); // Get window width
    int windowHeight = window.h(); // Get window height
    window.position((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
    window.show();

    // Set up the callback data
    inputValues.inputL = &inputL;
    inputValues.inputN = &inputN;
    inputValues.inputE = &inputE;
    inputValues.inputI = &inputI;

    // Callback function for the OK button
    okButton.callback([](Fl_Widget* widget, void* data) {
        InputValues* inputValues = static_cast<InputValues*>(data);

        // Retrieve user inputs
        const char* LValue = inputValues->inputL->value();
        const char* NValue = inputValues->inputN->value();
        const char* EValue = inputValues->inputE->value();
        const char* IValue = inputValues->inputI->value();

        // Convert string inputs to numeric values
        inputValues->L = std::atof(LValue);
        inputValues->N = std::atof(NValue);
        inputValues->E = std::atof(EValue);
        inputValues->I = std::atof(IValue);

        // Close the input window
        widget->window()->hide();
        Fl::run();
        }
    , &inputValues);

    // Run the FLTK event loop for the main window
    Fl::run();
}//-----Function to get user input via GUI END
//***********FLTK GUI Input Window END********************//


//***********FLTK GUI Select Loads Window********************//
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Output.H>
//Other header files as same as previous GUI section

int selectLoadType = 5; // A global variable needed due to limited 
//functionality of FLTK radio button
// selectLoadType: 1 is Point Load
// selectLoadType: 2 is UDL

//////------Function to setLoadType
void setLoadType(Fl_Widget *widget, void *data) {
    Fl_Radio_Round_Button *pointLoadButton = static_cast<Fl_Radio_Round_Button *>(data);

    if (pointLoadButton->value() == 1) {
        std::cout << "Selected Load Type: Point Load" << std::endl;
        selectLoadType = 1;
    }
    else {
        std::cout << "Selected Load Type: UDL" << std::endl;
        selectLoadType = 2;
    }

    // Hide the window instead of exiting
    Fl_Window *window = dynamic_cast<Fl_Window*>(Fl::first_window());
    if (window) {
        window->hide();
    }
}//------Function to setLoadType END

//////------Function to setLoadType
void getLoadType() {
    Fl_Window window(300, 150, "Load Type Selector");

    // First field
    Fl_Box messageBox(70, 10, 150, 25, "Select Load Type");
    messageBox.labelfont(FL_BOLD);

    // Second field
    Fl_Radio_Round_Button pointLoadButton(70, 40, 150, 25, "Point Load");

    // Third field
    Fl_Radio_Round_Button udlButton(70, 70, 150, 25, "Uniformly Dist Load");

    // Fourth field
    Fl_Button doneButton(70, 110, 150, 25, "DONE");
    doneButton.callback(setLoadType, &pointLoadButton);

    // Center the window on the screen
    window.position((Fl::w() - window.w()) / 2, (Fl::h() - window.h()) / 2);

    window.show();
    Fl::run();
}
//***********FLTK GUI Select Loads Window END********************//


//***********FLTK GUI Set Load Values Window*********************//
/////-----Structure to store load values
struct LoadValues {
    Fl_Input* inputN1;
    Fl_Input* inputF1;
    Fl_Input* inputM1;
    int N1;
    float F1;
    float M1;
    int N;  // New member to store N
};/////-----Structure to store load values END

/////-----Static callback function
static void okButtonCallback(Fl_Widget* widget, void* data) {
    LoadValues* loadValues = static_cast<LoadValues*>(data);

    // Retrieve user inputs
    const char* N1Value = loadValues->inputN1->value();
    const char* F1Value = loadValues->inputF1->value();
    const char* M1Value = loadValues->inputM1->value();

    // Convert string inputs to numeric values
    loadValues->N1 = std::atoi(N1Value);
    loadValues->F1 = std::atof(F1Value);
    loadValues->M1 = std::atof(M1Value);

    // Compare N1 with N
    if (loadValues->N1 > loadValues->N) {
        // Display error window with a specific background color
        Fl_Window errorWindow(300, 150, "Error");
        errorWindow.color(FL_RED);

        Fl_Box errorMessage(20, 30, 260, 50, "Node number greater than Total Nodes");
        errorMessage.color2(FL_RED);
        Fl_Button closeButton(100, 90, 100, 30, "Close");
        closeButton.callback([](Fl_Widget*, void*) { exit(0); }, nullptr);

        errorWindow.end();
        // Set the window position to the center of the screen
        int screenWidth = Fl::w(); // Get screen width
        int screenHeight = Fl::h(); // Get screen height
        int windowWidth = errorWindow.w(); // Get window width
        int windowHeight = errorWindow.h(); // Get window height
        errorWindow.position((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
        errorWindow.show();
        Fl::run();
        std::cerr << "Error: N1 cannot be greater than N.\n";
        return; // Don't close the window and exit the callback function
    }

    // Close the input window
    widget->window()->hide();
    Fl::run();
}//-----Static callback function END

/////-----Set Input Loads function
void setInputLoads(LoadValues& loadValues, int N) {
    // Assign N to the LoadValues member
    loadValues.N = N;

    // Create the main window
    Fl_Window window(390, 140, "Set Loads");

    // Create input boxes for N1, F1, and M1
    Fl_Input inputN1(315, 10, 50, 20, "Node number for load, N1 : ");
    Fl_Input inputF1(315, 40, 50, 20, "Point load on the given node, F1 (N) : ");
    Fl_Input inputM1(315, 70, 50, 20, "Point moment on the given node, M1 (Nm) : ");

    // Create the OK button
    Fl_Button okButton(315, 110, 50, 20, "OK");

    // Set up the window and show it
    window.end();
    int screenWidth = Fl::w();
    int screenHeight = Fl::h();
    int windowWidth = window.w();
    int windowHeight = window.h();
    window.position((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
    window.show();

    // Set up the callback data
    loadValues.inputN1 = &inputN1;
    loadValues.inputF1 = &inputF1;
    loadValues.inputM1 = &inputM1;

    // Callback function for the OK button
    okButton.callback(okButtonCallback, &loadValues);

    // Run the FLTK event loop for the main window
    Fl::run();
}//-----Set Input Loads function END
//***********FLTK GUI Set Load Values Window END*********************//


//***********FLTK GUI Set UDLoad Values Window*********************//
/////-----Structure to store uni dist load values
struct uniDistLoadValues {
    Fl_Input* inputS1;
    Fl_Input* inputF1;
    Fl_Input* inputE1;
    int S1;
    float F1;
    int E1;
    int N;  // New member to store N
};/////-----Structure to store uni dist load values END

/////-----Static callback function for UDL
static void okUDLButtonCallback(Fl_Widget* widget, void* data) {
    uniDistLoadValues* loadValues = static_cast<uniDistLoadValues*>(data);

    // Retrieve user inputs
    const char* S1Value = loadValues->inputS1->value();
    const char* F1Value = loadValues->inputF1->value();
    const char* E1Value = loadValues->inputE1->value();

    // Convert string inputs to numeric values
    loadValues->S1 = std::atoi(S1Value);
    loadValues->F1 = std::atof(F1Value);
    loadValues->E1 = std::atoi(E1Value);

    // Compare N1 with N
    if ((loadValues->S1 < 0) || (loadValues->E1 > loadValues->N)) {
        // Display error window with a specific background color
        Fl_Window errorWindow(300, 150, "Error");
        errorWindow.color(FL_RED);

        Fl_Box errorMessage(20, 30, 260, 50, "Node range out of limits [0 N]");
        errorMessage.color2(FL_RED);
        Fl_Button closeButton(100, 90, 100, 30, "Close");
        closeButton.callback([](Fl_Widget*, void*) { exit(0); }, nullptr);

        errorWindow.end();
        // Set the window position to the center of the screen
        int screenWidth = Fl::w(); // Get screen width
        int screenHeight = Fl::h(); // Get screen height
        int windowWidth = errorWindow.w(); // Get window width
        int windowHeight = errorWindow.h(); // Get window height
        errorWindow.position((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
        errorWindow.show();
        Fl::run();
        std::cerr << "Error: N1 cannot be greater than N.\n";
        return; // Don't close the window and exit the callback function
    }

    // Close the input window
    widget->window()->hide();
    Fl::run();
}//-----Static callback function for UDL END

/////-----Set UDL Input Loads function
void setUDLInputLoads(uniDistLoadValues& uniDistLoadValues, int N) {
    // Assign N to the LoadValues member
    uniDistLoadValues.N = N;

    // Create the main window
    Fl_Window window(250, 140, "Set Loads");

    // Create input boxes for N1, F1, and M1
    Fl_Input inputF1(180, 10, 50, 20, "UDL load value, f (N/m) : ");
    Fl_Input inputS1(180, 40, 50, 20, "Starting node, S1 : ");
    Fl_Input inputE1(180, 70, 50, 20, "Ending node, E1 : ");

    // Create the OK button
    Fl_Button okButton(180, 110, 50, 20, "OK");

    // Set up the window and show it
    window.end();
    int screenWidth = Fl::w();
    int screenHeight = Fl::h();
    int windowWidth = window.w();
    int windowHeight = window.h();
    window.position((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
    window.show();

    // Set up the callback data
    uniDistLoadValues.inputS1 = &inputS1;
    uniDistLoadValues.inputF1 = &inputF1;
    uniDistLoadValues.inputE1 = &inputE1;

    // Callback function for the OK button
    okButton.callback(okUDLButtonCallback, &uniDistLoadValues);

    // Run the FLTK event loop for the main window
    Fl::run();
}//-----Set UDL Input Loads function END
//***********FLTK GUI Set UDLoad Values Window END*********************//

#endif // USER_INPUTS_GUI_H