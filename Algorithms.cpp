#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

struct Process {
    char name;
    int arrival_time;
    int service_time;
    int priority;
    int remaining_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    double response_ratio;

    Process(char n, int at, int st, int pr = 0)
        : name(n), arrival_time(at), service_time(st), priority(pr), remaining_time(st), finish_time(0), 
          waiting_time(0), turnaround_time(0), response_ratio(0) {}
};

void print_trace(const std::string& algorithm, const std::vector<Process>& processes, int last_instant) {
    std::cout << algorithm << "  ";
    for (int i = 0; i <= last_instant; ++i) {
        std::cout << i % 10 << " ";
    }
    std::cout << "\n------------------------------------------------\n";
    for (const auto& p : processes) {
        std::cout << p.name << "     |";
        for (int t = 0; t <= last_instant; ++t) {
            if (t >= p.arrival_time && t < p.finish_time) {
                if (t >= p.arrival_time + p.waiting_time && t < p.arrival_time + p.waiting_time + p.service_time) {
                    std::cout << "*|";
                } else {
                    std::cout << ".|";
                }
            } else {
                std::cout << " |";
            }
        }
        std::cout << "\n";
    }
    std::cout << "------------------------------------------------\n";
}
void print_stats(const std::vector<Process>& processes) {
    std::cout << "Process    |";
    for (const auto& p : processes) std::cout << "  " << p.name << "  |";
    std::cout << "\nArrival    |";
    for (const auto& p : processes) std::cout << "  " << p.arrival_time << "  |";
    std::cout << "\nService    |";
    for (const auto& p : processes) std::cout << "  " << p.service_time << "  |";
    std::cout << " Mean|\nFinish     |";
    int total_turnaround = 0;
    for (const auto& p : processes) {
        std::cout << "  " << p.finish_time << "  |";
    }
    std::cout << "-----|\nTurnaround |";
    for (const auto& p : processes) {
        total_turnaround += p.turnaround_time;
        std::cout << "  " << p.turnaround_time << "  |";
    }
    double mean_turnaround = static_cast<double>(total_turnaround) / processes.size();
    std::cout << " " << std::fixed << std::setprecision(2) << mean_turnaround << "|\nNormTurn   |";
    for (const auto& p : processes) {
        double norm_turn = static_cast<double>(p.turnaround_time) / p.service_time;
        std::cout << " " << std::fixed << std::setprecision(2) << norm_turn << "|";
    }
    std::cout << " " << std::fixed << std::setprecision(2) << mean_turnaround / processes.size() << "|\n";
}
    