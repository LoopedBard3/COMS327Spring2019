#include <iostream>
#include <string>
#include <fstream>
#include "monster_definitions.h"

void monster_def_parser::parse(int output_enable)
{
    std::ifstream readFile;
    std::string env = getenv("HOME");
    std::string line;
    readFile.open(env + "/.rlg327/monster_desc.txt");
    int monster_count = 0;
    int reading_monster = 0;

    if (readFile.is_open())
    {
        std::cout << "Open" << std::endl;
    }

    std::getline(readFile, line);
    if(line.compare("RLG327 MONSTER DESCRIPTION 1") != 0){
        std::cout << "Not Correct File" << std::endl;
        return ;
    }

    while(std::getline(readFile, line)){
        if(line.compare("BEGIN MONSTER") == 0){
            monster_count++;
        }
    }

    readFile.clear();
    readFile.seekg(0, std::ios::beg);
    monster_def_list = (monster_definition *)malloc(sizeof(monster_definition));
    num_monsters = monster_count;
    monster_count = 0;
    
    while(std::getline(readFile, line)){
        if(!reading_monster && line.compare("BEGIN MONSTER") == 0){
            reading_monster = 1;
            reset_check();
            std::cout << "Found Start" << std::endl;
        }else if(reading_monster){
            if(line.find("SYMB") != std::string::npos){ 
                std::cout << "Found SYMB: "  + line << std::endl;
            }else if(line.find("COLOR") != std::string::npos){
                std::cout << "Found COLOR: " + line << std::endl;
            }else if(line.compare("END") == 0){
                //Check for proper monster
                reading_monster = 0;
                std::cout << "Found End" << std::endl;
            }
        }
    }

    if(output_enable){
        printMonsDefList();
    }

    //Close the file
    if (readFile.is_open())
    {
        readFile.close();
        std::cout << "Closed" << std::endl;
    }
}


void monster_def_parser::empty(){
    if(monster_def_list != NULL){
        free(monster_def_list);
    }
}

void monster_def_parser::reset_check(){
    int counter;
    for(counter = 0; counter < NUM_CHECK_FIELDS; counter++){
        fields_checked[counter] = 0;
    }
}

void monster_def_parser::printMonsDefList(){
    int counter;
    for(counter = 0; counter < num_monsters; counter++){
        std::cout << monster_def_list[counter].name << std::endl;
        std::cout << monster_def_list[counter].symbol << std::endl;
        std::cout << monster_def_list[counter].colors_selection[0] << std::endl;

    }
}