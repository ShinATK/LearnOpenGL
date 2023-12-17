#cg #opengl 

---
## GLSL

一种为图形计算定制的类 C 语言，包含一些针对向量和矩阵操作的有用特征。

基本格式：
- 开头声明版本
- 声明输入和输出变量
- uniform
- `main` 函数：处理输入变量并给出输出变量

典型结构如下：

```cpp
#version version_number
in type in_variable_name;
in type in_variable_name;
out type out_variable_name;
uniform type uniform_name;
int main()
{
    // 处理输入并进行图形操作
    ...
    // 输出处理过的结果到输出变量
    out_variable_name = weird_stuff_we_processed;
}
```

- **顶点属性** Vertex Attribute：每个输入变量
  - 硬件决定能够声明的顶点属性上限
  - OpenGL 确保至少 16 个包含 4 个分量的顶点属性可用
  - 通过 `GL_MAX_VERTEX_ATTRIBS` 来获取具体的上限

```cpp
int nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
std::cout<<"Maximum nr of vertex attributes supported:"<<nrAttributes<<std::endl;
```

## 数据类型

- 包含 C 等其他语言大部分的默认基础数据类型
- **两个容器类型**：
  - 向量 Vector
  - 矩阵 Matrix

这里先讨论向量

### 向量

| 类型  | 含义                      |
| ----- | ------------------------- |
| vecn  | 包含 `n` 个 float 分量        |
| bvecn | 包含 `n` 个 bool 分量         |
| ivecn | 包含 `n` 个 int 分量          |
| uvecn | 包含 `n` 个 unsigned int 分量 |
| dvecn | 包含 `n` 个 double 分量 |

大多数时候使用 `vecn`，float 足够用了

分量获取方式：`vec.x`

一种灵活的分量选择方式，**重组 Swizzling**：可以使用任意的 4 个分量（前提是有这个分量）来组合成任意一个含有目标分量个数的新向量，传递构造函数中也可以

```cpp
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec4 otherResult = vec4(result.xyz, 1.0);
```

### 输入与输出

GLSL 定义了 `in` 和 `out` 关键字来实现着色器的输入和输出（即，着色器之间的数据交流和传递）

每个着色器使用这两个关键字设定输入和输出，*只要一个输出变量与下一个着色器阶段的输入匹配*，就会传递下去

需要注意，**顶点和片段着色器中这种传递会有所不同**

- 顶点着色器：接收特殊形式的输入来保证效率
  - 直接从顶点数据中接收输入
  - 使用 `location` 元数据指定输入变量，从而在 CPU 上配置顶点属性
  - 如之前使用过的，`layout (location=0)` 给输入设置一个额外的 `layout` 标识，从而链接到顶点数据

> 还可以在 OpenGL 代码中使用 `glGetAttribLocation` 查询属性位置值（Location），但直接在 GLSL 着色器中设置更方便

- 片段着色器：需要一个 `vec4` 的颜色输出变量
  - 如果没有定义输出颜色，最终物体会被渲染为黑色/白色

综上，如果想要从一个着色器向另一个着色器发送数据
- 发送方着色器中：声明输出；接收方着色器中：声明类似输入
- 当二者匹配，两个变量会链接到一起，从而实现数据传递（链接程序对象时完成的）


### Uniform

**Uniform**，一种从 CPU 中的应用向 GPU 中的着色器发送数据的方式，但*uniform 和顶点属性不同*

- uniform 是**全局的**（Global）
  - 需要在每个着色器程序对象中独一无二
  - 可以被着色器程序的**任意着色器**在**任意阶段**访问
- uniform 会一直保存数据，直到被重置或更新
  - 通过 `glGetUniformLocation` 查询指定 shaderProgram 中的 uniform 变量位置
  - 利用 `glUniform4f` 设置目前激活的 shaderProgram 中的 uniform 值，**更新 uniform 时需要先激活对应的 shaderProgram**

例题：[利用uniform使三角形颜色随时间变化](../src/03-hello-shaders/examples/shaders-uniform/)


### 更多属性

这里将颜色数据添加为 3 个 float 值至 vertices 数组中，从而实现在三角形内部的颜色插值

例题：[向顶点属性中添加颜色数据实现三角形内部颜色插值效果](../src/03-hello-shaders/examples/shaders-vertexColor/)

问题：[顶点属性添加颜色数据但只显示了一个三角形](../error/添加顶点颜色属性.md)


## 自定义着色器类

### 从文件读取

主要内容：封装目前所学的知识到一个抽象对象中

具体的代码实现见：[03-hello-shaders/shaders-custom](./src/03-hello-shaders/examples/shaders-custom/)

主要封装内容：
- GLSL：glsl 的 `shaderSource` 部分
	- `vs.glsl`
	- `fs.glsl`
- Shader 类
	- `vertexShader` 和 `fragmentShader` 的创建和配置
	- `shaderProgram` 的创建、链接以及激活部分
	- 对 `uniform` 变量的 `set_` 类功能实现

## 练习

- [03-hello-shaders 练习题](../src/03-hello-shaders/03-hello-shaders%20练习题.md)