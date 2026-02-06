#pragma once
#include <windows.h>
#include <string>

// for memory info
struct MemoryMetrics {
    DWORDLONG totalPhysBytes;
    DWORDLONG availPhysBytes;
    double percentUsed;
};

// for cPU info
struct CpuMetrics {
    double usagePercent;
};

// for disk info
struct DiskMetrics {
    std::wstring path;
    unsigned long long totalBytes;
    unsigned long long freeBytes;
};

// for system info
struct SystemInfo {
    unsigned long long uptimeSeconds;
};

// Functions
void InitCpuMonitor();
CpuMetrics GetCpuMetrics();
MemoryMetrics GetMemoryMetrics();
DiskMetrics GetDiskMetrics(const std::wstring& path);
SystemInfo GetSystemInfo();