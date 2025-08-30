# CMSC 395 Lab 3 — Shared Memory Programming with PThreads

**Type:** Course Work  
**Institution:** Longwood University  
**Course:** CMSC 395 High Performance Computing  
**Date:** 25 March 2021  
**Grade:** 98/100  

---

## Description
This repository contains my submission for Lab 3 of *High Performance Computing*. The lab focused on shared memory programming with the POSIX threads (PThreads) library. The work builds on earlier labs involving sequential algorithms and message passing (MPI) by demonstrating how shared memory parallelism can be used to achieve higher performance for problems requiring frequent data sharing.

The project explored three main tasks:

1. **Network Summation Server**  
   A simple multi-threaded server built with sockets and PThreads. Each thread handled incoming client requests, summing contributions into a shared running total.

2. **Parallel Matrix Multiplication**  
   A multi-threaded version of matrix multiplication using PThreads. Work was divided across threads into blocks of rows and columns, timed, and compared against the sequential baseline. Performance was measured with different numbers of threads (1, 4, 16) and graphed in LibreOffice.

3. **The “Lair” Game with Synchronization**  
   A real-time trading game simulation provided by the instructor. The assignment required reintroducing synchronization primitives (`pthread_mutex`, `pthread_barrier`) to prevent race conditions, enforce fairness, and ensure correct game behavior under concurrent threads.

---

## Repository Contents
- `running_total.c`, `ezsocket.c`, `ezsocket.h` — Code for the multi-threaded summation server.  
- `matrix_mult.c` — Parallel implementation of matrix multiplication.  
- `generate_matrix.py` — Utility script for creating random test matrices.  
- `test1024.txt`, `output.txt` — Input and reference output files for verification.  
- `lair.c`, `player.h`, `player.c`, `offer.h`, `offer.c` — Game code requiring synchronization structures.  
- `Graphs.ods` — Performance results comparing thread counts.  
- `Lab3-PThreads.pdf` — Assignment specification.  

---

## How to Build and Run

### Summation Server
Compile:
```bash
gcc running_total.c ezsocket.c -pthread -o Total -g
```

Run server:
```bash
./Total 8889
```
Use a script to test multiple connections:

```bash
#!/bin/bash
count=0
while [ $count -lt 25 ]; do
  sleep 1
  echo 5 | nc localhost 8889
  count=$((count + 1))
done
```

### Parallel Matrix Multiplication
Compile:
```bash
Copy code
gcc -pthread matrix_mult.c -g -o Mult
```

Run:
```bash
./Mult test1024.txt > test.out
diff test.out output.txt
```

### Lair Game
Compile with pthread support:
```bash
Copy code
gcc lair.c player.c offer.c -pthread -o Lair
```

Run:
```bash
Copy code
./Lair
```

## Learning Objectives
- Understand the tradeoffs between MPI and shared memory programming
- Gain practical experience with thread creation, synchronization, and performance measurement
- Explore mutexes and barriers for preventing race conditions and ensuring fairness
- Apply high-performance computing concepts to real-world algorithms (matrix multiplication) and simulations (multi-threaded game AI)

## Notes
- This project was completed individually as part of CMSC 395 coursework
- The assignment details can be found in Lab3-PThreads.pdf
- Performance graphs were created in LibreOffice
