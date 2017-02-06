###Nginx配置项###
####块配置项####
####配置项语法格式####
	配置项名 配置项值1 配置项值2 ...；
	Note：配置项值可以使用单引号或双引号
####配置项注释####
	#pid logs/nginx.pid;
####配置项单位####
	空间单位：K/k/M/m
	时间单位：ms/s/m/h/d/w/M/y
####配置项中使用变量####
	log_format main '$remote_addr - [$time_local] "$request"';
	使用$符号限制变量名
###Nginx配置###
####用于调试进程和定位问题的配置项
	1.是否以守护进程的方式
		语法：daemon on|off;
		默认：daemon on;
	2.是否以master/worker方式工作
		语法：master_process on|off;
		默认：master_process on;
	3.error日志的设置
		语法：error_log /path/file level;
		默认：error_log logs/error.log error;
		level - debug,info,notice,warn,error,crit,alert,emerg
		Note:debug模式仅在configure --with-debug配置下有效
	4.是否处理几个特殊的调试点
		语法：debug_points [stop|abort]
		stop:nginx执行到调试点会发出SIGSTOP信号用于调试
		abort:nginx会产生一个coredump文件用于调试
	5.仅对指定的客户端输出debug级别的日志
		语法：debug_connection [IP|CIDR]
		Note：debug_connection必须放在events{...}中才有效
			events {
				debug_connection 192.168.0.1;  #IP
				debug_connection 192.168.0.0/8;  #CIDR
			}
		Note:debug模式仅在configure --with-debug配置下有效
	6.限制coredump文件的大小
		语法：worker_rlimit_core size;
	7.指定coredump文件生产目录
		语法：working_directory path;
