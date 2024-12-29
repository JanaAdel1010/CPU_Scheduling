#ifndef process_h
#define process_h
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
// header file for process class
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
Process* parse_process(const std::string& process);
Process** processesarray(const std::vector<std::string>& p);

#endif