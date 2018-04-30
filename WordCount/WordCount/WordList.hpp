#ifndef WORDLIST_HPP
#define WORDLIST_HPP

#include "WordOccurrence.hpp"

class WordList {
public:
    //CONSTRUCTORS
    WordList() : wordArray_(nullptr), size_(0) {} //Default Constructor
    WordList(const WordList & copy); //Copy Constructor
    ~WordList() { delete[] wordArray_; } //Deconstructor

    //OVERLOADS
    WordList& operator= (const WordList & rhs); //Overloaded Assignment

    //FUNCTIONS
    void addWord(const std::string & word); //Adds a word to the dynamic array.
    void formatWord(std::string & word);
    void removePunct(std::string & word);
    void castLowerCase(std::string & word);
    void insertSort();
    void justify(const std::string & word, int maxCharAmt);
    void print();

    //FRIEND FUNCTIONS
    friend bool equal(const WordList & list1, const WordList & list2);
private:
    WordOccurrence * wordArray_;  //A dynamically allocated array of WordOccurrences which can be sorted or not sorted.
    int size_; //Size of the dynamic array.
};
#endif