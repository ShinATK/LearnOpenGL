#error

在cmake编译构建[shaders-custom](../../Graphics/LearnOpenGL/src/03-hello-shaders/examples/shaders-custom/)过程中无问题。

但是运行exe后出现的三角形是全黑的，很明显是着色器没有成功激活。

经过排查发现是因为在`main.cpp`中，读取两个着色器的路径问题，原本的路径写的是相对路径，修改为绝对路径后运行无误，三角形成功显示颜色。

**更正**：相对路径没问题，似乎是之前在片段着色器中设置的uniform变量忘记去掉了而导致的着色器程序链接错误。

