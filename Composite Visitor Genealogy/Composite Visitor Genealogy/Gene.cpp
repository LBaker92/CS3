// traversing genealogical tree using Composite and Visitors
// Mikhail Nesterenko
// 10/28/2015

#include <iostream>
#include <string>
#include <vector>

using std::cout; using std::endl;
using std::cin;
using std::string;
using std::vector;

class Person { // component
public:
    Person(string firstName, Person *spouse, Person *father, Person *mother) : firstName_(firstName), spouse_(spouse), father_(father), mother_(mother) {}
    string getFirstName() { return firstName_; }
    Person *getSpouse() { return spouse_; }
    void setSpouse(Person *spouse) { spouse_ = spouse; }
    Person *getFather() { return father_; }
    Person *getMother() { return mother_; }

    virtual void accept(class PersonVisitor *) = 0;
    virtual ~Person() {}
private:
    string firstName_;
    Person *spouse_;
    Person *father_;
    Person *mother_;
};

// composites

// man has a last name 
class Man : public Person {
public:
    Man(string lastName, string firstName, Person *spouse, Person *father, Person *mother) : lastName_(lastName), Person(firstName, spouse, father, mother) {}
    string getLastName() { return lastName_; }
    void accept(class PersonVisitor *visitor) override;
private:
    string lastName_;
};

// woman has a list of children
class Woman : public Person {
public:
    Woman(vector<Person *> children, string firstName, Person *spouse, Person *father, Person *mother) : children_(children), Person(firstName, spouse, father, mother) {}
    const vector<Person *> & getChildren() { return children_; }
    void setChildren(const vector<Person *> &children) { children_ = children; }
    void accept(class PersonVisitor *visitor) override;
private:
    vector<Person *> children_;
};

// abstract visitor
class PersonVisitor {
public:
    virtual void visit(Man*) = 0;
    virtual void visit(Woman*) = 0;
    virtual ~PersonVisitor() {}
};

// composite methods that define traversal
void Man::accept(PersonVisitor *visitor) { visitor->visit(this); }

void Woman::accept(PersonVisitor *visitor) {
    // children traversal through mother only
    // father's children are not traversed so as not 
    // to traverse them twice: for mother and father
    visitor->visit(this);
    for (auto child : children_) // traversing descendants
        child->accept(visitor);
}

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her 
// spouse if she is married
// or by her father if she is not
class NamePrinter : public PersonVisitor {
public:
    void visit(Man *m) override {
        cout << m->getFirstName() << " " << m->getLastName() << endl;
    }
    void visit(Woman *w) override {
        cout << w->getFirstName() << " ";
        if (w->getSpouse() != nullptr)
            cout << static_cast<Man *>(w->getSpouse())->getLastName();
        else if (w->getFather() != nullptr)
            cout << static_cast<Man *>(w->getFather())->getLastName();
        else
            cout << "Doe";
        cout << endl;
    }
};

class MaidenNames : public PersonVisitor {
    void visit(Man *m) override {}
    void visit(Woman *w) override {
        Man *spouse = static_cast<Man *>(w->getSpouse());
        Man *father = static_cast<Man *>(w->getFather());
        if (father != nullptr) {    // She has a dad.
            cout << w->getFirstName() << ' ' << father->getLastName() << ' ';
            if (spouse != nullptr) {
                cout << "is married to " << spouse->getFirstName() << ' ' << spouse->getLastName();
            }
            else {
                cout << "is unmarried, father is " << father->getFirstName() << ' ' << father->getLastName();
            }
        }
        else {
            cout << w->getFirstName() << " Doe";
        }
        cout << endl;
    }
};

class ChildrenPrinter : public PersonVisitor {
public:
    void visit(Man *m) override {
        cout << m->getFirstName() << ": ";
        Woman *spouse = static_cast<Woman *>(m->getSpouse());
        if (spouse != nullptr)
            printNames(spouse->getChildren());
        cout << endl;
    }
    void visit(Woman *w) override {
        cout << w->getFirstName() << ": ";
        printNames(w->getChildren());
        cout << endl;
    }
private:
    void printNames(const vector<Person *> &children) {
        for (const auto c : children)
            cout << c->getFirstName() << ", ";
    }
};

class MarriageChecker : public PersonVisitor {
public:

    MarriageChecker() : eligible_(true), firstCandidateFirstName_(""), firstCandidateLastName_(""), secondCandidateFirstName_(""), secondCandidateLastName_("") {}

    void visit(Man *m) override {
        Man *spouse = static_cast<Man *>(m->getSpouse());   // Man's spouse is a Person*
        Man *father = static_cast<Man *>(m->getFather());   // Father is a Person*
        Woman *mother = static_cast<Woman *>(m->getMother());   // Mother is a Person*

        if (nameMatch(m->getFirstName(), m->getLastName())) {   // If the person is a candidate
            if (sameCandidate()) {  // If other candidate is the same person.
                eligible_ = false;
            }
            if (sameFamilyName()) {    // If both candidates share the same last name
                eligible_ = false;
            }
            if (mother != nullptr) {
                string motherLastName = "Doe";
                if (mother->getFather() != nullptr) {
                    motherLastName = static_cast<Man *>(mother->getFather())->getLastName();
                }

                if (related(motherLastName)) {
                    eligible_ = false;
                }
            }

        }
    }

