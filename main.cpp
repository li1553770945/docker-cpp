
#include <iostream>
#include <cstring>
#include "config/config.h"
#include "cgroup/cgroup.h"
#include "sheep_log.h"
#include "child/child.h"
#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <linux/sched.h>

using namespace std;
using namespace sheep_log;

Logger GetLogger()
{
    Logger logger = Logger("main");
    std::shared_ptr<StreamHandler> sh = std::make_shared<StreamHandler>();
    sh->SetLevel(Info);
    logger.AddHandler(sh);
    return logger;
}

int ChildFunction(void *arg)
{
    Config *config = (Config *)arg;
    cout << "this is child" << endl;
    sheep_log::Logger log = GetLogger();
    Child child(*config, log);
    child.Run();
    return 0;
}
int main(int argc, char **argv)
{

    Config config;
    sheep_log::Logger log = GetLogger();
    list<string> argv_list;
    for (int i = 1; i < argc; i++)
    {
        argv_list.push_back(argv[i]);
    }
    auto err = config.Parse(argv_list);
    if (err)
    {
        log[Fatal] << err.value().GetMessage() << endl;
        return 0;
    }

    if (!argv_list.empty())
    {
        log[Fatal] << "==>error:unknow parameter:" + argv_list.front() << endl;
        return 0;
    }
    Cgroup cgroup(&config,log);
    err = cgroup.InitCgroup();
    if (err)
    {
        log[Fatal] << err.value().GetMessage() << endl;
        return 0;
    }
    const int STACK_SIZE = 65536;
    char *stack = new char[STACK_SIZE];
    if (!stack)
    {
        log[Fatal] << "Can't create stack for child process";
        exit(1);
    }

    int flags = CLONE_NEWNS | CLONE_NEWCGROUP | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWUTS;
    int child_pid = clone(ChildFunction, stack + STACK_SIZE, flags | SIGCHLD, &config);
    if (child_pid == -1)
    {
        log[Error] << "clone3 syscall failed:" << strerror(errno) << endl;
        exit(1);
    }
    log[Info]<<"child pid:"<<child_pid<<endl;
    log[Info] << "this is father,waiting for child" << endl;
    wait(NULL); // 等待子进程结束

    free(stack);
    return 0;
}