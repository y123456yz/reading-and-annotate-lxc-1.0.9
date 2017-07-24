/* directory under which we mount the controllers - /run/lxcfs/controllers */


/*
[root@localhost lxcfs-lxcfs-2.0.0]# mount | grep /lxcfs/controllers
tmpfs on /run/lxcfs/controllers type tmpfs (rw,relatime,seclabel,size=100k,mode=700)
pids on /run/lxcfs/controllers/pids type cgroup (rw,relatime,pids)
freezer on /run/lxcfs/controllers/freezer type cgroup (rw,relatime,freezer)
hugetlb on /run/lxcfs/controllers/hugetlb type cgroup (rw,relatime,hugetlb)
blkio on /run/lxcfs/controllers/blkio type cgroup (rw,relatime,blkio)
cpuset on /run/lxcfs/controllers/cpuset type cgroup (rw,relatime,cpuset,clone_children)
memory on /run/lxcfs/controllers/memory type cgroup (rw,relatime,memory)
cpuacct,cpu on /run/lxcfs/controllers/cpuacct,cpu type cgroup (rw,relatime,cpuacct,cpu)
perf_event on /run/lxcfs/controllers/perf_event type cgroup (rw,relatime,perf_event)
devices on /run/lxcfs/controllers/devices type cgroup (rw,relatime,devices)
net_prio,net_cls on /run/lxcfs/controllers/net_prio,net_cls type cgroup (rw,relatime,net_prio,net_cls)
name=systemd on /run/lxcfs/controllers/name=systemd type cgroup (rw,relatime,xattr,release_agent=/usr/lib/systemd/systemd-cgroups-agent,name=systemd)
[root@localhost lxcfs-lxcfs-2.0.0]# ls /run/lxcfs
lxcfs/     lxcfs.pid  
[root@localhost lxcfs-lxcfs-2.0.0]# ls /run/lxcfs/
controllers/ root/        
[root@localhost lxcfs-lxcfs-2.0.0]# ls /run/lxcfs/controllers/
blkio  cpuacct,cpu  cpuset  devices  freezer  hugetlb  memory  name=systemd  net_prio,net_cls  perf_event  pids
[root@localhost lxcfs-lxcfs-2.0.0]# 
*/
#define basedir RUNTIME_PATH "/lxcfs/controllers"

extern int cg_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi);
extern int cg_mkdir(const char *path, mode_t mode);
extern int cg_chown(const char *path, uid_t uid, gid_t gid);
extern int cg_rmdir(const char *path);
extern int cg_chmod(const char *path, mode_t mode);
extern int cg_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
		struct fuse_file_info *fi);
extern int cg_releasedir(const char *path, struct fuse_file_info *fi);
extern int cg_release(const char *path, struct fuse_file_info *fi);
extern int cg_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi);
extern int cg_opendir(const char *path, struct fuse_file_info *fi);
extern int cg_getattr(const char *path, struct stat *sb);
extern int cg_open(const char *path, struct fuse_file_info *fi);
extern int cg_access(const char *path, int mode);

extern int proc_getattr(const char *path, struct stat *sb);
extern int proc_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
		struct fuse_file_info *fi);
extern int proc_release(const char *path, struct fuse_file_info *fi);
extern int proc_open(const char *path, struct fuse_file_info *fi);
extern int proc_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi);
extern int proc_access(const char *path, int mask);
