#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "monster_definitions.h"

void monster_def_parser::parse(int output_enable)
{
    std::ifstream readFile;
    std::string env = getenv("HOME");
    std::string line;
    readFile.open(env + "/.rlg327/monster_desc.txt");
    int monster_count = 0;
    int reading_monster = 0;

    if (!readFile.is_open())
    {
        std::cout << "Failed to open" << std::endl;
    }

    std::getline(readFile, line);
    if (line.compare("RLG327 MONSTER DESCRIPTION 1") != 0)
    {
        std::cout << "Not Correct File Header" << std::endl;
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
    num_monsters = monster_count;
    monster_def_list = (monster_definition *)malloc(sizeof(monster_definition) * num_monsters);
    monster_count = 0;

    while (std::getline(readFile, line) && monster_count < num_monsters)
    {
        if (!reading_monster && line.compare("BEGIN MONSTER") == 0)
        {
            reading_monster = 1;
            reset_check();
        }
        else if (reading_monster)
        {
            if (line.find("NAME") != std::string::npos)
            {
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[NAME])
                {
                    failed = 1;
                }
                else
                {
                    curr_monster.name = line;
                    fields_checked[NAME] = 1;
                }
            }
            else if (line.find("DESC") != std::string::npos)
            {
                std::string strHold;
                while (std::getline(readFile, line) && line.compare(".") != 0 && line.compare("END") != 0)
                {
                    strHold.append(line.append("\n"));
                }
                if (line.length() == 0 || fields_checked[DESC] || line.compare("END") == 0)
                {
                    failed = 1;
                }
                else
                {
                    curr_monster.description = strHold;
                    fields_checked[DESC] = 1;
                }
            }
            else if (line.find("SYMB") != std::string::npos)
            {
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[SYMBOL])
                {
                    failed = 1;
                }
                else
                {
                    curr_monster.symbol = line.at(0);
                    fields_checked[SYMBOL] = 1;
                }
            }
            else if (line.find("HP") != std::string::npos)
            {
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[HEALTH])
                {
                    failed = 1;
                }
                else
                {
                    readDice(&curr_monster.hp_base, &curr_monster.hp_dice, &curr_monster.hp_sides, line);
                    fields_checked[HEALTH] = 1;
                }
            }
            else if (line.find("DAM") != std::string::npos)
            {
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[ATK])
                {
                    failed = 1;
                }
                else
                {
                    readDice(&curr_monster.atk_base, &curr_monster.atk_dice, &curr_monster.atk_sides, line);
                    fields_checked[ATK] = 1;
                }
            }
            else if (line.find("SPEED") != std::string::npos)
            {
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[SPEED])
                {
                    failed = 1;
                }
                else
                {
                    readDice(&curr_monster.speed_base, &curr_monster.speed_dice, &curr_monster.speed_sides, line);
                    fields_checked[SPEED] = 1;
                }
            }
            else if (line.find("RRTY") != std::string::npos)
            {
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[RARITY])
                {
                    failed = 1;
                }
                else
                {
                    curr_monster.rarity = std::stoi(line);
                    fields_checked[RARITY] = 1;
                }
            }
            else if (line.find("COLOR") != std::string::npos)
            {
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[COLOR])
                {
                    failed = 1;
                }
                else
                {
                    readGetColors(&curr_monster, line);
                    fields_checked[COLOR] = 1;
                }
            }
            else if (line.find("ABIL") != std::string::npos)
            {
                line = line.substr(line.find_first_of(" ") + 1, std::string::npos);
                if (line.length() == 0 || fields_checked[ABIL])
                {
                    break;
                }
                else
                {
                    readGetAbilities(&curr_monster, line);
                }
                fields_checked[ABIL] = 1;
            }
            else if (line.compare("END") == 0)
            {
                //Check for proper monster and save
                fields_checked[NAME] = 1;
                fields_checked[DESC] = 1;
                if (checkMonster(curr_monster) == 0)
                {
                    saveMons(&curr_monster, monster_count++);
                }
                reading_monster = 0;
            }
            if (failed)
            {
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
    for (counter = 0; counter < NUMCOLORS; counter++)
    {
        curr_monster.colors_selection[counter] = 0;
    }
}

void monster_def_parser::printMonsDefList()
{
    int counter;
    for (counter = 0; counter < num_monsters; counter++)
    {
        std::cout << monster_def_list[counter].name << std::endl; //NAME
        std::cout << monster_def_list[counter].description;//DESC
        std::cout << monster_def_list[counter].symbol << std::endl;                                                                                                    //SYMB
        monster_def_list[counter].printColors();                                                                                                                       //COLOR
        std::cout << monster_def_list[counter].speed_base << "+" << monster_def_list[counter].speed_dice << "d" << monster_def_list[counter].speed_sides << std::endl; //SPEED
        monster_def_list[counter].printAbilities();                                                                                                                    //ABIL
        std::cout << monster_def_list[counter].hp_base << "+" << monster_def_list[counter].hp_dice << "d" << monster_def_list[counter].hp_sides << std::endl;          //HEALTH
        std::cout << monster_def_list[counter].atk_base << "+" << monster_def_list[counter].atk_dice << "d" << monster_def_list[counter].atk_sides << std::endl;       //DAMAGE
        std::cout << monster_def_list[counter].rarity << std::endl;                                                                                                    //RRTY
        std::cout << std::endl;
    }
}

