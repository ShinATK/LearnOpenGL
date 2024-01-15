#cg #opengl

# VS 2019 的配置过程

## GLFW

GLFW：一个专门针对 OpenGL 的 C 语言库，提供了一些渲染物体所需的最低限度的接口。
- 允许用户创建上下文
- 定义窗口参数
- 处理用户输入

### 构建 GLFW

GLFW 下载：[Download | GLFW](https://www.glfw.org/download.html)

这里将从编译源代码开始，源码包下载并解压。

明确需要的目标文件：
- 编译后生成的库
- include 文件夹

从源码编译库可以保证生成的库完全适合自己的操作系统和 CPU，而预编译的二进制文件则并非总是提供。

使用 CMake 来讲给定的.c/.cpp/.h/.hpp 来自己构建项目/解决方案

### CMake

[CMake](../../../CMake/CMake.md)：工程文件生成工具。通过预定义好的 CMake 脚本，根据自己的选择生成不同的 IDE 工程文件。

CMake 下载：[Download CMake](https://cmake.org/download/)

安装后，可以从命令行或 GUI 启动 CMake，这里使用 GUI 对下载的 GLFW 源代码进行编译。

![CMake 界面](img/Pasted%20image%2020231202205720.png)

按照步骤进行：
- 选择源代码目录和存放编译结果的目录（build）
- configure
- 选择工程的生成器，这里使用 VS2019，所以选择 VS16 选项
- 再次 configure 保存设置
- generate，工程文件生成在 build 文件夹中

### 编译

在 build 中打开 GLFW.sln，将配置修改为 64 位，点击 Build Solution，编译的库文件在`build/src/Debug`中

在 IDE 中添加库和头文件位置：
- 建立一个新目录包含所有第三方库和头文件，如
- OpenGL
	- Libs
	- Include

### 链接

此时还需要将 GLFW 库链接（Link）到工程，指定 glfw3.lib

- 项目属性 - VC++ 目录
- 在 Library Directories 和 Include Directiories 中添加对应路径
- 再选择 Linker - Input，Additional Dependencies 中添加 glfw3.lib 库文件名

最后还需添加条目链接到 OpenGL 的库

### VS 2019 配置 上的 GLAD 库

opengl32.lib 已经包含在 Microsoft SDK 中，VS 安装时已经默认安装，直接添加 opengl32.lib 即可

## GLAD

OpenGL 只是一个标准/规范，具体实现是由驱动开发商针对特定显卡实现的。而 OpenGL 驱动版本很多，大多数函数的位置都无法在编译时确定下来，需要在运行时查询，所以需要 GLAD 简化这个过程。

### 配置 GLAD

GLAD 是一个[开源](https://github.com/Dav1dde/glad)的库

使用[在线服务](http://glad.dav1d.de/)来获得配置，提供需要定义的 OpenGL 版本。

![GLAD 在线服务](img/Pasted%20image%2020231202205638.png)

- 语言设置：C/C++
- API 选项：3.3 以上，我个人选择的是 4.5
- Profile 设置：Core
- 确保选择了生成加载器（Generate a loader）
- 点击生成来获得库文件
- 下载压缩文件并解压：两个头文件目录，和一个**glad.c**文件
- 将两个 glad、KHR 头文件目录复制到先前建立的 Include 文件夹中
- 将 glad.c 添加到工程中
