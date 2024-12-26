#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;
struct Process
{
    char name;
    int arrival_time;
    int service_time;
    int priority;
    int remaining_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    double response_ratio;
};

void print_trace(const std::string &algorithm, const std::vector<Process> &processes, int last_instant)
{
    std::cout << algorithm << "  ";
    for (int i = 0; i <= last_instant; ++i)
    {
        std::cout << i % 10 << " ";
    }
    cout << "\n------------------------------------------------\n";

    for (const auto &p : processes)
    {
        std::cout << p.name << "     |";
        for (int t = 0; t <= last_instant; ++t)
        {
            if (t >= p.arrival_time && t < p.finish_time)
            {
                if (t >= p.arrival_time + p.waiting_time && t < p.arrival_time + p.waiting_time + p.service_time)
                {
                    std::cout << "*|";
                }
                else
                {
                    std::cout << ".|";
                }
            }
            else
            {
                std::cout << " |";
            }
        }
        std::cout << "\n";
    }
    std::cout << "------------------------------------------------\n";
}

void print_stats(const std::vector<Process> &processes)
{
    std::cout << "Process    |";
    for (const auto &p : processes)
        std::cout << "  " << p.name << "  |";
    std::cout << "\nArrival    |";
    for (const auto &p : processes)
        std::cout << "  " << p.arrival_time << "  |";
    std::cout << "\nService    |";
    for (const auto &p : processes)
        std::cout << "  " << p.service_time << "  |";
    std::cout << " Mean|\nFinish     |";
    int total_turnaround = 0;
    for (const auto &p : processes)
    {
        std::cout << "  " << p.finish_time << "  |";
    }
    std::cout << "-----|\nTurnaround |";
    for (const auto &p : processes)
    {
        total_turnaround += p.turnaround_time;
        std::cout << "  " << p.turnaround_time << "  |";
    }
    double mean_turnaround = static_cast<double>(total_turnaround) / processes.size();
    std::cout << " " << std::fixed << std::setprecision(2) << mean_turnaround << "|\nNormTurn   |";
    for (const auto &p : processes)
    {
        double norm_turn = static_cast<double>(p.turnaround_time) / p.service_time;
        std::cout << " " << std::fixed << std::setprecision(2) << norm_turn << "|";
    }
    std::cout << " " << std::fixed << std::setprecision(2) << mean_turnaround / processes.size() << "|\n";
}
void fcfs(std::vector<Process> &processes, int last_instant, bool trace_mode)
{
    int current_time = 0;
    for (auto &process : processes)
    {
        if (current_time < process.arrival_time)
        {
            current_time = process.arrival_time;
        }
        current_time += process.service_time;
        process.finish_time = current_time;
        process.turnaround_time = process.finish_time - process.arrival_time;
        process.waiting_time = process.turnaround_time - process.service_time;
    }
    if (trace_mode)
    {
        print_trace("FCFS", processes, last_instant);
    }
    else
    {
        print_stats(processes);
    }
}
void round_robin(std::vector<Process> &processes, int quantum, int last_instant, bool trace_mode)
{
    int current_time = 0;
    std::queue<Process *> ready_queue;
    for (auto &process : processes)
    {
        process.remaining_time = process.service_time;
    }

    int num_processes = processes.size();
    int completed = 0;
    size_t index = 0;

    while (completed < num_processes)
    {
        while (index < processes.size() && processes[index].arrival_time <= current_time)
        {
            ready_queue.push(&processes[index]);
            index++;
        }

        if (ready_queue.empty())
        {
            current_time++;
            continue;
        }

        Process *current = ready_queue.front();
        ready_queue.pop();

        int execution_time = std::min(current->remaining_time, quantum);
        current_time += execution_time;
        current->remaining_time -= execution_time;

        while (index < processes.size() && processes[index].arrival_time <= current_time)
        {
            ready_queue.push(&processes[index]);
            index++;
        }

        if (current->remaining_time > 0)
        {
            ready_queue.push(current);
        }
        else
        {
            current->finish_time = current_time;
            current->turnaround_time = current->finish_time - current->arrival_time;
            current->waiting_time = current->turnaround_time - current->service_time;
            completed++;
        }
    }
    if (trace_mode)
    {
        print_trace("RR", processes, last_instant);
    }
    else
    {
        print_stats(processes);
    }
}
void srt(std::vector<Process> &processes, int last_instant, bool trace_mode)
{
    int current_time = 0;
    int completed = 0;

    while (completed < processes.size())
    {
        Process *shortest = nullptr;
        for (auto &process : processes)
        {
            if (process.arrival_time <= current_time && process.remaining_time > 0)
            {
                if (!shortest || process.remaining_time < shortest->remaining_time)
                {
                    shortest = &process;
                }
            }
        }

        if (!shortest)
        {
            current_time++;
            continue;
        }

        shortest->remaining_time--;
        current_time++;

        if (shortest->remaining_time == 0)
        {
            shortest->finish_time = current_time;
            shortest->turnaround_time = shortest->finish_time - shortest->arrival_time;
            shortest->waiting_time = shortest->turnaround_time - shortest->service_time;
            completed++;
        }
    }

    if (trace_mode)
    {
        print_trace("SRT", processes, last_instant);
    }
    else
    {
        print_stats(processes);
    }
}
void hrrn(std::vector<Process> &processes, int last_instant, bool trace_mode)
{
    int current_time = 0;
    int completed = 0;

    while (completed < processes.size())
    {
        Process *highest = nullptr;
        for (auto &process : processes)
        {
            if (process.arrival_time <= current_time && process.remaining_time > 0)
            {
                process.response_ratio =
                    static_cast<double>(current_time - process.arrival_time + process.service_time) / process.service_time;
                if (!highest || process.response_ratio > highest->response_ratio)
                {
                    highest = &process;
                }
            }
        }

        if (!highest)
        {
            current_time++;
            continue;
        }

        current_time += highest->service_time;
        highest->remaining_time = 0;
        highest->finish_time = current_time;
        highest->turnaround_time = highest->finish_time - highest->arrival_time;
        highest->waiting_time = highest->turnaround_time - highest->service_time;
        completed++;
    }

    if (trace_mode)
    {
        print_trace("HRRN", processes, last_instant);
    }
    else
    {
        print_stats(processes);
    }
}
void fb1(std::vector<Process> &processes, int last_instant, bool trace_mode)
{
    int current_time = 0;
    std::queue<Process *> queues[10];
    size_t index = 0;
    int completed = 0;

    while (completed < processes.size())
    {
        while (index < processes.size() && processes[index].arrival_time <= current_time)
        {
            queues[0].push(&processes[index]);
            index++;
        }

        bool executed = false;
        for (int i = 0; i < 10; ++i)
        {
            if (!queues[i].empty())
            {
                Process *current = queues[i].front();
                queues[i].pop();
                executed = true;

                current_time++;
                current->remaining_time--;

                if (current->remaining_time > 0)
                {
                    queues[i + 1].push(current);
                }
                else
                {
                    current->finish_time = current_time;
                    current->turnaround_time = current->finish_time - current->arrival_time;
                    current->waiting_time = current->turnaround_time - current->service_time;
                    completed++;
                }
                break;
            }
        }

        if (!executed)
        {
            current_time++;
        }
    }

    if (trace_mode)
    {
        print_trace("FB-1", processes, last_instant);
    }
    else
    {
        print_stats(processes);
    }
}
void fb2i(std::vector<Process> &processes, int last_instant, bool trace_mode)
{
    int current_time = 0;
    std::queue<Process *> queues[10];
    size_t index = 0;
    int completed = 0;

    while (completed < processes.size())
    {
        while (index < processes.size() && processes[index].arrival_time <= current_time)
        {
            queues[0].push(&processes[index]);
            index++;
        }

        bool executed = false;
        for (int i = 0; i < 10; ++i)
        {
            if (!queues[i].empty())
            {
                Process *current = queues[i].front();
                queues[i].pop();
                executed = true;

                int quantum = std::pow(2, i);
                int execution_time = std::min(current->remaining_time, quantum);
                current_time += execution_time;
                current->remaining_time -= execution_time;

                if (current->remaining_time > 0)
                {
                    queues[i + 1].push(current);
                }
                else
                {
                    current->finish_time = current_time;
                    current->turnaround_time = current->finish_time - current->arrival_time;
                    current->waiting_time = current->turnaround_time - current->service_time;
                    completed++;
                }
                break;
            }
        }

        if (!executed)
        {
            current_time++;
        }
    }

    if (trace_mode)
    {
        print_trace("FB-2i", processes, last_instant);
    }
    else
    {
        print_stats(processes);
    }
}
void shortest_process_next(std::vector<Process> &processes, int last_instant, bool trace_mode)
{
    int current_time = 0;
    int completed = 0;

    while (completed < processes.size())
    {
        Process *shortest = nullptr;

        // Select the shortest process that has arrived and is not yet completed
        for (auto &process : processes)
        {
            if (process.arrival_time <= current_time && process.remaining_time > 0)
            {
                if (!shortest || process.remaining_time < shortest->remaining_time)
                {
                    shortest = &process;
                }
            }
        }

        if (!shortest)
        {
            current_time++;
            continue;
        }

        current_time += shortest->service_time;
        shortest->remaining_time = 0;
        shortest->finish_time = current_time;
        shortest->turnaround_time = shortest->finish_time - shortest->arrival_time;
        shortest->waiting_time = shortest->turnaround_time - shortest->service_time;
        completed++;
    }

    if (trace_mode)
    {
        print_trace("SPN", processes, last_instant);
    }
    else
    {
        print_stats(processes);
    }
}
void aging(std::vector<Process> &processes, int last_instant, int aging_increment, bool trace_mode)
{
    int current_time = 0;
    int completed = 0;
    std::vector<Process *> ready_queue;

    while (completed < processes.size())
    {
        // Add processes to the ready queue based on their arrival time
        for (auto &process : processes)
        {
            if (process.arrival_time <= current_time && process.remaining_time > 0)
            {
                auto it = std::find(ready_queue.begin(), ready_queue.end(), &process);
                if (it == ready_queue.end())
                {
                    ready_queue.push_back(&process);
                }
            }
        }

        if (ready_queue.empty())
        {
            current_time++;
            continue;
        }

        // Select the process with the highest priority
        Process *highest_priority = *std::max_element(ready_queue.begin(), ready_queue.end(), [](Process *a, Process *b)
                                                      { return a->priority < b->priority; });

        // Execute the process
        current_time++;
        highest_priority->remaining_time--;

        // Increase priority of all other processes in the queue
        for (auto &process : ready_queue)
        {
            if (process != highest_priority)
            {
                process->priority += aging_increment;
            }
        }

        // If the highest-priority process finishes, remove it from the queue
        if (highest_priority->remaining_time == 0)
        {
            highest_priority->finish_time = current_time;
            highest_priority->turnaround_time = highest_priority->finish_time - highest_priority->arrival_time;
            highest_priority->waiting_time = highest_priority->turnaround_time - highest_priority->service_time;
            ready_queue.erase(std::remove(ready_queue.begin(), ready_queue.end(), highest_priority), ready_queue.end());
            completed++;
        }
    }

    if (trace_mode)
    {
        print_trace("Aging", processes, last_instant);
    }
    else
    {
        print_stats(processes);
    }
}



