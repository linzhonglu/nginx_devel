#!/bin/sh

_PWD=`pwd`
_WORKSPACE=$_PWD/workspace
_EXEC=$_WORKSPACE/sbin/nginx
_CONF=$_WORKSPACE/conf/nginx_debug.conf
mkdir -p $_WORKSPACE

_configure() {
    . $_PWD/configure \
        --with-debug \
        --prefix=$_WORKSPACE \
        --add-module=$_PWD/modules_devel/ngx_upstream_jdomain \
        --add-module=$_PWD/modules_devel/ngx_http_lzler_module
}

_makefile() {
    make -j 2
}

_install() {
    make install
    sudo setcap CAP_NET_BIND_SERVICE=+ep $_EXEC
    cp $_PWD/nginx.conf $_CONF
}

############
_start() {
    echo $"$_EXEC -p $_WORKSPACE -c $_CONF"
}

_reload() {
    $_EXEC -p $_WORKSPACE -c $_CONF -s reload
}

_stop() {
    $_EXEC -p $_WORKSPACE -c $_CONF -s stop
}

case $1 in
    install)
        _install
        ;;
    make)
        _makefile
        ;;
    configure)
        _configure
        ;;
    all)
        _configure
        _makefile
        _install
        ;;
    start)
        _start
        ;;
    reload)
        _reload
        ;;
    stop)
        _stop
        ;;
    *)
        echo $"Usage: $0 {install|make|configure|all|start|reload|stop}"
        ;;
esac
