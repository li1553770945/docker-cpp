#pragma once
#include <string>
#include <optional>
#include "sheep_basic.h"
#include "sheep_log.h"
#include "config/config.h"
#include <list>
using namespace std;

class Cgroup
{
    Config * config;
    string base_path;
    string GenerateRandomHostname();
    sheep_log::Logger & log;
public:
    Cgroup(Config * config,sheep_log::Logger & logger);
    optional<sheep_basic::Error> InitCgroup();
    ~Cgroup();
};