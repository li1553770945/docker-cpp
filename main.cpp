
#include <iostream>
#include <cstring>
#include "config/config.h"
#include "args.h"
using namespace std;

optional <Config> InitConfig(int argc,char **argv)
{
    sheep_args::ArgumentParser args;
    args.AddArgument("dir","-m","diretory of the container image");
    args.Parse(argc,argv);
    auto dir = args.GetValue("dir");
    if(!dir.has_value())
    {
        return nullopt;
    }
    Config config;
    config.mount_dir = dir.value();
    return config;
}
int main (int argc, char **argv) 
{
    auto config1 = InitConfig(argc,argv);
    if(!config1.has_value())
    {
        return 0;
    }
    Config config = config1.value();
    cout<<config.mount_dir<<endl;
    // Step 1: parse CLI parameters, validate Linux kernel/cpu/..., generate hostname for container

    // Step 2: setup a socket pair for container sending messages to the parent process
    
    // Step 3: allocate stack space for `execve()`
   
    // Step 4: setup cgroup for the container for resource isolation
 

    // Step 5: launch container 将 mount, pid, ipc, network device, hostname/domainname 放到独立的 namespace
    

    // Step 6: error handling and cleanup
    return 0;
}