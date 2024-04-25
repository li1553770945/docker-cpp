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
    this->Init();
}
void Child::Run()
{
    const char *args[] = {"/bin/sh",  nullptr};
    log[Info]<<"running..."<<endl;
    execve("/bin/sh",const_cast<char* const*>(args), NULL);
    log[Info]<<"running end"<<endl;
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
    int res;
    res = mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL);
    if (res)
    {
        log[Fatal] << "set / to MS_PRIVATE FAILED! " << strerror(errno) << endl;
    }
    string mount_dir = "/tmp/" + config.hostname + "XXXXXX";
    mkdtemp(mount_dir.data());
    res = mount(config.mount_dir.data(), mount_dir.data(), NULL, MS_BIND | MS_PRIVATE, NULL);
    if (res)
    {
        log[Fatal] << "mount mount dir FAILED! " << strerror(errno) << endl;
    }
    log[Info] << "mount dir " << mount_dir << endl;



    string old_mount_basename = "/oldroot." + config.hostname + "XXXXXX";
    string old_mount_dir = mount_dir + old_mount_basename;
    old_mount_dir = mkdtemp(old_mount_dir.data());
    old_mount_basename = basename(old_mount_dir.data());

    log[Info] << "old mount dir" << old_mount_dir << endl;
    res = syscall(SYS_pivot_root, mount_dir.data(), old_mount_dir.data());
    if (res<0)
    {
        log[Fatal] << "pivot_root syscall FAILED! " << strerror(errno) << endl;
    }
    chdir("/"); // 取消挂载原来的根目录。因为旧的目录就挂载在新的根目录下面，因此先切换到新的根目录
    res = umount2(old_mount_basename.data(), MNT_DETACH);
    if (res)
    {
        log[Error] << "unmount old root failed! " << strerror(errno) << endl;
    }
    res = rmdir(old_mount_basename.data());
      if (res)
    {
        log[Error] << "delete old root failed! " << strerror(errno) << endl;
    }
}