###开发简单的HTTP模块###
	1.将应用模块嵌入到Nginx中。
	2.应用模块要接入到HTTP请求处理中。
###Nginx数据结构###
####整型数据结构####
	1.ngx_int_t -- int (typedef intptr_t ngx_int_t)
	2.ngx_uint_t  -- unsinged int (typedef uintptr_t ngx_uint_t)
####ngx_str_t数据结构####
	typedef struct {
		size_t len;
		u_char *data;
	} ngx_str_t;
	Note: data指针指向的并不是普通字符串，未必以'\0'结尾
####ngx_strncmp(s1, s2, n)函数####
	#default ngx_strncmp(s1,s2,n) strncmp((const char*)s1, (const char*)s2, n)
####ngx_list_t数据结构####
	typedef struct ngx_list_part_s ngx_list_part_t;

	struct ngx_list_part_s {
		void 			*elts;	//数组的起始地址
		ngx_uint_t 		nelts;	//数组中已使用了多少个元素
		ngx_list_part_t *next;	//下一个链表元素的地址
	};

	typedef struct {
		ngx_list_part_t *last;  //链表的最后一个数组元素
		ngx_list_part_t part;	//链表的首个数组元素
		size_t 			size;	//每个数组元素占用的内存最大值
		ngx_uint_t 		nalloc;	//数组中元素的最大个数
		ngx_pool_t 		*pool;	//链表中管理内存分配的内存池对象
	} ngx_list_t;
####ngx_table_elt_t数据结构####
	typedef struct {
		ngx_uint_t 		hash;
		ngx_str_t		key;
		ngx_str_t		value;
		u_char			*lowcase_key;
	} ngx_table_elt_t;
####ngx_buf_t数据结构####
	typedef ngx_buf_s	ngx_buf_t;
	typedef void *		ngx_buf_tag_t;

	struct ngx_buf_s {
		u_char		*pos;
		u_char		*last;

		off_t		file_pos;
		off_t		file_last;

		u_char		*start;
		u_char		*end;

		ngx_buf_tag_t	tag;
		ngx_file_t	*file;

		ngx_buf_t	*shadow;

		unsigned	temporary:1;
		unsigned	memory:1;
		unsigned	mmap:1;
		unsigned	recycled:1;
		unsigned	in_file:1;
		unsigned	flush:1;
		unsigned	sync:1;
		unsigned	last_buf:1;
		unsigned	last_in_chain:1;
		unsigned	last_shadow:1;
		unsigned	temp_file:1;
	};
####ngx_chain_t数据结构####
	typedef struct ngx_chain_s ngx_chain_t;

	struct ngx_chain_s {
		ngx_buf_t	*buf;
		ngx_chain_s *next;
	};
###编译模块到Nginx应用中###
	1.首先把源代码文件全部放到一个目录下，并且在该目录下编写一个名称为config的文件用于通知Nginx如何编译该模块；
	2.执行configure脚本时加入--add-module=PATH（PATH为第三方模块源码及config文件所在目录）
####config文件的写法####
	config文件是一个可执行的shell脚本。
	config文件需要定义三个变量：
		1.ngx_addon_name:仅在configure执行时使用，一般为模块名称
		2.CORE_MODULES:核心模块
		2.EVENT_MODULES:事件模块
		2.HTTP_MODULES:保存所有的HTTP模块名称，每个HTTP模块由空格相连
		2.HTTP_FILTER_MODULES:HTTP过滤模块
		2.HTTP_HEADER_FILTER_MODULES:HTTP头部过滤模块
		3.NGX_ADDON_SRCS:指定新增模块的源代码文件，多个带编译的源代码文件以空格符相连
		4.NGX_ADDON_DEPS:模块依赖的路径
		Note：NGX_ADDON_SRCS中可以使用$ngx_addon_dir变量，等价于configure参数中--add-module=PATH中的PATH变量值
	config文件示例：
		ngx_addon_name=ngx_http_lzler_module
		HTTP_MODULES="$HTTP_MODULES ngx_http_lzler_module"
		NGX_ADDON_SRCS="$NGX_ADDON_SRCS $ngx_addon_dir/ngx_http_lzler_module.c"
