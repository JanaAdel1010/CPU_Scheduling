#include "algorithm.h"
#include "print.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include "process.h"
using namespace std;
// Function to implement First Come First Served  scheduling algorithm
void FCFS(vector<string> processes, const int instants, string status, string Algorithmname)
{
    // Create a 2D output array to visualize the states of processes over time
    char **output = algo_output(processes.size(), instants);

    // Convert process descriptions into an array of Process objects
    Process **pr = processesarray(processes);

    queue<Process *> q;               // Queue to manage processes in FCFS order
    Process *currenlty_run = nullptr; // Pointer to the currently running process
    int service;                      // Remaining service time for the currently running process

    // Main loop to iterate through each time instant
    for (size_t i = 0; i < instants; i++)
    {
        // Check if any process has arrived at the current time instant
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime) // Process has arrived
            {
                if (pr[j]->ready == 0) // Process has not yet been added to the ready queue
                {
                    q.push(pr[j]);                         
                    pr[j]->ready = 1;                      
                    output[pr[j]->processNumber][i] = '.'; 
                }
                else if (pr[j]->ready != -1) // Process is ready but not completed
                {
                    output[pr[j]->processNumber][i] = '.'; 
                }
            }
        }

        // Assign the next process from the queue to the CPU if none is running
        if (currenlty_run == nullptr && !q.empty())
        {
            currenlty_run = q.front();            
            service = currenlty_run->ServiceTime; 
            q.pop();                              
        }

        // Execute the currently running process
        if (currenlty_run != nullptr)
        {
            service--;                                     
            output[currenlty_run->processNumber][i] = '*'; 

            // If the process has finished execution
            if (service == 0)
            {
                currenlty_run->ready = -1;            
                currenlty_run->FinishingTime = i + 1; 
                currenlty_run = nullptr;          
            }
        }
    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants, output, processes.size(), pr, Algorithmname.append("  "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}

