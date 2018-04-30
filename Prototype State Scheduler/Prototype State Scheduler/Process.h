#include <iostream>
#include <string>
#include <queue>
#include <ctime>
#include <stdlib.h>
#include <vector>

using std::queue;

using std::cin; using std::cout; using std::endl; using std::string;

class State;

class Process {
public:
    Process();
    Process* clone() { return new Process; }
    static int ID;

    void admitted();
    void interrupt();
    void exit();
    void eventCompletion();
    void schedulerDispatch();
    void eventWait();
    string report();

    void changeState(State* s) { state_ = s; }

private:
    State * state_;
    int ID_;
};

class State {
public:
    virtual void admitted(Process*) {}
    virtual void interrupt(Process*) {}
    virtual void exit(Process*) {}
    virtual void eventCompletion(Process*) {}
    virtual void schedulerDispatch(Process*) {}
    virtual void eventWait(Process*) {}

    virtual string report() = 0;

    void changeState(Process* p, State* s) {
        p->changeState(s);
    }
};

class New : public State {
public:
    static State* instance() {
        static State* onlyInstance = new New;
        return onlyInstance;
    }
    void admitted(Process *p);

    string report() { return "New"; }
private:

    // here and elsewhere should be stated private constructor/assignment
    // to correctly implement singleton, skipped to simplify code
};

class Ready : public State {
public:
    static State* instance() {
        static State* onlyInstance = new Ready;
        return onlyInstance;
    }

    void schedulerDispatch(Process *c);

    string report() { return "Ready"; }
};

class Running : public State {
public:
    static State* instance() {
        static State* onlyInstance = new Running;
        return onlyInstance;
    }

    void exit(Process *c);
    void interrupt(Process *c);
    void eventWait(Process *c);

    string report() { return "Running"; }
};

class Terminated : public State {
public:
    static State* instance() {
        static State* onlyInstance = new Terminated;
        return onlyInstance;
    }

    string report() { return "Terminated"; }
};

class Waiting : public State {
public:
    static State* instance() {
        static State* onlyInstance = new Waiting;
        return onlyInstance;
    }

    void eventCompletion(Process *c);

    string report() { return "Waiting"; }
};

void New::admitted(Process *c) { changeState(c, Ready::instance()); }

void Ready::schedulerDispatch(Process *c) { changeState(c, Running::instance()); }

void Running::exit(Process *c) { changeState(c, Terminated::instance()); }
void Running::interrupt(Process *c) { changeState(c, Ready::instance()); }
void Running::eventWait(Process *c) { changeState(c, Waiting::instance()); }

void Waiting::eventCompletion(Process *c) { changeState(c, Ready::instance()); }

void Process::admitted() { state_->admitted(this); }
void Process::interrupt() { state_->interrupt(this); }
void Process::exit() { state_->exit(this); }
void Process::eventCompletion() { state_->eventCompletion(this); }
void Process::schedulerDispatch() { state_->schedulerDispatch(this); }
void Process::eventWait() { state_->eventWait(this); }

Process::Process() { ID++; ID_ = ID;  state_ = New::instance(); }

string Process::report() {
    string report = "Process " + std::to_string(ID_);
    report = report + " is " + state_->report();
    cout << report << std::endl;
    return report;
}