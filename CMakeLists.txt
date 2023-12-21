cmake_minimum_required(VERSION 3.5.0)                       #版本号
project(LearnOpenGL)                                        #项目名
include_directories(./include)                                #把include纳入包含目录中
link_libraries("${PROJECT_SOURCE_DIR}/lib/libglfw3.a")      #链接之前生成的静态库文件
aux_source_directory(./src DIR_ALL_SRC)
aux_source_directory(./include/imgui DIR_ALL_SRC)
aux_source_directory(./include/glad DIR_ALL_SRC)

#--------------------------------------------------
# 设置目标文件夹
set(FOLDERNAME 2-6-multiple_light)
#--------------------------------------------------
# 设置相对路径
set(ShaderRelativePath "${PROJECT_SOURCE_DIR}/src/${FOLDERNAME}/shader")
set(TexturePath "${PROJECT_SOURCE_DIR}/texture")
configure_file(
    "${PROJECT_SOURCE_DIR}/config/DefaultSettings.h.in"
    "${PROJECT_SOURCE_DIR}/include/DefaultSettings/DefaultSettings.h"
)
#--------------------------------------------------
aux_source_directory(./src/${FOLDERNAME} DIR_ALL_SRC)				   #src目录下所有文件取个DIR_ALL_SRC名字
add_executable(LearnOpenGL ${DIR_ALL_SRC})                   #生成可执行文件

