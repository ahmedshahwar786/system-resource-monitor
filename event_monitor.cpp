#include "event_monitor.h"
#include <windows.h>
#include <winevt.h>
#include <vector>
#pragma comment(lib, "wevtapi.lib")

// to convert an event to XML text
static std::wstring RenderEventToXml(EVT_HANDLE hEvent) {
    DWORD used = 0, status = 0;
    if (!EvtRender(NULL, hEvent, EvtRenderEventXml, 0, NULL, &used, &status)) {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            std::vector<wchar_t> buf(used / sizeof(wchar_t));
            if (EvtRender(NULL, hEvent, EvtRenderEventXml, used, buf.data(), &used, &status))
                return std::wstring(buf.data());
        }
    }
    return L"";
}

// to get last events from a log channel
std::vector<EventRecord> GetRecentEvents(const std::wstring& channel, int maxRecords) {
    std::vector<EventRecord> out;
    EVT_HANDLE hQuery = EvtQuery(NULL, channel.c_str(), L"*[System]", 
                                 EvtQueryChannelPath | EvtQueryReverseDirection);
    if (!hQuery) return out;

    EVT_HANDLE events[8]; DWORD returned = 0;
    while ((int)out.size() < maxRecords) {
        if (!EvtNext(hQuery, 1, events, 0, 0, &returned)) break;
        EventRecord r{};
        r.message = RenderEventToXml(events[0]);
        out.push_back(r);
        EvtClose(events[0]);
    }
    EvtClose(hQuery);
    return out;
}