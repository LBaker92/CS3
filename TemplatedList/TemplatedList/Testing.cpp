// testing the implementation of templated list collection
// Mikhail Nesterenko
// 9/10/2015

#include <iostream>
#include <string>
#include "Collection.hpp" // template definition

using std::cout; 
using std::endl;
using std::string;

int main() {

    // manipulating integers
    Collection<int> cone;
    cout << "Integer collection: ";
    cone.AddItem(1);
    cone.AddItem(2);
    cone.AddItem(3);
    cone.AddItem(4);
    cone.AddItem(5);
    cone.AddItem(4);
    cone.PrintCollection();
    cone.RemoveItem(4);
    cout << "Collection after remove: ";
    cone.PrintCollection();

    if (Equal(cone, cone))
        cout << "cone is Equal to itself" << endl;


     // uncomment when you debug the code above


    // manipulating strings
    string sa[] = {"yellow", "orange", "green", "blue"};
    Collection<string> ctwo;

    for(auto s : sa)
    ctwo.AddItem(s);

    cout << "String collection: ";
    ctwo.PrintCollection();


    // manipulating character collections

    // individal collections
    Collection<char> a2g, h2n, o2u;
    for(char c='a'; c <='g'; ++c) a2g.AddItem(c);
    for(char c='h'; c <='n'; ++c) h2n.AddItem(c);
    for(char c='o'; c <='u'; ++c) o2u.AddItem(c);

    if(!Equal(a2g, h2n))
    cout << "a2g is not Equal to h2n" << endl;

    // collection of collections
    Collection<Collection<char>> cpile;

    // adding individual collections
    cpile.AddItem(a2g);
    cpile.AddItem(h2n);
    cpile.AddItem(o2u);

    // printing characters from last collection added
    cout << "Last added character collection: ";
    cpile.LastItem().PrintCollection();
}