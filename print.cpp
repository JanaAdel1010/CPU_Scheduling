#include "print.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include "process.h"
#include <string>

using namespace std;
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
    return output;
}
void printTrace(int instances, char **output, int NoOfProcesses, Process **p, string Algorithmname)
{
    cout << Algorithmname;
    for (int i = 0; i <= instances; i++)
    {
        cout << (i % 10) << " ";
    }
    cout << endl;
    cout << "------------------------------------------------\n";

    for (size_t i = 0; i < NoOfProcesses; i++)
    {
        printf("%c     ", p[i]->ProcessName);
        for (size_t j = 0; j < instances; j++)
        {
            cout << "|" << output[i][j];
        }
        cout << "| " << endl;
    }
    cout << "------------------------------------------------\n\n";
}

void printStats(Process **P, int NoOfProcesses, string Algorithmname)
{
    cout << Algorithmname;
    cout << endl;
    int count = NoOfProcesses;
    printf("Process    ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3c  ", P[i]->ProcessName);
    }
    printf("|\n");
    printf("Arrival    ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3d  ", P[i]->ArrivalTime);
    }
    printf("|\n");
    printf("Service    ");
    for (size_t i = 0; i < count; i++)
    {
        printf("|%3d  ", P[i]->ServiceTime);
    }
    printf("| Mean|\n");
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