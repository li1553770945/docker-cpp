#include <string>
#include "cgroup.h"
#include "sheep_random.h"
#include "sheep_args.h"
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>

using namespace std;

string CgroupConfig::GenerateRandomHostname()
{
    return sheep_random::RandomString(10, sheep_random::ALL);
}
optional<sheep_basic::Error> CgroupConfig::Parse(int argc, char **argv)
{
    sheep_args::ArgumentParser args;
    args.AddArgument("hostname", "--hostname", nullopt, this->GenerateRandomHostname());
    args.AddArgument("cpu_percent", "--cpu_percent", nullopt, nullopt);
    args.AddArgument("mem_high", "--mem_high", nullopt, nullopt);
    args.AddArgument("mem_max", "--mem_max", nullopt, nullopt);

    args.Parse(argc, argv);
    this->hostname = args.GetValue("hostname");

    auto keys = args.GetKeys();

    for (auto key : keys)
    {
        cout << key << endl;
        if (key == "cpu_percent")
        {
            int cpu_percent_temp = stoi(args.GetValue("cpu_percent"));
            if (cpu_percent_temp <= 0 || cpu_percent_temp > 100)
            {
                return {"cpu_percent must in range(1,100),can't be" + key};
            }
            this->cpu_percent = cpu_percent_temp;
        }

        if (key == "mem_high")
        {
            int mem_high_temp = stoi(args.GetValue("mem_high"));
            if (mem_high_temp <= 0)
            {
                return {"mem_high must in bigger than 0,can't be" + key};
            }
            this->mem_high = mem_high_temp;
        }

        if (key == "mem_max")
        {
            int mem_max_temp = stoi(args.GetValue("mem_max"));
            if (mem_max_temp <= 0)
            {
                return {"mem_max must in bigger than 0,can't be" + key};
            }
            this->mem_max = mem_max_temp;
        }
    }

    return nullopt;
}

optional<sheep_basic::Error> Cgroup::InitCgroup(CgroupConfig config)
{
    this->config = config;

    this->base_path = "/sys/fs/cgroup/" + this->config.hostname;
    cout << "==>hostname:" << this->config.hostname << endl;
    if (mkdir(base_path.data(), 0755) == -1)
    {
        return {sheep_basic::Error(-1, "create cgroup failded!")};
    }
    if (config.cpu_percent != 0)
    {
        string path = base_path + "/cpu.max";
        string value = to_string(config.cpu_percent * 1000);
        std::ofstream cpu_max_file(path);
        if (!cpu_max_file.is_open())
        {
            return {sheep_basic::Error(-1, "Failed to open cpu.max file")};
        }
        cpu_max_file << value; // 限制为10%的CPU使用率
        cpu_max_file.close();
        std::cout << "Cgroup and CPU limit set successfully." << std::endl;
    }

    string procs_path = base_path + "/cgroup.procs";
    std::ofstream procs_file(procs_path);
    if (!procs_file.is_open())
    {
        return {sheep_basic::Error(-1, "Failed to open cpu.max file")};
    }
    procs_file << 0; // 限制为10%的CPU使用率
    procs_file.close();
    cout << "init cgroup success!" << endl;
    return nullopt;
}

Cgroup::~Cgroup()
{
    // TODO:删除创建的cgroup
    if (rmdir(this->base_path.data()) == -1)
    {
        cout << "remove cgroup failed!" << endl;
    }
}