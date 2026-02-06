# System Resource & Event Monitor (Windows 11) – C++

A Windows 11 system monitoring tool written in C++ for defensive endpoint visibility. This prototype collects and summarizes real-time system metrics such as CPU usage, RAM usage, disk space, and system uptime, and (optionally) retrieves recent Windows Event Log entries. The project uses C++ `struct` best practices to model system data cleanly and prepares the codebase for modular expansion.

## Description (GitHub About)
C++ Windows 11 system resource and event monitor that tracks CPU, memory, disk, uptime, and (optionally) recent Windows Event Log entries for defensive endpoint monitoring.

## Features
- CPU usage monitoring (Windows API: `GetSystemTimes`)
- Memory usage monitoring (Windows API: `GlobalMemoryStatusEx`)
- Disk space monitoring (Windows API: `GetDiskFreeSpaceEx`)
- System uptime tracking (Windows API: `GetTickCount64`)
- Optional: Display latest System/Security/Application log entries (Windows Event Log API: `EvtQuery`, `EvtNext`)
- Structured reporting using well-defined C++ `struct`s
- Optional live refresh view (console clear / periodic polling)

## Tools & Technologies
- OS: Windows 11
- Language: C++
- Compiler: MSVC (Visual Studio) / MinGW g++ / Clang on Windows
- APIs: Win32 / Windows API, Windows Event Log API


## Struct Design (Example)
This project uses C++ `struct`s to encapsulate collected data:

- `CpuMetrics`
  - Holds CPU usage values derived from `GetSystemTimes`
- `MemoryMetrics`
  - Holds RAM totals/available/usage from `GlobalMemoryStatusEx`
- `DiskMetrics`
  - Holds free/total disk space from `GetDiskFreeSpaceEx`
- `EventMetadata` (optional)
  - Holds event ID, provider/source, level/category, timestamp, and message summary

Each struct is initialized safely, populated via API calls, and printed as a summarized report.

## How It Works
1. Collect CPU, memory, disk, and uptime metrics using Win32 API calls.
2. Store results into dedicated structs for clarity and maintainability.
3. Poll at a fixed interval (e.g., every 1–2 seconds) to simulate a live monitor.
4. (Optional) Query Windows Event Logs and display the most recent entries.

## Screenshot
<img width="1588" height="1007" alt="Output" src="https://github.com/user-attachments/assets/ca52c725-5291-491c-ad42-e6736a10ebce" />

## How to Compile & Run

 MinGW (g++)
```bash
g++ main.cpp cpu.cpp memory.cpp disk.cpp events.cpp -o monitor.exe -weffc++
.\monitor.exe
