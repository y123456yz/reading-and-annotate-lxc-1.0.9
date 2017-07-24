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
#ifndef __LXC_CONF_H
#define __LXC_CONF_H

#include "config.h"

#include <netinet/in.h>
#include <net/if.h>
#include <sys/param.h>
#include <sys/types.h>
#include <stdbool.h>

#include "list.h"
#include "start.h" /* for lxc_handler */

#if HAVE_SCMP_FILTER_CTX
typedef void * scmp_filter_ctx;
#endif

/* worth moving to configure.ac? */
#define subuidfile "/etc/subuid"
#define subgidfile "/etc/subgid"

enum {
	LXC_NET_EMPTY,
	LXC_NET_VETH,   //lxc.network.type = veth
	LXC_NET_MACVLAN,
	LXC_NET_PHYS,
	LXC_NET_VLAN,
	LXC_NET_NONE,
	LXC_NET_MAXCONFTYPE,
};

/*
 * Defines the structure to configure an ipv4 address
 * @address   : ipv4 address
 * @broadcast : ipv4 broadcast address
 * @mask      : network mask
 */
struct lxc_inetdev {
	struct in_addr addr;
	struct in_addr bcast;
	int prefix;
};

struct lxc_route {
	struct in_addr addr;
};

/*
 * Defines the structure to configure an ipv6 address
 * @flags     : set the address up
 * @address   : ipv6 address
 * @broadcast : ipv6 broadcast address
 * @mask      : network mask
 */
struct lxc_inet6dev {
	struct in6_addr addr;
	struct in6_addr mcast;
	struct in6_addr acast;
	int prefix;
};

struct lxc_route6 {
	struct in6_addr addr;
};

struct ifla_veth {
	char *pair; /* pair name */
	char veth1[IFNAMSIZ]; /* needed for deconf */
};

struct ifla_vlan {
	unsigned int   flags;
	unsigned int   fmask;
	unsigned short   vid;
	unsigned short   pad;
};

struct ifla_macvlan {
	int mode; /* private, vepa, bridge */
};

union netdev_p {
	struct ifla_veth veth_attr;
	struct ifla_vlan vlan_attr;
	struct ifla_macvlan macvlan_attr;
};

/*
 * Defines a structure to configure a network device
 * @link       : lxc.network.link, name of bridge or host iface to attach if any
 * @name       : lxc.network.name, name of iface on the container side
 * @flags      : flag of the network device (IFF_UP, ... )
 * @ipv4       : a list of ipv4 addresses to be set on the network device
 * @ipv6       : a list of ipv6 addresses to be set on the network device
 * @upscript   : a script filename to be executed during interface configuration
 * @downscript : a script filename to be executed during interface destruction
 */
struct lxc_netdev { //解析配置文件的lxc.network相关配置项获取
	int type; //LXC_NET_VETH等
	int flags;
	//只记录下来veth2的，veth1的没有记录  子进程用的是veth2的
	int ifindex; //每个网卡在内核都有对应的index,赋值见instantiate_veth  存储的是容器端的网卡的index，可以参考instantiate_veth
	char *link;//lxc.network.link = virbr0中的virbro字符串  ethx等
	char *name; //网卡接口名，如eth0等
	char *hwaddr;
	char *mtu;
	union netdev_p priv;
	struct lxc_list ipv4;
	struct lxc_list ipv6;
	struct in_addr *ipv4_gateway; //赋值见config_network_ipv4_gateway
	bool ipv4_gateway_auto; //赋值见config_network_ipv4_gateway  auto 或者gw地址
	struct in6_addr *ipv6_gateway;
	bool ipv6_gateway_auto;
	char *upscript; //lxc.network.script.up 配置网络脚步
	char *downscript;
};

/*
 * Defines a generic struct to configure the control group.
 * It is up to the programmer to specify the right subsystem.
 * @subsystem : the targeted subsystem
 * @value     : the value to set
 */
struct lxc_cgroup {  
	char *subsystem;//配置项中的key
	char *value;//配置项中的value
};

enum idtype {
	ID_TYPE_UID,
	ID_TYPE_GID
};

