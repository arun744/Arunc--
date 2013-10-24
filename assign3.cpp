#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>
#include <vector>
#include "bintree.h"

using namespace std;

// fill the tree with dictionary text file
void filltree(bintree<string> &treenode, const char * file);

//check for spelling against dictionary file
void checkspelling(bintree<string> & treenode, ifstream &fin);

//check for numbers
bool isNumeric(string stringToCheck);

//get words from the line of strings
void stringtoken (const string& str,
      vector<string>& tokens, const string& delimiters);

// extract word and find decimal values
void extractword(const string& getwords, vector<string>& words);

//check for decimal values
bool isDecimal(string line);

int main(int argc, char* argv[])
{
   if (argc < 2) 
   {
      cout << "Syntax : ./a.out textfile" << '\n';
      return 0;
   }

   ifstream infile(argv[1]);
   if (!infile.is_open()) 
   {
      cout << "ERROR - Unable to access " << argv[1] << '\n';
      return 0;
   }

   bintree<string> dictree;

   filltree(dictree, "dict.txt");
   checkspelling(dictree, infile);

   return 0;
}

// read the file and add to tree datastructure 

void filltree(bintree<string> &treenode, const char *file)
{
   ifstream in_file;
   string line;

   in_file.open(file);

   if (!in_file)
   {
      cout << "Unable to read from " << file << "\n";
      exit(0);
   }

   while (in_file.peek()!= EOF)
   {
      getline(in_file, line);
      treenode.insert(line);
   }
}

//convert words to lower character

string to_Lower(string words)
{
   for (int index = 0; index <(int)words.length(); index++)
      //convert to lower character
      words[index] = tolower(words[index]);
   return words;
}

// get words from line for strings

void stringtoken (const string& str,
    vector<string>& tokens, const string& delimiters)
{
   // find the beginning of the token
   unsigned int lastPos = str.find_first_not_of(delimiters, 0);
   
   // find end of the token
   unsigned int pos = str.find_first_of(delimiters, lastPos);

   while (string::npos != pos || string::npos != lastPos)
   {
      // word found add to vector
      string words = str.substr(lastPos, pos - lastPos);
      tokens.push_back(words);

      // Skip delimiters
      lastPos = str.find_first_not_of(delimiters, pos);

      // Find next "non-delimiter character"
      pos = str.find_first_of(delimiters, lastPos);
   }
}

// get words from the string

void extractword(const string& getwords, vector<string>& words)
{
   vector<string> tokens;
   
   const string delimiter = ". \t\r\n,;:\"~!#%^*()=+[]{}\\|<>?/";
   stringtoken(getwords, tokens, " \t\n,:!<>#\"()");

   for (int index = 0; index < (int)tokens.size() ; index++)
   {
      if(isDecimal(tokens[index]))
      {
         words.push_back(tokens[index]);
      }
      else
      {
         stringtoken(tokens[index], words, delimiter);
      }
   }
}

//check for decimal point

bool isDecimal(string line)
{ 
   for (int index = 0; index < (int)line.length(); index++)
   {
      char Prevchar = line[index -1];
      char Nextchar = line[index + 1];
      char current = line[index];
   
      if((Prevchar == ' '|| Prevchar == '\t' || Prevchar == '\r'
         || Prevchar == '\n' || isdigit(Prevchar))
         && current == '.' && isdigit(Nextchar))
         return true;

      if(isalpha(Prevchar) && current == '.' && (isdigit(Nextchar)))
         return false;
   }
   return false; 
}

bool isNumeric(string stringToCheck)
{
   // check for whole number and real number
   char *stringPtr;
   if (strtod(stringToCheck.c_str(), &stringPtr;) == 0)
      return false;

   int delim = 0;

   for (int index = 0; index < (int)stringToCheck.length(); index++)
      if (stringToCheck[index] == '.')
         delim++;
   if (delim > 1)
      return false;
   return true;
}

// read the testing file and check for spelling

void checkspelling(bintree<string> & treenode, ifstream &fin)
{
   string getwords;
   int unknownwords = 0;
   int totalwords =0;
   vector<string> words;

   do
   {
      getline(fin, getwords);

      //extract word from the file
      extractword(getwords, words);
   } while(fin.peek()!= EOF);

   for (int index = 0; index < (int)words.size(); index++)
   {
      //check for spelling against the dictionary file
      if (!isNumeric(words[index]) &&
         treenode.find(to_Lower(words[index])) == NULL)
      {
         cout << "Unknown word | "<<words[index] << endl;
         unknownwords++;
      }
      totalwords++;
   }

   cout << "A total of "  << totalwords 
      << " words were found in the file"<< endl;
   cout << unknownwords << " had unknown spelling " << endl;
}

