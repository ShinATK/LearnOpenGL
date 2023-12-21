#cg #opengl

# VSCode + MSYS2

VSCode + MSYS2 的配置过程可以参考下边链接：

- [(Archive)LearnOpenGL-01-搭建环境和创建项目-GCC](https://zhuanlan.zhihu.com/p/437840260)


## 基础开发环境

VSCode + MSYS2

## 添加 OpenGL 项目依赖

### OpenGL 中 glad、glew、glfw、Freeglut 库的区别


> 参考链接：https://www.zhihu.com/question/264132001


### 下载、编译依赖库并添加到项目中

#### 编译 glfw

[glfw 下载链接](https://www.glfw.org/download.html)

> MSYS2 中安装 GLFW
> `pacman -S mingw-w64-ucrt-x86_64-glfw`

解压压缩包，进入文件夹内并执行命令：

```shell
mkdir -p build
cd build
# 编译动态库
cmake -DBUILD_SHARED_LIBS=ON -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=OFF -DGLFW_VULKAN_STATIC=OFF -DCMAKE_BUILD_TYPE=Debug -G"MinGW Makefiles" ..
cmake --build . --parallel
# 编译静态库
cmake -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=OFF -DGLFW_VULKAN_STATIC=OFF -DCMAKE_BUILD_TYPE=Debug -G"MinGW Makefiles" ..
cmake --build . --parallel
```

> 参考链接：
> - [Compiling GLFW](https://www.glfw.org/docs/latest/compile_guide.html)
> - [How to Build a CMake-Based Project](https://preshing.com/20170511/how-to-build-a-cmake-based-project/)

在`build`目录的`src`目录下可以看到`glfw3.dll` `libglfw3dll.a`和`libfw3.a`

#### 添加 glfw 库到项目中

- GLFW as a DLL: To use GLFW as a DLL, link against the libglfw3dll.a file for your environment. This will add a load time dependency on glfw3.dll. The remaining files in the same directory are not needed.
- GLFW as a static library: To use GLFW as a static library, link against the libglfw3.a file for your environment. The other files in the same directory are not needed

两种添加方式：
- 动态链接
  - `libglfw3dll.a`放到项目的`lib`目录
  - `glfw3.dll`放到`bin`目录
- 静态链接
  - 

将 glfw 头文件同样放入项目中，glfw 源代码包中的`include`中的`GLFW`放到项目的`include`目录下

#### 下载（编译）并添加 GLAD

在 glad 的[在线服务网站](http://glad.dav1d.de/)获取配置



