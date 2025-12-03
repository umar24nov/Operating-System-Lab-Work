#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid;           // Process ID
    int burst_time;    // Burst time
    int priority;      // Priority (lower value = higher priority)
    int waiting_time;
    int turnaround_time;
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    // Input burst times and priorities
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "Enter burst time for process P" << p[i].pid << ": ";
        cin >> p[i].burst_time;
        cout << "Enter priority for process P" << p[i].pid << ": ";
        cin >> p[i].priority;
    }

    // Sort processes by priority (ascending)
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.priority < b.priority;
    });

    // Calculate waiting and turnaround times
    p[0].waiting_time = 0;
    p[0].turnaround_time = p[0].burst_time;

    for (int i = 1; i < n; i++) {
        p[i].waiting_time = p[i - 1].waiting_time + p[i - 1].burst_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
    }

    // Display results
    float total_wt = 0, total_tat = 0;
    cout << "\nProcess\tPriority\tBurst\tWaiting\tTurnaround\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].pid << "\t" << p[i].priority << "\t\t"
             << p[i].burst_time << "\t" << p[i].waiting_time << "\t"
             << p[i].turnaround_time << endl;
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    cout << "\nAverage Waiting Time: " << total_wt / n;
    cout << "\nAverage Turnaround Time: " << total_tat / n << endl;

    return 0;
}
