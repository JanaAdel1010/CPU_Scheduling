#ifndef process_h
#define process_h
#include <sstream>
#include <string>

typedef struct 
{
    char ProcessName;
    int processNumber;
    int ArrivalTime;
    int ServiceTime;
    int FinishingTime;
    int TurnaroundTime;
    int priority;
    int EntryFlag;
    int WaitingTime;
    float NormTurn;
    int ready;
}Process;
Process* create_Process(char ProcessName, int ArrivalTime, int ServiceTime)
{
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
Process* formulate_Process(char process) {
    stringstream ss;
    ss << process;
    string str[3];
    char i = 0;
    while (!ss.eof()) {
        getline(ss, str[i], ',');
        i++;
    }
    return create_Process(str[0][0], stoi(str[1]), stoi(str[2]));
}
Process* formulate_Process(char* process) {
    stringstream ss(process);
    string str[3];
    char i = 0;
    while (!ss.eof() && i < 3) { // Ensure we only process up to three fields
        getline(ss, str[i], ',');
        i++;
    }
    return create_Process(str[0][0], stoi(str[1]), stoi(str[2]));
}
Process** readyProcesses(vector<string> p) {
    Process** pr = (Process**)malloc(sizeof(Process*) * p.size());
    for (size_t i = 0; i < p.size(); i++) {
        char process[p.at(i).length() + 1];
        strcpy(process, p.at(i).c_str());
        pr[i] = formulate_Process(process);
        pr[i]->processNumber = i;
    }
    return pr;
}
#endif 