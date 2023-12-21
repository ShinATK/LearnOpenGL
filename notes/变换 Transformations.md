#cg #opengl 

---
目前我们的进度：
- 创建物体
- 着色
- 加入纹理
但这些物体还只是静态的，可以通过在每一帧修改物体的顶点并且重新配置缓冲区从而移动物体。但太繁琐而且会消耗很多时间。

使用（多个）矩阵（Matrix）对象可以更好的变换（Transform）一个物体。

---
# 向量

**向量**有一个*方向*(Direction)和*大小*(Magnitude，也叫做强度或长度)。

![](img/Pasted%20image%2020231207144928.png)

$$\bar{v} = \begin{pmatrix} \color{red}x \\ \color{green}y \\ \color{blue}z \end{pmatrix}$$
通常设定这个方向的原点为(0, 0, 0)，然后指向一个方向，对应一个点，使其变为*位置向量*(Position Vector)

## 向量与标量运算

**标量**(Scalar)只是一个数字（或者说是仅有一个分量的向量）。

$$
\begin{pmatrix} \color{red}1 \\ \color{green}2 \\ \color{blue}3 \end{pmatrix} + x = \begin{pmatrix} \color{red}1 + x \\ \color{green}2 + x \\ \color{blue}3 + x \end{pmatrix}
$$
> 数学上是没有向量+标量运算的
> 但在线性代数的库中会以 Broadcasting 的方式支持该操作

## 向量取反

对一个*向量取反*(Negate)会将其方向逆转。

$$
-\bar{v} = -\begin{pmatrix} \color{red}{v_x} \\ \color{blue}{v_y} \\ \color{green}{v_z} \end{pmatrix} = \begin{pmatrix} -\color{red}{v_x} \\ -\color{blue}{v_y} \\ -\color{green}{v_z} \end{pmatrix}
$$

## 向量加减

向量的加法可以被定义为是*分量的(Component-wise)相加*，即将一个向量中的每一个分量加上另一个向量的对应分量：

$$
\bar{v} = \begin{pmatrix} \color{red}1 \\ \color{green}2 \\ \color{blue}3 \end{pmatrix}, \bar{k} = \begin{pmatrix} \color{red}4 \\ \color{green}5 \\ \color{blue}6 \end{pmatrix} \rightarrow \bar{v} + \bar{k} = \begin{pmatrix} \color{red}1 + \color{red}4 \\ \color{green}2 + \color{green}5 \\ \color{blue}3 + \color{blue}6 \end{pmatrix} = \begin{pmatrix} \color{red}5 \\ \color{green}7 \\ \color{blue}9 \end{pmatrix}
$$

直观表示为：![](img/Pasted%20image%2020231207145244.png)

同样的减法为：![](img/Pasted%20image%2020231207145305.png)

## 长度

使用*勾股定理*(Pythagoras Theorem)来获取向量的长度(Length)/大小(Magnitude)。

![](img/Pasted%20image%2020231207145329.png)

$$
||\textcolor{red}{\bar{v}}|| = \sqrt{\textcolor{green}x^2 + \textcolor{blue}y^2}
$$

有一个特殊类型的向量叫做单位向量(Unit Vector)。单位向量有一个特别的性质——它的长度是1。我们可以用任意向量的每个分量除以向量的长度得到它的单位向量 $\hat{n}$：

$$
\hat{n} = \frac{\bar{v}}{||\bar{v}||}
$$

向量的*标准化*(Normalizing)

## 向量相乘

点乘(Dot Product)，记作 $\vec{v}\cdot\vec{k}$，另一个是叉乘(Cross Product)，记作 $\vec{v}\times\vec{k}$。

### 点乘

$$\bar{v} \cdot \bar{k} = \|\bar{v}\| \cdot \|\bar{k}\| \cdot \cos \theta
$$

### 叉乘

叉乘只在3D 空间中有定义，它需要两个不平行向量作为输入，生成一个正交于两个输入向量的第三个向量。如果输入的两个向量也是正交的，那么叉乘之后将会产生3个互相正交的向量。