    void visit(Woman *w) override {
        Man *spouse = static_cast<Man *>(w->getSpouse());   // Woman's spouse is Person*
        Man *father = static_cast<Man *>(w->getFather());   // Father is a Person*
        Woman *mother = static_cast<Woman *>(w->getMother());   // Mother is a Person*

        string lastName = "Doe";
        if (spouse != nullptr) {    // Already married
            lastName = spouse->getLastName();
        }
        else if (father != nullptr) {
            lastName = father->getLastName();
        }

        if (nameMatch(w->getFirstName(), lastName)) {   // If the person is a candidate
            if (spouse != nullptr) {
                eligible_ = false;
            }
            if (sameCandidate()) {  // If other candidate is the same person.
                eligible_ = false;
            }
            if (sameFamilyName()) {    // If both candidates share the same last name
                eligible_ = false;
            }
            if (mother != nullptr) {
                string motherLastName = "Doe";
                if (mother->getFather() != nullptr) {
                    motherLastName = static_cast<Man *>(mother->getFather())->getLastName();
                }

                if (related(motherLastName)) {
                    eligible_ = false;
                }
            }
        }
    }

    void setCandidateNames(const string &can1First, const string &can1Last, const string &can2First, const string &can2Last) {
        firstCandidateFirstName_ = can1First;
        firstCandidateLastName_ = can1Last;

        secondCandidateFirstName_ = can2First;
        secondCandidateLastName_ = can2Last;
    }

    void output() {
        if (eligible_) {
            cout << "They can marry." << endl;
        }
        else {
            cout << "They can't marry." << endl;
        }
    }

private:

    bool nameMatch(const string &firstName, const string &lastName) {
        if (firstName == firstCandidateFirstName_ && lastName == firstCandidateLastName_) { // Try to match with the first candidate.
            return true;
        }
        if (firstName == secondCandidateFirstName_ && lastName == secondCandidateLastName_) { // Try to match with the second candidate.
            return true;
        }
        return false;
    }

    bool sameCandidate() {
        if (firstCandidateFirstName_ == secondCandidateFirstName_ && firstCandidateLastName_ == secondCandidateLastName_) {
            return true;
        }
        return false;
    }

    bool sameFamilyName() {
        if (firstCandidateLastName_ == secondCandidateLastName_) {
            return true;
        }
        return false;
    }

    bool related(const string &familyName) {    // Looking at parent's family names
        if (familyName == firstCandidateLastName_ || familyName == secondCandidateLastName_) {
            return true;
        }
        return false;
    }
    bool eligible_;
    string firstCandidateFirstName_;
    string firstCandidateLastName_;
    string secondCandidateFirstName_;
    string secondCandidateLastName_;
};


// demonstrating the operation
int main() {

    // setting up the genealogical tree      
    // the tree is as follows
    //    
    //
    //       James Smith  <--spouse-->   Mary 
    //	                                  |
    //	                                 children -------------------------
    //	                                  |              |                |
    //	                                  |              |                |
    //	   William Johnson <--spouse-> Patricia      Robert Smith       Linda
    //	                                  |
    //	                                 children------------
    //	                                  |                 |
    //                                    |                 |
    //	   Jennifer  <--spouse-->  Michael Johnson      Barbara
    //	       |
    //	     children
    //	       |
    //         |
    //	     Susan


    // first generation
    Man *js = new Man("Smith", "James", nullptr, nullptr, nullptr);
    Woman *ms = new Woman({}, "Mary", nullptr, nullptr, nullptr);
    ms->setSpouse(js);
    js->setSpouse(ms);

    // second generation
    Woman *ps = new Woman({}, "Patricia", nullptr, js, ms);
    Man *wj = new Man("Johnson", "William", nullptr, nullptr, nullptr);
    ps->setSpouse(wj);
    wj->setSpouse(ps);

    vector<Person *> marysKids = { ps, new Man("Smith", "Robert", nullptr, js, ms), new Woman({}, "Linda", nullptr, js, ms) };
    ms->setChildren(marysKids);

    // third generation
    Man *mj = new Man("Johnson", "Michael", nullptr, wj, ps);
    vector<Person *> patsKids = { mj, new Woman({}, "Barbara", nullptr, wj, ps) };
    ps->setChildren(patsKids);

    Woman *jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
    vector<Person *> jensKids = { new Woman({}, "Susan", nullptr, mj ,jj) };

    jj->setSpouse(mj);
    mj->setSpouse(jj);
    jj->setChildren(jensKids);


    // defining two visitors
    ChildrenPrinter *cp = new ChildrenPrinter;
    NamePrinter *np = new NamePrinter;
    MaidenNames *mn = new MaidenNames;

    // executing the traversal with the composite
    // and the specific visitor
    cout << "CHILDREN LIST\n";
    ms->accept(cp);


    cout << "\nNAME LIST\n";
    ms->accept(np);
    cout << endl;

    cout << "\nMAIDEN NAME LIST\n";
    ms->accept(mn);

    cout << endl;

    MarriageChecker *mc = new MarriageChecker;

    string can1First = "", can1Last = "", can2First = "", can2Last = "";
    cout << "Enter first candidate: ";
    cin >> can1First >> can1Last;

    cout << "Enter second candidate: ";
    cin >> can2First >> can2Last;

    mc->setCandidateNames(can1First, can1Last, can2First, can2Last);
    ms->accept(mc);
    mc->output();
}