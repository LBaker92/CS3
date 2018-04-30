#include <iostream>
#include <string>
#include <queue>
#include <ctime>
#include <stdlib.h>
#include <vector>
#include "Process.h"

using std::queue;

using std::cin; using std::cout; using std::endl; using std::string;

int Process::ID = -1;

int main() {
    queue<Process*> processes;
    Process prototype;
    srand(static_cast<unsigned int>(time(nullptr)));
    std::vector<Process*> waiting;

    //prototype.initializeID(0);

    for (int i = 0; i < 4; i++) {
        processes.push(prototype.clone());
        processes.back()->admitted();
        processes.back()->report();
    }

    //processes.front()->schedulerDispatch();

    while (!processes.empty()) {
        cout << "--------------------------\n";

        int num = rand();
        int processChance = num % 3;

        processes.front()->schedulerDispatch();
        processes.front()->report();

        if (!waiting.empty()) {
            int complete = (num % (waiting.size()));
            processes.push(waiting[complete]);
            processes.back()->eventCompletion();
            processes.back()->report();
            waiting.erase(waiting.begin() + complete);
        }

        if (processChance == 0) {
            processes.front()->exit();
            processes.front()->report();
        }

        if (processChance == 1) {
            processes.front()->interrupt();
            processes.push(processes.front());
            processes.front()->report();

        }

        if (processChance == 2) {
            processes.front()->eventWait();
            waiting.push_back(processes.front());
            processes.front()->report();
        }
        processes.pop();
    }
    cout << "--------------------------\n";
    cout << "ALL PROCESSES FINISHED." << endl;
}