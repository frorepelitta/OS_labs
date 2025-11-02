#ifndef SYS_INFO_H
#define SYS_INFO_H

#include <string>
#include <cstdint>


#ifdef _WIN32
    #include <Windows.h>
    #include <VersionHelpers.h>
    #include <lmcons.h>
#elif __linux__
    #include <sys/utsname.h>
    #include <sys/sysinfo.h>
    #include <fstream>
    #include <unistd.h>
#endif

class SysInfo
{
public:
    std::string GetOSName() const;
    std::string GetOSVersion() const;
    uint64_t GetFreeMemory() const;
    uint64_t GetTotalMemory() const;
    unsigned GetProcessorCount() const;
};

#endif