// Function to implement Round Robin  scheduling algorithm
void RR(vector<string> processes, int quantum, const int instants, string status, string Algorithmname)
{
    // Create a 2D output array for visualizing process states over time
    char **output = algo_output(processes.size(), instants);

    // Convert process descriptions into an array of Process objects
    Process **pr = processesarray(processes);

    // Array to save original service times of processes
    int processesServiceTimes[processes.size()];
    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i] = pr[i]->ServiceTime;
    }

    queue<Process *> q;               
    Process *currenlty_run = nullptr; 
    Process *t = nullptr;             
    int currentConsumption = 0;      

    // Main loop iterating through each time instant
    for (size_t i = 0; i < instants; i++)
    {
        // Check if any process has arrived at the current time instant
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            { // Process has arrived
                if (pr[j]->ready == 0)
                {                                          
                    q.push(pr[j]);                         
                    pr[j]->ready = 1;                      
                    output[pr[j]->processNumber][i] = '.'; 
                }
                else if (pr[j]->ready > 0)
                {                                          
                    output[pr[j]->processNumber][i] = '.'; 
                }
            }
        }

        // Re-enqueue a previously preempted process, if any
        if (t != nullptr)
        {
            q.push(t);  
            t = nullptr; 
        }

        // Assign a new process to the CPU if none is running
        if (currenlty_run == nullptr)
        {
            if (!q.empty())
            {
                currenlty_run = q.front(); 
                q.pop();                  
            }
        }

        // Execute the currently running process
        if (currenlty_run != nullptr)
        {
            currenlty_run->ServiceTime -= 1;               
            currentConsumption++;                          
            output[currenlty_run->processNumber][i] = '*'; 

            // If the process has completed execution
            if (currenlty_run->ServiceTime == 0)
            {
                currenlty_run->ready = -1;           
                currenlty_run->FinishingTime = i + 1; 
                currenlty_run = nullptr;              
                currentConsumption = 0;               
            }
            // If the time quantum has been fully consumed
            else if (currentConsumption >= quantum)
            {
                t = currenlty_run;      
                currentConsumption = 0;  
                currenlty_run = nullptr; 
            }
        }
    }

    // Restore the original service times of processes
    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime = processesServiceTimes[i];
    }

    // Append the quantum value to the algorithm name
    string quantum_str = to_string(quantum);
    Algorithmname = Algorithmname + quantum_str;

    if (status.compare("trace") == 0)
    {
        printTrace(instants, output, processes.size(), pr, Algorithmname.append("  "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}

// Function to implement Shortest Process Next  scheduling algorithm
void SPN(vector<string> processes, const int instants, string status, string Algorithmname)
{
    // Priority queue to select processes with the shortest service time and earliest arrival time
    auto compare = compareByServiceTimeAndArrivalTime;
    priority_queue<Process *, vector<Process *>, decltype(compare)> q(compare);

    // Create a 2D output array for visualizing process states over time
    char **output = algo_output(processes.size(), instants);

    // Convert process descriptions into an array of Process objects
    Process **pr = processesarray(processes);

    Process *currenlty_run = nullptr; 
    int service;                      
    // Main loop iterating through each time instant
    for (size_t i = 0; i < instants; i++)
    {
        // Check all processes for arrival at the current time
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            { // Process has arrived
                if (pr[j]->ready == 0)
                {                                          
                    q.push(pr[j]);                         
                    pr[j]->ready = 1;                      
                    pr[j]->EntryFlag = 1;                  
                    output[pr[j]->processNumber][i] = '.'; 
                }
                else if (pr[j]->ready > 0)
                {                                          
                    pr[j]->EntryFlag = 0;                  
                    output[pr[j]->processNumber][i] = '.'; // Indicate waiting state
                }
            }
        }

        // Select the next process to run if no process is currently running
        if (currenlty_run == nullptr)
        {
            if (!q.empty())
            {
                currenlty_run = q.top();            
                service = currenlty_run->ServiceTime; 
                q.pop();
            }
        }

        // Execute the currently running process
        if (currenlty_run != nullptr)
        {
            service--;                                     
            output[currenlty_run->processNumber][i] = '*'; // Indicate running state

            // If the process has finished execution
            if (service == 0)
            {
                currenlty_run->ready = -1;            // Mark as completed
                currenlty_run->FinishingTime = i + 1; // Record finishing time
                currenlty_run = nullptr;            
            }
        }
    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants, output, processes.size(), pr, Algorithmname.append("   "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}
// Function to implement Shortest Remaining Time scheduling algorithm
void SRT(vector<string> processes, const int instants, string status, string Algorithmname)
{
    // Create a 2D output array for visualizing process states over time
    char **output = algo_output(processes.size(), instants);

    // Convert process descriptions into an array of Process objects
    Process **pr = processesarray(processes);

    // Store the original service times of processes
    vector<int> processesServiceTimes(processes.size());
    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i] = pr[i]->ServiceTime;
    }

    // Priority queue to manage processes by shortest remaining service time and arrival time
    // The comparator function compareByServiceTimeAndArrivalTime defines the ordering
    priority_queue<Process *, vector<Process *>, function<bool(Process *, Process *)>> q(compareByServiceTimeAndArrivalTime);

    Process *currenlty_run = nullptr; // Pointer to the currently running process

    // Main loop iterating through each time instant
    for (size_t i = 0; i < instants; i++)
    {
        // Check all processes for arrival at the current time
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            { // Process has arrived
                if (pr[j]->ready == 0)
                {                                          // Process is not yet ready
                    q.push(pr[j]);                         // Add process to the priority queue
                    pr[j]->ready = 1;                      // Mark as ready
                    output[pr[j]->processNumber][i] = '.'; // Indicate waiting state
                }
                else if (pr[j]->ready > 0)
                {                                          // Process is already ready
                    output[pr[j]->processNumber][i] = '.'; // Indicate waiting state
                }
            }
        }

        // If no process is currently running  select the next process
        if (currenlty_run == nullptr && !q.empty())
        {
            currenlty_run = q.top(); // Get the process with shortest remaining time
            q.pop();                 // Remove it from the queue
        }

        // If a process is running, execute it
        if (currenlty_run != nullptr)
        {
            currenlty_run->ServiceTime--;                  // Decrease the remaining service time
            output[currenlty_run->processNumber][i] = '*'; // Indicate running state

            if (currenlty_run->ServiceTime == 0)
            {                                         // process has finished execution
                currenlty_run->ready = -1;            // finished
                currenlty_run->FinishingTime = i + 1; // Record finishing time
                currenlty_run = nullptr;              // Release
            }
            else
            {                            // If process has not finished
                q.push(currenlty_run);   // Reinsert it into the queue
                currenlty_run = nullptr; // Release  for potential preemption
            }
        }
    }

    // Restore the original service times of processes for statistics or further use
    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime = processesServiceTimes[i];
    }

    if (status == "trace")
    {
        printTrace(instants, output, processes.size(), pr, Algorithmname.append("   ")); // Print trace
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname); // Print statistics
    }
}

