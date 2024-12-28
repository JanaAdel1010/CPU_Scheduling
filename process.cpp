#include "process.h"
using namespace std;
Process* create_Process(char ProcessName, int ArrivalTime, int ServiceTime) {
    Process *p = (Process *)malloc(sizeof(Process));
    p->ProcessName = ProcessName;
    p->processNumber = 0;
    p->ready = 0;
    p->FinishingTime = 0;
    p->TurnaroundTime = 0;
    p->NormTurn = 0;
    p->WaitingTime = 0;
    p->EntryFlag = -1;
    p->ArrivalTime = ArrivalTime;
    p->ServiceTime = ServiceTime;
    p->priority = ServiceTime;
    return p;
}

Process* formulate_Process(const string& process) {
    stringstream ss(process);
    string str[3];
    int i = 0;
    while (!ss.eof() && i < 3) {
        getline(ss, str[i], ',');
        i++;
    }
    return create_Process(str[0][0], stoi(str[1]), stoi(str[2]));
}

Process** readyProcesses(const vector<std::string>& p) {
    Process** pr = (Process**)malloc(sizeof(Process*) * p.size());
    for (size_t i = 0; i < p.size(); i++) {
        pr[i] = formulate_Process(p.at(i));
        pr[i]->processNumber = i;
    }
    return pr;
}