#include "WordList.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>

WordList::WordList(const WordList & copy) {
    size_ = copy.size_;
    if (copy.wordArray_) {
        wordArray_ = new WordOccurrence[size_];
        for (int i = 0; i < size_; ++i) {
            wordArray_[i] = copy.wordArray_[i];
        }
    }
    else {
        wordArray_ = NULL;
    }
}

WordList& WordList::operator=(const WordList & rhs)
{
    if (this == &rhs) //If the RHS of the argument is the same as the LHS
    {
        return *this;
    }

    delete[] wordArray_;
    size_ = rhs.size_;
    if (rhs.wordArray_) {
        wordArray_ = new WordOccurrence[size_];
        for (int i = 0; i < size_; ++i) {
            wordArray_[i] = rhs.wordArray_[i];
        }
    }
    else {
        wordArray_ = NULL;
    }
    return *this;
}

void WordList::addWord(const std::string & word) {
    for (int i = 0; i < size_; ++i) {
        if (wordArray_[i].matchWord(word)) {
            wordArray_[i].increment();
            return;
        }
    }
    WordOccurrence * newWordArray = new WordOccurrence[size_ + 1];
    for (int i = 0; i < size_; ++i) {
        newWordArray[i] = wordArray_[i];
    }
    newWordArray[size_] = word;
    newWordArray[size_].increment();
    delete[] wordArray_;
    wordArray_ = newWordArray;
    ++size_;
}

void WordList::formatWord(std::string & word) {
    removePunct(word);
    castLowerCase(word);
}

void WordList::removePunct(std::string & word) {
    for (int i = 0; i < word.size(); ++i) {
        if (!isalnum(word[i])) {
            word.erase(i, 1);
        }
    }
}

void WordList::castLowerCase(std::string & word) {
    for (int i = 0; i < word.size(); ++i) {
        if (isupper(word[i])) {
            word[i] = tolower(word[i]);
        }
    }
}

void WordList::insertSort() {
    for (int i = 1; i < size_; ++i) {
        for (int j = i; j > 0 && wordArray_[j - 1].getNum() < wordArray_[j].getNum(); --j) {
            WordOccurrence temp = wordArray_[j];
            wordArray_[j] = wordArray_[j - 1];
            wordArray_[j - 1] = temp;
        }
    }
    for (int i = 1; i < size_; ++i) {
        for (int j = i; j > 0 && wordArray_[j - 1].getNum() == wordArray_[j].getNum(); --j) {
            if (wordArray_[j - 1].getWord() > wordArray_[j].getWord()) {
                WordOccurrence temp = wordArray_[j];
                wordArray_[j] = wordArray_[j - 1];
                wordArray_[j - 1] = temp;
            }
        }
    }
}

void WordList::justify(const std::string & word, int maxCharAmt) {
    auto spaces = maxCharAmt - word.size();
    std::string spaceStr = "";
    for (int i = 0; i < spaces; ++i) {
        std::cout << ' ';
    }
}

void WordList::print() {
    const static int maxDictWordSize = 45;
    for (int i = 0; i < size_; ++i) {
        std::cout << "Word: \"" << wordArray_[i].getWord() << '\"';
        justify(wordArray_[i].getWord(), maxDictWordSize);
        std::cout << "Occurrence(s): " << wordArray_[i].getNum() << '\n';
    }
    std::cout << std::endl;
}

bool equal(const WordList & list1, const WordList & list2) {
    if (list1.size_ != list2.size_) {
        return false;
    }
    for (int i = 0; i < list1.size_; ++i) {
        if (list1.wordArray_[i].getWord() != list2.wordArray_[i].getWord() || list1.wordArray_[i].getNum() != list2.wordArray_[i].getNum()) {
            return false;
        }
    }
    return true;
}