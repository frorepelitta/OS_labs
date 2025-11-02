#include "sys-info.h"

#ifdef _WIN32
    #include <Windows.h>
    #include <VersionHelpers.h>
    #include <lmcons.h>
#elif __linux__
    #include <sys/utsname.h>
    #include <sys/sysinfo.h>
    #include <fstream>
#endif

std::string SysInfo::GetOSName() const {
#ifdef _WIN32
    if (IsWindows10OrGreater()) return "Windows 10+";
    if (IsWindows8OrGreater()) return "Windows 8+";
    if (IsWindows7OrGreater()) return "Windows 7+";
    return "Windows";
#elif __linux__
    struct utsname info;
    uname(&info);
    return info.sysname;
#endif
}

std::string SysInfo::GetOSVersion() const {
#ifdef _WIN32
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    
    if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
        return std::to_string(osvi.dwMajorVersion) + "." + 
               std::to_string(osvi.dwMinorVersion) + "." + 
               std::to_string(osvi.dwBuildNumber);
    }
    return "Unknown";
#elif __linux__
    struct utsname info;
    uname(&info);
    return info.release;
#endif
}

uint64_t SysInfo::GetTotalMemory() const {
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return memInfo.ullTotalPhys;
    }
    return 0;
#elif __linux__
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.totalram * info.mem_unit;
    }
    return 0;
#endif
}

uint64_t SysInfo::GetFreeMemory() const {
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return memInfo.ullAvailPhys;
    }
    return 0;
#elif __linux__
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.freeram * info.mem_unit;
    }
    return 0;
#endif
}

unsigned SysInfo::GetProcessorCount() const {
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
#elif __linux__
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}
