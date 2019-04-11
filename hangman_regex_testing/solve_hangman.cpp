#include "solve_hangman.h"


#define malloc(size) ({          \
  void *_tmp;                    \
  assert((_tmp = malloc(size))); \
  _tmp;                          \
})

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-d|--dict <dictionary file>]\n",
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
  word *indvWords;

  /* Initialize the variables that will be used by the program */
  customDictionary = longArg = i = mostTopOpen = 0;

  /* Use the flags -d or --dict to set a custom dictionary *
   * for using custom types of languages.                  */
 if (argc > 1) {
    for (i = 1, longArg = 0; i < argc; i++, longArg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          longArg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 'd':
          if ((!longArg && argv[i][2]) ||
              (longArg && strcmp(argv[i], "-dict"))) {
            usage(argv[0]);
          }
          customDictionary = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            dictionaryPath = std::string(argv[++i]);
          }
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
  }

  //Try to load in the dictionary
  if(!customDictionary) dictionaryPath = DEFAULT_DICTIONARY_PATH;
  fileInput.open(dictionaryPath, std::ios::in);

  //Make sure the file opened
  if(!fileInput.is_open()){
    std::cout << "Could not open file at: " << dictionaryPath << std::endl;
  }

  //Ask for the layout of the word
  numWords = 0;

  //Malloc the word array to the correct size
  indvWords = (word *) malloc(numWords * sizeof(word));

  //Get the starting formats

  //Start round loop

  //Print out the dictionary for testing
  while(std::getline(fileInput, lineHold) && mostTopOpen){
    checkForMatch(indvWords, numWords, lineHold);
  }

  //Print out the top matches for the round

  //Finish the round loop

  //Close everything and print out that we finished it.
  if(fileInput.is_open()) fileInput.close();
  std::cout << "You made it to the end!!" << std::endl;
  return 0;
}

int checkForMatch(word wdArray[], int numWords, std::string currWord){
  int matchesLeft = 10;
  //Check each word for match and if there is a match add it to the word. Return the number of matches left in the lowest word.
  std::cout << "Checking " << numWords << " words against" << currWord << std::endl;
  return 1;
}
