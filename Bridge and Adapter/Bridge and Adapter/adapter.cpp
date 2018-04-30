// figures: class adapter pattern
// Mikhail Nesterenko
// 2/16/2014

#include <iostream>

using std::cout; using std::cin; using std::endl;

// base interface
class Figure {
public:
    virtual void draw() = 0;
    virtual void resize(int newSize) = 0;
    virtual int size() = 0;
    virtual ~Figure() {}
};

// adaptee/implementer
class LegacyRectangle {
public:
    LegacyRectangle(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) : topLeftX_(topLeftX), topLeftY_(topLeftY), bottomRightX_(bottomRightX), bottomRightY_(bottomRightY) {}

    void oldDraw() {
        for (int i = 0; i < bottomRightY_; ++i) {
            for (int j = 0; j < bottomRightX_; ++j)
                if (i >= topLeftY_ && j >= topLeftX_)
                    cout << '*';
                else
                    cout << ' ';
            cout << endl;
        }
    }
    void move(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
        topLeftX_ = topLeftX;
        topLeftY_ = topLeftY;
        bottomRightX_ = bottomRightX;
        bottomRightY_ = bottomRightY;
    }

    int getTopLeftX() { return topLeftX_; }
    int getTopLeftY() { return topLeftY_; }
    int getBottomRightX() { return bottomRightX_; }
    int getBottomRightY() { return bottomRightY_; }

private: // defining top/left and bottom/right coordinates 
    int topLeftX_;
    int topLeftY_;
    int bottomRightX_;
    int bottomRightY_;
};

// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter : public Figure, private LegacyRectangle {
public:
    SquareAdapter(int size) : LegacyRectangle(0, 0, size, size) {}

    void draw() override {
        oldDraw();
    }
    void resize(int newSize) override { // changes the size of the figure by moving its bottom right corner.
        move(0, 0, newSize, newSize);
    }

    int size() { // returns the current square size;
        int lengthOfSide = abs((getTopLeftX() - getBottomRightX())); // Take the absolute value of the top left x - bottom right x to get the distance between the two points.
        return lengthOfSide * lengthOfSide;
    }
};

int main() {
    cout << "Enter the size of your square: ";
    int userSize = 0; 
    cin >> userSize;
    cout << endl;

    Figure *square = new SquareAdapter(userSize);
    square->draw();
    cout << "Size: " << square->size() << endl << endl;

    cout << "Enter the new size of your square: ";
    cin >> userSize;
    cout << endl << endl;

    square->resize(userSize);
    square->draw();
    cout << "Size: " << square->size() << endl << endl;
}