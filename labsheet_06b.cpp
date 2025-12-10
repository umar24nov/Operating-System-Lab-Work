#include <windows.h>
#include <iostream>
#include <queue>
#include <cstdlib>

using namespace std;

const int BUFFER_SIZE = 5;
queue<int> buffer;

CRITICAL_SECTION cs;
HANDLE semEmpty; // Empty slots
HANDLE semFull;  // Full slots

DWORD WINAPI producer(LPVOID param) {
    int id = (int)(size_t)param;
    int item = 0;
    while (true) {
        Sleep(500 + rand() % 500);
        item++;

        WaitForSingleObject(semEmpty, INFINITE);
        EnterCriticalSection(&cs);
        buffer.push(item);
        cout << "Producer " << id << " produced item " << item << endl;
        LeaveCriticalSection(&cs);
        ReleaseSemaphore(semFull, 1, NULL);
    }
    return 0;
}

DWORD WINAPI consumer(LPVOID param) {
    int id = (int)(size_t)param;
    while (true) {
        WaitForSingleObject(semFull, INFINITE);
        EnterCriticalSection(&cs);
        int item = buffer.front();
        buffer.pop();
        cout << "Consumer " << id << " consumed item " << item << endl;
        LeaveCriticalSection(&cs);
        ReleaseSemaphore(semEmpty, 1, NULL);

        Sleep(500 + rand() % 500);
    }
    return 0;
}

int main() {
    InitializeCriticalSection(&cs);
    semEmpty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    semFull = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);

    HANDLE p1 = CreateThread(NULL, 0, producer, (LPVOID)(size_t)1, 0, NULL);
    HANDLE p2 = CreateThread(NULL, 0, producer, (LPVOID)(size_t)2, 0, NULL);
    HANDLE c1 = CreateThread(NULL, 0, consumer, (LPVOID)(size_t)1, 0, NULL);
    HANDLE c2 = CreateThread(NULL, 0, consumer, (LPVOID)(size_t)2, 0, NULL);

    WaitForMultipleObjects(4, new HANDLE[4]{p1, p2, c1, c2}, TRUE, INFINITE);

    DeleteCriticalSection(&cs);
    CloseHandle(semEmpty);
    CloseHandle(semFull);

    return 0;
}
