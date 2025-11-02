#ifndef SYS_INFO_WIN_H
#define SYS_INFO_WIN_H

#include <iostream>
#include <stdint.h>
#include <Windows.h>
#include <VersionHelpers.h>
#include <vector>
#include <lmcons.h> 
#include <psapi.h>


class SysInfoWin {
public:
    std::string GetOSVersion() const;
    void GetRamInfo() const;
    unsigned int GetProcessorCount() const;
    std::string GetPCName() const;
    std::string GetNameUser() const;
    std::string GetArchitecture() const;
    void GetPageFile() const;
    void GetDrivesInfo() const;
};




#endif