#include <iostream>
using namespace std;

class canGoWrong {
public:
    canGoWrong() {
        char * pMemory = new char[99999999999];
        delete [] pMemory;
    }
};

class myException : public exception {
    virtual const char* what() const throw() {
        return "meow";
    }
};

int main() {
    try {
    canGoWrong wentWrong;
    }
    catch(exception &e) {
        cout << "Caught Exception : " << e.what() << endl;
    }
    try {
        throw myException();
    }
    catch (exception &e) {
        cout << "Caught Exception : " << e.what() << endl;
    }
    return 0;
}