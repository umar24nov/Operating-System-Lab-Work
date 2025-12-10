#include <windows.h>
#include <iostream>
#include <cstdlib>

using namespace std;

const int N = 5;
int maxEating = 2;
int eatingCount = 0;

// Synchronization
CRITICAL_SECTION cs;                // For eatingCount and printing
HANDLE chopsticks[N];               // Binary semaphores for chopsticks
HANDLE semEating;                   // Semaphore to control maxEating

DWORD WINAPI philosopher(LPVOID param) {
    int id = (int)(size_t)param;
    while (true) {
        // THINKING
        EnterCriticalSection(&cs);
        cout << "Philosopher " << id << " is THINKING." << endl;
        LeaveCriticalSection(&cs);
        Sleep(500 + rand() % 500);

        // Wait if maxEating reached
        WaitForSingleObject(semEating, INFINITE);

        int left = id;
        int right = (id + 1) % N;

        // Pick chopsticks
        if (id % 2 == 0) {
            WaitForSingleObject(chopsticks[left], INFINITE);
            WaitForSingleObject(chopsticks[right], INFINITE);
        } else {
            WaitForSingleObject(chopsticks[right], INFINITE);
            WaitForSingleObject(chopsticks[left], INFINITE);
        }

        // EATING
        EnterCriticalSection(&cs);
        eatingCount++;
        cout << "Philosopher " << id << " is EATING." << endl;
        LeaveCriticalSection(&cs);

        Sleep(500 + rand() % 500);

        // Release chopsticks
        ReleaseSemaphore(chopsticks[left], 1, NULL);
        ReleaseSemaphore(chopsticks[right], 1, NULL);

        EnterCriticalSection(&cs);
        eatingCount--;
        LeaveCriticalSection(&cs);

        ReleaseSemaphore(semEating, 1, NULL); // Allow next philosopher
    }
    return 0;
}

int main() {
    // Initialize synchronization
    InitializeCriticalSection(&cs);
    semEating = CreateSemaphore(NULL, maxEating, maxEating, NULL);

    for (int i = 0; i < N; i++)
        chopsticks[i] = CreateSemaphore(NULL, 1, 1, NULL);

    // Create philosopher threads
    HANDLE threads[N];
    for (int i = 0; i < N; i++)
        threads[i] = CreateThread(NULL, 0, philosopher, (LPVOID)(size_t)i, 0, NULL);

    // Wait for all threads (they run infinitely)
    WaitForMultipleObjects(N, threads, TRUE, INFINITE);

    // Cleanup (never reached here)
    DeleteCriticalSection(&cs);
    for (int i = 0; i < N; i++)
        CloseHandle(chopsticks[i]);
    CloseHandle(semEating);

    return 0;
}
