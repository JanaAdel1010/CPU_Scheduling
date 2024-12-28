#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "process.h"
#include <vector>
#include <string>
#include <queue>
#include <functional>
using namespace std;
void FCFS(std::vector<string> processes, const int instants,string status, string Algorithmname);
void RR(std::vector<string> processes, int quantum, const int instants, string status, string Algorithmname);
void SPN(vector<string> processes, const int instants, string status, string Algorithmname);
void SRT(vector<string> processes, const int instants, string status, string Algorithmname);
void HRRN(vector<string> processes, const int instants, string status, string Algorithmname);
void FB(vector<string> processes, int quantum, const int instants, string status, string Algorithmname);
void Aging(vector<string> processes, int quantum, const int instants, string status, string Algorithmname);
bool compareByServiceTimeAndArrivalTime(Process *p1, Process *p2);
bool compareByHRRN(Process *p1, Process *p2);
bool compareByPriorityAndWaitingTime(Process *p1, Process *p2);
int count_algo(string p);
int Queue_empty(vector<queue<Process *>> q, int start, int end);
void FB(vector<string> processes, int quantum, const int instants, string status, string Algorithmname);
void UpdatePriority(priority_queue<Process *, vector<Process *>, function<bool(Process *, Process *)>> &q);
#endif