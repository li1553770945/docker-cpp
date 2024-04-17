
#include <iostream>
#include <cstring>
#include "config/config.h"
#include "cgroup/cgroup.h"
using namespace std;

int main(int argc, char **argv)
{
    Config config;
    auto err = config.Parse(argc, argv);
    if (err)
    {
        cout << err.value().GetMessage() << endl;
        return 0;
    }

    cout << config.mount_dir << endl;
    cout << config.command << endl;

    CgroupConfig cgroup_config;
    err = cgroup_config.Parse(argc, argv);
    if (err)
    {
        cout << err.value().GetMessage() << endl;
        return 0;
    }
    cout<<cgroup_config.cpu_percent<<endl;

    Cgroup cgroup;
    err = cgroup.InitCgroup(cgroup_config);
    if (err)
    {
        cout << err.value().GetMessage() << endl;
        return 0;
    }
    // Step 1: parse CLI parameters, validate Linux kernel/cpu/..., generate hostname for container

    // Step 2: setup a socket pair for container sending messages to the parent process

    // Step 3: allocate stack space for `execve()`

    // Step 4: setup cgroup for the container for resource isolation

    // Step 5: launch container 将 mount, pid, ipc, network device, hostname/domainname 放到独立的 namespace

    // Step 6: error handling and cleanup
    while(true)
    {
        ;
    }
    int x;
    cin>>x;
    return 0;
}