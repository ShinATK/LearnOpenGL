#cg #opengl 

---
## 图形渲染管线

[图形学渲染管线](图形学渲染管线.md)两个主要部分：
- 3 D 坐标转换为 2 D 坐标
- 2 D 坐标转变为实际的有颜色的像素

> **2 D 坐标和像素是不同的**
> 2 D 坐标：一个点在 2 D 空间中的精确位置
> 2 D 像素：这个点的近似值，受屏幕分辨率/窗口分辨率的限制

图形渲染管线可以被划分几个具有*并行执行*特性的阶段，从而可以在 GPU 上为每一个（渲染管线）阶段运行各自的小程序，这些小程序叫做**着色器**（Shader）

OpenGL 着色器是用**OpenGL 着色器语言（OpenGL Shading Language，GLSL）** 写成的

下图，图形渲染管线的抽象示意图，其中的**蓝色**：代表可以注入自定义的着色器的部分

![图形渲染管线的抽象展示](img/Pasted%20image%2020231202211423.png)

## 顶点输入

将输入顶点数据发送给 GPU

- 定义顶点数据：
```cpp
float vertices[] = { 
	-0.5f, -0.5f, 0.0f, 
	0.5f, -0.5f, 0.0f, 
	0.0f, 0.5f, 0.0f 
};
```

OpenGL 仅当 3 D 坐标在 3 个轴（x、y 和 z）上 -1.0 到 1.0 的范围内时才进行处理，这个范围内的坐标叫做[标准化设备坐标 NDC](标准化设备坐标%20NDC.md)（Normalized Device Coordinates）

- 利用 glGenBuffers 函数和一个缓冲 ID 生成一个 VBO 对象
```cpp
// 利用 glGenBuffers 函数和一个缓冲 ID 生成一个 VBO 对象
unsigned int VBO;
glGenBuffers(1, &VBO);
```

顶点缓冲对象的缓冲类型：`GL_ARRAY_BUFFER`

- glBindBuffer 函数把新创建的缓冲绑定到 GL_ARRAY_BUFFER 目标上
```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
```

- 调用 glBufferData 函数，它会把之前定义的顶点数据复制到缓冲的内存中：
	- 第一个参数是*目标缓冲的类型*：顶点缓冲对象当前绑定到 GL_ARRAY_BUFFER 目标上
	- 第二个参数*指定传输数据的大小*(以字节为单位)；用一个简单的 `sizeof` 计算出顶点数据大小就行
	- 第三个参数是*希望发送的*实际数据
	- 第四个参数指定了希望显卡如何管理给定的数据，三种形式
		- GL_STATIC_DRAW ：数据不会或几乎不会改变
		- GL_DYNAMIC_DRAW：数据会被改变很多。
		- GL_STREAM_DRAW ：数据每次绘制时都会改变。
```cpp
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

现在我们已经把**顶点数据**储存在显卡的内存中，用**VBO**这个**顶点缓冲对象**管理。

## 创建、链接、删除着色器

- 设置 GPU 如何处理顶点和片段着色器中的处理

### 顶点着色器

一个非常基础的 GLSL 顶点着色器的源代码：
```cpp
// 以版本声明开始，和 OpenGL 版本匹配，core 表示核心模式
#version 450 core 
// 关键字 in，声明输入顶点属性 Input Vertex Attribute
// vec3:3 个分量的 vector，变量名为 aPos
// layout (location = 0)：输入变量的位置值
layout (location = 0) in vec3 aPos;

void main()
{
	// gl_Postion 则为顶点着色器的输出
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

- **向量 (Vector)**
	- 在图形编程中我们经常会使用向量这个数学概念，因为它简明地表达了任意空间中的位置和方向，并且它有非常有用的数学属性。
	- 在 GLSL 中一个向量有最多 4 个分量，每个分量值都代表空间中的一个坐标，它们可以通过 `vec.x`、`vec.y`、`vec.z` 和 `vec.w` 来获取。
	- 注意 `vec.w` 分量不是用作表达空间中的位置的（我们处理的是 3 D 不是 4 D），而是用在所谓透视除法 (Perspective Division) 上。我们会在后面的教程中更详细地讨论向量。

这里没有对输入数据做什么处理，真实的程序里通常都不是标准化设备坐标，所以需要首先把输入数据转换至 OpenGL 的可视区域里


### 编译着色器

将顶点着色器的源代码硬编码在代码文件顶部的 C 风格字符串中，在运行时动态编译：

```cpp
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
```

- 创建着色器对象
	- 使用 ID 来引用
	- glCreateShader 创建着色器
```cpp
unsigned int vertexShader;
vertextShader = glCreateShader(GL_VERTEX_SHADER);
```

### 片段着色器



### 着色器程序

着色器程序对象 (Shader Program Object) 是多个着色器*合并*之后并最终*链接*完成的版本。

- 创建一个程序对象
- 添加着色器
- 链接着色器
- 可选
	- 检测链接着色器程序是否失败，并获取日志
	- 链接成功则删除着色器对象
- 激活该程序对象


## 链接顶点属性

设置如何解释内存中的顶点数据，以及如何将顶点数据连接到顶点着色器的属性

## 练习

- [02-hello-triangle 练习题](../src/02-hello-triangle/02-hello-triangle%20练习题.md)