// hrrn compare by highest response ratio next
// highest response ratio next= (waiting time + service time) / service time
// Function to implement the Highest Response Ratio Next scheduling algorithm
void HRRN(vector<string> processes, const int instants, string status, string Algorithmname)
{
    char **output = algo_output(processes.size(), instants);
    Process **pr = processesarray(processes);
    vector<int> processesServiceTimes(processes.size());

    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i] = pr[i]->ServiceTime;
    }

    priority_queue<Process *, vector<Process *>, function<bool(Process *, Process *)>> q(compareByHRRN);
    Process *currenlty_run = nullptr;

    for (size_t i = 0; i < instants; i++)
    {
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if (pr[j]->ready == 0)
                {
                    q.push(pr[j]);
                    pr[j]->ready = 1;
                    output[pr[j]->processNumber][i] = '.';
                    pr[j]->WaitingTime++;
                }
                else if (pr[j]->ready > 0)
                {
                    pr[j]->WaitingTime++;
                    output[pr[j]->processNumber][i] = '.';
                }
            }
        }
        // If no process is running select the next process based on HRRN
        if (currenlty_run == nullptr && !q.empty())
        {
            currenlty_run = q.top();
            q.pop();
            currenlty_run->WaitingTime--;
        }

        // If a process is running, execute it
        if (currenlty_run != nullptr)
        {
            currenlty_run->ServiceTime--;
            output[currenlty_run->processNumber][i] = '*';

            if (currenlty_run->ServiceTime == 0)
            {
                currenlty_run->ready = -1;
                currenlty_run->FinishingTime = i + 1;
                currenlty_run = nullptr;
            }
        }
    }

    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime = processesServiceTimes[i];
    }

    if (status == "trace")
    {
        printTrace(instants, output, processes.size(), pr, Algorithmname.append("  "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}
// check if queue is empty
int Queue_empty(vector<queue<Process *>> q, int start, int end)
{
    for (size_t i = start; i < end; i++)
    {
        if (!q.at(i).empty())
        {
            return 0;
        }
    }
    return 1;
}
// Function to implement the feedback scheduling algorithm

void FB(vector<string> processes, int quantum, const int instants, string status, string Algorithmname)
{
    char **output = algo_output(processes.size(), instants);
    Process **pr = processesarray(processes);
    vector<queue<Process *>> q;
    for (size_t i = 0; i < processes.size(); i++)
    {
        queue<Process *> myq;
        q.push_back(myq);
    }
    // Store the original service times of processes to restore later
    int processesServiceTimes[processes.size()];
    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i] = pr[i]->ServiceTime;
    }
    Process *currenlty_run = nullptr;
    Process *t = nullptr;
    int cq = 0;
    int rq;
    char LastName;
    // Check all processes for arrival
    for (size_t i = 0; i < instants; i++)
    {

        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if (pr[j]->ready == 0)
                {
                    q.at(0).push(pr[j]);
                    pr[j]->ready = 1;
                    output[pr[j]->processNumber][i] = '.';
                }
                else if (pr[j]->ready > 0)
                {

                    output[pr[j]->processNumber][i] = '.';
                }
            }
        }
        if (t != nullptr)
        {
            if (rq == q.size() - 1 || Queue_empty(q, 0, q.size()) == 1)
            {
                q.at(rq).push(t);
            }
            else
            {
                q.at(rq + 1).push(t);
            }
            t = nullptr;
        }
        // Assign a process to run if the currently running process is idle
        if (currenlty_run == nullptr)
        {
            for (size_t k = 0; k < q.size(); k++)
            {
                if (!q.at(k).empty())
                {
                    Process *pt = q.at(k).front();
                    if (pt->ProcessName == LastName && Queue_empty(q, k + 1, q.size()) == 0)
                    {
                        for (size_t h = k + 1; h < q.size(); i++)
                        {
                            if (!q.at(h).empty())
                            {
                                currenlty_run = q.at(h).front();
                                q.at(h).pop();
                                rq = h;
                                break;
                            }
                        }
                    }
                    else
                    {
                        currenlty_run = q.at(k).front();
                        q.at(k).pop();
                        rq = k;
                    }
                    break;
                }
            }
        }
        // Execute the process if one is running
        if (currenlty_run != nullptr)
        {
            currenlty_run->ServiceTime = currenlty_run->ServiceTime - 1;
            cq++;
            output[currenlty_run->processNumber][i] = '*';

            // If process completes execution
            if (currenlty_run->ServiceTime == 0)
            {
                currenlty_run->ready = -1;
                currenlty_run->FinishingTime = i + 1;
                currenlty_run = nullptr;
                cq = 0;
            }
            // If quantum is fully consumed, preempt the process
            else if (cq == pow(quantum, rq))
            {
                t = currenlty_run;
                LastName = currenlty_run->ProcessName;
                cq = 0;
                currenlty_run = nullptr;
            }
        }
    }
    // Restore original service times of processes
    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime = processesServiceTimes[i];
    }

    string s = "trace";
    if (status.compare(s) == 0)
    {
        if (quantum == 1)
        {
            Algorithmname.append("  ");
        }
        else
        {
            Algorithmname.append(" ");
        }
        printTrace(instants, output, processes.size(), pr, Algorithmname);
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}
// update the priority of the processes in the queue
void UpdatePriority(priority_queue<Process *, vector<Process *>, function<bool(Process *, Process *)>> &q)
{
    queue<Process *> tempQueue;
    int count = q.size();

    for (int i = 0; i < count; i++)
    {
        Process *p = q.top();
        q.pop();
        p->WaitingTime++;
        p->priority++;
        tempQueue.push(p);
    }

    while (!tempQueue.empty())
    {
        Process *p = tempQueue.front();
        tempQueue.pop();
        q.push(p);
    }
}
// Function to implement the Aging scheduling algorithm
void Aging(vector<string> processes, int quantum, const int instants, string status, string Algorithmname)
{
    // Create a 2D output array for visualizing process states over time
    char **output = algo_output(processes.size(), instants);
    // Convert process descriptions into an array of Process objects
    Process **pr = processesarray(processes);
    // Create a priority queue to manage processes based on priority and waiting time
    priority_queue<Process *, vector<Process *>, std::function<bool(Process *, Process *)>> q(compareByPriorityAndWaitingTime);
    Process *currenly_running = nullptr; // Pointer to the currently running process
    int cq = 0;                          // Tracks the amount of quantum consumed by the current process

    for (size_t i = 0; i < instants; i++)
    {
        // Iterate over all processes to check their arrival times and readiness
        for (size_t j = 0; j < processes.size(); j++)
        {
            if (i >= pr[j]->ArrivalTime)
            {
                if (pr[j]->ready == 0)
                {
                    q.push(pr[j]);
                    pr[j]->ready = 1; // Mark the process as ready
                    output[pr[j]->processNumber][i] = '.';
                }
                else if (pr[j]->ready > 0)
                {
                    output[pr[j]->processNumber][i] = '.';
                }
            }
        }
        // Update priorities of processes in the priority queue
        UpdatePriority(q);

        // If the Currenly running  is idle and there are processes in the queue -->assign a process
        if (currenly_running == nullptr && !q.empty())
        {
            currenly_running = q.top();
            q.pop();
            currenly_running->ready = -2;
        }

        if (currenly_running != nullptr)
        {
            cq++;
            output[currenly_running->processNumber][i] = '*';
            if (cq >= quantum)
            {
                currenly_running->priority = currenly_running->ServiceTime - 1;
                currenly_running->ready = 0;
                currenly_running->WaitingTime = 0;
                cq = 0;
                currenly_running = nullptr;
            }
        }
    }

    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants, output, processes.size(), pr, Algorithmname.append(" "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}
// count numbers of algorithms
int count_algo(string p)
{
    int count = 0;
    for (size_t i = 0; i < p.size(); i++)
    {
        if (p.at(i) == ',')
            count++;
    }
    return count + 1;
}
// compare by service time and arrival time
bool compareByServiceTimeAndArrivalTime(Process *p1, Process *p2)
{
    if (p1->ServiceTime == p2->ServiceTime)
    {
        return p1->ArrivalTime > p2->ArrivalTime;
    }
    return p1->ServiceTime > p2->ServiceTime;
}
// compare by highest response ratio next if same then arrival time
bool compareByHRRN(Process *p1, Process *p2)
{
    float ratio1 = (p1->WaitingTime + p1->ServiceTime) / (float)p1->ServiceTime;
    float ratio2 = (p2->WaitingTime + p2->ServiceTime) / (float)p2->ServiceTime;
    if (ratio1 == ratio2)
    {
        return p1->ArrivalTime < p2->ArrivalTime;
    }
    return ratio1 < ratio2;
}
// compare by priorty if same priorty by waiting time
bool compareByPriorityAndWaitingTime(Process *p1, Process *p2)
{
    if (p1->priority == p2->priority)
    {
        return p1->WaitingTime < p2->WaitingTime;
    }
    return p1->priority < p2->priority;
}