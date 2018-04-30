// Games, Template Method example
// Mikhail Nesterenko
// 2/4/2014

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>

using std::cout; using std::endl;

// template for any game where players take turns to make moves
// and there is a winner
class Game {
public:
    Game() : playersCount_(0), movesCount_(0), playerWon_(noWinner) {}

    // template method
    void playGame(const int playersCount = 0) {
        playersCount_ = playersCount;
        movesCount_ = 0;

        initializeGame();

        for (int i = 0; !endOfGame(); i = (i + 1) % playersCount_) {
            makeMove(i);
            if (i == playersCount_ - 1) ++movesCount_;
        }

        printWinner();
    }

    virtual ~Game() {}

protected:
    // primitive operations
    virtual void initializeGame() = 0;
    virtual void makeMove(int player) = 0;
    virtual void printWinner() = 0;
    virtual bool endOfGame() { return playerWon_ != noWinner; } // this is a hook
                                                                // returns true if winner is decided
    static const int noWinner = -1;

    int playersCount_;
    int movesCount_;
    int playerWon_;
};

// Monopoly - a concrete game implementing primitive 
// operations for the template method
class Monopoly : public Game {
public:
    // implementing concrete methods
    void initializeGame() {
        playersCount_ = rand() % numPlayers_ + 1; // initialize players
    }

    void makeMove(int player) {
        if (movesCount_ > minMoves_) {
            const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
            if (chance < movesCount_) {
                playerWon_ = player;
            }
        }
    }

    void printWinner() {
        cout << "Monopoly, player " << playerWon_ << " won in "
            << movesCount_ << " moves." << endl;
    }

private:
    static const int numPlayers_ = 8; // max number of players
    static const int minMoves_ = 20; // nobody wins before minMoves_
    static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing primitive operations
class Chess : public Game {
public:
    void initializeGame() {
        playersCount_ = numPlayers_; // initalize players
        for (int i = 0; i < numPlayers_; ++i)
            experience_[i] = rand() % maxExperience_ + 1;
    }

    void makeMove(int player) {
        if (movesCount_ > minMoves_) {
            const int chance = (rand() % maxMoves_) / experience_[player];
            if (chance < movesCount_) playerWon_ = player;
        }
    }

    void printWinner() {
        cout << "Chess, player " << playerWon_
            << " with experience " << experience_[playerWon_]
            << " won in " << movesCount_ << " moves over"
            << " player with experience " << experience_[playerWon_ == 0 ? 1 : 0]
            << endl;
    }

private:
    static const int numPlayers_ = 2;
    static const int minMoves_ = 2; // nobody wins before minMoves_
    static const int maxMoves_ = 100; // somebody wins before maxMoves_
    static const int maxExperience_ = 3; // player's experience
                                         // the higher, the greater probability of winning
    int experience_[numPlayers_];
};

class Dice : public Game {
public:
    Dice() : highScore_{ 0, 0 }, passed_{ false, false }, name_{ "Computer", "You" } {}

    void initializeGame() {
        playersCount_ = numPlayers_;
    }

    bool endOfGame() {
        if (movesCount_ == maxMoves_) {
            if (highScore_[0] > highScore_[1] || highScore_[0] == highScore_[1]) {  //If PC wins or ties with you
                playerWon_ = 0;
            }
            else {
                playerWon_ = 1;
            }
            return true;
        }
        if (passed_[0] && passed_[1]) {
            return true;
        }
        return false;
    }

    void makeMove(int player) {
        if (!endOfGame()) {
            if (player == 0) {
                std::cout << "Computer rolled: ";
                if (!passed_[player]) {
                    int rollTotal = 0;
                    for (int i = 0; i < 5; ++i) {
                        const int roll = rand() % 6 + 1;
                        rollTotal += roll;
                        std::cout << roll << ' ';
                    }
                    if (rollTotal > highScore_[player]) {
                        highScore_[player] = rollTotal;
                    }
                    std::cout << "= " << rollTotal << ", computer's highest score = " << highScore_[player] << std::endl;
                    const int choice = rand() % 2;
                    if (choice == 0) {
                        passed_[player] = false;
                    }
                    else {
                        passed_[player] = true;
                    }
                }
                else {
                    std::cout << "passed, computer's highest score = " << highScore_[player] << std::endl;
                }
            }
            if (player == 1) {
                std::cout << "You rolled: ";
                if (!passed_[player]) {
                    int rollTotal = 0;
                    for (int i = 0; i < 5; ++i) {
                        const int roll = rand() % 6 + 1;
                        rollTotal += roll;
                        std::cout << roll << ' ';
                    }
                    if (rollTotal > highScore_[player]) {
                        highScore_[player] = rollTotal;
                    }
                    std::cout << "= " << rollTotal << ", your highest score = " << highScore_[player] << std::endl;
                    char choice = ' ';
                    std::cout << "Reroll?[y/n]: ";
                    std::cin >> choice;
                    if (choice == 'y') {
                        passed_[player] = false;
                    }
                    else {
                        passed_[player] = true;
                    }
                }
                else {
                    std::cout << "passed, your highest score = " << highScore_[player] << std::endl;
                }
            }
        }
    }

    void printWinner() {
        if (playerWon_ == 0) {
            std::cout << "YOU LOSE";
        }
        else {
            std::cout << "YOU WIN";
        }
        std::cout << std::endl << std::endl;
    }

private:
    static const int numPlayers_ = 2;
    static const int minRoll_ = 5;
    static const int maxRoll_ = 30;
    static const int maxMoves_ = 3;

    int highScore_[numPlayers_];
    bool passed_[numPlayers_];
    std::string name_[numPlayers_];
};

int main() {
    srand(time(nullptr));

    Game* gp = nullptr;

    // play chess 10 times
    for (int i = 0; i < 10; ++i) {
        gp = new Chess;
        gp->playGame();
        delete gp;
    }

    std::cout << std::endl;

    // play monopoly 5 times
    for (int i = 0; i < 5; ++i) {
        gp = new Monopoly;
        gp->playGame();
        delete gp;
    }

    std::cout << std::endl;

    // play dice 5 times
    for (int i = 0; i < 5; ++i) {
        gp = new Dice;
        gp->playGame();
        delete gp;
    }
    std::cout << std::endl;

}