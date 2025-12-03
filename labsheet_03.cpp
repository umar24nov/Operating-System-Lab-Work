#include <iostream>
#include <vector>
using namespace std;

struct Process {
    int pid;         // Process ID
    int burst_time;  // Burst Time
    int remaining;   // Remaining Time
    int waiting_time;
    int turnaround_time;
};

int main() {
    int n, quantum;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    // Input burst times
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "Enter burst time for process P" << p[i].pid << ": ";
        cin >> p[i].burst_time;
        p[i].remaining = p[i].burst_time;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
    }

    cout << "Enter time quantum: ";
    cin >> quantum;

    int current_time = 0;
    bool done;

    // Loop until all processes are done
    do {
        done = true;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0) {
                done = false;  // At least one process is not finished

                if (p[i].remaining > quantum) {
                    // Execute for quantum time
                    current_time += quantum;
                    p[i].remaining -= quantum;
                } else {
                    // Process completes in this cycle
                    current_time += p[i].remaining;
                    p[i].waiting_time = current_time - p[i].burst_time;
                    p[i].remaining = 0;
                }
            }
        }
    } while (!done);

    // Calculate Turnaround Time
    for (int i = 0; i < n; i++)
        p[i].turnaround_time = p[i].burst_time + p[i].waiting_time;

    // Display results
    float total_wt = 0, total_tat = 0;
    cout << "\nProcess\tBurst\tWaiting\tTurnaround\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].pid << "\t"
             << p[i].burst_time << "\t"
             << p[i].waiting_time << "\t"
             << p[i].turnaround_time << endl;
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    cout << "\nAverage Waiting Time: " << total_wt / n;
    cout << "\nAverage Turnaround Time: " << total_tat / n << endl;

    return 0;
}