####正常运行的配置项####
	1.定义环境变量
		语法：env VAR|VAR=VALUE;
	2.嵌入其他配置文件
		语法：include /path/file.conf|path/*.conf;
	3.pid文件路径
		语法：pid path/file;
		默认：pid logs/nginx.pid;
	4.worker进程运行的用户及用户组
		语法：user username [groupname]
		默认：user nobody [nobody];
	5.worker进程可以打开的最大句柄描述符个数
		语法：worker_rlimit_nofile limit;
	6.限制信号队列
		语法：worker_rlimit_sigpending limit;
####优化性能的配置项####
	1.worker进程个数
		语法：worker_processes number;
		默认：worker_processes 1;
	2.绑定worker进程到指定的CPU内核
		语法：worker_cpu_affinity cpumask [cpumask ...]
	3.SSL硬件加速
		语法：ssl_engine device;
		Note:openssl engine -t检查服务器是否支持ssl硬件加速功能
	4.系统调用gettimeofday的执行频率
		语法：timer_resolution t;
	5.worker进程优先级设置
		语法：worker_priority nice;
		默认：worker_priority 0;
####事件类配置项####
	1.是否开启accept锁
		语法：accept_mutex [on|off];
		默认：accept_mutex on;
	2.lock文件的位置(供accept锁使用)
		语法：lock_file path/file;
		默认：lock_file logs/nginx.lock;
	3.使用accept锁后到真正建立连接之间的延迟时间
		语法：accept_mutex_deply Nms;
		默认：accept_mutex_deply 500ms;
	4.批量建立新连接
		语法：multi_accept [on|off];
		默认：multi_accept off;
	5.选择事件模型
		语法：use [kqueue|rtsig|epoll|/dev/poll|select|poll|eventport];
		默认：Nginx会自动选择最合适的事件模型
	6.每个worker的最大连接数
		语法：worker_connections number;
###HTTP配置项###
####虚拟主机与请求的分发####
	1.监听端口
		语法：listen address:port [default_server|[backlog=num|rcvbuf=size|sndbuf=size|accept_filter=filter|deffered|bind|ipv6only=[on|off]|ssl]];
		默认：listen 80;
		配置块：server
		default_server：将所在的server块作为整个web服务的默认server块。
		backlog=num：表示TCP中backlog队列的大小。默认为-1，表示不予设置。在TCP建立三次握手过程中，进程还没有开始处理监听句柄，这是backlog队列将会放置这些新连接。可如果backlog队列已满，还有新的客户端试图通过三次握手建立TCP连接，这时客户端将会建立连接失败。
		rcvbuf=size：设置监听句柄的SO_RCVBUF参数。
		sndbuf=size：设置监听句柄的SO_SNDBUF参数。
		accept_filter：设置accept过滤器，仅对FreeBSD操作系统有效。
		deferred：设置该参数后，若用户发起建立连接的请求，并且完成了TCP的三次握手，内核也不会为了这次的链接调度worker进程来处理，只有用户真的发送请求数据时，内核才会唤醒worker进程处理这个连接。
		bind：绑定当前端口/地址对。只有同时对一个端口监听多个地址才会生效。
		ssl：在当前监听的端口上建立的链接必须基于SSL协议。
	2.主机名称
		语法：server_name name [...];
		默认：server_name "";
		配置项：server
		server_name与Host的匹配优先级：
			1.首先选择所有字符串完全匹配的server_name，如www.youku.com
			2.其次选择通配符在前面的server_name，如*.youku.com
			3.再次选择通配符在后面的server_name，如www.youku.*
			4.最后选择使用正则表达式的server_name，如~^www\.youku\.com$
			5.优先选择在listen配置项后加入[default_server]的server块
			6.找到匹配listen端口的第一个server块
	3.设置server_name散列桶占用内存的大小
		语法：server_names_hash_bucket_size size;
		默认：server_names_hash_bucket-size 32|64|128;
		配置块：http,server,location
		Note：为了提高快速寻找相应server_name的能力，Nginx使用散列表存储server_name
	4.设置server_name散列桶的最大值
		语法：server_names_hash_max_size size;
		默认：server_names_hash_max_size 512;
		配置块：http,server,location
	5.重定向主机名的处理
		语法：server_name_in_redirect on|off;
		默认：server_name_in_redirect on;
		配置块：http,server或location
		Note：在使用on打开时，表示在重定向请求时会使用server_name里配置的第一个主机名代替原先请求中的Host头部，而使用off关闭时，表示重定向请求时使用请求本身的Host头部。
	6.location
		语法：location [=|~|~*|^~|@] /uri/ {...}
		配置块：server
		location匹配规则：
			1.=：表示把URI作为字符串，以便于参数中的uri做完全匹配。
			2.~：表示匹配URI时，是字母大小写敏感的。
			3.~*：表示匹配URI时，忽略大小写问题。
			4.^~：表示匹配URI时，只需其前半部分与uri参数匹配即可。
			5.@：表示仅用于Nginx服务内部请求之间的重定向，不处理用户请求。
		Note：location / {...} 配置所有的http请求。
####文件路径的定义####
	1.以root方式设置资源路径
		语法：root path;
		默认：root html;
		配置块：http,server,location,if
		Note：
			location /download/ {
				root /opt/web/html;
			}
			如果有一个请求的URI是/download/index.html，那么返回web服务器上/opt/web/html/download/index.html文件内容。
	2.以alias方式设置资源路径
		语法：alias path;
		配置块：location
		Note:
			如果有一个请求URI是/conf/nginx.conf，而用户实际想访问的文件在/usr/local/nginx/conf/nginx.conf
			使用alias设置如下：
			location /conf {
				alias /usr/local/nginx/conf/;
			}
			使用root设置如下：
			location /conf {
				root /usr/local/nginx/;
			}
			使用alias时，在URI向实际文件路径的映射过程中，已经把location后面的/conf这部分字符串丢弃掉了，因此/conf/nginx.conf请求将根据alias path映射为path/nginx.conf。root则不同，它会根据完整的URI请求来映射，因此/conf/nginx.conf请求会根据root path映射为path/conf/nginx.conf。这也是root可以放置到http,server,location,if块中，而alias只能放置到location块中的原因。
			alias后面还可以使用正则表达式：
			location ~ ^/test/(\w+)\.(\w+)$ {
				alias /usr/local/nginx/$2/$1.$s2;
			}
	3.访问首页
		语法：index file ...;
		默认值：index index.html;
		匹配块：http,server,location
		Note：index配置由ngx_http_index_module模块提供
		location / {
			root path;
			index /index.html /html/index.php /index.php;
		}
		接收请求后，Nginx首先会尝试访问path/index.php文件，如果可以访问，就直接返回文件内容结束请求，否则再试图返回path/html/index.php文件内容，以此类推。
	4.根据HTTP返回码重定向页面
		语法：error_page code [code...] [=|=answer-code]uri|@named_location
		匹配块：http,server,location,if
		Note：
		1.当对于某个请求返回错误码时，如果匹配上了error_page中设置的code，则重定向到新的URI中：
			error_page 404 /404.html;
			error_page 502 503 504 /50x.html;
		2.可以通过=来改变返回的错误码
			error_page 404 =200 /empty.gif;
		3.也可以不指定错误码，而是由重定向后实际处理的真实结果来决定
			error_page 404 = /empty.gif;
		4.如果不想修改URI，只是想让这样的请求重定向到另一个location中进行处理
			error_page 404 @fallback;
	5.是否允许递归使用error_page
		语法：recursive_error_pages [on|off];
		默认：recursive_error_pages off;
		匹配块：http,server,location
	6.try_files
		语法：try_files path1 [path2] uri;
		匹配块：server,location
		Note：try_files后面要跟若干路径，如path1 path2 ...，而且最后必须要有uri参数。尝试按照顺序访问每一个path，如果可以有效地读取，就直接向用户返回这个path对应的文件结束请求，否则继续向下访问。如果所有的path都找不到有效的文件，就重定向到最后的参数uri上。因此最后这个参数uri必须存在，而且它应该是可以有效重定向的。
####内存及磁盘资源的分配####
	1.HTTP包体只存储在磁盘文件中
		语法：client_body_in_file_only on|clean|off;
		默认：client_body_in_file_only off;
		配置块：http,server,location
		Note：非off - 用户请求中的http包体一律存在到磁盘文件中，即使只有0字节也会存储为文件；on - 文件不会被删除；clean - 文件会被删除
	2.HTTP包体尽量写入到一个内存buffer中
		语法：client_body_in_single_buffer on|off;
		默认：client_body_in_single_buffer off;
		配置块：http,server
		Note：用户请求中的HTTP包体一律存储到内存buffer中。如果HTTP包体的大小超过client_body_buffer_size设置的值，包体还是会写入到磁盘文件中。
	3.存储HTTP头部的内存buffer大小
		语法：client_header_buffer_size size;
		默认：client_header_buffer_size 1k;
		配置块：http,server
		Note：Nginx接收用户请求中HTTP header部分时分配的内存buffer大小。有时请求中的HTTP Header部分可能会超过这个大小，这时large_client_header_buffer定义的buffer将会生效。
	4.存储超大HTTP头部的内存buffer大小
		语法：large_client_header_buffers number size;
		默认：large_client_header_buffers 4 8k;
		配置块：http,server
		Note：Nginx接收一个超大HTTP头部请求的buffer个数和每个buffer的大小。如果HTTP请求行(GET /index HTTP/1.1)的大小超过上面的单个buffer，则返回“Request URI too large”(414)。请求中一般会有许多header，每个header的大小也不能超过单个buffer的大小，否则会返回“Bad Request”(400)。当然请求行和请求头部的总和不可以超过buffer个数*buffer大小。
	5.存储HTTP包体的内存buffer大小
		语法：client_body_buffer_size size;
		默认：client_body_buffer_size 8k/16k;
		配置块：http,server,location
		Note：Nginx接收HTTP包体的内存缓冲区大小。即HTTP包体会先接收到指定的这块缓存中，之后才决定是否写入磁盘。如果用户请求中含有HTTP头部Content-Length，并且其表示的长度小于定义的buffer大小，那么Nginx会自动降低本次请求所使用的内存buffer，以降低内存消耗。
	6.HTTP包体的临时存放目录
		语法：client_body_temp_path dir-path [level1[level2[level3]]];
		默认：client_body_temp_path client_body_temp;
		配置块：http,server,location
		Note：定义HTTP包体存放的临时目录。在接收HTTP包体时，如果包体的大小大于client_body_buffer_size,则会以一个递增的整数命名并且存放到client_body_temp_path指定的目录中。后面跟着的level1,level2,level3是为了防止一个目录下的文件数量大多从而导致性能下降，因此使用了level参数，这样可以按照临时文件名最多再加三层目录。
		如：client_body_temp_path /opt/nginx/client_temp 1 2;
			/opt/nginx/client_temp/6/45/0000000123456
	7.链接内存池大小
		语法：connection_pool_size
		默认：connection_pool_size 256;
		配置块：http,server
		Note：Nginx对于每个建立成功的TCP链接会预先分配一个内存池，上面的size配置项将指定这个内存池的初始大小，用于减少内核对于小块内存的分配次数。
	8.请求内存池大小
		语法：request_pool_size size;
		默认：request_pool_size 4k;
		配置块：http,server
		Note：Nginx开始处理HTTP请求时，将会为每个请求都分配一个内存池，size配置项将指定这个内存池的初始大小，用于减少内核对于小块内存的分配次数。TCP连接关闭时会销毁connection_pool_size指定的链接内存池，HTTP请求结束时会销毁request_pool_size指定的HTTP请求内存池，但他们的创建，销毁时间并不一致，因为一个TCP链接可能被复用于多个HTTP请求。
####网络连接的设置####
	1.读取HTTP头部的超时时间
		语法：client_header_timeout time;(单位：秒)
		默认：client_header_timeout 60;
		配置块：http,server,location
		Note：如果超时，将向客户端返回408（“Request timed out”）
	2.读取HTTP包体的超时时间
		语法：client_body_timeout time;(单位：秒)
		默认：client_body_timeout 60;
		配置块：http,server,location
		Note：如果超时，将向客户端返回408（“Request timed out”）
	3.发送响应的超时时间
		语法：send_timeout time;
		默认：send_timeout 60;
		配置块：http,server,location
		Note：如果超时，Nginx将会关闭该链接
	4.reset_timeout_connection
		语法：reset_timeout_connection on|off;
		默认：reset_timeout_connection off;
		配置块：http,server,location
		Note：链接超时后将通过向客户端发送RST包来重置链接。开启后，Nginx会在某个链接超时后，不是使用正常情形下的四次握手关闭TCP链接，而是直接向用户发送RST重置包，不再等待用户的应答，直接释放Nginx服务器上关于这个套接字使用的所有缓存。相比正常关闭方式，它是的服务器避免产生许多处于FIN_WAIT_1,FIN_WAIT_2,TIME_WAIT状态的TCP链接。使用RST重置包关闭链接会带来一些问题，默认不开启。
	5.Nginx关闭用户链接的方式
		语法：lingering_close off|on|always;
		默认：lingering_close on;
		配置块：http,server,location
		Note：always表示关闭用户链接前必须无条件地处理链接上所有用户发送的数据；off表示关闭链接时完全不管链接上是否已经有准备就绪的来自用户的数据；on表示中间值，一般情况下在关闭链接前都会处理连接上的用户发送数据，除了有些情况下在业务上认为这个之后的数据是不必要的。
	6.Nginx关闭用户链接的时间
		语法：lingering_time time;
		默认：lingering_time 30s;
		配置项：http,server,location
		Note：lingering_close启用后，对于上传大文件很有用。当用户请求的Content-Length大于max_client_body_size配置时，Nginx服务会立刻向用户发送413(Request entity too large)响应。但很多客户端可能不关心413返回值，仍然持续不断上传HTTP body，此时经过lingering_time设置的时间后，Nginx将不管用户是否仍在上传，都会把链接关闭掉。
	7.Nginx关闭用户链接的超时时间
		语法：lingering_timeout time;
		默认：lingering_timeout 5s;
		配置块：http,server,location
		Note：lingering_close生效后，在关闭链接前，会检查是否有用户发送的数据达到服务器，如果超过lingering_timeout时间后还是没有数据可读，就直接关闭链接；否则必须在读取完链接缓冲区上的数据并丢弃掉后才会关闭链接。
	8.对于某些浏览器禁用keepalived功能
		语法：keepalive_disable [msie6|safari|none]...
		默认：keepalive_disable msie6 safari;
		配置块：http,server,location
	9.keepalive超时时间
		语法：keepalive_timeout time;(单位：秒)
		默认：keepalive_timeout 75;
		配置块：http,server,location
	10.一个keepalived长连接上允许承载的请求最大数
		语法：keepalive_request n;
		默认：keepalive_request 100;
		配置块：http,server,location
	11.tcp_nodelay
		语法：tcp_nodelay on|off;
		默认：tcp_nodelay on;
		配置块：http,server,location
		Note：确定keepalive链接是否使用TCP_NODELAY选项。
	12.tcp_nopush
		语法：tcp_nopush on|off;
		默认：tcp_nopush off;
		配置块：http,server,location
		Note：在开启sendfile选项时，确定是否开启FreeBSD系统上的TCP_NOPUSH或Linux系统上的TCP_CORK功能。开启tcp_nopush，将会在发送响应时把整个响应包头放到一个TCP包中发送。
####MIME类型的设置####
	1.MIME type与文件扩展的映射
		语法：type {...};
		配置块：http,server,location
		types {
			text/html html;
			text/html conf;
			image/gif gif;
			image/jpeg jpg;
		}
	2.默认MIME type
		语法：default_type MIME-type;
		默认：default_type text/plain;
		配置块：http,server,location
		Note：找不到MIME-type与文件扩展名之间映射时，使用默认MIME-type作为HTTP header中的Content-Type。
	3.设置散列桶占用的内存大小
		语法：types_hash_bucket_size size;
		默认：types_hash_bucket_size 32|64|128;
		配置块：http,server,locaiton
		Note：Nginx使用散列表存储MIME type与文件扩展名的映射关系
	4.设置散列桶的个数
		语法：types_hash_max_size size;
		默认：types_hash_max_size 1024;
		配置块：http,server,location
####对客户端请求的限制####
	1.按HTTP方法名限制用户请求
		语法：limit_except method ... {...};
		配置块：location
		Note：GET,HEAD,POST,PUT,DELETE,MKCOL,COPY,MOVE,OPTIONS,PROPFIND,PROPPATCH,LOCK,UNLOCAK,PATCH
	2.HTTP请求包体的最大值
		语法：client_max_body_size size;
		默认：client_max_body_size 1m;
		配置块：http,server,location
		Note：该参数是用来限制Content-Length所示值大小的。
	3.对请求的限速
		语法：limit_rate speed;
		默认：limit_rate 0;
		配置块：http,server,location,if
		Note：0表示不限速。
	4.对请求采用条件限速
		语法：limit_rate_after time;
		默认：limit_rate_after 1m;
		配置块：http,server,location,if
		Note：Nginx向客户端发送的响应时长超过limit_rate_after后才开始限速。
####文件操作的优化####
	1.sendfile系统调用
		语法：sendfile on|off;
		默认：sendfile off;
		配置块：http,server,location
		Note：启动Linux上的sendfile系统调用来发送文件，它减少了内核态与用户态之间的两次内存复制，这样就会从磁盘中读取文件后直接在内核态发送到网卡设备，提高了发送文件的效率。
	2.AIO系统调用
		语法：aio on|off;
		默认：aio off;
		配置块：http,server,location
		Note：在FreeBSD或Linux系统上启用内核级的异步文件IO功能。与sendfile功能互斥。
	3.directio
		语法：directio size|off;
		默认：directio off;
		配置块：http,server,location
		Note：在FreeBSD和Linux系统上使用O_DIRECT选项去读取文件，缓冲区大小为size，通常对大文件的读取速度有优化作用。与sendfile功能互斥。
	4.directio_alignment
		语法：directio_alignment size;
		默认：directio_alignment 512;
		配置块：http,server,location
		Note：与directio配合使用，指定以directio方式读取文件是的对其方式。
	5.打开文件缓存
		语法：open_file_cache max=N [inactive=time]|off;
		默认：open_file_cache off;
		配置块：http,server,location
		Note：文件缓存会在内存中存储以下3中信息：文件句柄，文件大小和上次修改时间；已经打开过的目录结构；没有找到或者没有权限操作的文件信息。
		参数：max表示在内存中存储元素的最大个数，达到最大限制数量后，采用LRU算法从缓存中淘汰最近最少使用的元素；inactive表示在指定时间段内没有被访问过的元素将会被淘汰，默认60秒；off表示关闭缓存功能。
	6.是否缓存打开文件错误的信息
		语法：open_file_cache_errors on|off;
		默认：open_file_cache_errors off;
		配置块：http,server,location
		Note：是否在文件缓存中缓存打开文件时出现的找不到路径，没有权限等错误信息。
	7.不被淘汰的最小访问次数
		语法：open_file_cache_min_uses number;
		默认：open_file_cache_min_uses 1;
		配置块：http,server,location
		Note：与open_file_cache中的inactive参数配合使用。如果在inactive指定的时间段内，访问次数超过了open_file_min_uses指定的最小次数，那么将不会被淘汰出缓存。
	8.检验缓存中元素有效性的频率
		语法：open_file_cache_valid time;
		默认：open_file_cache_valid 60s;
		配置项：http,server,location
		Note：每60秒检查一次缓存中的元素是否仍有效。
####对客户端请求的特殊处理####
	1.忽略不合法的HTTP头部
		语法：ignore_invalid_headers on|off;
		默认：ignore_invalid_headers on;
		配置块：http,server
		Note：off表示当出现不合法的HTTP头部时，Nginx会拒绝服务，并直接向客户端发送400(Bad Request)错误；on表示忽略此HTTP头部。
	2.HTTP头部是否允许下划线
		语法：underscores_in_headers on|off;
		默认：underscores_in_headers off;
		配置块：http,server
	3.对If-Modified-Since头部的处理策略
		语法：if_modified_since [off|exact|before];
		默认：if_modified_since exact;
		配置块：http,server,location
		Note：出于性能考虑，web浏览器一般会在客户端本地缓存一些文件，并且存储当时获取的时间。这样下次向web服务器获取缓存过的资源时，就可以用if-modified-since头部把上次获取的时间捎带上，而if_modified_since将根据后面的参数决定如何处理If-Modified-Since头部。
		参数：off表示忽略用户请求中的If-Modified-Since头部；exact表示将If-Modified-Since头部包含的时间与将要返回的文件上次修改时间做精确比较，如果没有匹配上，则返回200和文件的实际内容，如果匹配上，则表示浏览器缓存的文件内容已经是最新的，没有必要在返回文件从而浪费时间和带宽，这时会返回304 Not Modified，浏览器收到后会直接读取自己的本地缓存；before表示进行更宽松的比较，只要文件的上次修改时间等于或者早于用户请求中的If-Modified-Since头部时间，就会向客户端返回304 Not Modified。
	4.文件未找到时是否记录到error日志
		语法：log_not_found on|off;
		默认：log_not_found on;
		配置块：http,server,location
	5.是否合并请求中相邻的"/"
		语法：merge_slashes on|off;
		默认：merge_slashes on;
		配置块：http,server,location
		Note：on表示合并斜杠后匹配location；off表示不合并斜杠，严格匹配location。
	6.DNS解析地址
		语法：resolver address ...;
		配置块：http,server,location
	7.DNS解析的超时时间
		语法：resolver_timeout time;
		默认：resolver_timeout 30s;
		配置块：http,server,location
	8.返回错误页面时是否在Server中注明Nginx版本
		语法：server_tokens on|off;
		默认：server_tokens on;
		配置块：http,server,location
###ngx\_http\_core\_module模块提供的变量###
	| 参数名 | 意义 |
	| -- | -- |
	| $arg_PARAMETER | HTTP请求中某个参数值 |
	| $args | HTTP请求中完整参数 |
	| $binary_remote_addr | 二进制格式的客户端地址 |
	| $body_bytes_sent | 向客户端发送的http响应中，包体部分的字节数 |
	| $content_length | 客户端请求头部中Content-Length字段值 |
	| $content_type | 客户端请求头部中Content-Type字段值 |
	| $cookie_COOKIE | 客户端请求头部中cookie字段值 |
	| $document_root | 当前请求所使用的root配置项值 |
	| $uri | 当前请求的URI，不带任何参数 |
	| $document_uri | 与uri相同 |
	| $request_uri | 客户端发来的原始URI，带完整参数 |
	| $host | 客户端请求头部中的Host字段 |
	| $hostname | Nginx所在机器的名称 |
	| $http_HEADER | 当前请求中相应头部的值 |
	| $sent_http_HEADER | 返回给客户端HTTP响应中响应头部的值 |
	| $is_args | 请求中的URI是否带参数。带参数返回？；不带参数返回空字符串。 |
	| $limit_rate | 当前链接的限速是多少。0表示不限速 |
	| $nginx_version | Nginx版本号 |
	| $query_string | 请求中的参数，只读模式 |
	| $remote_addr | 客户端地址 |
	| $remote_port | 客户端端口号 |
	| $remote_user | 使用Auth Basic Module时定义的用户名 |
	| $request_filename | 用户请求中的URI经过root或alias转换后的文件路径 |
	| $request_body | 请求中的包体，只在proxy_pass或fastcgi_pass中有效 |
	| $request_body_file | 请求中的包体存储的临时文件名 |
	| $request_completion | 当请求全部完成时，其值为ok；否则为空字符串 |
	| $request_method | 请求的方法名 |
	| $scheme | 请求的scheme |
	| $server_addr | 服务器地址 |
	| $server_port | 服务器端口 |
	| $server_protocol | 服务器向客户端发送响应的协议,http/1.0或http/1.1 |