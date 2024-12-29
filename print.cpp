#include "print.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include "process.h"
#include <string>

using namespace std;
// Function to create a 2D character array initialized with spaces
char **algo_output(int r, int c)
{
    char **output = (char **)malloc(sizeof(char *) * r);
    for (size_t i = 0; i < r; i++)
    {
        output[i] = (char *)malloc(sizeof(char) * c);
        for (size_t j = 0; j < c; j++)
        {
            output[i][j] = ' ';
        }
    }
    // Return the pointer to the 2D array   
    return output;
}
// Function to print a trace of the scheduling process
void printTrace(int instances, char **output, int NoOfProcesses, Process **p, string Algorithmname)
{
    // Print the name of the scheduling algorithm
    cout << Algorithmname;
    // Print the timeline instances (mod 10 for wrapping numbers)
    for (int i = 0; i <= instances; i++)
    {
        cout << (i % 10) << " ";
    }
    cout << endl;
    cout << "------------------------------------------------\n";
    // Print the trace for each process
    for (size_t i = 0; i < NoOfProcesses; i++)
    {
        printf("%c     ", p[i]->ProcessName);
        // Print the output for each instance
        for (size_t j = 0; j < instances; j++)
        {
            cout << "|" << output[i][j];
        }
        cout << "| " << endl;
    }
    cout << "------------------------------------------------\n\n";
}
// Function to print statistical information about the processes
void printStats(Process **P, int NoOfProcesses, string Algorithmname)
{
    cout << Algorithmname;
    cout << endl;
    int count = NoOfProcesses;
    // Print the header row for process names
    printf("Process    ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3c  ", P[i]->ProcessName);
    }
    printf("|\n");
    // Print the arrival times of each process
    printf("Arrival    ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3d  ", P[i]->ArrivalTime);
    }
    printf("|\n");
    // Print the service times of each process
    printf("Service    ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3d  ", P[i]->ServiceTime);
    }
    printf("| Mean|\n");
    // Print the finishing times of each process
    printf("Finish     ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3d  ", P[i]->FinishingTime);
    }
    printf("|-----|\n");
    printf("Turnaround ");
    float TotalTurnAround = 0;
    size_t i;
    for (i = 0; i < count; i++)
    {
        P[i]->TurnaroundTime = P[i]->FinishingTime - P[i]->ArrivalTime;
        TotalTurnAround += P[i]->TurnaroundTime;
        printf("|%3d  ", P[i]->TurnaroundTime);
    }
    printf("|%5.2f|\n", (TotalTurnAround / i));
    printf("NormTurn   ");
    float TotalNormTime = 0;
    for (i = 0; i < count; i++)
    {
        P[i]->NormTurn = ((float)P[i]->TurnaroundTime / P[i]->ServiceTime);
        TotalNormTime += P[i]->NormTurn;
        printf("|%5.2f", P[i]->NormTurn);
    }
    printf("|%5.2f|\n\n", (TotalNormTime / i));
}