/*
 * id_map is an id map entry.  Form in confile is:
 * lxc.id_map = u 0    9800 100
 * lxc.id_map = u 1000 9900 100
 * lxc.id_map = g 0    9800 100
 * lxc.id_map = g 1000 9900 100
 * meaning the container can use uids and gids 0-99 and 1000-1099,
 * with [ug]id 0 mapping to [ug]id 9800 on the host, and [ug]id 1000 to
 * [ug]id 9900 on the host.
 */
struct id_map {
	enum idtype idtype;
	unsigned long hostid, nsid, range;
};

/*
 * Defines a structure containing a pty information for
 * virtualizing a tty
 * @name   : the path name of the slave pty side
 * @master : the file descriptor of the master
 * @slave  : the file descriptor of the slave
 */
struct lxc_pty_info {
	char name[MAXPATHLEN];
	int master;
	int slave;
	int busy;
};

/*
 * Defines the number of tty configured and contains the
 * instantiated ptys
 * @nbtty = number of configured ttys
 */ //lxc_conf.tty_info成员是该结构
struct lxc_tty_info {//赋值生效见lxc_create_tty
	int nbtty; //配置的lxc.tty数
	struct lxc_pty_info *pty_info;
};

struct lxc_tty_state;

/*
 * Defines the structure to store the console information
 * @peer   : the file descriptor put/get console traffic
 * @name   : the file name of the slave pty
 */
struct lxc_console {
	int slave;
	int master;
	int peer;
	struct lxc_pty_info peerpty;
	struct lxc_epoll_descr *descr;
	char *path;
	char *log_path;
	int log_fd;
	char name[MAXPATHLEN];
	struct termios *tios;
	struct lxc_tty_state *tty_state;
};

/*
 * Defines a structure to store the rootfs location, the
 * optionals pivot_root, rootfs mount paths
 * @rootfs     : a path to the rootfs
 * @pivot_root : a path to a pivot_root location to be used
 */ //把path rootfs->path 挂载到rootfs->mount	

/*
[root@localhost rootfs]# pwd
/usr/local/lib/lxc/rootfs
[root@localhost rootfs]# ls
bin  dev  etc  fstab  lib  lib64  proc  sbin  sys  usr  var
[root@localhost rootfs]# 
[root@localhost rootfs]# ls /root/yyzdir/
clonehostname   common.conf.d/  common.seccomp  rootfs/         rootfs.tar.gz   
[root@localhost rootfs]# ls /root/yyzdir/rootfs
bin  boot  cccccc  dev  etc  home  lib  lib64  media  mnt  opt  proc  root  run  sbin  selinux  srv  sys  test----  test--1  tmp  usr  var
[root@localhost rootfs]# 
*/
struct lxc_rootfs { //把path mount到mount(lxc.rootfs配置位置),见setup_rootfs
    char *path; //lxc.rootfs配置  DEBUG("mounted '%s' on '%s'", rootfs->path, rootfs->mount);  path中创建的东西或者在容器中创建的东西，容器和主机相互可见
    //在setup_rootfs_pivot_root中指定子进程跟目录为setup_rootfs_pivot_root
	char *mount; //默认值default_rootfs_mount  ./configure --with-rootfs-path=/lxc/rootfs/ 设置  挂载点
	char *pivot;
	char *options;
};

/*
 * Automatic mounts for LXC to perform inside the container
 */
enum {
	LXC_AUTO_PROC_RW              = 0x001,   /* /proc read-write */
	LXC_AUTO_PROC_MIXED           = 0x002,   /* /proc/sys and /proc/sysrq-trigger read-only */
	LXC_AUTO_PROC_MASK            = 0x003,

	LXC_AUTO_SYS_RW               = 0x004,   /* /sys */
	LXC_AUTO_SYS_RO               = 0x008,   /* /sys read-only */
	LXC_AUTO_SYS_MASK             = 0x00C,

