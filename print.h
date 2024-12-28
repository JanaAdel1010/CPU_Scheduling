#ifndef PRINT_H
#define PRINT_H
#include "process.h"
#include <string>
using namespace std;
void printStats(Process **P, int NoOfProcesses, string Algorithmname);
void printTrace(int instances, char **output, int NoOfProcesses, Process **p, string Algorithmname);
char **algo_output(int r, int c);
#endif