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
optional<sheep_basic::Error> CgroupConfig::Parse(list<string> &argv_list)
{
    sheep_args::ArgumentParser args;
    args.AddArgument("hostname").AddAlias("--hostname").SetDefaultValue(this->GenerateRandomHostname());
    args.AddArgument("cpu_percent").AddAlias("--cpu_percent").SetDefaultValue(100);
    args.AddArgument("mem_high").AddAlias( "--mem_high").SetDefaultValue(1024*1024*1024);
    args.AddArgument("mem_max").AddAlias("--mem_max").SetDefaultValue(1024*1024*1024);

    args.Parse(argv_list);
    this->hostname = args.GetValue<string>("hostname");

    auto keys = args.GetKeys();

    for (auto key : keys)
    {
        if (key == "cpu_percent")
        {
            int cpu_percent = args.GetValue<int>("cpu_percent");
            if (cpu_percent <= 0 || cpu_percent > 100)
            {
                return {"cpu_percent must in range(1,100),can't be " + to_string(cpu_percent)};
            }
            this->cpu_percent = move(cpu_percent);
        }

        if (key == "mem_high")
        {
            int mem_high = args.GetValue<int>("mem_high");
            if (mem_high <= 0)
            {
                return {"mem_high must in bigger than 0,can't be " + to_string(mem_high)};
            }
            this->mem_high = move(mem_high);
        }

        if (key == "mem_max")
        {
            int mem_max = args.GetValue<int>("mem_max");
            if (mem_max <= 0)
            {
                return {"mem_max must in bigger than 0,can't be " + to_string(mem_max)};
            }
            this->mem_max = mem_max;
        }
    }

    return nullopt;
}

optional<sheep_basic::Error> Cgroup::InitCgroup(const CgroupConfig & config)
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
        ofstream cpu_max_file(path);
        if (!cpu_max_file.is_open())
        {
            return {sheep_basic::Error(-1, "Failed to open cpu.max file")};
        }
        cpu_max_file << value;
        cpu_max_file.close();
        cout << "==> CPU limit set " + to_string(config.cpu_percent) + " successfully." << endl;
    }

    if (config.mem_high != 0)
    {
        string path = base_path + "/memory.high";
        string value = to_string(config.mem_high);
        ofstream mem_high_file(path);
        if (!mem_high_file.is_open())
        {
            return {sheep_basic::Error(-1, "Failed to open cpu.max file")};
        }
        mem_high_file << value;
        mem_high_file.close();
        cout << "==> mem_high limit set " + to_string(config.mem_high) + " successfully." << endl;
    }

    if (config.mem_max != 0)
    {
        string path = base_path + "/memory.max";
        string value = to_string(config.mem_max);
        ofstream mem_max_file(path);
        if (!mem_max_file.is_open())
        {
            return {sheep_basic::Error(-1, "Failed to open cpu.max file")};
        }
        mem_max_file << value;
        mem_max_file.close();
        cout << "==> mem_max set " + to_string(config.mem_max) + " successfully." << endl;
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