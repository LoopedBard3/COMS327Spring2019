#ifndef SOLVE_HANGMAN_H
#define SOLVE_HANGMAN_H

#include <cassert>
#include <cstring> 
#include <iostream> 
#include <fstream>
#include <stdio.h>
#include <string>
#include <regex>
#include <vector>

#define DEFAULT_DICTIONARY_PATH "words"

class word {
    public:
        word() : topMatches(), wordFormat(""), wordFilled(""), reg("", std::regex_constants::icase), numTopMatches(0)
        {}
        std::vector<std::string> topMatches;
        std::string wordFormat;
        std::string wordFilled;
        std::regex reg;
        int numTopMatches;
};
void clearMatches(std::vector<word> * wdArray, int numWords);
void printCurrentLine(std::vector<word> wdArray, int numWords);
void printMatches(std::vector<word> wdArray, int numWords);
void updateRegex(std::vector<word> * wdArray, int numWords, std::string knownChars);
int checkForMatch(std::vector<word> * wdArray, int numWords, std::string currWord);
int saveLine(std::vector<word> * wdArray, int numWords, std::string currLine);
#endif