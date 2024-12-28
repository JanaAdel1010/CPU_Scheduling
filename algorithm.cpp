#include "algorithm.h"
#include "print.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include "process.h"
using namespace std;
void FCFS(vector<string> processes, const int instants, string status, string Algorithmname)
{
    char **output = algo_output(processes.size(), instants);
    Process **pr = readyProcesses(processes);
    queue<Process *> q;
    Process *CPU = nullptr;
    int service;
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
                }
                else if (pr[j]->ready != -1)
                {

                    output[pr[j]->processNumber][i] = '.';
                }
            }
        }
        if (CPU == nullptr)
        {
            CPU = q.front();
            service = CPU->ServiceTime;
            q.pop();
        }
        if (CPU != nullptr)
        {
            service--;
            output[CPU->processNumber][i] = '*';
            if (service == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i + 1;
                CPU = nullptr;
            }
        }
    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants, output, processes.size(), *pr, Algorithmname.append("  "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}

void RR(vector<string> processes, int quantum, const int instants, string status, string Algorithmname)
{
    char **output = algo_output(processes.size(), instants);
    Process **pr = readyProcesses(processes);
    int processesServiceTimes[processes.size()];
    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i] = pr[i]->ServiceTime;
    }
    queue<Process *> q;
    Process *CPU = nullptr;
    Process *t = nullptr;
    int currentConsumption = 0;
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
                }
                else if (pr[j]->ready > 0)
                {

                    output[pr[j]->processNumber][i] = '.';
                }
            }
        }
        if (t != nullptr)
        {
            q.push(t);
            t = nullptr;
        }
        if (CPU == nullptr)
        {
            CPU = q.front();
            q.pop();
        }
        if (CPU != nullptr)
        {
            CPU->ServiceTime = CPU->ServiceTime - 1;
            currentConsumption++;
            output[CPU->processNumber][i] = '*';
            if (CPU->ServiceTime == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i + 1;
                CPU = nullptr;
                currentConsumption = 0;
            }
            else if (currentConsumption >= quantum)
            {
                t = CPU;
                currentConsumption = 0;
                CPU = nullptr;
            }
        }
    }
    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime = processesServiceTimes[i];
    }
    string s = "trace";
    string quantum_str = to_string(quantum);
    Algorithmname = Algorithmname + quantum_str;
    if (status.compare(s) == 0)
    {
        printTrace(instants, output, processes.size(), *pr, Algorithmname.append("  "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}

void SPN(vector<string> processes, const int instants, string status, string Algorithmname)
{
    auto compare = compareByServiceTimeAndArrivalTime;
    priority_queue<Process *, vector<Process *>, decltype(compare)> q(compare);

    char **output = algo_output(processes.size(), instants);
    Process **pr = readyProcesses(processes);
    Process *CPU = nullptr;
    int service;

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
                    pr[j]->EntryFlag = 1;
                    output[pr[j]->processNumber][i] = '.';
                }
                else if (pr[j]->ready > 0)
                {
                    pr[j]->EntryFlag = 0;
                    output[pr[j]->processNumber][i] = '.';
                }
            }
        }
        if (CPU == nullptr)
        {
            if (!q.empty())
            {
                CPU = q.top();
                service = CPU->ServiceTime;
                q.pop();
            }
        }
        if (CPU != nullptr)
        {
            service--;
            output[CPU->processNumber][i] = '*';
            if (service == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i + 1;
                CPU = nullptr;
            }
        }
    }
    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants, output, processes.size(), *pr, Algorithmname.append("   "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}

