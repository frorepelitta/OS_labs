#include <iostream>
#include <iomanip>
#include "sys-info.h"

int main() {
    SysInfo sys;
    std::cout << "OS Name: " << sys.GetOSName() << std::endl;
    std::cout << "OS Version: " << sys.GetOSVersion() << std::endl;
    std::cout << "Processors: " << sys.GetProcessorCount() << std::endl;
    
    uint64_t totalMem = sys.GetTotalMemory();
    uint64_t freeMem = sys.GetFreeMemory();
    uint64_t usedMem = totalMem - freeMem;
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "RAM: " << usedMem << " GB / " <<totalMem<< " GB" << std::endl;
    std::cout << "Free Memory: " << freeMem / (1024.0 * 1024 * 1024) << " GB" << std::endl;
    
    return 0;
}
