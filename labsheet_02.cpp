    #include <iostream>
    #include <vector>
    #include <algorithm>
    using namespace std;

    class Process {
    public:
        int pid;  // Process ID
        int at;   // Arrival Time
        int bt;   // Burst Time
        int ct;   // Completion Time
        int tat;  // Turnaround Time
        int wt;   // Waiting Time
        bool done; // Whether process has been scheduled

        Process(int id, int arrival, int burst) {
            pid = id;
            at = arrival;
            bt = burst;
            ct = tat = wt = 0;
            done = false;
        }
    };

    class SJF {
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
            int completed = 0;
            int currentTime = 0;
            int n = processes.size();

            while (completed < n) {
                // find process with minimum BT among arrived processes and not done yet
                int idx = -1;
                int minBT = INT32_MAX;

                for (int i = 0; i < n; i++) {
                    if (!processes[i].done && processes[i].at <= currentTime) {
                        if (processes[i].bt < minBT) {
                            minBT = processes[i].bt;
                            idx = i;
                        }
                    }
                }

                if (idx == -1) {
                    // no process has arrived yet, jump to next arrival
                    int nextArrival = INT32_MAX;
                    for (int i = 0; i < n; i++) {
                        if (!processes[i].done && processes[i].at < nextArrival) {
                            nextArrival = processes[i].at;
                        }
                    }
                    currentTime = nextArrival;
                    continue;
                }

                // execute this process
                currentTime += processes[idx].bt;
                processes[idx].ct = currentTime;
                processes[idx].tat = processes[idx].ct - processes[idx].at;
                processes[idx].wt = processes[idx].tat - processes[idx].bt;
                processes[idx].done = true;
                completed++;
            }
        }

        void displayResults() {
            double avgTAT = 0, avgWT = 0;

            cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
            // display sorted by PID for neatness
            sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
                return a.pid < b.pid;
            });

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

        SJF scheduler;
        scheduler.inputProcesses(n);
        scheduler.schedule();
        scheduler.displayResults();

        return 0;
    }
