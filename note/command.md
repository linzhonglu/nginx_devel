###nginx命令行参数说明###
	$ nginx -h
	nginx version: openresty/1.7.4.1
	Usage: nginx [-?hvVtq] [-s signal] [-c filename] [-p prefix] [-g directives]
	
	Options:
	  -?,-h         : this help
	  -v            : show version and exit
	  -V            : show version and configure options then exit
	  -t            : test configuration and exit
	  -q            : suppress non-error messages during configuration testing
	  -s signal     : send signal to a master process: stop, quit, reopen, reload
	  -p prefix     : set prefix path (default: /usr/local/openresty/nginx/)
	  -c filename   : set configuration file (default: conf/nginx.conf)
	  -g directives : set global directives out of configuration file
###默认方式启动###
	nginx
###指定配置文件启动###
	nginx -c nginx.config
###指定工作目录启动###
	nginx -p workspace
###指定全局变量启动###
	nginx -g 'pid logs/nginx.pid'
###测试配置文件###
	nginx -t
###显示版本信息###
	nginx -v
###显示编译阶段参数###
	nginx -V
###强制停止服务###
	nginx -s stop
	kill -s SIGTERM <master or worker pid>
	kill -s SIGINT <master or worker pid>
	kill -s SIGQUIT <master pid>
###停止服务###
	nginx -s quit
	kill -s SIGWINCH <worker pid>
###重启服务###
	nginx -s reload
	kill -s SIGHUP <master pid>
###日志回滚###
	nginx -s reopen
	kill -s SIGUSR1 <master pid>
###平滑升级###
	1. cp nginx_new nginx_old (使用新nginx二进制文件覆盖旧nginx二进制文件)
	2. kill -s SIGUSR2 <master pid> (发送SIGUSR2信号给旧nginx-master-pid)
	2. nginx_new (启动新nginx程序)
	3. kill -s SIGQUIT <nginx_old master pid> (停止就nginx程序)