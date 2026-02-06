#include "system_monitor.h"

// to track CPU usage 
static ULONGLONG prevIdle = 0, prevKernel = 0, prevUser = 0;
static bool cpuInitialized = false;

// to convert FILETIME to 64 bit 
static ULONGLONG FileTimeToULL(const FILETIME &ft) {
    return (((ULONGLONG)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

void InitCpuMonitor() {
    FILETIME idle, kernel, user;
    if (GetSystemTimes(&idle, &kernel, &user)) {
        prevIdle = FileTimeToULL(idle);
        prevKernel = FileTimeToULL(kernel);
        prevUser = FileTimeToULL(user);
        cpuInitialized = true;
    }
}

// to get Get CPU usage %
CpuMetrics GetCpuMetrics() {
    CpuMetrics m{};
    FILETIME idle, kernel, user;
    if (!cpuInitialized) InitCpuMonitor();
    if (!GetSystemTimes(&idle, &kernel, &user)) return m;

    ULONGLONG id = FileTimeToULL(idle);
    ULONGLONG kr = FileTimeToULL(kernel);
    ULONGLONG ur = FileTimeToULL(user);

    ULONGLONG sys = kr + ur;
    ULONGLONG prevSys = prevKernel + prevUser;
    ULONGLONG sysDelta = sys - prevSys;
    ULONGLONG idleDelta = id - prevIdle;

    if (sysDelta > 0)
        m.usagePercent = (double)(sysDelta - idleDelta) * 100.0 / sysDelta;

    prevIdle = id; prevKernel = kr; prevUser = ur;
    return m;
}

//to get memory usage
MemoryMetrics GetMemoryMetrics() {
    MemoryMetrics mm{};
    MEMORYSTATUSEX ms{ sizeof(ms) };
    if (GlobalMemoryStatusEx(&ms)) {
        mm.totalPhysBytes = ms.ullTotalPhys;
        mm.availPhysBytes = ms.ullAvailPhys;
        mm.percentUsed = 100.0 * (double)(ms.ullTotalPhys - ms.ullAvailPhys) / ms.ullTotalPhys;
    }
    return mm;
}

// to get disk usage 
DiskMetrics GetDiskMetrics(const std::wstring& path) {
    DiskMetrics dm{}; dm.path = path;
    ULARGE_INTEGER freeBytes, totalBytes, freeTotal;
    if (GetDiskFreeSpaceExW(path.c_str(), &freeBytes, &totalBytes, &freeTotal)) {
        dm.totalBytes = totalBytes.QuadPart;
        dm.freeBytes = freeTotal.QuadPart;
    }
    return dm;
}

// to get system uptime
SystemInfo GetSystemInfo() {
    SystemInfo si{};
    si.uptimeSeconds = GetTickCount64() / 1000ULL;
    return si;
}