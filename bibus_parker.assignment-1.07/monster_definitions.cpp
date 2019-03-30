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
    if (line.compare("RLG327 MONSTER DESCRIPTION 1") != 0)
    {
        std::cout << "Not Correct File" << std::endl;
        return;
    }

    while (std::getline(readFile, line))
    {
        if (line.compare("BEGIN MONSTER") == 0)
        {
            monster_count++;
        }
    }

    readFile.clear();
    readFile.seekg(0, std::ios::beg);
    monster_def_list = (monster_definition *)malloc(sizeof(monster_definition));
    num_monsters = monster_count;
    monster_count = 0;

    while (std::getline(readFile, line) && monster_count < num_monsters)
    {
        if (!reading_monster && line.compare("BEGIN MONSTER") == 0)
        {
            reading_monster = 1;
            reset_check();
            std::cout << "Found Start" << std::endl;
        }
        else if (reading_monster)
        {
            if (line.find("NAME") != std::string::npos)
            {
                std::cout << "Found NAME: " << line << std::endl;
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[NAME])
                {
                    failed = 1;
                    std::cout << "NAME FAILURE: " << line << std::endl;
                }
                else
                {
                    curr_monster.name = line;
                    fields_checked[NAME] = 1;
                    std::cout << "NAME SET: " << curr_monster.name << " Monster: " << num_monsters << std::endl;
                }
            }
            else if (line.find("SYMB") != std::string::npos)
            {
                std::cout << "Found SYMB: " << line << std::endl;
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[SYMBOL])
                {
                    failed = 1;
                    std::cout << "SYMB FAILURE: " << line << std::endl;
                }
                else
                {
                    curr_monster.symbol = line.at(0);
                    fields_checked[SYMBOL] = 1;
                    std::cout << "SYMB SET: " << curr_monster.symbol << " Monster: " << num_monsters << std::endl;
                }
            }
            else if (line.find("HP") != std::string::npos)
            {
                std::cout << "Found HP: " << line << std::endl;
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[HEALTH])
                {
                    failed = 1;
                    std::cout << "HP FAILURE: " << line << std::endl;
                }
                else
                {
                    readDice(&curr_monster.hp_base, &curr_monster.hp_dice, &curr_monster.hp_sides, line);
                    fields_checked[HEALTH] = 1;
                    std::cout << "HP SET: " << curr_monster.atk_base << " Monster: " << num_monsters << std::endl;
                }
            }
            else if (line.find("DAM") != std::string::npos)
            {
                std::cout << "Found DAM: " << line << std::endl;
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[ATK])
                {
                    failed = 1;
                    std::cout << "DAM FAILURE: " << line << std::endl;
                }
                else
                {
                    readDice(&curr_monster.atk_base, &curr_monster.atk_dice, &curr_monster.atk_sides, line);
                    fields_checked[ATK] = 1;
                    std::cout << "DAM SET: " << curr_monster.atk_base << " Monster: " << num_monsters << std::endl;
                }
            }
            else if (line.find("SPEED") != std::string::npos)
            {
                std::cout << "Found SPEED: " << line << std::endl;
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[SPEED])
                {
                    failed = 1;
                    std::cout << "SPEED FAILURE: " << line << std::endl;
                }
                else
                {
                    readDice(&curr_monster.speed_base, &curr_monster.speed_dice, &curr_monster.speed_sides, line);
                    fields_checked[SPEED] = 1;
                    std::cout << "SPEED SET: " << curr_monster.speed_base << " Monster: " << num_monsters << std::endl;
                }
            }
            else if (line.find("RRTY") != std::string::npos)
            {
                std::cout << "Found RRTY: " << line << std::endl;
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[RARITY])
                {
                    failed = 1;
                    std::cout << "RRTY FAILURE: " << line << std::endl;
                }
                else
                {
                    curr_monster.rarity = std::stoi(line);
                    fields_checked[RARITY] = 1;
                    std::cout << "RRTY SET: " << curr_monster.rarity << " Monster: " << num_monsters << std::endl;
                }
            }
            else if (line.find("COLOR") != std::string::npos)
            {
                std::cout << "Found COLOR: " << line << std::endl;
            }
            else if (line.compare("END") == 0)
            {
                //Check for proper monster and save
                fields_checked[NAME] = 1;
                fields_checked[DESC] = 1;
                fields_checked[COLOR] = 1;
                fields_checked[ABIL] = 1;
                if(checkMonster(curr_monster) == 0){
                    saveMons(&curr_monster, monster_count++);
                }
                reading_monster = 0;
                std::cout << "Found End" << std::endl;
            }
            if(failed){
                reading_monster = 0;
            }
        }
    }
    num_monsters = monster_count;
    if (output_enable)
    {
        printMonsDefList();
    }

    //Close the file
    if (readFile.is_open())
    {
        readFile.close();
        std::cout << "Closed" << std::endl;
    }
}