void SRT(vector<string> processes, const int instants, string status, string Algorithmname)
{
    char **output = algo_output(processes.size(), instants);
    Process **pr = readyProcesses(processes);
    vector<int> processesServiceTimes(processes.size());

    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i] = pr[i]->ServiceTime;
    }

    priority_queue<Process *, vector<Process *>, function<bool(Process *, Process *)>> q(compareByServiceTimeAndArrivalTime);
    Process *CPU = nullptr;

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
                }
                else if (pr[j]->ready > 0)
                {
                    output[pr[j]->processNumber][i] = '.';
                }
            }
        }

        if (CPU == nullptr && !q.empty())
        {
            CPU = q.top();
            q.pop();
        }

        if (CPU != nullptr)
        {
            CPU->ServiceTime--;
            output[CPU->processNumber][i] = '*';

            if (CPU->ServiceTime == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i + 1;
                CPU = nullptr;
            }
            else
            {
                q.push(CPU);
                CPU = nullptr;
            }
        }
    }

    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime = processesServiceTimes[i];
    }

    if (status == "trace")
    {
        printTrace(instants, output, processes.size(), *pr, Algorithmname.append("   "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}

void HRRN(vector<string> processes, const int instants, string status, string Algorithmname)
{
    char **output = algo_output(processes.size(), instants);
    Process **pr = readyProcesses(processes);
    vector<int> processesServiceTimes(processes.size());

    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i] = pr[i]->ServiceTime;
    }

    priority_queue<Process *, vector<Process *>, function<bool(Process *, Process *)>> q(compareByHRRN);
    Process *CPU = nullptr;

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

        if (CPU == nullptr && !q.empty())
        {
            CPU = q.top();
            q.pop();
            CPU->WaitingTime--;
        }

        if (CPU != nullptr)
        {
            CPU->ServiceTime--;
            output[CPU->processNumber][i] = '*';

            if (CPU->ServiceTime == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i + 1;
                CPU = nullptr;
            }
        }
    }

    for (size_t i = 0; i < processes.size(); i++)
    {
        pr[i]->ServiceTime = processesServiceTimes[i];
    }

    if (status == "trace")
    {
        printTrace(instants, output, processes.size(), *pr, Algorithmname.append("  "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}

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
void FB(vector<string> processes, int quantum, const int instants, string status, string Algorithmname)
{
    char **output = algo_output(processes.size(), instants);
    Process **pr = readyProcesses(processes);
    vector<queue<Process *>> q;
    for (size_t i = 0; i < processes.size(); i++)
    {
        queue<Process *> myq;
        q.push_back(myq);
    }
    int processesServiceTimes[processes.size()];
    for (size_t i = 0; i < processes.size(); i++)
    {
        processesServiceTimes[i] = pr[i]->ServiceTime;
    }
    Process *CPU = nullptr;
    Process *t = nullptr;
    int consumedQuantum = 0;
    int retriveQueueNum;
    char LastName;
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
            if (retriveQueueNum == q.size() - 1 || Queue_empty(q, 0, q.size()) == 1)
            {
                q.at(retriveQueueNum).push(t);
            }
            else
            {
                q.at(retriveQueueNum + 1).push(t);
            }
            t = nullptr;
        }
        if (CPU == nullptr)
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
                                CPU = q.at(h).front();
                                q.at(h).pop();
                                retriveQueueNum = h;
                                break;
                            }
                        }
                    }
                    else
                    {
                        CPU = q.at(k).front();
                        q.at(k).pop();
                        retriveQueueNum = k;
                    }
                    break;
                }
            }
        }
        if (CPU != nullptr)
        {
            CPU->ServiceTime = CPU->ServiceTime - 1;
            consumedQuantum++;
            output[CPU->processNumber][i] = '*';

            if (CPU->ServiceTime == 0)
            {
                CPU->ready = -1;
                CPU->FinishingTime = i + 1;
                CPU = nullptr;
                consumedQuantum = 0;
            }
            else if (consumedQuantum == pow(quantum, retriveQueueNum))
            {
                t = CPU;
                LastName = CPU->ProcessName;
                consumedQuantum = 0;
                CPU = nullptr;
            }
        }
    }
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
        printTrace(instants, output, processes.size(), *pr, Algorithmname);
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}
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

void Aging(vector<string> processes, int quantum, const int instants, string status, string Algorithmname)
{
    char **output = algo_output(processes.size(), instants);
    Process **pr = readyProcesses(processes);
    priority_queue<Process *, vector<Process *>, std::function<bool(Process *, Process *)>> q(compareByPriorityAndWaitingTime);
    Process *CPU = nullptr;
    int consumedQuantum = 0;

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
                }
                else if (pr[j]->ready > 0)
                {
                    output[pr[j]->processNumber][i] = '.';
                }
            }
        }

        UpdatePriority(q);

        if (CPU == nullptr && !q.empty())
        {
            CPU = q.top();
            q.pop();
            CPU->ready = -2;
        }

        if (CPU != nullptr)
        {
            consumedQuantum++;
            output[CPU->processNumber][i] = '*';
            if (consumedQuantum >= quantum)
            {
                CPU->priority = CPU->ServiceTime - 1;
                CPU->ready = 0;
                CPU->WaitingTime = 0;
                consumedQuantum = 0;
                CPU = nullptr;
            }
        }
    }

    string s = "trace";
    if (status.compare(s) == 0)
    {
        printTrace(instants, output, processes.size(), *pr, Algorithmname.append(" "));
    }
    else
    {
        printStats(pr, processes.size(), Algorithmname);
    }
}

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
bool compareByServiceTimeAndArrivalTime(Process *p1, Process *p2)
{
    if (p1->ServiceTime == p2->ServiceTime)
    {
        return p1->ArrivalTime > p2->ArrivalTime;
    }
    return p1->ServiceTime > p2->ServiceTime;
}

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

bool compareByPriorityAndWaitingTime(Process *p1, Process *p2)
{
    if (p1->priority == p2->priority)
    {
        return p1->WaitingTime < p2->WaitingTime;
    }
    return p1->priority < p2->priority;
}