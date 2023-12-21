# GLSL编写shaderSource中的换行符问题

在做[03-hello-shaders](../src/03-hello-shaders)的例题的时候发现了一个问题，如果在编写shaderSource的时候，缺少某个换行符如下：

```cpp
const char *fragmentShaderSource = 
    "#version 450 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vertexColor;//从顶点着色器传来的输入变量（名称相同，类型相同）" // 比如这里不加换行符
    "void main()\n"
    "{\n"
    "   FragColor = vertexColor;\n"
    "}\n\0";
```

最终输出三角形为黑色，而不是目标颜色

原因：由于改行后边有注释，去掉换行符后导致下一行的`void main()`一并被注释掉，该shaderSource没有正确运行，故出现了shader之间数据传输问题，从而导致最后的黑色

经测试，如果去掉后边注释内容，如下：

```cpp
const char *fragmentShaderSource = 
    "#version 450 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vertexColor;" // 比如这里去掉注释，但不加换行符
    "void main()\n"
    "{\n"
    "   FragColor = vertexColor;\n"
    "}\n\0";
```

编译运行后最后结果能够显示目标颜色。

所以，

**在编写GLSL时，要注意加上换行符，以避免出现不必要的错误**
