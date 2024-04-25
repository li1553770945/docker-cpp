#pragma once
#include <string>
#include <optional>
#include "sheep_basic.h"
#include "config/config.h"
#include <list>
using namespace std;

class Cgroup
{
    Config * config;
    string base_path;
    string GenerateRandomHostname();
public:
    Cgroup(Config * config);
    optional<sheep_basic::Error> InitCgroup();
    ~Cgroup();
};