#include "solve_hangman.h"

#define MAX_MATCHES 10

void usage(char *name)
{
  fprintf(stdout,
          "Usage: %s [-d|--dict <dictionary file>] [-n|--numwords <num words>]\n[-h|--help]",
          name);

  exit(-1);
}

/* Created by Parker Bibus */

int main(int argc, char *argv[])
{

  /* Declare the variables that will be used by the program */
  int customDictionary, longArg, i, numWords, mostTopOpen;
  std::string dictionaryPath;
  std::ifstream fileInput;
  std::string lineHold;
  std::string triedChars;
  std::vector<word> indvWords;

  /* Initialize the variables that will be used by the program */
  customDictionary = longArg = i = numWords = 0;

  /* Use the flags -d or --dict to set a custom dictionary *
   * for using custom types of languages.                  */
  if (argc > 1)
  {
    for (i = 1, longArg = 0; i < argc; i++, longArg = 0)
    {
      if (argv[i][0] == '-')
      { /* All switches start with a dash */
        if (argv[i][1] == '-')
        {
          argv[i]++;   /* Make the argument have a single dash so we can */
          longArg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1])
        {
        case 'd':
          if ((!longArg && argv[i][2]) ||
              (longArg && strcmp(argv[i], "-dict")))
          {
            usage(argv[0]);
          }
          customDictionary = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-')
          {
            dictionaryPath = std::string(argv[++i]);
          }
          break;

        case 'n':
          if ((!longArg && argv[i][2]) ||
              (longArg && strcmp(argv[i], "-numwords")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%d", &numWords))
          {
            usage(argv[0]);
          }
          break;

        case 'h':
          if ((!longArg && argv[i][2]) ||
              (longArg && strcmp(argv[i], "-help")))
          {
            usage(argv[0]);
          }
          usage(argv[0]);
          break;

        default:
          usage(argv[0]);
        }
      }
      else
      { /* No dash */
        usage(argv[0]);
      }
    }
  }

  //Try to load in the dictionary
  if (!customDictionary)
    dictionaryPath = DEFAULT_DICTIONARY_PATH;
  fileInput.open(dictionaryPath, std::ios::in);

  //Make sure the file opened
  if (!fileInput.is_open())
  {
    std::cout << "Could not open file at: " << dictionaryPath << std::endl;
    return 1;
  }

  //Ask for the layout of the word
  if (numWords == 0)
  {
    std::cout << "How many words are there?: " << std::endl;
    std::getline(std::cin, lineHold);
    numWords = std::stoi(lineHold);
  }

  if (numWords == 0)
  {
    if (fileInput.is_open())
      fileInput.close();
    std::cout << "No words to solve!!" << std::endl;
    return 0;
  };

  //Initialize the word array
  word wd;
  for (i = 0; i < numWords; i++)
  {
    indvWords.push_back(wd);
  }

  //at the starting formats
  std::cout << "Please enter the answer format here with - as unknown" << std::endl
            << "Example: ANS = Hello there, Unknown input = ----- -----" << std::endl;
  std::getline(std::cin, lineHold);
  numWords = saveLine(&indvWords, numWords, lineHold);

  //Start round loop
  while (1)
  {
    if (lineHold == "quit")
      break;

    //Put the regex into the strings
    updateRegex(&indvWords, numWords, triedChars);
    clearMatches(&indvWords, numWords);

    fileInput.clear();
    fileInput.seekg(0, std::ios::beg);
    //Check for matches but also replace carriage returns
      mostTopOpen = MAX_MATCHES;
    while (mostTopOpen && std::getline(fileInput, lineHold))
    {
      lineHold = std::regex_replace(lineHold, std::regex("\r"), std::string(""));
      mostTopOpen = checkForMatch(&indvWords, numWords, lineHold);
    }

    //Print out the top matches for the round
    printMatches(indvWords, numWords);

    //Finish the round loop and at new line
    std::cout << "Please enter used character, quit to quit, or skip to re-enter knowns: " << std::endl;
    std::getline(std::cin, lineHold);
    if (lineHold == "quit")
      break;
    else if (lineHold == "skip")
      ;
    else if (lineHold.size() > 1)
      std::cout << "Error atting character, reenter knowns and try again: " << std::endl;
    else
      triedChars += lineHold;
    std::cout << "You have tried: " << triedChars << std::endl;

    std::cout << "Please enter new format with filled knowns, SAME for no changes, or quit to quit: " << std::endl;
    std::cout << "Currently you have: " << std::endl;
    printCurrentLine(indvWords, numWords);
    std::getline(std::cin, lineHold);
    if (lineHold == "quit")
      break;

    //Try over and over to get enough lines
    while (lineHold != "SAME" && lineHold != "same" && saveLine(&indvWords, numWords, lineHold) < numWords)
    {
      std::cout << "Not enough words, please try again" << std::endl;
      std::cout << "Please enter new format with filled knowns or quit to quit: " << std::endl;
      std::getline(std::cin, lineHold);
      if (lineHold == "quit")
        break;
    }
  }

  //Close everything and print out that we finished it.
  if (fileInput.is_open())
    fileInput.close();
  for (; indvWords.size();)
  {
    indvWords.pop_back();
  }
  return 0;
}

int checkForMatch(std::vector<word> *wdArray, int numWords, std::string currWord)
{
  //Check each word for match and if there is a match add it to the word. Return the number of matches left in the lowest word.
  uint32_t smallestSize = MAX_MATCHES;
  int wordCount;
  for (wordCount = 0; wordCount < numWords; wordCount++)
  {
    if (wdArray->at(wordCount).topMatches.size() < MAX_MATCHES && std::regex_match(currWord, wdArray->at(wordCount).reg))
    {
      wdArray->at(wordCount).topMatches.push_back(currWord);
    }
    if (wdArray->at(wordCount).topMatches.size() < smallestSize)
    {
      smallestSize = wdArray->at(wordCount).topMatches.size();
    }
  }
  return MAX_MATCHES - smallestSize;
}

int saveLine(std::vector<word> *wdArray, int numWords, std::string currLine)
{
  std::stringstream stream(currLine);
  std::string format;
  int counter = 0;
  while (stream >> format && counter < numWords)
  {
    if (wdArray->at(counter).wordFormat.size() == 0)
    {
      wdArray->at(counter).wordFormat = std::string(format);
      wdArray->at(counter).wordFilled = std::string(format);
    }
    else
    {
      wdArray->at(counter).wordFilled = std::string(format);
    }
    counter++;
  }
  return counter;
}

void printCurrentLine(std::vector<word> wdArray, int numWords)
{
  int count = 0;
  if (wdArray.at(count).wordFilled.size() == 0)
  {
    //Do the format
    while (count < numWords)
    {
      std::cout << wdArray.at(count).wordFormat << " ";
      count++;
    }
  }
  else
  {
    //Otherwise print the known
    while (count < numWords)
    {
      std::cout << wdArray.at(count).wordFilled << " ";
      count++;
    }
  }
  std::cout << std::endl;
}

void updateRegex(std::vector<word> *wdArray, int numWords, std::string knownChars)
{
  int wordCount;
  std::string regString;
  for (wordCount = 0; wordCount < numWords; wordCount++)
  {
    regString = std::regex_replace(wdArray->at(wordCount).wordFilled, std::regex("-"), std::string("[^" + knownChars + "]")) + "[\r]?$";
    wdArray->at(wordCount).reg = std::regex(regString, std::regex::icase);
    wdArray->at(wordCount).topMatches.clear();
  }
}

void printMatches(std::vector<word> wdArray, int numWords)
{
  int wordCounter;
  uint32_t matchCounter;
  std::string lineHold;
  std::cout << "Top " << MAX_MATCHES << " Matches By Word" << std::endl;
  for (matchCounter = 0; matchCounter < MAX_MATCHES; matchCounter++)
  {
    std::cout << matchCounter+1 << ". ";
    for (wordCounter = 0; wordCounter < numWords; wordCounter++)
    {
      if (wdArray.at(wordCounter).topMatches.size() > matchCounter)
      {
        std::cout << wdArray.at(wordCounter).topMatches.at(matchCounter) + " ";
      }
      else
      {
        std::cout << wdArray.at(wordCounter).wordFilled + " ";
      }
    }
    std::cout << std::endl;
  }
}

void clearMatches(std::vector<word> * wdArray, int numWords){
  uint32_t counter;
  for(counter = 0; counter < wdArray->size(); counter++){
    while(wdArray->at(counter).topMatches.size()){
      wdArray->at(counter).topMatches.at(wdArray->at(counter).topMatches.size() - 1).pop_back();
    }
  }
}