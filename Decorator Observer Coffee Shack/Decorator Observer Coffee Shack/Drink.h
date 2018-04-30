#ifndef DRINK_H_
#define DRINK_H_

// drink class to be used in Coffee Shack lab
// Mikhail Nesterenko
// 11/15/2016

#include <string>

enum class DrinkType { small, medium, large };

class Drink {
public:
    Drink(DrinkType type = DrinkType::small, double price = 0.0) : type_(type), price_(price), hasToppings_(false), name_("") {
        switch (type_) {
        case DrinkType::small:
            size_ = "small";
            break;
        case DrinkType::medium:
            size_ = "medium";
            break;
        case DrinkType::large:
            size_ = "large";
            break;
        }
    }
    bool operator==(const Drink &b) const {
        if (price_ != b.price_) {
            return false;
        }
        if (type_ != b.type_) {
            return false;
        }
        if (size_ != b.size_) {
            return false;
        }
        return true;
    }
    virtual ~Drink() {}
    void setToppings(bool t) { hasToppings_ = t; }
    void setName(const std::string &name) { name_ = name; }
    bool getToppings() const { return hasToppings_; }
    virtual double getPrice() const { return price_; }
    virtual std::string getName() const { return size_ + " coffee"; }
    std::string getSize() const { return size_; }
    std::string getCustName() const { return name_; }
private:
    double price_;
    DrinkType type_;
    std::string size_;
    bool hasToppings_;
    std::string name_;
};

class Sprinkles : public Drink {
public:
    Sprinkles(const Drink* drink) : drink_(drink) {}
    double getPrice() const override { return drink_->getPrice() + 0.50; }
    std::string getName() const override { return drink_->getName() + ", sprinkles"; }
    ~Sprinkles() { delete drink_; }
private:
    const Drink* drink_;
};

class Caramel : public Drink {
public:
    Caramel(const Drink* drink) : drink_(drink) {}
    double getPrice() const override { return drink_->getPrice() + 0.20; }
    std::string getName() const override { return drink_->getName() + ", caramel"; }
    ~Caramel() { delete drink_; }
private:
    const Drink* drink_;
};

class Foam : public Drink {
public:
    Foam(const Drink* drink) : drink_(drink) {}
    double getPrice() const override { return drink_->getPrice() + 0.40; }
    std::string getName() const override { return drink_->getName() + ", foam"; }
    ~Foam() { delete drink_; }

private:
    const Drink* drink_;
};


class Ice : public Drink {
public:
    Ice(const Drink* drink) : drink_(drink) {}
    double getPrice() const override { return drink_->getPrice() + 0.10; }
    std::string getName() const override { return drink_->getName() + ", ice"; }
    ~Ice() { delete drink_; }
private:
    const Drink* drink_;
};

#endif