![](img/Pasted%20image%2020231207145724.png)

$$
\begin{pmatrix} \color{red}{A_{x}} \\ \color{green}{A_{y}} \\ \color{blue}{A_{z}} \end{pmatrix} \times \begin{pmatrix} \color{red}{B_{x}} \\ \color{green}{B_{y}} \\ \color{blue}{B_{z}}  \end{pmatrix} = \begin{pmatrix} \color{green}{A_{y}} \cdot \color{blue}{B_{z}} - \color{blue}{A_{z}} \cdot \color{green}{B_{y}} \\ \color{blue}{A_{z}} \cdot \color{red}{B_{x}} - \color{red}{A_{x}} \cdot \color{blue}{B_{z}} \\ \color{red}{A_{x}} \cdot \color{green}{B_{y}} - \color{green}{A_{y}} \cdot \color{red}{B_{x}} \end{pmatrix}
$$


# 矩阵

## 矩阵的加减

矩阵与矩阵之间的加减就是两个矩阵对应元素的加减运算

- 矩阵与标量的加减同样采用 broadcasting 机制

## 矩阵的数乘

- 标量就是用它的值**缩放**(Scale)矩阵的所有元素

## 矩阵相乘

相乘还有一些限制：

1. 只有当左侧矩阵的列数与右侧矩阵的行数相等，两个矩阵才能相乘。
2. 矩阵相乘不遵守交换律(Commutative)，也就是说 $A\cdot B \ne B\cdot A$

# 矩阵与向量相乘

向量，它其实就是一个**N×1**矩阵，N表示向量分量的个数（也叫N维(N-dimensional)向量）

这个矩阵乘以我们的向量将**变换**(Transform)这个向量。

## 单位矩阵

在 OpenGL 中，由于某些原因我们通常使用**4×4**的变换矩阵(齐次化坐标)

最简单的变换矩阵就是**单位矩阵**(Identity Matrix)，向量不会改变

$$
\begin{bmatrix} \color{red}1 & \color{red}0 & \color{red}0 & \color{red}0 \\ \color{green}0 & \color{green}1 & \color{green}0 & \color{green}0 \\ \color{blue}0 & \color{blue}0 & \color{blue}1 & \color{blue}0 \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix} \cdot \begin{bmatrix} 1 \\ 2 \\ 3 \\ 4 \end{bmatrix} = \begin{bmatrix} \color{red}1 \cdot 1 \\ \color{green}1 \cdot 2 \\ \color{blue}1 \cdot 3 \\ \color{purple}1 \cdot 4 \end{bmatrix} = \begin{bmatrix} 1 \\ 2 \\ 3 \\ 4 \end{bmatrix}
$$

## 缩放

对一个向量进行**缩放**(Scaling)就是对向量的长度进行缩放，而保持它的方向不变。

> 记住，OpenGL 通常是在 3 D 空间进行操作的，对于 2 D 的情况我们可以把 z 轴缩放 1 倍，这样 z 轴的值就不变了。
> 这种缩放操作是**不均匀** (Non-uniform)缩放，因为每个轴的缩放因子 (Scaling Factor)都不一样。
> 如果每个轴的缩放因子都一样那么就叫**均匀缩放**(Uniform Scale)。

- 构造一个变换矩阵来提供缩放功能，$(\textcolor{red}{S_1},\textcolor{green}{S_2},\textcolor{blue}{S_3})$

$$
\begin{bmatrix} \color{red}{S_1} & \color{red}0 & \color{red}0 & \color{red}0 \\ \color{green}0 & \color{green}{S_2} & \color{green}0 & \color{green}0 \\ \color{blue}0 & \color{blue}0 & \color{blue}{S_3} & \color{blue}0 \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix} \cdot \begin{pmatrix} x \\ y \\ z \\ 1 \end{pmatrix} = \begin{pmatrix} \color{red}{S_1} \cdot x \\ \color{green}{S_2} \cdot y \\ \color{blue}{S_3} \cdot z \\ 1 \end{pmatrix}
$$

