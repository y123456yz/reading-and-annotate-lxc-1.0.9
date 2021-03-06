/*
 * lxc: linux Container library
 *
 * (C) Copyright IBM Corp. 2007, 2008
 *
 * Authors:
 * Daniel Lezcano <daniel.lezcano at free.fr>
 * Michel Normand <normand at fr.ibm.com>
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
#ifndef __LXC_ARGUMENTS_H
#define __LXC_ARGUMENTS_H

#include <getopt.h>
#include <stdint.h>

struct lxc_arguments;

typedef int (*lxc_arguments_parser_t) (struct lxc_arguments *, int, char*);
typedef int (*lxc_arguments_checker_t) (const struct lxc_arguments *);

struct lxc_arguments {
	const char *help;
	void(*helpfn)(const struct lxc_arguments *);
	const char *progname;
	const struct option* options;
	lxc_arguments_parser_t parser;
	lxc_arguments_checker_t checker;

	const char *name; //容器名
	char *log_file; //日志名
	char *log_priority; //日志等级  赋值参考lxc_log_priority_to_int
	int quiet;
	int daemonize; //-d启用daemon
	const char *rcfile; //配置文件
	const char *console;
	const char *console_log;
	const char *pidfile;
	//和-P参数有关
	const char **lxcpath;
	int lxcpath_cnt; //赋值见lxc_arguments_lxcpath_add
	/* set to 0 to accept only 1 lxcpath, -1 for unlimited */
	int lxcpath_additional; //-P 0,只接受一个lxcpath

	/* for lxc-start */ //里面值见LXC_NS_IPC等
	const char *share_ns[32]; // size must be greater than LXC_NS_MAX

	/* for lxc-console */
	int ttynum;
	char escape;

	/* for lxc-wait */
	char *states;
	long timeout;

	/* for lxc-autostart */
	int shutdown;

    /*
    Options :
      -n, --name=NAME   NAME of the container
      -r, --reboot      reboot the container
      -W, --nowait      don't wait for shutdown or reboot to complete
      -t, --timeout=T   wait T seconds before hard-stopping
      -k, --kill        kill container rather than request clean shutdown
          --nolock      Avoid using API locks
          --nokill      Only request clean shutdown, don't force kill after timeout
    */
	/* for lxc-stop */
	int hardstop;
	int nokill;
	int nolock;
	int nowait;
	int reboot;

	/* for lxc-destroy */
	int force;

	/* close fds from parent? */
	int close_all_fds;

	/* lxc-create */
	char *bdevtype, *configfile, *template;
	char *fstype;
	uint64_t fssize;
	char *lvname, *vgname, *thinpool;
	char *zfsroot, *lowerdir, *dir;

	/* auto-start */
	int all;
	int list;
	char *groups;

	/* remaining arguments */
	char *const *argv; //存放命令行参数信息
	int argc;

	/* private arguments */
	void *data;
};

#define LXC_COMMON_OPTIONS \
	{"name", required_argument, 0, 'n'}, \
	{"help", no_argument, 0, 'h'}, \
	{"usage", no_argument,	0, OPT_USAGE}, \
	{"version", no_argument,	0, OPT_VERSION}, \
	{"quiet", no_argument,	0, 'q'}, \
	{"logfile", required_argument, 0, 'o'}, \
	{"logpriority", required_argument, 0, 'l'}, \
	{"lxcpath", required_argument, 0, 'P'}, \
	{0, 0, 0, 0}

/* option keys for long only options */
#define	OPT_USAGE 0x1000
#define	OPT_VERSION OPT_USAGE-1

extern int lxc_arguments_parse(struct lxc_arguments *args,
			       int argc, char *const argv[]);

extern int lxc_arguments_str_to_int(struct lxc_arguments *args, const char *str);

#define lxc_error(arg, fmt, args...) if (!(arg)->quiet)			\
	fprintf(stderr, "%s: " fmt "\n", (arg)->progname,  ## args)

#endif