####利用configure脚本将第三方模块加入到Nginx中####
####直接修改Makefile文件增加新特性或修改某些特性####
	执行完configure后，对objs/ngx_modules.c和objs/Makefile文件进行修改
		1.重新决定ngx_module_t *ngx_modules[]数组中各个模块的顺序
		2.编译源代码时需要加入一些独特的编译选项
###Nginx模块的数据结构###
	typedef struct ngx_modules_s	ngx_modules_t;

	struct ngx_modules_s {
		#define NGX_MODULE_V1	0,0,0,0,0,0,1

		ngx_uint_t		ctx_index;	//类型模块索引
		ngx_uint_t		index;		//模块索引
		/*保留字段*/
		ngx_uint_t		spare0;
		ngx_uint_t		spare1;
		ngx_uint_t		spare2;
		ngx_uint_t		spare3;
		ngx_uint_t		version;	//版本号

		/*类型模块上下文*/
		void			*ctx;

		/*nginx.conf配置项*/
		ngx_command_t	*commands;

		/*模块类型*/
		ngx_uint_t		type;

		/*Nginx启动，停止过程中的7个回调函数*/
		ngx_int_t		(*init_master)(ngx_log_t *log);
		ngx_int_t		(*init_module)(ngx_cycle_t *cycle);
		ngx_int_t		(*init_process)(ngx_cycle_t *cycle);
		ngx_int_t		(*init_thread)(ngx_cycle_t *cycle);
		void			(*exit_thread)(ngx_cycle_t *cycle);
		void			(*exit_process)(ngx_cycle_t *cycle);
		void			(*exit_master)(ngx_cycle_t *cycle);

		/*保留字段*/
		uintptr_t		spare_hook0;
		uintptr_t		spare_hook1;
		uintptr_t		spare_hook2;
		uintptr_t		spare_hook3;
		uintptr_t		spare_hook4;
		uintptr_t		spare_hook5;
		uintptr_t		spare_hook6;
		uintptr_t		spare_hook7;
	};

	typedef struct {
		ngx_int_t		(*preconfiguration)(ngx_conf_t *cf);
		ngx_int_t		(*postconfiguration)(ngx_conf_t *cf);
		void			*(*create_main_conf)(ngx_conf_t *cf);
		char			*(*init_main_conf)(ngx_conf_t *cf, void *conf);
		void			*(*create_srv_conf)(ngx_conf_t *cf);
		char			*(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);
		void			*(*create_loc_conf)(ngx_conf_t *cf);
		char			*(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);
	} ngx_http_module_t;

	typedef struct ngx_command_s		ngx_command_t;
	struct ngx_command_s {
		ngx_str_t		name;
		ngx_uint_t		type;
		char			*(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
		ngx_uint_t		conf;
		ngx_uint_t		offset;
		void			*post;
	};

	#define ngx_null_command {ngx_null_string, 0, NULL, 0, 0, NULL}
###Nginx模块开发###
	Nginx框架
	HTTP框架
####Nginx模块配置项处理开发####
	/*定义模块配置项*/
	static ngx_command_t ngx_http_lzler_commands[] = {
		{ngx_string("lzler"),
		 NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS
         ngx_http_lzler,
		 NGX_HTTP_LOC_CONF_OFFSET,
		 0,
		 NULL}，
		ngx_null_command
	}；

	/*解析配置项及其配置项回调函数*/
	static char *
	ngx_http_lzler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
		//声明配置变量
		ngx_http_core_loc_conf_t *clcf;
		//获取配置项
		clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
		//配置项回调函数
		clcf->handler = ngx_http_lzler_handler;

		return NGX_CONF_OK;
	}

	/*HTTP框架初始化*/
	static ngx_http_module_t ngx_http_lzler_module_ctx = {
		NULL,	/*preconfiguration*/
		NULL,	/*postconfiguration*/

		NULL,	/*create main configuration*/
		NULL,	/*init main configuration*/

		NULL,	/*create servr configuration*/
		NULL,	/*merge server configuration*/

		NULL,	/*create location configuration*/
		NULL	/*merge location configuration*/
	};

	ngx_module_t ngx_http_lzler_module = {
		NGX_MODULE_V1,
		&ngx_http_lzler_module_ctx,		/*module context*/
		ngx_http_lzler_commands,		/*module directives*/
		NGX_HTTP_MODULE,				/*module type*/
		NULL,							/*init master*/
		NULL,							/*init module*/
		NULL,							/*init process*/
		NULL,							/*init thread*/
		NULL,							/*exit thread*/
		NULL,							/*exit process*/
		NULL,							/*exit master*/
		NGX_MODULE_V1_APPING
	};
