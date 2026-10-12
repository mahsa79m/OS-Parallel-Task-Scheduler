#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <limits>

#define MAX_TASKS 100
#define MAX_WORKERS 30

using namespace std;

void assignTasks(int tasks, int workers, int durations[MAX_TASKS], int ability[MAX_TASKS][MAX_WORKERS], int workerAssignments[MAX_TASKS], int timeLimit) {
    int minMaxDiff = numeric_limits<int>::max();
    int bestAssignments[MAX_TASKS];

    time_t startTime = time(0); 
    while (difftime(time(0), startTime) < timeLimit) { 
        memset(workerAssignments, 0, sizeof(int) * MAX_TASKS);
        int totalTime[MAX_WORKERS] = { 0 };

        for (int i = 0; i < tasks; ++i) {
            int assigned = false;
            while (!assigned) {
                int randWorker = rand() % workers;
                if (ability[i][randWorker] == 1) {
                    workerAssignments[i] = randWorker;
                    totalTime[randWorker] += durations[i];
                    assigned = true;
                }
            }
        }
        int maxTime = totalTime[0];
        int minTime = totalTime[0];
        for (int j = 1; j < workers; ++j) {
            if (totalTime[j] > maxTime) maxTime = totalTime[j];
            if (totalTime[j] < minTime) minTime = totalTime[j];
        }

        int diff = maxTime - minTime;
        if (diff < minMaxDiff) {
            minMaxDiff = diff; 
            memcpy(bestAssignments, workerAssignments, sizeof(int) * MAX_TASKS); 
        }
    }

    cout << "Best Assignment (Task -> Worker):" << endl;
    for (int i = 0; i < tasks; ++i) {
        cout << "Task " << i + 1 << " assigned to Worker " << bestAssignments[i] + 1 << endl;
    }
    cout << "Minimum difference between max and min work: " << minMaxDiff << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    ifstream file("input.txt");
    if (!file) {
        cout << "Can not open file!" << endl;
        return 1;
    }
    int tasks, workers;
    file >> tasks >> workers;

    int durations[MAX_TASKS];
    for (int i = 0; i < tasks; ++i) {
        file >> durations[i];
    }

    int ability[MAX_TASKS][MAX_WORKERS];
    for (int i = 0; i < tasks; ++i) {
        for (int j = 0; j < workers; ++j) {
            file >> ability[i][j];
        }
    }

    file.close();

    cout << "Enter time limit in seconds: ";
    int timeLimit;
    cin >> timeLimit;

    int workerAssignments[MAX_TASKS];
    assignTasks(tasks, workers, durations, ability, workerAssignments, timeLimit);

    return 0;
}