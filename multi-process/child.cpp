#include <iostream>
#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include <climits> 
using namespace std;
int main() {

	TCHAR Name[] = TEXT("NUMBER");
	HANDLE h = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, Name);
	int* buf = (int*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 4 * sizeof(int));
	int numtasks = buf[0];
	int numworker = buf[1];
	int time_run = buf[2];
	int numcore = buf[3];

	TCHAR cpu[] = TEXT("CPU");
	HANDLE cpu_1 = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, cpu);
	int* buf_cpu = (int*)MapViewOfFile(cpu_1, FILE_MAP_ALL_ACCESS, 0, 0, numcore * sizeof(int));
	int processId = GetCurrentProcessId();
	int id;
	for (int i = 0; i < numcore; i++) {
		if (buf_cpu[i] == processId)
		{
			id = i;
			break;
		}
		if (i == numcore - 1)
			i = -1;
	}
	int* timework = new int[numtasks];
	TCHAR Name_task[] = TEXT("TASKS");
	HANDLE r = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, Name_task);
	int* buf_task = (int*)MapViewOfFile(r, FILE_MAP_ALL_ACCESS, 0, 0, numtasks * sizeof(int));
	for (int i = 0; i < numtasks; i++) {
		timework[i] = buf_task[i];
	}

	int n = numtasks * numworker;
	int** ability = new int* [numtasks];
	for (int j = 0; j < numtasks; j++) {
		ability[j] = new int[numworker];
	}
	TCHAR Name_w[] = TEXT("DONE_JOB");
	HANDLE g = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, Name_w);
	int* buf_ability = (int*)MapViewOfFile(g, FILE_MAP_ALL_ACCESS, 0, 0, (n * sizeof(int)));
	for (int i = 0; i < numtasks; i++) {
		for (int j = 0; j < numworker; j++) {
			ability[i][j] = buf_ability[i * numworker + j];
		}
	}
	TCHAR job[] = TEXT("job");
	HANDLE job1 = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, job);
	int* assign = (int*)MapViewOfFile(job1, FILE_MAP_ALL_ACCESS, 0, 0, (numcore)*numtasks * sizeof(int));

	TCHAR sum_job[] = TEXT("sum_job");
	HANDLE sum_job1 = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, sum_job);
	int* sum_j = (int*)MapViewOfFile(sum_job1, FILE_MAP_ALL_ACCESS, 0, 0, (numcore)*numworker * sizeof(int));

	TCHAR max_min_1[] = TEXT("max_min");
	HANDLE max_min_h = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, max_min_1);
	int* buf_max_min = (int*)MapViewOfFile(max_min_h, FILE_MAP_ALL_ACCESS, 0, 0, (2 * numcore) * sizeof(int));

	TCHAR resault[] = TEXT("RESAULT");
	HANDLE resaul_1 = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, resault);
	int* buf_resault = (int*)MapViewOfFile(resaul_1, FILE_MAP_ALL_ACCESS, 0, 0, numcore * sizeof(int));


	srand(time(0) + GetCurrentProcessId());
	int* best_sum = new int[numworker];
	int* best_section_worker = new int[numtasks];
	int max_min;
	int worker;
	int best_answer = INT_MAX;
	int max_answer = INT_MIN;
	int min_answer = INT_MAX;
	int t = time(NULL);
	while (time(NULL) - t < time_run) {
		int* sum = new int[numworker];
		int* section_worker = new int[numtasks];
		int temp;
		int min_1 = INT_MAX;
		for (int i = 0; i < numworker; i++) {
			sum[i] = 0;
		}
		int max_value = INT_MIN;
		int min_value = INT_MAX;
		for (int i = 0; i < numtasks; i++) {
			do {
				worker = rand() % numworker;
				temp = sum[worker];
				temp += timework[i];
			} while (ability[i][worker] == 0 && temp > min_1);
			sum[worker] += timework[i];
			section_worker[i] = worker;
			if (sum[worker] < min_1) {
				min_1 = sum[worker];
			}
		}
		int max1 = INT_MIN;
		int min = INT_MAX;
		for (int i = 0; i < numworker; i++) {
			if (sum[i] > max1) {
				max1 = sum[i];
			}
			if (sum[i] < min) {
				min = sum[i];
			}
		}
		max_min = max1 - min;
		max_value = max1;
		min_value = min;
		if (max_min < best_answer) {
			best_answer = max_min;
			max_answer = max_value;
			min_answer = min_value;
			for (int i = 0; i < numtasks; i++) {
				best_section_worker[i] = section_worker[i];
			}
			for (int i = 0; i < numworker; i++) {
				best_sum[i] = sum[i];
			}
		}
	}
	for (int i = 0; i < numtasks; i++) {
		assign[i + (numtasks * id)] = best_section_worker[i];
	}
	for (int i = 0; i < numworker; i++) {
		sum_j[i + (numworker * id)] = best_sum[i];
	}
	buf_resault[id] = best_answer;
	buf_max_min[id * 2] = max_answer;
	buf_max_min[(id * 2) + 1] = min_answer;
	UnmapViewOfFile(buf);
	UnmapViewOfFile(buf_task);
	UnmapViewOfFile(buf_ability);
	UnmapViewOfFile(assign);
	CloseHandle(h);
	CloseHandle(r);

}