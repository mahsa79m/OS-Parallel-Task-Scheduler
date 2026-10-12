# Operating System Task Scheduler

A C++ project focused on task allocation and workload balancing using randomized assignment. The project includes both single-process and multi-process implementations, with the multi-process version using Windows API and shared memory.

## Features

- Randomized task assignment to eligible workers
- Workload calculation for each worker
- Minimization of the difference between the maximum and minimum workloads
- Single-process implementation
- Multi-process implementation using parent and child processes
- Inter-process communication through shared memory

## Technologies Used

- C++
- Windows API
- Process Management
- Shared Memory
- File I/O

## Project Structure

```text
OS-Parallel-Task-Scheduler/
├── single-process/
│   └── main.cpp
├── multi-process/
│   ├── parent.cpp
│   └── child.cpp
├── sample-input/
│   └── input.txt
├── README.md
└── README-fa.md
```

## How It Works

The program reads task durations and worker eligibility information from an input file.

It randomly assigns tasks to eligible workers, calculates each worker's total workload, and compares the difference between the maximum and minimum workloads.

The program repeats this process to search for a more balanced task allocation.

### Single-Process Version

The single-process version performs task allocation and workload evaluation within one program.

### Multi-Process Version

The multi-process version consists of a parent process and a child program.

The parent process creates multiple child processes using the Windows API. Each child explores task assignments, and shared memory is used to exchange results between the processes.

## Running the Project

**Requirements:**
- Windows
- C++ compiler with Windows API support
- Visual Studio or a compatible development environment

**Single-process version:**

1. Compile `single-process/main.cpp`.
2. Place `input.txt` in the program's working directory.
3. Run the compiled executable.

**Multi-process version:**

1. Compile `multi-process/child.cpp` as `child.exe`.
2. Compile `multi-process/parent.cpp`.
3. Place `child.exe` and `input.txt` in the working directory.
4. Run the parent executable.

## Notes

- The project uses randomized assignment rather than an exact optimization algorithm.
- The multi-process implementation uses separate processes, not threads.
- The original scheduling approach and program structure have been preserved.

## License

No license has been specified.
