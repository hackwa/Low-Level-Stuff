#include <iostream>
using namespace std;

void mightGoWrong()
{
    bool error = true;
    if(error) {
        throw string("huha");
    }
}

int main()
{
    try {
        mightGoWrong();
    }
    catch (string &a) {
        cout << "Error code " << a << endl;
    }
    return 0;
}