
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



    CgroupConfig cgroup_config;
    err = cgroup_config.Parse(argc, argv);
    if (err)
    {
        cout << err.value().GetMessage() << endl;
        return 0;
    }

    Cgroup cgroup;
    err = cgroup.InitCgroup(cgroup_config);
    if (err)
    {
        cout << err.value().GetMessage() << endl;
        return 0;
    }

    int i=0;
    while(true)
    {
        cout<<i<<endl;
        i++;
        int * p =new int[256];
    }
    return 0;
}