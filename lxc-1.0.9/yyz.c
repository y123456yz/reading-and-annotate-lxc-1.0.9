/*
/proc/cgroups 列出了系统中cgroup的subsystem
/proc/XXX/cgroup  列出进程所属的cgroup
http://www.cnblogs.com/zhengah/p/4936962.html

mount -t cgroup -o cpu cpu /sys/fs/cgroup/cpu
 
cgroup数hierarchy理解:  https://yq.aliyun.com/articles/64965
rmdir lxc

http://blog.csdn.net/shichaog/article/details/41378145
内核同样为每个 /proc/PID/ns构建了符号链接，以使其指向包含标识命名空间类型字符串（字符串以inode号结尾）的名字。我们可以通过ls -l或者readlink命令查看名字。

*/
