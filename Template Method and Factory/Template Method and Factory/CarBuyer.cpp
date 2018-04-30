// needed for lab
// Mikhail Nesterenko
// 3/14/2016

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "CarFactory.h"

using std::vector; using std::string;
using std::cout; using std::endl;

class CarLot {
public:
    CarLot();
    void startOver() {
        cars4sale_.clear();
        cars4sale_.shrink_to_fit();
    }

    void nextCar() {
        Car * newCar = factories_[rand() % 4]->requestCar();
        cars4sale_.push_back(newCar);
        currentCar_ = newCar;
    }

    bool lotFull() {
        if (cars4sale_.size() == 4) {
            return true;
        }
        return false;
    }

    Car *testDriveCar() {
        return currentCar_;
    }

    // if a car is bought, requests a new one
    Car *buyCar() {
        startOver();
        vector<string> makeChoices{ "Toyota", "Ford" };
        vector<string> toyotaModels{ "Corolla", "Camry", "Prius", "4Runner", "Yaris" };
        vector<string> fordModels{ "Focus", "Mustang", "Explorer", "F-150" };

        string custMakeChoice = makeChoices[rand() % makeChoices.size()];
        string custModelChoice = "";
        if (custMakeChoice == "Toyota") {
            custModelChoice = toyotaModels[rand() % toyotaModels.size()];
        }
        else {
            custModelChoice = fordModels[rand() % fordModels.size()];
        }

        if (currentCar_ == nullptr) {   //If there are no cars on the lot, dealer gets one for the customer to look at.
            nextCar();
        }

        while (!lotFull()) {    // While the lot have at most 4 cars
            if (currentCar_->getMake() == custMakeChoice && currentCar_->getModel() == custModelChoice) {   // If the car matches what the customer wants, they buy it.
                cout << "Buyer found his make and model!" << endl;
                return currentCar_;
            }
            else {  // Otherwise, the dealer gets a new car for them to look at.
                nextCar();
            }
        }
        cout << "Buyer walks away." << endl;
        return nullptr;
    }

private:
    vector<Car *> cars4sale_; // cars for sale at the lot
    vector<CarFactory *> factories_;
    Car *currentCar_;
};

CarLot::CarLot() {
    // creates 2 Ford factories and 2 Toyota factories 
    factories_.push_back(new FordFactory());
    factories_.push_back(new ToyotaFactory());
    factories_.push_back(new FordFactory());
    factories_.push_back(new ToyotaFactory());

    currentCar_ = nullptr;
}

CarLot *carLotPtr = nullptr; // global pointer instantiation

int main() {
    srand(size_t(time(nullptr)));
    carLotPtr = new CarLot();
    Car *toBuy = carLotPtr->buyCar();
    while (toBuy == nullptr) {  // Keep running buyCar until it finds a car for the buyer.
        toBuy = carLotPtr->buyCar();
    }
}