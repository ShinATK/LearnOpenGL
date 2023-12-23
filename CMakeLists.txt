
cmake_minimum_required(VERSION 3.5.0)                       #版本号
project(LearnOpenGL)                                        #项目名
include_directories(./include)                                #把include纳入包含目录中
link_libraries("${PROJECT_SOURCE_DIR}/lib/libglfw3.a")      #链接之前生成的静态库文件
link_libraries("${PROJECT_SOURCE_DIR}/lib/libassimp.dll.a")
aux_source_directory(./src DIR_ALL_SRC)
aux_source_directory(./include/imgui DIR_ALL_SRC)
aux_source_directory(./include/glad DIR_ALL_SRC)

#--------------------------------------------------
# 设置目标文件夹
set(FOLDERNAME 4-3-blend_testing)
#--------------------------------------------------
# 设置相对路径
# set(ImgPath "${PROJECT_SOURCE_DIR}/img/")
# set(ShaderPath "${PROJECT_SOURCE_DIR}/shader/")
# set(TexturePath "${PROJECT_SOURCE_DIR}/texture/")
# 目标文件下的文件夹路径
set(FileFolderPath "${PROJECT_SOURCE_DIR}/src/${FOLDERNAME}/")
set(WorkFolderPath "${PROJECT_SOURCE_DIR}/")
# set(ShaderRelativePath "${PROJECT_SOURCE_DIR}/src/${FOLDERNAME}/shader/")
# set(TextureRelativePath "${PROJECT_SOURCE_DIR}/src/${FOLDERNAME}/texture/")
# 配置文件
configure_file(
    "${PROJECT_SOURCE_DIR}/config/pathSettings.h.in"
    "${PROJECT_SOURCE_DIR}/include/tools/pathSettings.h"
)
#--------------------------------------------------
aux_source_directory(./src/${FOLDERNAME} DIR_ALL_SRC)				   #src目录下所有文件取个DIR_ALL_SRC名字
add_executable(${FOLDERNAME} ${DIR_ALL_SRC})                   #生成可执行文件
