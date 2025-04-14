cmake_minimum_required(VERSION 3.10)

# 项目名和语言
project(DeviceConfigProject C)

# 设置源文件列表
set(SOURCES
    config.c
    main.c
    queue.c
    gateway.c
)

# 设置输出路径为源代码所在目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# 添加可执行文件
add_executable(output ${SOURCES})

# 添加执行字符集参数（等效于 -fexec-charset=GBK）
target_compile_options(output PRIVATE -fexec-charset=GBK)