	LXC_AUTO_CGROUP_RO            = 0x010,   /* /sys/fs/cgroup (partial mount, read-only) */
	LXC_AUTO_CGROUP_RW            = 0x020,   /* /sys/fs/cgroup (partial mount, read-write) */
	LXC_AUTO_CGROUP_MIXED         = 0x030,   /* /sys/fs/cgroup (partial mount, paths r/o, cgroup r/w) */
	LXC_AUTO_CGROUP_FULL_RO       = 0x040,   /* /sys/fs/cgroup (full mount, read-only) */
	LXC_AUTO_CGROUP_FULL_RW       = 0x050,   /* /sys/fs/cgroup (full mount, read-write) */
	LXC_AUTO_CGROUP_FULL_MIXED    = 0x060,   /* /sys/fs/cgroup (full mount, parent r/o, own r/w) */
	/* These are defined in such a way as to retain
	 * binary compatibility with earlier versions of
	 * this code. If the previous mask is applied,
	 * both of these will default back to the _MIXED
	 * variants, which is safe. */
	LXC_AUTO_CGROUP_NOSPEC        = 0x0B0,   /* /sys/fs/cgroup (partial mount, r/w or mixed, depending on caps) */
	LXC_AUTO_CGROUP_FULL_NOSPEC   = 0x0E0,   /* /sys/fs/cgroup (full mount, r/w or mixed, depending on caps) */
	LXC_AUTO_CGROUP_MASK          = 0x0F0,

	LXC_AUTO_ALL_MASK             = 0x0FF,   /* all known settings */
};

/*
 * Defines the global container configuration
 * @rootfs     : root directory to run the container
 * @pivotdir   : pivotdir path, if not set default will be used
 * @mount      : list of mount points
 * @tty        : numbers of tty
 * @pts        : new pts instance
 * @mount_list : list of mount point (alternative to fstab file)
 * @network    : network configuration
 * @utsname    : container utsname
 * @fstab      : path to a fstab file format
 * @caps       : list of the capabilities to drop
 * @keepcaps   : list of the capabilities to keep
 * @tty_info   : tty data
 * @console    : console data
 * @ttydir     : directory (under /dev) in which to create console and ttys
 * @lsm_aa_profile : apparmor profile to switch to or NULL
 * @lsm_se_context : selinux type to switch to or NULL
 */
enum lxchooks {
	LXCHOOK_PRESTART, LXCHOOK_PREMOUNT, LXCHOOK_MOUNT, LXCHOOK_AUTODEV,
	LXCHOOK_START, LXCHOOK_POSTSTOP, LXCHOOK_CLONE, NUM_LXC_HOOKS};
extern char *lxchook_names[NUM_LXC_HOOKS];

//lxc_conf.saved_nics为该类型
struct saved_nic {
	int ifindex; //物理网卡对应的index
	char *orig_name; //lxc.network.link配置，见save_phys_nics
};

//赋值见parse_line->lxc_getconfig  config
struct lxc_conf {
	int is_execute;
	char *fstab;
	int tty;
	int pts;
	int reboot;
	int need_utmp_watch;
	signed long personality;
	struct utsname *utsname;
	struct lxc_list cgroup; //解析lxc.cgroup.xx配置获取存入该链表，见config_cgroup
	struct lxc_list id_map;
	//赋值是通过解析配置文件中的lxc.network.xxx相关的配置项，然后通过config_network_type加入到链表
	struct lxc_list network; 
	struct saved_nic *saved_nics;
	int num_savednics; //赋值见save_phys_nics
	int auto_mounts;
	struct lxc_list mount_list;
	struct lxc_list caps;
	struct lxc_list keepcaps;
	//生效见lxc_create_tty
	struct lxc_tty_info tty_info; 
	struct lxc_console console;
	struct lxc_rootfs rootfs;
	char *ttydir;
	int close_all_fds;

	//赋值见config_hook，生效见run_lxc_hooks 解析到lxc.hook.pre-start等配置，然后执行对应配置的脚本
	struct lxc_list hooks[NUM_LXC_HOOKS];

