#cg #opengl 
-  [坐标系统 - LearnOpenGL CN](https://learnopengl-cn.github.io/01%20Getting%20started/08%20Coordinate%20Systems/) 

---

OpenGL 希望在每次*顶点着色器运行后*，我们可见的所有顶点都为[标准化设备坐标 NDC](标准化设备坐标%20NDC.md) (Normalized Device Coordinate, NDC)
- 即每个顶点的**x**，**y**，**z**坐标都应该在 **-1.0** 到 **1.0** 之间，超出这个坐标范围的顶点都将不可见

在流水线中，物体的顶点在最终转化为屏幕坐标之前还会被变换到多个坐标系统(Coordinate System)，在这些**过渡**坐标系(Intermediate Coordinate System)中，*一些操作或运算更加方便和容易*。

总共有 5 个比较重要的坐标系统：
- **局部空间**(Local Space，或者称为物体空间(Object Space))
- **世界空间**(World Space)
- **观察空间**(View Space，或者称为视觉空间(Eye Space))
- **裁剪空间**(Clip Space)
- **屏幕空间**(Screen Space)

## 概述

将坐标从一个坐标系转换为另一个坐标系需要几个变换矩阵，最重要的几个为：
- **模型** (Model)
- **观察** (View)
- **投影**(Projection)

- 顶点坐标起始于**局部空间**(Local Space)，在这里它称为**局部坐标**(Local Coordinate)

之后会依次变为：

- **世界坐标**(World Coordinate)
- **观察坐标**(View Coordinate)
- **裁剪坐标**(Clip Coordinate)
- 最后以**屏幕坐标**(Screen Coordinate)的形式结束

![](img/Pasted%20image%2020231207175714.png)

![](img/Pasted%20image%2020240113102506.png)