void monster_def_parser::saveMons(monster_definition *md, int position)
{
    int counter;
    //monster_def_list[position].name = md->name;  //setName(&md->name);
    monster_def_list[position].setName(md->name);
    monster_def_list[position].setDesc(md->description);
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
    monster_def_list[position].abilities = md->abilities;
    for (counter = 0; counter < NUMCOLORS; counter++)
    {
        monster_def_list[position].colors_selection[counter] = md->colors_selection[counter];
    }
}

void monster_def_parser::readDice(int32_t *base, int32_t *dice, int32_t *sides, std::string str)
{
    *base = std::stoi(str.substr(0, str.find_first_of("+")));
    str = str.substr(str.find_first_of("+") + 1);
    *dice = std::stoi(str.substr(0, str.find_first_of("d")));
    str = str.substr(str.find_first_of("d") + 1);
    *sides = std::stoi(str);
}

int monster_def_parser::checkMonster(monster_definition md)
{
    int count;
    for (count = 0; count < NUM_CHECK_FIELDS; count++)
    {
        if (!fields_checked[count])
        {
            return 1;
        }
    }
    return 0;
}

void monster_def_parser::readGetColors(monster_definition *md, std::string line)
{
    std::string color;
    std::stringstream stream(line);
    while (stream >> color)
    {
        if (color.substr(0, color.find_first_of(" \n\t")).compare("BLACK") == 0)
            md->colors_selection[BLACK] = 1;
        else if (color.substr(0, color.find_first_of(" \n\t")).compare("RED") == 0)
            md->colors_selection[RED] = 1;
        else if (color.substr(0, color.find_first_of(" \n\t")).compare("GREEN") == 0)
            md->colors_selection[GREEN] = 1;
        else if (color.substr(0, color.find_first_of(" \n\t")).compare("YELLOW") == 0)
            md->colors_selection[YELLOW] = 1;
        else if (color.substr(0, color.find_first_of(" \n\t")).compare("BLUE") == 0)
            md->colors_selection[BLUE] = 1;
        else if (color.substr(0, color.find_first_of(" \n\t")).compare("MAGENTA") == 0)
            md->colors_selection[MAGENTA] = 1;
        else if (color.substr(0, color.find_first_of(" \n\t")).compare("CYAN") == 0)
            md->colors_selection[CYAN] = 1;
        else if (color.substr(0, color.find_first_of(" \n\t")).compare("WHITE") == 0)
            md->colors_selection[WHITE] = 1;
        else
        {
            failed = 1;
            return;
        }
    }
}

void monster_def_parser::readGetAbilities(monster_definition *md, std::string line)
{
    std::string ability;
    std::stringstream stream(line);
    md->abilities = 0;
    while (stream >> ability)
    {
        if (ability.substr(0, ability.find_first_of(" \n\t")).compare("SMART") == 0)
            md->abilities = md->abilities | NPC_SMART;
        else if (ability.substr(0, ability.find_first_of(" \n\t")).compare("TELEPATH") == 0)
            md->abilities = md->abilities | NPC_TELEPATH;
        else if (ability.substr(0, ability.find_first_of(" \n\t")).compare("TUNNEL") == 0)
            md->abilities = md->abilities | NPC_TUNNEL;
        else if (ability.substr(0, ability.find_first_of(" \n\t")).compare("ERRATIC") == 0)
            md->abilities = md->abilities | NPC_ERRATIC;
        else if (ability.substr(0, ability.find_first_of(" \n\t")).compare("PASS") == 0)
            md->abilities = md->abilities | NPC_PASS;
        else if (ability.substr(0, ability.find_first_of(" \n\t")).compare("PICKUP") == 0)
            md->abilities = md->abilities | NPC_PICKUP;
        else if (ability.substr(0, ability.find_first_of(" \n\t")).compare("DESTROY") == 0)
            md->abilities = md->abilities | NPC_DESTROY;
        else if (ability.substr(0, ability.find_first_of(" \n\t")).compare("UNIQ") == 0)
            md->abilities = md->abilities | NPC_UNIQ;
        else if (ability.substr(0, ability.find_first_of(" \n\t")).compare("BOSS") == 0)
            md->abilities = md->abilities | NPC_BOSS;
    }
}

void monster_definition::printAbilities()
{
    if (this->abilities & NPC_SMART)
        std::cout << "SMART ";
    if (this->abilities & NPC_TELEPATH)
        std::cout << "TELEPATH ";
    if (this->abilities & NPC_TUNNEL)
        std::cout << "TUNNEL ";
    if (this->abilities & NPC_ERRATIC)
        std::cout << "ERRATIC ";
    if (this->abilities & NPC_PASS)
        std::cout << "PASS ";
    if (this->abilities & NPC_PICKUP)
        std::cout << "PICKUP ";
    if (this->abilities & NPC_DESTROY)
        std::cout << "DESTROY ";
    if (this->abilities & NPC_UNIQ)
        std::cout << "UNIQ ";
    if (this->abilities & NPC_BOSS)
        std::cout << "BOSS ";
    std::cout << std::endl;
}

void monster_definition::printColors()
{
    if (this->colors_selection[BLACK])
        std::cout << "BLACK ";
    if (this->colors_selection[RED])
        std::cout << "RED ";
    if (this->colors_selection[GREEN])
        std::cout << "GREEN ";
    if (this->colors_selection[YELLOW])
        std::cout << "YELLOW ";
    if (this->colors_selection[BLUE])
        std::cout << "BLUE ";
    if (this->colors_selection[MAGENTA])
        std::cout << "MAGENTA ";
    if (this->colors_selection[CYAN])
        std::cout << "CYAN ";
    if (this->colors_selection[WHITE])
        std::cout << "WHITE ";
    std::cout << std::endl;
}
