#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

//Struct for character entry, holds name, rank in first file, rank in second file, and difference of ranks
struct charaEntry {
   string name;
   int firstRank = 0;
   int secondRank = 0;
   int diff = 0;
};

void loadEntries(vector<charaEntry> &entries, int &avgChange);
void getCharStats(vector<charaEntry> &entries);
void getLargestFalls(vector<charaEntry> &entries, vector<charaEntry> &lowest);
void getLargestRises(vector<charaEntry> &entries, vector<charaEntry> &highest);
void getNoChange(vector<charaEntry> &entries);

int main()
{
   //Vectors for holding all entries, greatest falls in rank, and greatest rises in rank
   vector<charaEntry> entries;
   vector<charaEntry> lowest;
   vector<charaEntry> highest;
   int avgChange = 0;

   //getting data from sorter files and filling vectors
   loadEntries(entries, avgChange);
   getLargestFalls(entries, lowest);
   getLargestRises(entries, highest);
   //Printing characters from highest and lowest vectors & average change in rank
   vector<charaEntry>::iterator it;
   cout << "The following character(s) fell " << abs(lowest.front().diff) << " ranks:" << endl;
   for (it = lowest.begin(); it != lowest.end(); it++) {
      cout << it->name << endl;
   }
   cout << endl;
   cout << "The following character(s) rose " << highest.front().diff << " ranks:" << endl;
   for (it = highest.begin(); it != highest.end(); it++) {
      cout << it->name << endl;
   }
   cout << endl;
   getNoChange(entries);
   cout << "The average rank change between the two sorters is " << avgChange <<  " ranks." << endl;

   bool getChar = true;
   string in;
   while (getChar) {
      getCharStats(entries);
      cout << "Do you want to check another character? (Y/N) ";
      cin >> in;
      if (in == "N")
      getChar = false;
   }
   
}

//Function for loading charaEntries from files into vector passed by reference
//Also computes average change in rank across all entries
void loadEntries(vector<charaEntry> &entries, int &avgChange) {
   //basic charaEntry for loading data and inserting into vector
      struct charaEntry entry;

   //file streams for sorter files
   ifstream firstFile;
   ifstream secondFile;
   string path = "./sorters/";
   cout << "Sorter files in directory: " << endl;
   for (const auto &entry : fs::directory_iterator(path)) {
      cout << entry << endl;
   }
   //Getting two files to compare
   string firstFilename;
   string secondFilename;
   cout << "Enter the first file: ";
   getline(cin, firstFilename);
   cout << "Enter the second file: ";
   getline(cin, secondFilename);
   //Opening files if dates are valid
   firstFile.open("sorters/" + firstFilename);
   secondFile.open("sorters/" + secondFilename);

   //Strings for reading lines of each file
   string firstLine;
   string secondLine;

   //Staring w/ second file because it may have more entries
   //Parsing chara name and second rank from file and inserting entry into vector
   while (getline(secondFile, secondLine)) {
      entry.name = secondLine.substr(0, secondLine.find('/'));
      entry.secondRank = stoi(secondLine.substr(secondLine.find('/') + 1, secondLine.length()));
      entries.push_back(entry);
   }
   secondFile.close();

   //For first file, searches through vector for entry w/ matching name
   //When found, stores first rank and calculated rank difference
   //Positive diff means chara moved up, negative diff means chara moved down
   while (getline(firstFile, firstLine)) {
      for (vector<charaEntry>::iterator it = entries.begin(); it != entries.end(); it++) {

         if (it -> name == firstLine.substr(0, firstLine.find('/'))) {
            it -> firstRank = stoi(firstLine.substr(firstLine.find('/') + 1, firstLine.length()));
            it -> diff = it->firstRank - it->secondRank;
            avgChange += abs(it -> diff);
         }
      }
   }
      firstFile.close();
      avgChange = avgChange / entries.size();
}

//Function for getting stats of specific character
void getCharStats(vector<charaEntry> &entries) {

   //Getting user input for character 
   string character;
   cout << "Enter a character to get stats of: ";
   getline(cin, character);

   //Searching through vector to find entered character
   //If found, prints ranks and change in rank
   vector<charaEntry>::iterator it;
   for (it = entries.begin(); it != entries.end(); it++) {
      if (it->name == character) {
         cout << "First rank: " << it->firstRank << endl;
         cout << "Second rank: " << it->secondRank << endl;
         if (it-> diff < 0) {
            cout << character + " went down " + to_string(abs(it->diff)) + " ranks" << endl;
         }
         else if (it->diff > 0) {
            cout << character + " went up " + to_string(it->diff) + " ranks" << endl;
         }
         else
            cout << character + "did not change rank";
         break;
      }
   }

   //If iterator reaches end, character input is invalid
   if (it == entries.end())
      cout << "Invalid character input" << endl;
}

//Function for getting characters with largest fall in rank between the two sorts
void getLargestFalls(vector<charaEntry> &entries, vector<charaEntry> &lowest) {
   int largestFall = INT_MAX;
   for (vector<charaEntry>::iterator it = entries.begin(); it != entries.end(); it++) {
      if (it->diff < largestFall) {
         largestFall = it->diff;
         lowest.clear();
         lowest.push_back(*it);
      }
      else if (it->diff == largestFall) {
         lowest.push_back(*it);
      }
   }
}

//Function for getting characters with largest rise in rank between the two sorts
void getLargestRises(vector<charaEntry> &entries, vector<charaEntry> &highest) {
   int largestRise = INT_MIN;
   for (vector<charaEntry>::iterator it = entries.begin(); it != entries.end(); it++) {
      if (it->diff > largestRise) {
         largestRise = it->diff;
         highest.clear();
         highest.push_back(*it);
      }
      else if (it->diff == largestRise) {
         highest.push_back(*it);
      }
   }
}

void getNoChange(vector<charaEntry> &entries) {
   cout << "The following characters did not change rank:" << endl;
   for (vector<charaEntry>::iterator it = entries.begin(); it != entries.end(); it++) {
      //If firstRank = 0, character was not in first sorter, so skip entry
      if (it->diff == 0 && it->firstRank != 0) {
         cout << it->name << endl;
      } 
     }
     cout << endl;
}