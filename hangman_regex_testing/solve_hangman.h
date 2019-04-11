#ifndef SOLVE_HANGMAN_H
#define SOLVE_HANGMAN_H

#include <cassert>
#include <cstring> 
#include <iostream> 
#include <fstream>
#include <stdio.h>
#include <string>
#include <regex>

#define DEFAULT_DICTIONARY_PATH "words"

class word {
    public:
        std::string topMatches[10];
        std::string wordFormat;
        std::string wordFilled;
        std::regex reg;
        int numTopMatches;
};
int checkForMatch(word wdArray[], int numWords, std::string currWord);
int saveLine(word wdArray[], int numWords, std::string currLine);

#endif