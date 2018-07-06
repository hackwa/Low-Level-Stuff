#include"tokenizer.h"

int main()
{
    std::string line;
    std::cout<<"Meow"<<std::endl;
    std::ifstream input("input");
    while(std::getline(input,line)){
        //cout<<line<<endl;
        tokenize(line);
    }
    input.close();
    return 0;
}