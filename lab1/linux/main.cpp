#include "sys-info-linux.h"

int main() {
    SysInfoLinux out;
    out.core_and_distr_version();
    out.total_and_free_ram();
    out.nprocs();
    out.architecture();
    out.load_cpu();
    out.user_and_hostname();
    out.virtual_ram();
    out.drives();
    return 0;
}
