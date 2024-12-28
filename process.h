#ifndef process_h
#define process_h
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>

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
} Process;

Process* create_Process(char ProcessName, int ArrivalTime, int ServiceTime);
Process* formulate_Process(const std::string& process);
Process** readyProcesses(const std::vector<std::string>& p);

#endif