- 注意，第四个缩放向量仍然是1，因为在3D空间中缩放w分量是无意义的

## 位移

**位移**(Translation)是在原始向量的基础上加上另一个向量从而获得一个在不同位置的新向量的过程，从而在位移向量基础上**移动**了原始向量

把位移向量表示为 $(\textcolor{red}{T_x},\textcolor{green}{T_y},\textcolor{blue}{T_z})$:

$$
\begin{bmatrix}  \color{red}1 & \color{red}0 & \color{red}0 & \color{red}{T_x} \\ \color{green}0 & \color{green}1 & \color{green}0 & \color{green}{T_y} \\ \color{blue}0 & \color{blue}0 & \color{blue}1 & \color{blue}{T_z} \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix} \cdot \begin{pmatrix} x \\ y \\ z \\ 1 \end{pmatrix} = \begin{pmatrix} x + \color{red}{T_x} \\ y + \color{green}{T_y} \\ z + \color{blue}{T_z} \\ 1 \end{pmatrix}
$$

> **齐次坐标 (Homogeneous Coordinates)**
> 
> 向量的 w 分量也叫齐次坐标。想要从齐次向量得到 3 D 向量，我们可以把 x、y 和 z 坐标分别除以 w 坐标。我们通常不会注意这个问题，因为 w 分量通常是 1.0。使用齐次坐标有几点好处：它允许我们在 3 D 向量上进行位移（如果没有 w 分量我们是不能位移向量的），而且下一章我们会用 w 值创建 3 D 视觉效果。
> 
> 如果一个向量的齐次坐标是0，这个坐标就是方向向量(Direction Vector)，因为 w 坐标是0，这个向量就不能位移（译注：这也就是我们说的不能位移一个方向）。

## 旋转

2D或3D空间中的旋转用角(Angle)来表示。角可以是角度制或弧度制的，周角是360角度或2 [PI](https://en.wikipedia.org/wiki/Pi)弧度

> 大多数旋转函数需要用弧度制的角，但幸运的是角度制的角也可以很容易地转化为弧度制的：
> 
> - 弧度转角度：`角度 = 弧度 * (180.0f / PI)`
> - 角度转弧度：`弧度 = 角度 * (PI / 180.0f)`
> 
> `PI`约等于3.14159265359。

在3D空间中旋转需要定义一个**角**和一个**旋转轴**(Rotation Axis)

旋转矩阵在3D 空间中每个单位轴都有不同定义，旋转角度用θ�表示：

沿x轴旋转：

$$
\begin{bmatrix} \color{red}1 & \color{red}0 & \color{red}0 & \color{red}0 \\ \color{green}0 & \color{green}{\cos \theta} & - \color{green}{\sin \theta} & \color{green}0 \\ \color{blue}0 & \color{blue}{\sin \theta} & \color{blue}{\cos \theta} & \color{blue}0 \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix} \cdot \begin{pmatrix} x \\ y \\ z \\ 1 \end{pmatrix} = \begin{pmatrix} x \\ \color{green}{\cos \theta} \cdot y - \color{green}{\sin \theta} \cdot z \\ \color{blue}{\sin \theta} \cdot y + \color{blue}{\cos \theta} \cdot z \\ 1 \end{pmatrix}
$$

沿y轴旋转：

$$
\begin{bmatrix} \color{red}{\cos \theta} & \color{red}0 & \color{red}{\sin \theta} & \color{red}0 \\ \color{green}0 & \color{green}1 & \color{green}0 & \color{green}0 \\ - \color{blue}{\sin \theta} & \color{blue}0 & \color{blue}{\cos \theta} & \color{blue}0 \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix} \cdot \begin{pmatrix} x \\ y \\ z \\ 1 \end{pmatrix} = \begin{pmatrix} \color{red}{\cos \theta} \cdot x + \color{red}{\sin \theta} \cdot z \\ y \\ - \color{blue}{\sin \theta} \cdot x + \color{blue}{\cos \theta} \cdot z \\ 1 \end{pmatrix}
$$

