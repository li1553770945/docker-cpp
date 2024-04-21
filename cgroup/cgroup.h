#include <string>
#include <optional>
#include "sheep_basic.h"
#include <list>
using namespace std;
class CgroupConfig
{
    friend class Cgroup;

public:
    string hostname;
    int cpu_percent;
    int mem_high;
    int mem_max;

public:
    string GenerateRandomHostname();
    optional<sheep_basic::Error> Parse(list<string> &argv_list);
};
class Cgroup
{
    CgroupConfig config;
    string base_path;

public:
    optional<sheep_basic::Error> InitCgroup(const CgroupConfig & config);
    ~Cgroup();
};