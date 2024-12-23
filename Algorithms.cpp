#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

struct Process(
    char name;
    std::chrono::system_clock::time_point arrivalTime;
    std::chrono::system_clock::time_point inishTime;
    int serviceTime;
    int priority;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    bool started;
    Process(char n, std::chrono::system_clock::time_point at, int st, int p = 0)
    : name(n), arrivalTime(at), serviceTime(st), priority(p), remainingTime(st),
    waitingTime(0), turnaroundTime(0), responseTime(-1), started(false) {}
};