沿z轴旋转：

$$
\begin{bmatrix} \color{red}{\cos \theta} & - \color{red}{\sin \theta} & \color{red}0 & \color{red}0 \\ \color{green}{\sin \theta} & \color{green}{\cos \theta} & \color{green}0 & \color{green}0 \\ \color{blue}0 & \color{blue}0 & \color{blue}1 & \color{blue}0 \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix} \cdot \begin{pmatrix} x \\ y \\ z \\ 1 \end{pmatrix} = \begin{pmatrix} \color{red}{\cos \theta} \cdot x - \color{red}{\sin \theta} \cdot y  \\ \color{green}{\sin \theta} \cdot x + \color{green}{\cos \theta} \cdot y \\ z \\ 1 \end{pmatrix}
$$

[万向节死锁（Gimbal Lock）](https://www.youtube.com/watch?v=zc8b2Jo7mno)

避免万向节死锁的真正解决方案是使用**四元数(Quaternion)**，它不仅更安全，而且计算会更有效率

---
> 译注
> 
> 对四元数的理解会用到非常多的数学知识。如果你想了解四元数与 3 D 旋转之间的关系，可以来阅读我的[教程](https://krasjet.github.io/quaternion/)。如果你对万向节死锁的概念仍不是那么清楚，可以来阅读我教程的 [Bonus章节](https://krasjet.github.io/quaternion/bonus_gimbal_lock.pdf)。
> 
> 现在3Blue1Brown 也已经开始了一个四元数的视频系列，他采用球极平面投影(Stereographic Projection)的方式将四元数投影到3D 空间，同样有助于理解四元数的概念（仍在更新中）：[https://www.youtube.com/watch?v=d4EgbgTm0Bg](https://www.youtube.com/watch?v=d4EgbgTm0Bg)
---
## 矩阵的组合

根据矩阵之间的乘法，我们可以把多个变换组合到一个矩阵中。

$$
Trans . Scale = \begin{bmatrix} \color{red}1 & \color{red}0 & \color{red}0 & \color{red}1 \\ \color{green}0 & \color{green}1 & \color{green}0 & \color{green}2 \\ \color{blue}0 & \color{blue}0 & \color{blue}1 & \color{blue}3 \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix} . \begin{bmatrix} \color{red}2 & \color{red}0 & \color{red}0 & \color{red}0 \\ \color{green}0 & \color{green}2 & \color{green}0 & \color{green}0 \\ \color{blue}0 & \color{blue}0 & \color{blue}2 & \color{blue}0 \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix} = \begin{bmatrix} \color{red}2 & \color{red}0 & \color{red}0 & \color{red}1 \\ \color{green}0 & \color{green}2 & \color{green}0 & \color{green}2 \\ \color{blue}0 & \color{blue}0 & \color{blue}2 & \color{blue}3 \\ \color{purple}0 & \color{purple}0 & \color{purple}0 & \color{purple}1 \end{bmatrix}
$$

- 应该**从右向左**读这个乘法
- 建议的变换顺序：
	- 缩放
	- 旋转
	- 位移
- 为什么这么安排顺序？
	- *如果先位移，再缩放，那么位移的向量也会被缩放*

---
# 实践

专门为 OpenGL 量身定做的数学库，GLM

## GLM

[GLM](https://glm.g-truc.net/0.9.8/index.html) 是 Open**GL** **M**athematics 的缩写，它是一个**只有头文件的**库，也就是说我们只需包含对应的头文件就行了，不用链接和编译

`0.9.9` 及以上的版本：将所有的矩阵初始化改为 `glm::mat4 mat = glm::mat4(1.0f)`，否则默认初始化为零矩阵

- 常用的 3 个头文件
```
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
```

- [05-hello-transformation](../src/05-hello-transformation/05-hello-transformation.md)

