#ifndef WORDOCCURRENCE_HPP
#define WORDOCCURRENCE_HPP

#include <string>

class WordOccurrence {
public:
    //CONSTRUCTORS
    WordOccurrence(const std::string & word = "", int num = 0) : word_(word), num_(num) {} //Default Constructor with default parameters.

    //ACCESSORS
    std::string getWord() const { return word_; }
    int getNum() const { return num_; }

    //FUNCTIONS
    bool matchWord(const std::string & word) { return word == word_; } //Returns true if the word matches the stored word.
    void increment() { ++num_; }
private:
    std::string word_;
    int num_;
};
#endif