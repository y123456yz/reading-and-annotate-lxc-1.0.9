/*
 * lxc: linux Container library
 *
 * (C) Copyright IBM Corp. 2007, 2008
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
#ifndef __LXC_START_H
#define __LXC_START_H

#include <signal.h>
#include <sys/param.h>

#include "config.h"
#include "state.h"
#include "namespace.h"

struct lxc_conf;

struct lxc_handler;

/*
例如start.c中的
static struct lxc_operations start_ops = {
	.start = start,
	.post_start = post_start
};

*/
struct lxc_operations {
	int (*start)(struct lxc_handler *, void *);
	int (*post_start)(struct lxc_handler *, void *);
};

struct cgroup_desc;

enum {
	LXC_NS_MNT,
	LXC_NS_PID,
	LXC_NS_UTS,
	LXC_NS_IPC,
	LXC_NS_USER,
	LXC_NS_NET,
	LXC_NS_MAX
};

struct ns_info {
	const char *proc_name;
	int clone_flag;
};

extern const struct ns_info ns_info[LXC_NS_MAX];

//初始化赋值见lxc_init
struct lxc_handler {
	pid_t pid; //lxc_clone返回值，见lxc_spawn，也就是子进程pid
	char *name; //容器名
	lxc_state_t state; //STARTING 等
	int clone_flags; //CLONE_NEWUTS等，或
	int sigfd;
	sigset_t oldmask;
	struct lxc_conf *conf;
	struct lxc_operations *ops;
	void *data;
	//lxc父进程和lxc_clone创建的子进程通过该sv 管道通信
	int sv[2]; //初始化见lxc_sync_init //父进程给lxc_clone创建的clone的子进程发送LXC_SYNC_STARTUP信息，子进程受到后会回送LXC_SYNC_STARTUP
	int pinfd;
	const char *lxcpath; //config_path 赋值见lxc_init  默认/usr/local/var/lib/lxc  -P参数修改
	void *cgroup_data; //赋值见cgroup_init  类型为cgfs_data
	int nsfd[LXC_NS_MAX]; //获取lxc_clone创建的进程中的各种ns中的文件fd   见lxc_spawn
};

extern struct lxc_handler *lxc_init(const char *name, struct lxc_conf *, const char *);

extern int lxc_check_inherited(struct lxc_conf *conf, int fd_to_ignore);
int __lxc_start(const char *, struct lxc_conf *, struct lxc_operations *,
		void *, const char *);

#endif

