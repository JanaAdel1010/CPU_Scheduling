#include "process.h"
using namespace std;
// Function to create a new Process structure and initialize its fields
Process* create_Process(char ProcessName, int ArrivalTime, int ServiceTime) {
    // Allocate memory for a Process 
    Process *p = (Process *)malloc(sizeof(Process));
    // Initialize the process fields
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
 // Return the pointer to the newly created Process object
    return p;
}
// Function to parse a string describing a process and create a Process object

Process* parse_process(const string& process) {
    // Create a stringstream object to parse the input string
    stringstream ss(process);
    // Array to store the parsed substrings (process name, arrival time, service time)
    string str[3];
    int i = 0;
    // Parse the string by splitting it at commas
    while (!ss.eof() && i < 3) {
        getline(ss, str[i], ',');
        i++;
    }
    // Convert the parsed substrings to appropriate types and create a Process object
    return create_Process(str[0][0], stoi(str[1]), stoi(str[2]));
}

// Function to create an array of Process

Process** processesarray(const vector<std::string>& p) {
    // Allocate memory for an array of Process pointers
    Process** pr = (Process**)malloc(sizeof(Process*) * p.size());
    for (size_t i = 0; i < p.size(); i++) {
        pr[i] = parse_process(p.at(i));
        pr[i]->processNumber = i;
    }
    // Return the pointer to the array of Process objects
    return pr;
}