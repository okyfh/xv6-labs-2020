#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"//lab2-2

//将trace的参数mask作为一个变量存到结构体struct proc中
uint64 
sys_trace(void) {
    int mask;
    // 获取整数类型的系统调用参数
    if (argint(0, &mask) < 0) {
        return -1;
    }
    // 存入proc 结构体的 mask 变量中
    myproc()->mask = mask;
    return 0;
}

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//将trace的参数mask作为一个变量存到结构体struct proc中
uint64
sys_trace(void) {
    int mask;
    // 获取整数类型的系统调用参数
    if (argint(0, &mask) < 0) {
        return -1;
    }
    // 存入proc 结构体的 mask 变量中
    myproc()->mask = mask;
    return 0;
}

uint64
sys_sysinfo(void) {
    uint64 info_addr;
    struct sysinfo info;

    if (argaddr(0, &info_addr) < 0) {
        return -1;
    }
    
    // 计算freemem和nproc
    info.freemem = getfreemem();
    info.nproc = getnproc();
    // 将结构体由内核态拷贝至用户态
    if (copyout(myproc()->pagetable, info_addr,
                (char *) &info, sizeof(info)) < 0) {
        return -1;
    }
    return 0;
}