####ngx_http_request_t####
	typedef struct ngx_http_request_s ngx_http_request_t;

	struct ngx_http_request_s {
		...
		ngx_buf_t				*header_in;
		ngx_http_headers_in_t	headers_in;
		...
		ngx_uint_t		method;
		ngx_uint_t		http_version;

		ngx_str_t		request_line;
		ngx_str_t		uri;
		ngx_str_t		args;
		ngx_str_t		exten;
		ngx_str_t		unparsed_uri;

		ngx_str_t		method_name;
		ngx_str_t		http_protocol;

		u_char			*uri_start;
		u_char			*uri_end;
		u_char			*uri_ext;
		u_char			*args_start;
		u_char			*request_start;
		u_char			*request_end;
		u_char			*method_end;
	};
####ngx_http_headers_in_t####
	typedef struct {
		ngx_list_t			headers;

		ngx_table_elt_t		*host;
		ngx_table_elt_t		*connection;
		ngx_table_elt_t		*if_modified_since;
		ngx_table_elt_t		*if_unmodified_since;
		ngx_table_elt_t		*if_match;
		ngx_table_elt_t		*if_none_match;
		ngx_table_elt_t		*user_agent;
		ngx_table_elt_t		*referer;
		ngx_table_elt_t		*content_length;
		ngx_table_elt_t		*content_type;
		ngx_table_elt_t		*range;
		ngx_table_elt_t		*if_range;
		ngx_table_elt_t		*transfer_encoding;
		ngx_table_elt_t		*expect;
		ngx_table_elt_t		*upgrade;
	#if (NGX_HTTP_GZIP)
		ngx_table_elt_t		*accept_encoding;
		ngx_table_elt_t		*via;
	#endif
		ngx_table_elt_t		*authorization;
		ngx_table_elt_t		*keep_alive;
	#if (NGX_HTTP_X_FORWARDED_FOR)
		ngx_array_t			*x_forwarded_for;
	#endif
	#if (NGX_HTTP_READIP)
		ngx_table_elt_t		*x_real_ip;
	#endif
	#if (NGX_HTTP_HEADERS)
		ngx_table_elt_t		*accept;
		ngx_table_elt_t		*accept_language;
	#endif
	#if (NGX_HTTP_DAV)
		ngx_table_elt_t		*depth;
		ngx_table_elt_t		*destination;
		ngx_table_elt_t		*overwrite;
		ngx_table_elt_t		*date;
	#endif
		ngx_str_t			user;
		ngx_str_t			passwd;
		ngx_array_t			cookies;
		ngx_str_t			server;
		off_t				content_length_n;
		time_t				keep_alive_n;
		unsigned			connction_type:2;
		unsigned			chunked:1;
		unsigned			msie:1;
		unsigned			msie6:1;
		unsigned			opera:1;
		unsigned			gechko:1;
		unsigned			chrome:1;
		unsigned			safari:1;
		unsigned			konqueror:1;
	} ngx_http_headers_in_t;
####获取HTTP包体####
	HTTP框架提供了一个异步读取包体的函数
	ngx_int_t ngx_http_read_client_request_body(ngx_http_request_t　*r, ngx_http_client_body_handler_pt post_handler);
	丢弃客户端的HTTP包体
	ngx_int_t ngx_http_discard_request_body(ngx_http_request_t *r);
###发送响应###
	HTTP框架发送响应
		1.HTTP框架发送响应头部（响应行和响应头）
		2.HTTP框架发送响应包体
####发送HTTP响应头部####

####发送HTTP响应包体####