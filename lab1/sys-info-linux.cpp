#include "sys-info-linux.h"


struct utsname uname_info;
struct sysinfo sys_info;
struct mntent* drive_info;
struct statvfs stat_info;

void SysInfoLinux::core_and_distr_version() {
    std::ifstream read_OS_release("/etc/os-release");
    std::string finded_line;
    
    if(read_OS_release.is_open()) {
        std::string line;
        while(std::getline(read_OS_release, line)) {
            if(line.find("PRETTY_NAME=") == 0) {
                finded_line = line;
                break;
            }
        }
    }
    finded_line = finded_line.substr(13, finded_line.length());
    finded_line.pop_back();
    std::cout <<"OS: " << finded_line << std::endl;

    uname(&uname_info);
    std::cout << "Kernel: " << uname_info.release << std::endl;
    

    read_OS_release.close();
}


void SysInfoLinux::total_and_free_ram() {
    sysinfo(&sys_info);
    std::cout << "RAM: " << (sys_info.totalram * sys_info.mem_unit) / (1024*1024) << " MB total / "<<\
    (sys_info.freeram * sys_info.mem_unit) / (1024*1024) << " MB free" << std::endl;
}


void SysInfoLinux::nprocs() {
    std::cout << "Processors: " << get_nprocs() << std::endl;
}


void SysInfoLinux::architecture() {
    uname(&uname_info);
    std::cout << "Architecture: " << uname_info.machine << std::endl;
}


void SysInfoLinux::load_cpu() { // как я понял это типа среднее количество процесов в очереди к цпу на выполнение
    sysinfo(&sys_info);
    std::cout << "Load average: "<<sys_info.loads[0]/65536.0<<" "<<sys_info.loads[1]/65536.0<<" "<<sys_info.loads[2]/65536.0<<std::endl;
}


void SysInfoLinux::user_and_hostname() {
    char* username = getlogin();
    char hostname[128];
    gethostname(hostname, sizeof(hostname));

    std::cout << "User:" << username << std::endl;
    std::cout << "Hostname:" << hostname << std::endl;
}


void SysInfoLinux::virtual_ram() { // теоритический лимит 
    std::ifstream read_meminfo("/proc/meminfo");
    std::string finded_line;
    
    if(read_meminfo.is_open()) {
        std::string line;
        while(std::getline(read_meminfo, line)) {
            if(line.find("VmallocTotal:") == 0) {
                finded_line = line;
                break;
            }
        }
    }
    finded_line = finded_line.substr(finded_line.find(":")+1, finded_line.find(" kB") - (finded_line.find(":")+1));
    finded_line.erase(0, finded_line.find_first_not_of(" "));
    long mem = std::stol(finded_line) / 1024.0 ;
    std::cout << "Virtual Memory: " << mem << " MB" << std::endl;

    read_meminfo.close();
}


void SysInfoLinux::drives() {
    FILE* mounts_file = setmntent("/proc/mounts", "r");
    if(!mounts_file) {
        std::cout << "Ошибка с выводом информации о дисках" << std::endl;
        return;
    }

    while((drive_info = getmntent(mounts_file)) != NULL) {
        std::string device = drive_info->mnt_fsname;
        std::string mount_point = drive_info->mnt_dir;
        std::string fs_type = drive_info->mnt_type;
        if (device.find("/dev/") == 0 || device.find("/mnt/") == 0 || mount_point == "/") {

            if (statvfs(mount_point.c_str(), &stat_info) == 0) {
                double total_gb = (stat_info.f_blocks * stat_info.f_frsize) / (1024.0 * 1024.0 * 1024.0);
                double free_gb = (stat_info.f_bfree * stat_info.f_frsize) / (1024.0 * 1024.0 * 1024.0);

                std::cout << "  " << mount_point << " " << fs_type<< " " << free_gb 
                << "GB free / " << total_gb << "GB total" << std::endl;
            }
        }
    }
        endmntent(mounts_file);
    }
    
    