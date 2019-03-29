#include <iostream>
#include <string>
#include <fstream>
#include "monster_definitions.h"


void monster_def_parser::parseWOutput(){
    std::ifstream readFile;
    readFile.open("$HOME/.rlg327/monster_definitions.txt");
    if(readFile.is_open()){
        std::cout << "Open" << std::endl;
    }
     std::cout << "Closed" << std::endl;

}