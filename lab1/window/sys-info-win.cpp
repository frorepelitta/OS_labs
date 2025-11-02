#include "sys-info-win.h"


MEMORYSTATUSEX memory_info;
SYSTEM_INFO system_info;
PERFORMANCE_INFORMATION performance_information;



std::string SysInfoWin::GetOSVersion() const{
    if(IsWindows10OrGreater()) {
        return "OS: Windows 10 or Greater";
    }
    if(IsWindows8OrGreater()) {
        return "OS: Windows 8 or Greater";
    }
    if(IsWindows7OrGreater()) {
        return "OS: Windows 7 or Greater";
    }
    return "Either you have a very old Windows or a very new one";
}


void SysInfoWin::GetRamInfo() const{
    memory_info.dwLength = sizeof(memory_info);

    if (GlobalMemoryStatusEx(&memory_info)) {
        double virtual_mem = (double)memory_info.ullTotalPageFile / (1024 * 1024);
        double physical_mem = (double)memory_info.ullTotalPhys / (1024 * 1024);
        double available_mem = (double)memory_info.ullAvailPhys / (1024 * 1024);
        int usagePercent = memory_info.dwMemoryLoad;

        std::cout << "RAM: "<<available_mem<<"MB / "<<physical_mem<<"MB" << std::endl;
        std::cout << "Virtual Memory: " << virtual_mem << "MB" << std::endl;
        std::cout << "Memory Load: " << usagePercent << "%" << std::endl;
    }
}


unsigned int SysInfoWin::GetProcessorCount() const {
    GetSystemInfo(&system_info);
    return system_info.dwNumberOfProcessors;
}


std::string SysInfoWin::GetPCName() const {
    DWORD size = MAX_COMPUTERNAME_LENGTH+1;
    std::vector<CHAR> buffer(size);
    
    if (GetComputerNameA(buffer.data(), &size)) {
        return buffer.data();
    }
    return "GetPcName Error";
}
  


std::string SysInfoWin::GetNameUser() const {
    DWORD size = UNLEN + 1;
    std::vector<CHAR> buffer(size);
    
    if (GetUserNameA(buffer.data(), &size)) {
        return buffer.data();
    }
    return "GetUserName Error";
}


std::string SysInfoWin::GetArchitecture() const {
    int architec = system_info.wProcessorArchitecture;
    if(architec == 0) {
        return "x86";
    }else if(architec == 5) {
        return "ARM";
    }else if(architec == 9) {
        return "x64";
    }else if(architec == 12) {
        return "ARM64";
    } else {
        return "GetArchiteture Error";
    }
}


void SysInfoWin::GetPageFile() const{
    memory_info.dwLength = sizeof(memory_info);

    if (GlobalMemoryStatusEx(&memory_info)) {
        uint64_t totalPageFile = memory_info.ullTotalPageFile;
        uint64_t availablePageFile = memory_info.ullAvailPageFile;
        uint64_t usedPageFile = totalPageFile - availablePageFile;
        
        std::cout << "PageFile: " << usedPageFile / (1024.0 * 1024) 
                  << "MB / " << totalPageFile / (1024.0 * 1024) << "MB" << std::endl;
    }
}


void SysInfoWin::GetDrivesInfo() const {
        std::cout << "Drives:" << std::endl;
        for (char drive = 'C'; drive <= 'Z'; drive++) {
            std::string path = std::string(1, drive) + ":\\";
            ULARGE_INTEGER free, total;
            
            if (GetDiskFreeSpaceExA(path.c_str(), &free, &total, NULL)) {
                std::cout << "  -  " << path << free.QuadPart / (1024*1024*1024) 
                          << " GB / " << total.QuadPart / (1024*1024*1024) << " GB" << std::endl;
            }
        }
    }
