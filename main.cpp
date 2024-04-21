
#include <iostream>
#include <cstring>
#include "config/config.h"
#include "cgroup/cgroup.h"
#include <string>
#include <vector>
#include <list>
using namespace std;

int main(int argc, char **argv)
{
    Config config;
    list <string> argv_list;
    for(int i=1;i<argc;i++)
    {
        argv_list.push_back(argv[i]);
    }
    auto err = config.Parse(argv_list);
    if (err)
    {
        cout << err.value().GetMessage() << endl;
        return 0;
    }


    CgroupConfig cgroup_config;
    err = cgroup_config.Parse(argv_list);
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
    cout<<argv_list.size()<<endl;
    if(!argv_list.empty())
    {
        cout<<"==>error:unknow parameter:"+argv_list.front()<<endl;
        return 0;
    }
    int i=0;
    // while(true)
    // {
    //     cout<<i<<endl;
    //     i++;
    //     int * p =new int[256];
    // }
    return 0;
}