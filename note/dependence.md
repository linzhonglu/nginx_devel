###依赖###
	yum install -y gcc
	yum install -y gcc-c++
	yum install -y pcre pcre-devel
	yum install -y zlib zlib-devel
	yum install -y openssl openssl-devel
###/etc/sysctl.conf###
	fs.file-max = 999999 (进程可以同时打开的最大句柄数)
	net.ipv4.tcp_tw_reuse = 1 (将TIME_WAIT状态的socket重新用于新的TCP连接)
	net.ipv4.tcp_keepalive_time = 600 (当启用keepalived时，设置发送keepalived消息的频道)
	net.ipv4.tcp_fin_timeout = 30 (当服务器主动关闭连接时，socket保持再FIN-WAIT-2状态的最大时间)
	net.ipv4.tcp_max_tw_buckets = 5000 (服务器允许TIME_WAIT套接字数量的最大值)
	net.ipv4.ip_local_port_range = 1024 61000 (UDP和TCP连接中本地端口的取值范围)
	net.ipv4.tcp_rmem = 4096 32768 262142 (TCP接收缓存的最小值、默认值、最大值)
	net.ipv4.tcp_wmem = 4096 32768 262142 (TCP发送缓存的最小值、默认值、最大值)
	net.ipv4.tcp_max_syn.backlog = 1024 (TCP三次握手建立连接阶段接收SYN请求队列的最大长度)
	net.core.netdev_max_backlog = 8096 (用于设置当网卡接收数据包的速度大于内核处理速度时，保存数据包的最大值)
	net.core.rmem_default = 262142 (内核套接字接收缓存的默认值)
	net.core.wmem_default = 262142 (内核套接字发送缓存的默认值)
	net.core.rmem_max = 2097152 (内核套接字接收缓存的最大值)
	net.core.wmem_max = 2097152 (啮合套接字发送缓存的最大值)
	vm.swappiness = 0 (最大限度的使用物理内存)
