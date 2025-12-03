#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class Process {
public:
    int pid; // Process ID
    int at;  // Arrival Time
    int bt;  // Burst Time
    int ct;  // Completion Time
    int tat; // Turnaround Time
    int wt;  // Waiting Time

    Process(int id, int arrival, int burst) {
        pid = id;
        at = arrival;
        bt = burst;
        ct = tat = wt = 0;
    }
};

class FCFS {
private:
    vector<Process> processes;

public:
    void inputProcesses(int n) {
        for (int i = 0; i < n; i++) {
            int at, bt;
            cout << "Enter Arrival Time of P" << i + 1 << ": ";
            cin >> at;
            cout << "Enter Burst Time of P" << i + 1 << ": ";
            cin >> bt;
            processes.emplace_back(i + 1, at, bt);
        }
    }

    void schedule() {
        // Sort by Arrival Time
        sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
            return a.at < b.at;
        });

        int currentTime = 0;
        for (auto &p : processes) {
            if (currentTime < p.at) {
                currentTime = p.at; // CPU idle until process arrives
            }
            currentTime += p.bt;
            p.ct = currentTime;
            p.tat = p.ct - p.at;
            p.wt = p.tat - p.bt;
        }
    }

    void displayResults() {
        double avgTAT = 0, avgWT = 0;

        cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
        for (auto &p : processes) {
            cout << "P" << p.pid << "\t" << p.at << "\t" << p.bt << "\t"
                 << p.ct << "\t" << p.tat << "\t" << p.wt << "\n";
            avgTAT += p.tat;
            avgWT += p.wt;
        }

        cout << "\nAverage Turnaround Time = " << avgTAT / processes.size();
        cout << "\nAverage Waiting Time = " << avgWT / processes.size() << "\n";
    }
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    FCFS scheduler;
    scheduler.inputProcesses(n);
    scheduler.schedule();
    scheduler.displayResults();

    return 0;
}
