cmake_minimum_required(VERSION 2.8)
project(kore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(includes/)
set(SOURCE_FILES
        includes/http.h
        includes/jsonrpc.h
        includes/kore.h
        includes/pgsql.h
        includes/tasks.h
        src/accesslog.c
        src/auth.c
        src/bsd.c
        src/buf.c
        src/cli.c
        src/config.c
        src/connection.c
        src/domain.c
        src/http.c
        src/jsonrpc.c
        src/keymgr.c
        src/kore.c
        src/linux.c
        src/mem.c
        src/module.c
        src/msg.c
        src/net.c
        src/pgsql.c
        src/pool.c
        src/tasks.c
        src/timer.c
        src/utils.c
        src/validator.c
        src/websocket.c
        src/worker.c
        src/video_cache.c)
link_libraries( curl pthread )
add_executable(kore ${SOURCE_FILES})