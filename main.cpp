#include <iostream>
#include <thread>
#include <chrono>
#include "system_monitor.h"
#include "event_monitor.h"

// it will convert bytes to readable format 
static std::string BytesToHuman(unsigned long long bytes) {
    const char* suffix[] = {"B","KB","MB","GB","TB"};
    double val = (double)bytes;
    int i = 0;
    while (val >= 1024.0 && i < 4) { val /= 1024.0; ++i; }
    char buf[64];
    sprintf_s(buf, "%.2f %s", val, suffix[i]);
    return std::string(buf);
}

int main() {
    using namespace std::chrono_literals;
    std::cout << "Windows System Monitor\n";
    std::cout << "Press Ctrl + C to stop.\n\n";

    InitCpuMonitor();

    while (true) {
        CpuMetrics cpu = GetCpuMetrics();
        MemoryMetrics mem = GetMemoryMetrics();
        DiskMetrics disk = GetDiskMetrics(L"C:\\");
        SystemInfo si = GetSystemInfo();

        // to clear console for new display
        system("cls");
        std::cout << "Windows 11 System Monitor\n";
        std::cout << "-------------------------\n";
        printf("CPU Usage: %.2f%%\n", cpu.usagePercent);
        printf("RAM Used: %.2f%% (Avail: %s / Total: %s)\n",
               mem.percentUsed,
               BytesToHuman(mem.availPhysBytes).c_str(),
               BytesToHuman(mem.totalPhysBytes).c_str());
        printf("Disk (C:) Free: %s / Total: %s\n",
               BytesToHuman(disk.freeBytes).c_str(),
               BytesToHuman(disk.totalBytes).c_str());
        printf("Uptime: %llu seconds (~%llu hours)\n",
               si.uptimeSeconds, si.uptimeSeconds/3600ULL);

        // recent events
        std::cout << "\nRecent Events : :\n";

        auto appEvents = GetRecentEvents(L"Application", 3);
        auto sysEvents = GetRecentEvents(L"System", 3);

        std::cout << "\n * Application Events * \n";
        if (appEvents.empty()) std::cout << "(none)\n";
        for (auto &e : appEvents) {
            std::wcout << e.message << L"\n---\n";
        }

        std::cout << "\n ** System Events ** \n";
        if (sysEvents.empty()) std::cout << "(none)\n";
        for (auto &e : sysEvents) {
            std::wcout << e.message << L"\n---\n";
        }

        std::this_thread::sleep_for(10s); // it will refresh every 10 seconds
    }

    return 0;
}