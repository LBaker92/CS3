/*PROJECT:  Procedural Difference
  AUTHOR:   Logan Baker
  DATE:     01-18-2018
  DESC:     Implement a program that compares two text files character-by-character. The program should output the line numbers that contain different characters and the place where the two lines start to              differ (the difference is denoted by ^ (caret) character. If both files are identical, the program should output nothing.
*/

#include <iostream>
#include <string>
#include <fstream>

const std::string textFormatting = ": ";

int spacesBeforeString(const char* fileName, const int lineNumber); //Returns the total number of spaces needed before the beginning of a string.
int stringDiffIndex(const std::string &firstString, const std::string &secondString); //Returns the index of the element where two strings are different.
std::string stringOfSpaces(const int numOfSpaces);

int main(int argc, char* argv[]) {
    //If the user did not follow the right format, return with code 1.
    //IE: ./diff file1.txt file2.txt
    if (argc != 3) {
        return 1;
    }
    std::ifstream file1(argv[1]), file2(argv[2]); //Construct two ifsteam objects that open the two files the user specified.

    //Check if the files were opened correctly, if not, return with code 1.
    if (!file1) {
        return 1;
    }
    if (!file2) {
        return 1;
    }

    int lineCount = 1;
    bool finished = false; //Looping variable becomes true if both files are at their ends.
    while (!finished) {
        std::string f1String = "", f2String = "";
        //Put each line from the two files into these strings.
        std::getline(file1, f1String);
        std::getline(file2, f2String);
        if (f1String != f2String) { //If the two strings are not the same
            int spaces = spacesBeforeString(argv[1], lineCount) + stringDiffIndex(f1String, f2String); //Gets the total number of spaces before the string + the index where two strings differ.
            std::string spaceString = stringOfSpaces(spaces);
            std::cout << argv[1] << textFormatting << lineCount << textFormatting << f1String << '\n';
            std::cout << argv[2] << textFormatting << lineCount << textFormatting << f2String << '\n';
            std::cout << spaceString << '^' << std::endl;
        }
        ++lineCount;
        if (file1.eof() && file2.eof()) {
            finished = true;
        }
    }
    file1.close();
    file2.close();
}

int spacesBeforeString(const char* fileName, const int lineNumber) {
    std::string totalLine = fileName + textFormatting + std::to_string(lineNumber) + textFormatting;
    return totalLine.length();
}

int stringDiffIndex(const std::string &firstString, const std::string &secondString) {
    int index = 0;
    for (const char &e : firstString) {
        if (e != secondString[index]) {
            return index;
        }
        ++index;
    }
    return index;
}

std::string stringOfSpaces(const int numOfSpaces) {
    std::string spaceString = "";
    for (int i = 0; i < numOfSpaces; ++i)
    {
        spaceString += ' ';
    }
    return spaceString;
}