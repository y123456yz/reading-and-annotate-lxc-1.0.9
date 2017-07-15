/*
 * lxc: linux Container library
 *
 * (C) Copyright IBM Corp. 2007, 2009
 *
 * Authors:
 * Daniel Lezcano <daniel.lezcano at free.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef __LXC_NAMESPACE_H
#define __LXC_NAMESPACE_H

#include <sys/syscall.h>
#include <sched.h>

#include "config.h"

/*
Namespace    系统调用参数    隔离内容
UTS         CLONE_NEWUTS    主机名与域名

IPC

CLONE_NEWIPC

信号量、消息队列和共享内存

PID

CLONE_NEWPID

进程编号

Network

CLONE_NEWNET

网络设备、网络栈、端口等等

Mount

CLONE_NEWNS

挂载点（文件系统）

User

CLONE_NEWUSER

用户和用户组

*/
#ifndef CLONE_FS
#  define CLONE_FS                0x00000200
#endif
#ifndef CLONE_NEWNS           
#  define CLONE_NEWNS             0x00020000  //挂载点（文件系统）
#endif
#ifndef CLONE_NEWUTS                      
#  define CLONE_NEWUTS            0x04000000  //主机名与域名
#endif
#ifndef CLONE_NEWIPC
#  define CLONE_NEWIPC            0x08000000  //信号量、消息队列和共享内存
#endif
#ifndef CLONE_NEWUSER
#  define CLONE_NEWUSER           0x10000000  //用户和用户组
#endif
#ifndef CLONE_NEWPID
#  define CLONE_NEWPID            0x20000000  //进程编号
#endif
#ifndef CLONE_NEWNET
#  define CLONE_NEWNET            0x40000000  //网络设备、网络栈、端口等等
#endif

#if defined(__ia64__)
int __clone2(int (*__fn) (void *__arg), void *__child_stack_base,
             size_t __child_stack_size, int __flags, void *__arg, ...);
#else
int clone(int (*fn)(void *), void *child_stack,
	int flags, void *arg, ...
	/* pid_t *ptid, struct user_desc *tls, pid_t *ctid */ );
#endif


extern pid_t lxc_clone(int (*fn)(void *), void *arg, int flags);

extern int lxc_namespace_2_cloneflag(char *namespace);
extern int lxc_fill_namespace_flags(char *flaglist, int *flags);

#endif
