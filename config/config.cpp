#include "config.h"
#include "optional"
#include "sheep_args.h"

optional<sheep_basic::Error> Config::Parse(int argc, char **argv)
{
    sheep_args::ArgumentParser args;
    args.AddArgument("mount", "-m", "diretory of the container image");
    args.AddArgument("command", "-c", "command of the container");
    args.Parse(argc, argv);

    auto mount = args.GetValueWithError("mount");
    if (!mount.has_value() )
    {
        return sheep_basic::Error("missing required parameter -m, "+args.GetHelp("mount"));
    }

    auto command = args.GetValueWithError("command");
    if(!command.has_value())
    {
        return sheep_basic::Error("missing required parameter -c,"+args.GetHelp("command"));
    }

    this->mount_dir = mount.value();
    this->command = command.value();
    return nullopt;
}