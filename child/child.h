#include "config/config.h"
#include "cgroup/cgroup.h"
#include "sheep_log.h"
using namespace sheep_log;
class Child
{
    const Config & config;
    Logger log;
     vector<string> tokens;
    std::vector<char*> args;
    void Init();
    void InitHostname();
    void InitMount();
    void InitArgs();
public:
    Child(const Config &config, const Logger &log);
    
    void Run();
};