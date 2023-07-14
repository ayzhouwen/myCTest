cmake_minimum_required(VERSION 3.4.2)

project(myCTest)
#指定可执行文件输出路径，设置为build文件夹下的bin目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#发现一个目录下所有的源代码文件并将其存储在一个变量(DIR_SRCS)中
aux_source_directory(./ DIR_SRCS)
#是否启用调试，注释掉无法debug
set(CMAKE_BUILD_TYPE Debug)

add_executable(myCTest ${DIR_SRCS})

target_link_libraries(myCTest -lpthread -luuid)