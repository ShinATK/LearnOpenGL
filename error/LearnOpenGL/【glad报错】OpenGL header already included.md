#error

在cmake编译构建[shaders-custom](../../Graphics/LearnOpenGL/src/03-hello-shaders/examples/shaders-custom/)过程中出现报错，

`error: #error OpenGL header already included, remove this include, glad already provides it`

根据[参考链接](https://blog.csdn.net/derbi123123/article/details/106578594)进行修正，发现是`#include <glad/glad.h>`位置必须放在所有头文件之前，无论是.h还是.cpp中都需要。

修正后运行无误。
