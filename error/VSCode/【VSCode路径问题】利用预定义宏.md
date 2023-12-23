
- **绝对路径**：文件在硬盘上的真正路径
- **相对路径**：相对于当前目标文件的路径

**问题描述**：

利用vscode + cmake来编译运行cpp文件出现了相对路径的读取问题，手动在`launch.json`中修改`"cwd": "${fileDirname}"`，修改后仍然出现无法读取相对路径问题，如果修改为绝对路径没有任何问题。

为了保证代码的间接以及路径的普适性，这里尝试了另一种方法：利用**预定义宏**来将相对路径转为绝对路径

**大体思路**：
- 利用cmake自动获取当前目标文件所在路径
- 将该路径保存在某个预定义宏中
- 将该预定义宏与相对路径合并传入原路径参数位置

**这样的好处**：
- 该预定义宏参数会通过cmake文件自动更新路径，不需要人为填写臃肿的绝对路径
- 在目标文件中填写路径时填写相对路径，能够保证vscode中弹出的路径提示内容是一致的，填写起来方便
- 因为是cmake自动获取路径，所以对于不同平台是普适性的，运行不同项目时只需要在cmake中修改当前项目的`main.cpp`所在的文件夹名称
- 最重要的是：**It just work.**

**设置步骤**：
- 当前vscode的根目录下建立文件夹`config`，在其中建立文件`DefaultSettings.h.in`，内容如下：

```cpp
#ifndef __DEFAULT_SETTINGS_H_
#define __DEFAULT_SETTINGS_H_

// 预定义宏变量
#define WORK_FOLDER_PATH "@WorkFolderPath@" // vscode的根目录路径
#define FILE_FOLDER_PATH "@FileFolderPath@" // 指定工作项目文件夹路径

#endif //__DEFAULT_SETTINGS_H_
```

- 同样在vscode根目录下建立cmake文件，`CMakeLists.txt`，内容如下：

```m
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
set(FOLDERNAME 3-3-model)
#--------------------------------------------------
# 目标文件下的文件夹路径
set(FileFolderPath "${PROJECT_SOURCE_DIR}/src/${FOLDERNAME}/")
set(WorkFolderPath "${PROJECT_SOURCE_DIR}/")
# 配置文件
configure_file(
    "${PROJECT_SOURCE_DIR}/config/pathSettings.h.in"
    "${PROJECT_SOURCE_DIR}/include/pathSettings/pathSettings.h"
)
#--------------------------------------------------
aux_source_directory(./src/${FOLDERNAME} DIR_ALL_SRC)				   #src目录下所有文件取个DIR_ALL_SRC名字
add_executable(${FOLDERNAME} ${DIR_ALL_SRC})                   #生成可执行文件
```

- 本问题主要在于三处：**设置目标文件夹**、**目标文件下的文件夹路径**、**配置文件**
  - **设置目标文件夹**：当前`main.cpp`文件所在的项目文件夹名称
  - **目标文件下的文件夹路径**：当前项目文件夹的路径
  - **配置文件**：先前设置好的设置文件所在路径、以及预定义宏变量保存位置

- 在`main.cpp`及其他需要读取路径处，将`预定义宏变量+相对路径`形式的路径传入即可，这样相对路径填写时仍然可以参考vscode的提示





