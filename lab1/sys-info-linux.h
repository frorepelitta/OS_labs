#ifndef SYS_INFO_LINUX_H

#define SYS_INFO_LINUX_H

#include <iostream>
#include <sys/utsname.h>
#include <fstream>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <mntent.h>
#include <sys/statvfs.h>


class SysInfoLinux {
public:
    void core_and_distr_version();
    void total_and_free_ram();
    void nprocs();
    void architecture();
    void load_cpu();
    void user_and_hostname();
    void virtual_ram();
    void drives();
};


#endif