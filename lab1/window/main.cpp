#include "sys-info-win.h"


int main() {
    SysInfoWin out;
    std::cout << out.GetOSVersion() << std::endl;
    out.GetRamInfo();
    std::cout << "Processors: " << out.GetProcessorCount() << std::endl;
    std::cout << "Computer Name: " << out.GetPCName() << std::endl; 
    std::cout << "User Name: " << out.GetNameUser() << std::endl;
    std::cout << "Architecture: " << out.GetArchitecture() << std::endl;
    out.GetPageFile();
    out.GetDrivesInfo();
    return 0;

}
