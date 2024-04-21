#include "config.h"
#include "optional"
#include "sheep_args.h"

optional<sheep_basic::Error> Config::Parse(list<string> &argv_list)
{
    sheep_args::ArgumentParser args;
    args.AddArgument("mount").AddAlias("-m").SetHelp("diretory of the container image");
    args.AddArgument("command").AddAlias("-c").SetHelp("command of the container");
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
    return nullopt;
}