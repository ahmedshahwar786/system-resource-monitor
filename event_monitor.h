#pragma once
#include <string>
#include <vector>

// it will hold a simple event message
struct EventRecord {
    std::wstring message;
};

// to get recent events from Windows Event Log
std::vector<EventRecord> GetRecentEvents(const std::wstring& channel, int maxRecords = 5);