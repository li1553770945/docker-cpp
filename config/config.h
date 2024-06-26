#pragma once
#include <unistd.h>
#include <string>
#include "optional"
#include "sheep_basic.h"
#include <list>
using namespace std;

class Config
{
public:
    optional <sheep_basic::Error> Parse(list <string> & argv_list);
    string mount_dir;
    string command;
     string hostname;
    int cpu_percent;
    int mem_high;
    int mem_max;
};