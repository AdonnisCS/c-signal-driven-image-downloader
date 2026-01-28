# C Signal-Driven Downloader

A lightweight systems programming project demonstrating **Inter-Process Communication (IPC)** via POSIX signals and **concurrent process management** in C.

The program sits in an idle loop waiting for user-defined signals. Upon receiving a specific signal (`SIGUSR1`), it spawns a child process to download data asynchronously, ensuring the main program remains responsive. It also implements a graceful shutdown mechanism to ensure all child processes are properly reaped before exiting.

## Key Concepts Implemented

* **Signal Handling:** Custom handlers for `SIGUSR1` (trigger action) and `SIGINT` (shutdown).
* **Concurrency:** Uses `fork()` to create child processes for handling network requests without blocking the main execution loop.
* **Process Management:** Implements `waitpid()` loops to prevent zombie processes and ensure clean resource release.
* **System Calls:** Direct usage of UNIX system calls (`unistd.h`, `sys/wait.h`, `sys/types.h`).

## Prerequisites

* GCC Compiler
* Linux/Unix environment (or WSL on Windows)
* `curl` installed on the system
* A target web server (Default is configured for a specific UChicago CS internal endpoint)

## Usage

### 1. Compilation
Compile the program using `gcc`.

```bash
gcc -o downloader main.c
