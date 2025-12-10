#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m; // n = number of processes, m = number of resources
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resources: ";
    cin >> m;

    vector<vector<int>> Allocation(n, vector<int>(m));
    vector<vector<int>> Max(n, vector<int>(m));
    vector<vector<int>> Need(n, vector<int>(m));
    vector<int> Available(m);

    // Input Allocation matrix
    cout << "\nEnter Allocation Matrix (Process x Resource):\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> Allocation[i][j];

    // Input Max matrix
    cout << "\nEnter Max Matrix (Process x Resource):\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> Max[i][j];

    // Input Available matrix
    cout << "\nEnter Available Resources (Total of each resource type):\n";
    for (int j = 0; j < m; j++)
        cin >> Available[j];

    // Calculate Need matrix = Max - Allocation
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            Need[i][j] = Max[i][j] - Allocation[i][j];

    // Display Need matrix
    cout << "\nNeed Matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            cout << Need[i][j] << " ";
        cout << endl;
    }

    // Banker's Algorithm: Safety Check
    vector<bool> Finish(n, false);
    vector<int> SafeSeq(n);
    vector<int> work = Available;

    int count = 0;
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!Finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < m; j++)
                    if (Need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                if (canAllocate) {
                    for (int j = 0; j < m; j++)
                        work[j] += Allocation[i][j];
                    SafeSeq[count++] = i;
                    Finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            cout << "\nSystem is NOT in a safe state!\n";
            return 0;
        }
    }

    // Print safe sequence
    cout << "\nSystem is in a safe state.\nSafe Sequence: ";
    for (int i = 0; i < n; i++) {
        cout << "P" << SafeSeq[i];
        if (i != n - 1) cout << " -> ";
    }
    cout << endl;

    // Handle new resource request
    int reqProcess;
    cout << "\nEnter the process number making new resource request: ";
    cin >> reqProcess;

    vector<int> Request(m);
    cout << "Enter the request for each resource: ";
    for (int j = 0; j < m; j++)
        cin >> Request[j];

    // Check if request <= Need
    bool valid = true;
    for (int j = 0; j < m; j++)
        if (Request[j] > Need[reqProcess][j]) {
            valid = false;
            break;
        }

    if (!valid) {
        cout << "Error: Process has exceeded its maximum claim.\n";
        return 0;
    }

    // Check if request <= Available
    for (int j = 0; j < m; j++)
        if (Request[j] > Available[j]) {
            cout << "Resources not available. Process must wait.\n";
            return 0;
        }

    // Tentatively allocate requested resources
    for (int j = 0; j < m; j++) {
        Available[j] -= Request[j];
        Allocation[reqProcess][j] += Request[j];
        Need[reqProcess][j] -= Request[j];
    }

    // Safety check again
    Finish.assign(n, false);
    work = Available;
    count = 0;
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!Finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < m; j++)
                    if (Need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                if (canAllocate) {
                    for (int j = 0; j < m; j++)
                        work[j] += Allocation[i][j];
                    SafeSeq[count++] = i;
                    Finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    if (count == n) {
        cout << "\nAfter request, system is still in a safe state.\nSafe Sequence: ";
        for (int i = 0; i < n; i++) {
            cout << "P" << SafeSeq[i];
            if (i != n - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "\nAfter request, system is NOT in a safe state. Rolling back allocation.\n";
        for (int j = 0; j < m; j++) {
            Available[j] += Request[j];
            Allocation[reqProcess][j] -= Request[j];
            Need[reqProcess][j] += Request[j];
        }
    }

    return 0;
}
