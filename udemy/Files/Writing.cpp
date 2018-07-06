#include <iostream>
#include <fstream>
using namespace std;

int main () {
    ofstream outFile;
    ifstream inFile;
    string filename = "heha.txt";
    outFile.open(filename.c_str());

    if (outFile.is_open()) {
        outFile << "meow" << endl;
    }
    else {
        cout << "Could not create file : " << filename << endl;
    }

    inFile.open(filename.c_str());
    if (inFile.is_open()) {
        string line;
        getline(inFile, line);
        cout << line << endl;
        inFile.close();
    }
    else {
        cout << "Could not open file : " << filename << endl;
    }
    return 0;
}