#include "WordList.hpp"
#include "WordOccurrence.hpp"
#include <string>
#include <fstream>
#include <ctype.h>
#include <algorithm>
#include <iostream>


int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cout << "Wrong amount of command line arguments given.";
    }

    WordList wList;
    WordOccurrence wOccur;
    std::ifstream inFile(argv[1]);
    if (!inFile) {
        std::cout << "Error opening file!";
    }
    std::string fileStr = "";
    while (inFile >> fileStr) {
        wList.removePunct(fileStr);   //Scans the string for punctuation and removes it.
        wList.castLowerCase(fileStr);
        wList.addWord(fileStr);
    }
    wList.insertSort();
    wList.print();
}