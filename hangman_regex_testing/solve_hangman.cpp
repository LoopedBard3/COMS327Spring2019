#include "solve_hangman.h"

#define malloc(size) ({          \
  void *_tmp;                    \
  assert((_tmp = malloc(size))); \
  _tmp;                          \
})

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-d|--dict <dictionary file>] [-n|--numwords <num words>]\n",
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
  mostTopOpen = 10;
  freopen("log.txt", "w", stderr);

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
              (longArg && strcmp(argv[i], "-numword")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%d", &numWords))
          {
            usage(argv[0]);
          }
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
    std::getline(std::cin, lineHold, '\n');
    numWords = std::stoi(lineHold);
  }

  if (numWords == 0)
  {
    if (fileInput.is_open())
      fileInput.close();
    std::cout << "You made it to the end!!" << std::endl;
    return 0;
  };

  //Malloc the word array to the correct size
  //indvWords = (word *)malloc(numWords * sizeof(word));
  word wd;
  for (i = 0; i < numWords; i++)
  {
    indvWords.push_back(wd);
    std::cerr << i << std::endl;
  }

  //at the starting formats
  std::cout << "Please enter the answer format here with - as unknown" << std::endl
            << "Example: ANS = Hello there, Unknown input = ----- -----" << std::endl;
  std::getline(std::cin, lineHold, '\n');
  numWords = saveLine(&indvWords, numWords, lineHold);

  //Start round loop
  while (1)
  {
    if (lineHold == "quit")
      break;

    //Put the regex into the strings
    updateRegex(&indvWords, numWords, triedChars);

    fileInput.clear();
    fileInput.seekg(0, std::ios::beg);
    //Print out the dictionary for testing
    while (mostTopOpen && std::getline(fileInput, lineHold))
    {
      mostTopOpen = checkForMatch(&indvWords, numWords, lineHold);
    }

    //Print out the top matches for the round
    //printMatches(indvWords, numWords);

    //Finish the round loop and at new line
    std::cout << "Currently you have: " << std::endl;
    printCurrentLine(indvWords, numWords);
    std::cout << "Please enter used character, quit to quit, or skip to re-enter knowns: " << std::endl;
    std::getline(std::cin, lineHold, '\n');
    if (lineHold == "quit")
      break;
    else if (lineHold == "skip")
      ;
    else if (lineHold.size() > 1)
      std::cout << "Error atting character, reenter knowns and try again: " << std::endl;
    else
      triedChars += lineHold;
    std::cout << "You have tried: " << triedChars << std::endl;

    std::cout << "Please enter new format with filled knowns or quit to quit: " << std::endl;
    std::getline(std::cin, lineHold, '\n');
    if (lineHold == "quit")
      break;

    //Try over and over to get enough lines
    while (saveLine(&indvWords, numWords, lineHold) < numWords)
    {
      std::cout << "Not enough words, please try again" << std::endl;
      std::cout << "Please enter new format with filled knowns or quit to quit: " << std::endl;
      std::getline(std::cin, lineHold, '\n');
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
  std::cerr << "Checking " << numWords << " words against " << currWord << std::endl;
  uint32_t smallestSize = 100;
  int wordCount;
  for (wordCount = 0; wordCount < numWords; wordCount++)
  {
    std::cerr << "Doing Word: " << currWord << std::endl;
    if (wdArray->at(wordCount).topMatches.size() < 10 && std::regex_match(currWord, wdArray->at(wordCount).reg))
    {
      wdArray->at(wordCount).topMatches.push_back(currWord);
      std::cerr << "Updating regex to: " << currWord << std::endl;
    }
    if(wdArray->at(wordCount).topMatches.size() < smallestSize){
      std::cerr << "Setting size: " <<  wdArray->at(wordCount).topMatches.size() << std::endl;
      smallestSize = wdArray->at(wordCount).topMatches.size();
    }
    std::cerr << "Tested size: " << wdArray->at(wordCount).topMatches.size() << std::endl;    
  }
  return 10 - smallestSize;
}

int saveLine(std::vector<word> *wdArray, int numWords, std::string currLine)
{
  std::stringstream stream(currLine);
  std::string format;
  int counter = 0;
  while (stream >> format && counter < numWords)
  {
    std::cerr << counter << std::endl;
    if (wdArray->at(counter).wordFormat.size() == 0)
    {
      wdArray->at(counter).wordFormat = std::string(format);
      wdArray->at(counter).wordFilled = std::string(format);
      std::cerr << "Saving format: " << wdArray->at(counter).wordFormat << std::endl;
    }
    else
    {
      wdArray->at(counter).wordFilled = std::string(format);
      std::cerr << "Saving filled format: " << wdArray->at(counter).wordFilled << std::endl;
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
    regString = std::regex_replace(wdArray->at(wordCount).wordFilled, std::regex("-"), std::string("[^" + knownChars + "]"));
    wdArray->at(wordCount).reg = "^" + regString + "$";
    std::cerr << "Updating regex to: " << "^" << regString << "$" << std::endl;
  }
}