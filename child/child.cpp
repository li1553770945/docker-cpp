#include "child.h"
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <linux/sched.h>
Child::Child(const Config &config, const Logger &log) : log(log),
                                                        config(config)
{
}
void Child::Run()
{
    this->Init();
    const int STACK_SIZE = 65536;
    char *stack = new char[STACK_SIZE];
    if (!stack)
    {
        log[Fatal] << "Can't create stack for child process";
        exit(1);
    }

    struct clone_args cl_args = {
        .flags = CLONE_NEWNS | CLONE_NEWCGROUP | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWUTS,
        .pidfd = 0,
        .child_tid = 0,
        .parent_tid = 0,
        .exit_signal = SIGCHLD,
        .stack = (uintptr_t)stack,
        .stack_size = STACK_SIZE,
        .tls = 0,
        .set_tid = 0,
        .set_tid_size = 0,
        .cgroup = 0};

    int child_pid = syscall(SYS_clone3, &cl_args, sizeof(cl_args));
    if (child_pid == -1)
    {
        log[Fatal] << "clone3 syscall failed";
        exit(1);
    }

    wait(NULL);
    free(stack);
}

void Child::Init()
{
    this->InitMount();
}
void Child::InitMount()
{
}