// sorting apples
// non STL-algorithm code to be replaced by algorthms
// Mikhail Nesterenko
// 2/16/2018


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Apples {
    double weight; // oz
    string color;  // red or green
    void print() const { cout << color << ", " << weight << endl; }
};

// Apples RandomApple();

int main() {
    srand(unsigned(time(nullptr)));
    constexpr double minWeight = 8.;
    constexpr double maxWeight = 3.;

    cout << "Input crate size: ";
    int size;
    cin >> size;

    vector <Apples> crate(size);

    //assign random weight and color to apples in the crate
    //replace with generate()
    auto RandomApple = [=]() {
        Apples a;
        a.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight);
        a.color = rand() % 2 == 1 ? "green" : "red";
        return a;
    };
    std::generate(crate.begin(), crate.end(), RandomApple);

    cout << "Enter weight to find: ";
    double toFind;
    cin >> toFind;

    // count_if()
    auto greater_than = [=](const Apples& a) { return a.weight > toFind; };
    auto count = std::count_if(crate.begin(), crate.end(), greater_than);
    cout << "There are " << count << " apples heavier than " << toFind << " oz" << endl;

    // find_if()
    auto it = std::find_if(crate.begin(), crate.end(), greater_than);
    if (it != crate.end()) {
        cout << "At positions: ";
        do {
            cout << it - crate.begin() << ", ";
            it = find_if(it + 1, crate.end(), greater_than);
        } while (it != crate.end());
    }
    cout << endl;

    // max_element()
    auto less_than = [=](const Apples& a, const Apples& b) { return a.weight < b.weight; };
    auto heaviest = max_element(crate.begin(), crate.end(), less_than);
    cout << "The heaviest apple weights: " << heaviest->weight << " oz." << endl;

    // for_each() or accumulate()
    double sum = 0.0;
    auto total = [&](const Apples a) { return sum += a.weight; };
    std::for_each(crate.begin(), crate.end(), total);
    cout << "The weight of all apples in the crate is: " << sum << " oz." << endl;

    // transform();
    cout << "How much should they grow: ";
    double toGrow = 0.0;
    cin >> toGrow;

    auto rate = [=](Apples& a) { 
        a.weight += toGrow; 
        return a;
    };
    std::transform(crate.begin(), crate.end(), crate.begin(), rate);

    // remove_if()
    cout << "Input minimum acceptable weight: ";
    double minAccept;
    cin >> minAccept;
    auto min = [=](Apples& a) { return a.weight < minAccept; };
    auto remove_it = std::remove_if(crate.begin(), crate.end(), min);
    crate.erase(remove_it, crate.end());
    cout << "removed " << size - crate.size() << " elements" << endl;

    // bubble sort, replace with sort()
    std::sort(crate.begin(), crate.end(), less_than);

    // moving all red apples from crate to peck
    // remove_copy_if() with back_inserter() or front_inserter()
    deque<Apples> peck;
    auto apple_not_green = [](const Apples a) { return a.color == "green"; };   //If the apple is green, it is excluded.
    std::remove_copy_if(crate.begin(), crate.end(), std::back_inserter(peck), apple_not_green);

    auto apple_not_red = [](const Apples a) { return a.color == "red"; };   //Apple is red, it is excluded.
    auto new_end = std::remove_if(crate.begin(), crate.end(), apple_not_red);
    crate.erase(new_end, crate.end());

    // for_each() possibly
    int apples_in_crate = 0;
    auto count_crate = [&](const Apples a) { return ++apples_in_crate; };
    cout << "apples in the crate" << endl;
    std::for_each(crate.begin(), crate.end(), count_crate);
    std::cout << apples_in_crate << std::endl;

    // for_each() possibly
    int apples_in_peck = 0;
    auto count_peck = [&](const Apples a) { return ++apples_in_peck; };
    cout << "apples in the peck" << endl;
    std::for_each(peck.begin(), peck.end(), count_peck);
    std::cout << apples_in_peck << std::endl;
}