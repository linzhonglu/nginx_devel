###反向代理-Reverse Proxy###
	当客户端发来HTTP请求时，Nginx并不会立刻转发到上游服务器，而是先把用户的请求（包括HTTP包体）完整的接收到Nginx所在服务器的硬盘或内存中，然后再向上游服务器发起链接，把缓存的客户端请求转发到上游服务器。
	Nginx这种工作方式的优缺点：缺点是延长了一个请求的处理时间，并增加了用于缓存请求内容的内存和磁盘空间；优点是降低了上游服务器的负载，尽量把压力放到Nginx服务器上。
####负载均衡的基本配置####
	1.upstream块
		语法：upstream name {...};
		配置块：http
		Note：定义上游服务器集群
	2.server
		语法：server name [parameters];
		配置块：upstream
		Note：定义一个上游服务器的名字，域名，IP，Unix句柄等
		参数：1.weight=number 设置向上游服务器转发的权重，默认值为1。
			2.max_fails=number 与fail_timeout配合使用，在fail_timeout时间段内，如果向当前的上游服务器的转发失败次数超过number，则认为在当前的fail_timeout时间段内这个上游服务器不可用。默认值为1，如果设置为0，则表示不检查失败次数。
			3.fail_timeout=time 表示该段时间内转发失败多少次后就认为上游服务器暂不可用。默认值为10秒。
			4.down 表示当前的上游服务器永久下线，仅在使用ip_hash配置项时可用。
			5.backup 表示当前的上游服务器只是备份服务器，只有所有非备份上游服务器转发失败后才向上游备份服务器转发，仅在禁止ip_hash配置时可用。
	3.ip_hash
		语法：ip_hash
		配置块：upstream
		Note：根据客户端的IP地址计算出一个key，将key按照upstream集群里的上游服务器数量进行取模，然后以取模后的结果把请求转发到相应的上游服务器中，这样确保同一个客户端的请求只转发到指定的上游服务器中。
		ip_hash与weight配置不可同时使用。
		如果upstream集群中有一台上游服务器集群暂时不可用，不可直接删除，而是使用down进行标识，确保转发策略的一惯性。
	4.记录日志时支持的变量
		| 变量名 | 意义 |
		| -- | -- |
		| $upstream_addr | 处理请求的上游服务器地址 |
		| $upstream_cache_status | 是否命中缓存，MISS,EXPIRED,UPDATING,STALE,HIT |
		| $upstream_status | 上游服务器返回的响应中HTTP响应码 |
		| $upstream_response_time | 上游服务器的响应时间，毫秒 |
		| $upstream_http_$HEADER | HTTP头部 |
####反向代理的基本配置####
	1.设置转发URL
		语法：proxy_pass URL;
		配置块：location,if
	2.转发时的协议方法名
		语法：proxy_method method;
		配置块：http,server,location
	3.指定哪些HTTP头部字段不转发给客户端
		语法：proxy_hide_header the_header;
		配置块：http,server,location
	4.指定哪些HTTP头部字段转发给客户端
		语法：proxy_pass_header the_header;
		配置块：http,server,location
	5.设置向上游服务器转发包体
		语法：proxy_pass_request_body on|off;
		默认：proxy_pass_request_body on;
		配置块：http,server,location
	6.设置向上游服务器转发包头
		语法：proxy_pass_request_headers on|off;
		默认：proxy_pass_request_header on;
		配置块：http,server,location
	7.proxy_redirect
		语法：proxy_redirect [default|off|redirect replacement];
		默认：proxy_redirect default;
		配置块：http,server,location
		Note：当上游服务器返回的响应是重定向或者刷新请求（301或302）时，proxy_redirect可以重设HTTP请求头部的location或refresh字段。
	8.proxy_next_upstream
		语法：proxy_next_upstream [error|timeout|invalid_header|http_500|http_502|http_503|http_504|http_404|off]
		默认：proxy_next_upstream error timeout;
		配置块：http,server,location
		Note：当向一台上游服务器转发请求失败时，继续换一台上游服务器处理这个请求。
			1.error：当向上游服务器发起链接，发送请求，读取响应是出错
			2.timeout：发送请求或读取响应时超时
			3.invalid_header：上游服务器发送的响应是不合法的
			4.http_500：上游服务器返回的响应码是500
			5.http_502：上游服务器返回的响应码是502
			6.http_503：上游服务器返回的响应码是503
			7.http_504：上游服务器返回的响应码是504
			8.http_404：上游服务器返回的响应码是404
			9.off：关闭proxy_next_upstream功能，不再向上游服务器转发