int main()
{
    std::ifstream file("02a-input.txt");
    if (!file)
    {
        std::cerr << "Error: Could not open input file.\n";
        return 1;
    }

    string line;
    string mode;
    // Line 1: "trace" or "stats"
    getline(file, mode);
    cout << "Line 1: " << mode << endl;

    // Line 2: CPU scheduling policies and their parameters
    getline(file, line);
    cout << "Line 2: " << line << endl;
    size_t dashPos = line.find('-');
    int algorithm;
    int parameter = 0;
    if (dashPos != string::npos)
    {
        // Policy with a parameter
        algorithm = stoi(line.substr(0, dashPos));  // Extract algorithm number
        parameter = stoi(line.substr(dashPos + 1)); // Extract parameter
        cout << "Algorithm: " << algorithm << ", Parameter: " << parameter << endl;
    }
    else
    {
        // Policy without a parameter
        algorithm = stoi(line);
        cout << "Algorithm: " << algorithm << " (No parameter)" << endl;
    }

    // Line 3: Last instant to be used in simulation
    int lastInstant;
    file >> lastInstant;
    cout << "Line 3: Last instant to be used: " << lastInstant << endl;
    file.ignore(); // To ignore the newline after the integer

    // Line 4: Number of processes to simulate
    int numProcesses;
    file >> numProcesses;
    cout << "Line 4: Number of processes: " << numProcesses << endl;
    file.ignore(); // To ignore the newline after the integer

    // Line 5 onwards: Description of processes
    vector<Process> processes;
    cout << "Line 5 onwards: Process descriptions:" << endl;
    for (int i = 0; i < numProcesses; ++i)
    {
        Process p;
        getline(file, line); // Read the line describing the process

        stringstream ss(line);
        string item;

        // Read process name, arrival time, and service time
        getline(ss, item, ',');
        p.name = item[0]; // Process name (single character)

        getline(ss, item, ',');
        p.arrival_time = stoi(item); // Arrival Time

        getline(ss, item, ',');
        p.service_time = stoi(item); // Service Time

        processes.push_back(p);

        // Output the process details
        cout << "Process " << i + 1 << ": "
             << "Name: " << p.name
             << ", Arrival Time: " << p.arrival_time
             << ", Service Time: " << p.service_time << endl;
    }

    // Copy original processes to reset between algorithms
    const std::vector<Process> original_processes = processes;

    // Determine whether we are in trace or stats mode
    bool trace_mode = (mode == "trace");

    // Parse algorithms and execute them
        processes = original_processes; // Reset processes for each algorithm
        if (algorithm== 1)
        {
            fcfs(processes, lastInstant, trace_mode);
        }
        else if (algorithm== 2)
        {
            int quantum = parameter;
            round_robin(processes, quantum, lastInstant, trace_mode);
        }
        else if (algorithm== 3)
        {
            shortest_process_next(processes, lastInstant, trace_mode);
        }
        else if (algorithm==4)
        {
            srt(processes, lastInstant, trace_mode);
        }
        else if (algorithm== 5)
        {
            hrrn(processes, lastInstant, trace_mode);
        }
        else if (algorithm== 6)
        {
            fb1(processes, lastInstant, trace_mode);
        }
        else if (algorithm== 7)
        {
            fb2i(processes, lastInstant, trace_mode);
        }
        else if (algorithm== 8)
        {
            int quantum = parameter;
            int aging_increment = 1; // Set an appropriate aging increment
            aging(processes, lastInstant, aging_increment, trace_mode);
        }
        else
        {
            std::cerr << "Unknown algorithm: " << algorithm << "\n";
        }


    return 0;
}
