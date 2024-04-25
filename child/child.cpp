#include "child.h"
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <linux/sched.h>
#include <sys/mount.h>
#include <memory.h>

Child::Child(const Config &config, const Logger &log) : log(log),
                                                        config(config)
{
}
void Child::Run()
{
    this->Init();
    execve(config.command.data(),NULL, NULL);
}

void Child::Init()
{
    this->InitHostname();
    this->InitMount();
}
void Child::InitHostname()
{
    sethostname(config.hostname.data(), config.hostname.length());
}
void Child::InitMount()
{
    mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL);

    string mount_dir = "/tmp/"+config.hostname;
    mkdtemp(mount_dir.data());
    mount(config.mount_dir.data(), mount_dir.data(), NULL, MS_BIND | MS_PRIVATE, NULL);
    log[Info]<<"mount dir "<<mount_dir<<endl;
    string old_mount_basename ="/oldroot."+config.hostname;
    string old_mount_dir = mount_dir + old_mount_basename;
    mkdtemp(old_mount_dir.data());
    log[Info]<<"old mount dir"<<old_mount_dir<<endl;
    syscall(SYS_pivot_root, mount_dir.data(), old_mount_dir.data());


    chdir("/"); //取消挂载原来的根目录。因为旧的目录就挂载在新的根目录下面，因此先切换到新的根目录
    umount2(old_mount_basename.data(), MNT_DETACH);
    rmdir(old_mount_basename.data());
}