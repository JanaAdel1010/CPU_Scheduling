#include "process.h"
#include "algorithm.h"
#include "print.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
int main()
{
    string temp;
    vector<string> info;
    vector<string> processes;
    int line_number = 0;
    while (getline(cin, temp))
    {
        if (line_number < 4)
        {
            info.push_back(temp);
        }
        else
        {
            processes.push_back(temp);
        }
        line_number++;
    }
    string Algorithmline = info.at(1);
    int Algorithm_no = count_algo(Algorithmline);
    string Algorithms[Algorithm_no];
    stringstream ss(Algorithmline);
    int u = 0;
    while (!ss.eof())
    {
        getline(ss, Algorithms[u], ',');
        u++;
    }
    for (size_t i = 0; i < Algorithm_no; i++)
    {
        int Algorithm = 0;
        int quantum = 0;
        if (Algorithms[i].size() > 1)
        {
            stringstream ss2(Algorithms[i]);
            int r = 0;
            string str[2];
            while (!ss2.eof())
            {
                getline(ss2, str[r], '-');
                r++;
            }
            Algorithm = stoi(str[0]);
            quantum = stoi(str[1]);
        }
        else
        {
            Algorithm = stoi(Algorithms[i]);
        }
        switch (Algorithm)
        {
        case 1:
            FCFS(processes, stoi(info.at(2)), info.at(0), "FCFS");
            break;
        case 2:
            RR(processes, quantum, stoi(info.at(2)), info.at(0), "RR-");
            break;
        case 3:
            SPN(processes, stoi(info.at(2)), info.at(0), "SPN");
            break;
        case 4:
            SRT(processes, stoi(info.at(2)), info.at(0), "SRT");
            break;
        case 5:
            HRRN(processes, stoi(info.at(2)), info.at(0), "HRRN");
            break;
        case 6:
            quantum = 1;
            FB(processes, quantum, stoi(info.at(2)), info.at(0), "FB-1");
            break;
        case 7:
            quantum = 2;
            FB(processes, quantum, stoi(info.at(2)), info.at(0), "FB-2i");
            break;
        case 8:
            Aging(processes, quantum, stoi(info.at(2)), info.at(0), "Aging");
        }
    }
}