	char *lsm_aa_profile;
	char *lsm_se_context;
	int tmp_umount_proc;
	char *seccomp;  // filename with the seccomp rules
#if HAVE_SCMP_FILTER_CTX
	scmp_filter_ctx seccomp_ctx;
#endif
	int maincmd_fd;  //域套接字对应的链接sock
	int autodev;  // if 1, mount and fill a /dev at start
	int haltsignal; // signal used to halt container  lxc.haltsignal 配置
	int stopsignal; // signal used to hard stop container
	int kmsg;  // if 1, create /dev/kmsg symlink
	//赋值见  lxc_config_read   -f指定的配置文件
	char *rcfile;	// Copy of the top level rcfile we read

	// Logfile and logleve can be set in a container config file.
	// Those function as defaults.  The defaults can be overriden
	// by command line.  However we don't want the command line
	// specified values to be saved on c->save_config().  So we
	// store the config file specified values here.
	char *logfile;  // the logfile as specifed in config
	int loglevel;   // loglevel as specifed in config (if any)

	int inherit_ns_fd[LXC_NS_MAX];

	int start_auto;
	int start_delay;
	int start_order;
	struct lxc_list groups;
	int nbd_idx;

	/* set to true when rootfs has been setup */
	bool rootfs_setup; //rootfs是否已mount，见do_rootfs_setup
};

int run_lxc_hooks(const char *name, char *hook, struct lxc_conf *conf,
		  const char *lxcpath, char *argv[]);

extern int detect_shared_rootfs(void);

/*
 * Initialize the lxc configuration structure
 */
extern struct lxc_conf *lxc_conf_init(void);
extern void lxc_conf_free(struct lxc_conf *conf);

extern int pin_rootfs(const char *rootfs);

extern int lxc_requests_empty_network(struct lxc_handler *handler);
extern int lxc_create_network(struct lxc_handler *handler);
extern void lxc_delete_network(struct lxc_handler *handler);
extern int lxc_assign_network(struct lxc_list *networks, pid_t pid);
extern int lxc_map_ids(struct lxc_list *idmap, pid_t pid);
extern int lxc_find_gateway_addresses(struct lxc_handler *handler);

extern int lxc_create_tty(const char *name, struct lxc_conf *conf);
extern void lxc_delete_tty(struct lxc_tty_info *tty_info);

extern int lxc_clear_config_network(struct lxc_conf *c);
extern int lxc_clear_nic(struct lxc_conf *c, const char *key);
extern int lxc_clear_config_caps(struct lxc_conf *c);
extern int lxc_clear_config_keepcaps(struct lxc_conf *c);
extern int lxc_clear_cgroups(struct lxc_conf *c, const char *key);
extern int lxc_clear_mount_entries(struct lxc_conf *c);
extern int lxc_clear_automounts(struct lxc_conf *c);
extern int lxc_clear_hooks(struct lxc_conf *c, const char *key);
extern int lxc_clear_idmaps(struct lxc_conf *c);
extern int lxc_clear_groups(struct lxc_conf *c);
extern int lxc_delete_autodev(struct lxc_handler *handler);

extern int do_rootfs_setup(struct lxc_conf *conf, const char *name,
			   const char *lxcpath);

/*
 * Configure the container from inside
 */

struct cgroup_process_info;
extern int lxc_setup(struct lxc_handler *handler);

extern void lxc_restore_phys_nics_to_netns(int netnsfd, struct lxc_conf *conf);

extern int find_unmapped_nsuid(struct lxc_conf *conf, enum idtype idtype);
extern int mapped_hostid(unsigned id, struct lxc_conf *conf, enum idtype idtype);
extern int chown_mapped_root(char *path, struct lxc_conf *conf);
extern int ttys_shift_ids(struct lxc_conf *c);
extern int userns_exec_1(struct lxc_conf *conf, int (*fn)(void *), void *data);
extern int parse_mntopts(const char *mntopts, unsigned long *mntflags,
			 char **mntdata);
extern void tmp_proc_unmount(struct lxc_conf *lxc_conf);
void remount_all_slave(void);
extern void suggest_default_idmap(void);
struct lxc_list *sort_cgroup_settings(struct lxc_list* cgroup_settings);
#endif
