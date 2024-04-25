#include "config/config.h"
#include "cgroup/cgroup.h"
#include "sheep_log.h"
using namespace sheep_log;
class Child
{
    const Config & config;
    Logger log;
    void Init();
    void InitHostname();
    void InitMount();
public:
    Child(const Config &config, const Logger &log);
    
    void Run();
};