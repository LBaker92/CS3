#include "Drink.h"
#include "Barista.h"

#include <iostream>
#include <iomanip>
#include <ctime>


using std::cout; using std::cin; using std::endl;
using std::string;

int main() {
    std::srand(std::time(nullptr));
    Barista *b = new JuniorBarista(new SeniorBarista(new Manager(nullptr)));
    Drink *d = nullptr;

    while (true) {
        b->takeOrder();
        int rand = std::rand() % 3;

        if (rand == 0) {
            b->takeOrder();
        }
        else {
            b->notify();
            for (const auto &e : b->getCustomers()) {
                if (e->getStatus()) {
                    b->detach(e);
                }
            }
        }
    }
}