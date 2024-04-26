#include "config.h"
#include "optional"
#include "sheep_args.h"

optional<sheep_basic::Error> Config::Parse(list<string> &argv_list)
{
    sheep_args::ArgumentParser args;
    args.AddArgument("mount").AddAlias("-m").SetHelp("diretory of the container image");
    args.AddArgument("command").AddAlias("-c").SetHelp("command of the container");
    args.AddArgument("hostname").AddAlias("--hostname");
    args.AddArgument("cpu_percent").AddAlias("--cpu_percent");
    args.AddArgument("mem_high").AddAlias("--mem_high");
    args.AddArgument("mem_max").AddAlias("--mem_max");
    args.Parse(argv_list);

    if (!args.IsHasValue("mount"))
    {
        return sheep_basic::Error("missing required parameter -m, " + args.GetHelp("mount"));
    }
    string mount = args.GetValue<string>("mount");

    if (!args.IsHasValue("command"))
    {
        return sheep_basic::Error("missing required parameter -c," + args.GetHelp("command"));
    }
    string command = args.GetValue<string>("command");
    this->mount_dir = move(mount);
    this->command = move(command);

    auto keys = args.GetKeys(true,true);
    for (auto key : keys)
    {
        auto item = args[key];
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