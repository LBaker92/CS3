// filled/hollow figures demonstrates Bridge Design Pattern,
// square is either hollow or square and is painted with a particular character,
// it is bridged over Figure --> Fill abstract body/handle
// Mikhail Nesterenko
// 2/12/2014

#include <iostream>

using std::cout; using std::endl; using std::cin;

// abstract body
class Fill {
public:
    Fill(char borderChar, char internalChar) : borderFillChar_(borderChar), internalFillChar_(internalChar) {}
    virtual char getBorder() = 0;
    virtual char getInternal() = 0;
    virtual ~Fill() {}
protected:
    char borderFillChar_;
    char internalFillChar_;
};

// concrete body
class Hollow : public Fill {
public:
    Hollow(char borderChar) : Fill(borderChar, ' ') {}
    char getBorder() override { return borderFillChar_; }
    char getInternal() override { return internalFillChar_; }
    ~Hollow() {}
};

// another concrete body
class Filled : public Fill {
public:
    Filled(char fillChar) : Fill(fillChar, fillChar) {}
    char getBorder() override { return borderFillChar_; }
    char getInternal() override { return internalFillChar_; }
    ~Filled() {}
};

// Create another derived paint class EnhancedFilled that inherits from Filled but initializes both paint characters to two different values.
// Demonstrate the operation of all three paint classes by drawing three differently painted squares.

class EnhancedFill : public Fill {
public:
    EnhancedFill() : Fill('+', '-') {}
    char getBorder() override { return borderFillChar_; }
    char getInternal() override { return internalFillChar_; }
    ~EnhancedFill() {}
};

// abstract handle
class Figure {
public:
    Figure(int size, Fill* fill) : size_(size), fill_(fill) {}
    virtual void draw() = 0;
    virtual void changeFill(const Fill* fill) { fill_ = const_cast<Fill *>(fill); }
    virtual ~Figure() {}
protected:
    int size_;
    Fill *fill_;
};

// concrete handle
class Square : public Figure {
public:
    Square(int size, Fill* fill) : Figure(size, fill) {}
    void draw() override;
    ~Square() {}
};

void Square::draw() {
    for (int i = 0; i < size_; ++i) {
        for (int j = 0; j < size_; ++j)
            if (i == 0 || j == 0 || i == size_ - 1 || j == size_ - 1)
                cout << fill_->getBorder();
            else
                cout << fill_->getInternal();
        cout << endl;
    }
}

int main() {
    Fill* hollowPaintY = new Hollow('Y');
    Fill* filledPaintStar = new Filled('*');

    Figure *smallBox = new Square(4, filledPaintStar);
    Figure *bigBox = new Square(10, hollowPaintY);
    Figure *myBox = new Square(5, new EnhancedFill());

    smallBox->draw();
    cout << endl;

    bigBox->draw();
    cout << endl;

    myBox->draw();
    cout << endl;

    smallBox->changeFill(hollowPaintY);
    smallBox->draw();
    cout << endl;

    bigBox->changeFill(filledPaintStar);
    bigBox->draw();
    cout << endl;

    smallBox->changeFill(filledPaintStar);
    smallBox->draw();
    cout << endl;

    bigBox->changeFill(hollowPaintY);
    bigBox->draw();
    cout << endl;

    // ask user for figure parameters
    cout << "Enter fill character: ";
    char fchar; cin >> fchar;
    cout << "Filled or hollow? [f/h] ";
    char ifFilled; cin >> ifFilled;
    cout << "Enter size: "; int size; cin >> size;

    //Figure *userBox = new Square(size, ifFilled == 'f' ? static_cast<Fill *>(new Filled(fchar)) : static_cast<Fill *>(new Hollow(fchar)));
    Figure *userBox = ifFilled == 'f' ? new Square(size, new Filled(fchar)) : new Square(size, new Hollow(fchar));

    userBox->draw();

    delete hollowPaintY;
    delete filledPaintStar;
    delete smallBox;
    delete bigBox;
    delete myBox;
    delete userBox; // This is not being deleted for some reason.
}