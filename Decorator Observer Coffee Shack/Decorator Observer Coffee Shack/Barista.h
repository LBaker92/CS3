#ifndef BARISTA_H
#define BARISTA_H

#include "Drink.h"

#include <string>
#include <iostream>
#include <unordered_set>
#include <list>
#include <iomanip>

//Customer is the observer.
class Customer {
public:
    Customer(const std::string &name, class Barista *b);
    ~Customer();
    void update(Drink *d);
    void setDrink(Drink *d) { d_ = d; }
    Drink* getDrink() const { return d_; }
    void setStatus(bool status) { processedOrder_ = status; }
    bool getStatus() const { return processedOrder_; }
    void setName(const std::string &name) { name_ = name; }
    std::string getName() const { return name_; }
private:
    bool processedOrder_;
    Drink *d_;
    Barista *b_;
    std::string name_;
};

//Barista is the subject
class Barista {
public:
    Barista(Barista *successor = nullptr) : successor_(successor) {}
    virtual ~Barista() {
        delete successor_;
    }
    virtual void attach(Customer *o) { customers_.insert(o); }
    virtual void detach(Customer *o) { customers_.erase(o); }
    virtual void notify() {
        Drink *d = *drinks_.begin();
        std::cout << "An order for " << d->getCustName() << " is ready!\n" << std::endl;
        for (auto e : customers_) {
            e->update(d);
        }
        std::cout << std::endl;
        drinks_.pop_front();
    }
    void takeOrder() {

        Drink *d = nullptr;
        Customer *c = nullptr;

        std::cout << "Welcome to Coffee Shack, can I get you [l]arge, [m]edium, or [s]mall coffee? ";
        char sizeChoice = ' ';
        std::cin >> sizeChoice;
        switch (sizeChoice) {
        case 's':
            d = new Drink(DrinkType::small, 1.00);
            break;
        case 'm':
            d = new Drink(DrinkType::medium, 2.00);
            break;
        case 'l':
            d = new Drink(DrinkType::large, 3.00);
            break;
        default:
            std::cout << "Invalid drink size." << std::endl;
            exit(1);
        }

        char topping = ' ';
        do {
            std::cout << "Would you like to add [s]prinkles, [c]aramel, milk [f]oam, [i]ce or [n]ot? ";
            std::cin >> topping;
            switch (topping) {
            case 's':
                d = new Sprinkles(d);
                d->setToppings(true);
                break;
            case 'c':
                d = new Caramel(d);
                d->setToppings(true);
                break;
            case 'f':
                d = new Foam(d);
                d->setToppings(true);
                break;
            case 'i':
                d = new Ice(d);
                d->setToppings(true);
                break;
            }
        } while (topping != 'n');

        std::string name;
        std::cout << "Can I get your name? ";
        std::cin >> name;

        d->setName(name);
        c = new Customer(name, this);
        c->setDrink(d);

        drinks_.push_back(d);
        prepareDrink(d);
    }
    virtual void prepareDrink(Drink *d) {
        if (successor_ != nullptr) {
            successor_->prepareDrink(d);
        }
        else {
            std::cout << "Sorry, nobody can handle this request." << std::endl;
        }
    }
    std::unordered_set<Customer*> getCustomers() const { return customers_; }
private:
    std::list<Drink*> drinks_;
    std::unordered_set<Customer*> customers_;
    Barista *successor_;
};

class JuniorBarista : public Barista {
public:
    JuniorBarista(Barista *successor = nullptr) : Barista(successor) {}
    void prepareDrink(Drink *d) override {
        if (!d->getToppings()) {
            std::cout << std::endl;
            std::cout << d->getCustName() << ", a Junior Barista is preparing your " << d->Drink::getName() << " and will be ready soon!\n";
            std::cout << "It will be $" << std::fixed << std::setprecision(2) << d->getPrice() << ", please.\n" << std::endl;
        }
        else {
            Barista::prepareDrink(d);
        }
    }
};

class SeniorBarista : public Barista {
public:
    SeniorBarista(Barista *successor = nullptr) : Barista(successor) {}
    void prepareDrink(Drink *d) override {
        if (d->getToppings()) {
            if (d->getName().find("foam") == std::string::npos) {   //If the drink does not contain milk foam
                std::string substring = "coffee";
                int pos = d->getName().find(substring) + substring.size();

                std::string ingredients = d->getName().substr(pos);
                ingredients.erase(ingredients.find_first_of(','), 2);

                std::cout << std::endl;
                std::cout << d->getCustName() << ", a Senior Barista is preparing your " << d->Drink::getName() << " with " << ingredients << " and will be ready soon!\n";
                std::cout << "It will be $" << std::fixed << std::setprecision(2) << d->getPrice() << ", please.\n" << std::endl;
            }
            else {
                Barista::prepareDrink(d);
            }
        }
    }
};

class Manager : public Barista {
public:
    Manager(Barista *successor = nullptr) : Barista(successor) {}
    void prepareDrink(Drink *d) override {
        if (d->getToppings()) {
            std::string substring = "coffee";
            int pos = d->getName().find(substring) + substring.size();

            std::string ingredients = d->getName().substr(pos);
            ingredients.erase(ingredients.find_first_of(','), 2);

            std::cout << std::endl;
            std::cout << d->getCustName() << ", a Manager is preparing your " << d->Drink::getName() << " with " << ingredients << " and will be ready soon!\n";
            std::cout << "It will be $" << std::fixed << std::setprecision(2) << d->getPrice() << ", please.\n" << std::endl;
        }
        else {
            Barista::prepareDrink(d);
        }
    }
};

Customer::Customer(const std::string &name, Barista *b) : name_(name), b_(b), d_(nullptr) {
    b->attach(this);
}

void Customer::update(Drink *d) {
    if (d == d_) {
        std::cout << name_ << ": ### That's my drink! ###";
        this->setStatus(true);
    }
    else {
        std::cout << name_ << ": That's NOT my drink...";
    }
    std::cout << std::endl;
}

Customer::~Customer() {
    delete d_;
    delete b_;
}

#endif