void monster_def_parser::empty()
{
    if (monster_def_list != NULL)
    {
        free(monster_def_list);
    }
}

void monster_def_parser::reset_check()
{
    int counter;
    for (counter = 0; counter < NUM_CHECK_FIELDS; counter++)
    {
        fields_checked[counter] = 0;
    }
}

void monster_def_parser::printMonsDefList()
{
    int counter;
    for (counter = 0; counter < num_monsters; counter++)
    {
        //NAMEstd::cout << monster_def_list[counter].name << std::endl;
        //DESC
        std::cout << monster_def_list[counter].symbol << std::endl;//SYMB
        //COLOR
        std::cout << monster_def_list[counter].speed_base << "+" << monster_def_list[counter].speed_dice << "d" << monster_def_list[counter].speed_sides << std::endl; //SPEED
        //ABIL
        std::cout << monster_def_list[counter].hp_base << "+" << monster_def_list[counter].hp_dice << "d" << monster_def_list[counter].hp_sides << std::endl; //HEALTH
        std::cout << monster_def_list[counter].atk_base << "+" << monster_def_list[counter].atk_dice << "d" << monster_def_list[counter].atk_sides << std::endl; //DAMAGE
        std::cout << monster_def_list[counter].rarity << std::endl;//RRTY
        
        std::cout << std::endl;
        //std::cout << monster_def_list[counter].colors_selection[0] << std::endl;
    }
}

void monster_def_parser::saveMons(monster_definition *md, int position)
{
    //monster_def_list[position].name = md->name;  //setName(&md->name);
    monster_def_list[position].symbol = md->symbol;
    monster_def_list[position].hp_base = md->hp_base;
    monster_def_list[position].hp_dice = md->hp_dice;
    monster_def_list[position].hp_sides = md->hp_sides;
    monster_def_list[position].speed_base = md->speed_base;
    monster_def_list[position].speed_dice = md->speed_dice;
    monster_def_list[position].speed_sides = md->speed_sides;
    monster_def_list[position].atk_base = md->atk_base;
    monster_def_list[position].atk_dice = md->atk_dice;
    monster_def_list[position].atk_sides = md->atk_sides;
    monster_def_list[position].rarity = md->rarity;
    //monster_def_list[position].colors_selection = &md->colors_selection;
}

void monster_def_parser::readDice(int32_t *base, int32_t *dice, int32_t *sides, std::string str)
{
    std::cout << str << std::endl;
    *base = std::stoi(str.substr(0, str.find_first_of("+")));
    str = str.substr(str.find_first_of("+") + 1);
    *dice = std::stoi(str.substr(0, str.find_first_of("d")));
    str = str.substr(str.find_first_of("d") + 1);
    *sides = std::stoi(str);
}

int monster_def_parser::checkMonster(monster_definition md){
    int count;
    for(count = 0; count < NUM_CHECK_FIELDS; count++){
        if(!fields_checked[count]){
            std::cout << "Invalid Monster" << std::endl;
            return 1;
        }
    }
    return 0;
}