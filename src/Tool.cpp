#include "Tool.hpp"
#include <string>
#include <iostream>

using namespace std;
Tool::Tool():Item(){
    this->durability=10;
}

Tool::Tool(string name, string type, int durability):Item(name,type){
    this->durability=durability;
}

int Tool::getDurability()const{
    return this->durability;
}

void Tool::setDurability(int pengganti){
    this->durability=pengganti;
}

void Tool::printInfo()const{
    cout<< "NonTool\t:"<<endl;
    Item::printInfo();
    cout << "Durability\t: "<< this->durability <<endl;
    cout << "Kind\t\t: "<< this->kind <<endl<<endl;
}