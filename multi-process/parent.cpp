#include <iostream>
#include <windows.h>
#include <fstream>
#include <ctime>
#include <cstdlib>  
#include <climits> 
using namespace std;

int main() {

	int time_run;
	cout << "enter the time >> ";
	cin >> time_run;

	SYSTEM_INFO sysinfo; GetSystemInfo(&sysinfo);
	int numcore = sysinfo.dwNumberOfProcessors;

	ifstream file;
	file.open("input.txt");
	int numtasks, numworker;
	file >> numtasks >> numworker;
	cout << numtasks << " " << numworker << endl;

	int* timework = new int[numtasks];
	for (int i = 0; i < numtasks; i++) {
		file >> timework[i];
	}
	for (int i = 0; i < numtasks; i++) {
		cout << timework[i] << " ";
	}
	cout << endl;
	int** ability = new int* [numtasks];
	for (int j = 0; j < numtasks; j++) {
		ability[j] = new int[numworker];
	}
	for (int i = 0; i < numtasks; i++) {
		for (int j = 0; j < numworker; j++) {
			file >> ability[i][j];
		}
	}
	for (int i = 0; i < numtasks; i++) {
		for (int j = 0; j < numworker; j++) {
			cout << ability[i][j] << " ";
		}
		cout << endl;
	}
	file.close();


	TCHAR Name[] = TEXT("NUMBER");
	HANDLE h = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4 * sizeof(int), Name);
	int* buf = (int*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 4 * sizeof(int));
	buf[0] = numtasks;
	buf[1] = numworker;
	buf[2] = time_run;
	buf[3] = numcore;

	TCHAR cpu[] = TEXT("CPU");
	HANDLE cpu_1 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, numcore * sizeof(int), cpu);
	int* buf_cpu = (int*)MapViewOfFile(cpu_1, FILE_MAP_ALL_ACCESS, 0, 0, numcore * sizeof(int));

	TCHAR Name_task[] = TEXT("TASKS");
	HANDLE r = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, numtasks * sizeof(int), Name_task);
	int* buf_task = (int*)MapViewOfFile(r, FILE_MAP_ALL_ACCESS, 0, 0, numtasks * sizeof(int));
	for (int i = 0; i < numtasks; i++) {
		buf_task[i] = timework[i];
	}

	int n;
	n = numtasks * numworker;
	TCHAR Name_w[] = TEXT("DONE_JOB");
	HANDLE g = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (n * sizeof(int)), Name_w);
	int* buf_ability = (int*)MapViewOfFile(g, FILE_MAP_ALL_ACCESS, 0, 0, (n * sizeof(int)));
	for (int i = 0; i < numtasks; i++) {
		for (int j = 0; j < numworker; j++) {
			buf_ability[i * numworker + j] = ability[i][j];
		}
	}
	TCHAR job[] = TEXT("job");
	HANDLE job1 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (numcore)*numtasks * sizeof(int), job);
	int* assign = (int*)MapViewOfFile(job1, FILE_MAP_ALL_ACCESS, 0, 0, (numcore)*numtasks * sizeof(int));

	TCHAR sum_job[] = TEXT("sum_job");
	HANDLE sum_job1 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (numcore)*numworker * sizeof(int), sum_job);
	int* sum_j = (int*)MapViewOfFile(sum_job1, FILE_MAP_ALL_ACCESS, 0, 0, (numcore)*numworker * sizeof(int));

	TCHAR max_min_1[] = TEXT("max_min");
	HANDLE max_min_h = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (2 * numcore) * sizeof(int), max_min_1);
	int* buf_max_min = (int*)MapViewOfFile(max_min_h, FILE_MAP_ALL_ACCESS, 0, 0, (2 * numcore) * sizeof(int));

	TCHAR resault[] = TEXT("RESAULT");
	HANDLE resaul_1 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, numcore * sizeof(int), resault);
	int* buf_resault = (int*)MapViewOfFile(resaul_1, FILE_MAP_ALL_ACCESS, 0, 0, numcore * sizeof(int));

	STARTUPINFO* s = new STARTUPINFO[numcore];
	PROCESS_INFORMATION* p = new PROCESS_INFORMATION[numcore];
	for (int i = 0; i < numcore; i++) {
		ZeroMemory(&s[i], sizeof(s[i]));
		s[i].cb = sizeof(s[i]);
		ZeroMemory(&p[i], sizeof(p[i]));
		bool b = CreateProcess(TEXT("child.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &s[i], &p[i]);
		if (!b) {
			cout << "create process failed" << endl;
			exit(-1);
		}
		buf_cpu[i] = p[i].dwProcessId;
		
	}
	
	for (int i = 0; i < numcore; i++) {
		WaitForSingleObject(p[i].hProcess, INFINITE);
		CloseHandle(p[i].hProcess);
		CloseHandle(p[i].hThread);
	}
	int* max_value = new int[numcore];
	int* min_value = new int[numcore];
	int max_min;
	int best_answer = INT_MAX;
	int child;
	for (int i = 0; i < numcore; i++) {
		cout << "child " << i + 1 << " : ";
		if (buf_resault[i]) {
			max_min = buf_resault[i];
			cout << "max - min = " << max_min << endl;
			if (max_min < best_answer) {
				best_answer = max_min;
				child = i+1;
			}
		}

	}
	cout << "_________________________________" << endl;
	cout << "best answer = " << "child " << child << ":" <<" max - min = " << best_answer << endl;
	for (int k = child-1; k < child; k++) {
		cout << "child " << k + 1 << " : " << endl;
		for (int i = 0; i < numtasks; i++) {
			cout << "work " << i + 1 << "to " << "worker " << assign[i + (k * numtasks)] + 1 << endl;
		}
		for (int i = 0; i < numworker; i++) {
			cout << "worker " << i + 1 << " = " << sum_j[i + (k * numworker)] << endl;
		}

		max_value[k] = buf_max_min[k * 2];
		min_value[k] = buf_max_min[(k * 2) + 1];
		cout << "max = " << max_value[k] << endl;
		cout << "min = " << min_value[k] << endl;
		cout << "max - min = " << max_value[k] - min_value[k] << endl;
		cout << "_________________________________" << endl;
	}
}