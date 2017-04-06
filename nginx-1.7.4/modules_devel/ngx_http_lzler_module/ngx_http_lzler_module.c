/*
 * this module (C) lzler
 * this module (C) YOUKU, Inc.
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>

static char *
ngx_http_lzler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_int_t
ngx_http_lzler_hander(ngx_http_request_t *r);

// 定义HTTP模块command
static ngx_command_t ngx_http_lzler_commands[] = {
    {
        ngx_string("lzler"),  /* configure name */
        NGX_CONF_NOARGS|NGX_HTTP_LOC_CONF,            /* configure type */
        ngx_http_lzler,  /* configure handle function */
        NGX_HTTP_LOC_CONF_OFFSET,                  /* configure offset */
        0,                  /* configure offset */
        NULL,               /* post configure handle function */
    },

    ngx_null_command
};


// 定义HTTP模块context
static ngx_http_module_t ngx_http_lzler_module_ctx = {
    NULL,                       /* preconfiguration */
    NULL,                       /* postconfiguration */

    NULL,                       /* create main configuration */
    NULL,                       /* init main configuration */

    NULL,                       /* create server configuration */
    NULL,                       /* merge server configuration */

    NULL,                       /* create location configuration */
    NULL,                       /* merge location configuration */
};

// 定义HTTP模块
ngx_module_t ngx_http_lzler_module = {
    NGX_MODULE_V1,
    &ngx_http_lzler_module_ctx, /* module context */
    ngx_http_lzler_commands,    /* module directives */
    NGX_HTTP_MODULE,            /* module type */
    NULL,                       /* init master */
    NULL,                       /* init module */
    NULL,                       /* init process */
    NULL,                       /* init thread */
    NULL,                       /* exit thread */
    NULL,                       /* exit process */
    NULL,                       /* exit master */
    NGX_MODULE_V1_PADDING
};

/*
 * 配置文件处理函数
 */
static char *
ngx_http_lzler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    printf("lzler module output: configure file handler function - ngx_http_lzler\n");
    //ngx_http_core_loc_conf_t *clcf;
    //clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    //clcf->handler = ngx_http_lzler_hander;

    return NGX_CONF_OK;
}

/*
 * 请求处理函数
 */
static ngx_int_t
ngx_http_lzler_hander(ngx_http_request_t *r)
{
    //ngx_table_elt_t *h = ngx_list_push(&r->headers_out.headers);
    //if (h == NULL) {
    //    return NGX_ERROR;
    //}

    //h->hash = 1;
    //h->key.len = sizeof("lzlerHeaderKey") - 1;
    //h->key.data = (u_char *)"lzlerHeaderKey";
    //h->value.len = sizeof("lzlerHeaderValue") - 1;
    //h->value.data = (u_char *)"lzlerHeaderValue";

    printf("lzler module output: request handler function - ngx_http_lzler_handler\n");
    return NGX_OK;
}
