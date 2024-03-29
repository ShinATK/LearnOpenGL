#cg #opengl 
- [视差贴图 - LearnOpenGL CN](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/05%20Parallax%20Mapping/)
---
# 视差贴图
## 什么是视差贴图？

视差贴图 **Parallax Mapping**：位移贴图（Displacement Mapping）技术的一种，根据储存在纹理中的几何信息对顶点进行位移或偏移，而像这样每个纹理像素包含了高度值纹理叫做**高度贴图**，如下图：![](img/Pasted%20image%2020240110092116.png)

类似于法线贴图，*利用视错觉极大提升了表面细节并赋予深度感*

在应用贴图后，能够根据材质的几何属性，将平坦的平面变换成凹凸不平的表面，如下图：![](img/Pasted%20image%2020240110092224.png)

## 为什么需要视差贴图？

直至目前，应用法线贴图能够利用光照营造出表面的凹凸质感，但这种质感如果从一个大的倾斜角度去看过去会发现表面仍然是一个平坦的平面。

而视差贴图，因为其为位移贴图的一种，如果是法线贴图是从感觉上营造凹凸感，则位移贴图是真正会将顶点进行一定程度位移或偏移，从而真正实现凹凸感，配合法线贴图能够实现很多难以置信的效果

## 如何实现/应用视差贴图？

**视差贴图的思想**：根据观察方向和高度贴图，通过修改纹理坐标使一个 fragment 的表面看起来比实际的更高或更低，可以参照下方示意图理解![](img/Pasted%20image%2020240110092640.png)

上图描述的是，我们以黄线方向进行观察，我们会看到 A 点的纹理，但如果我们应用高度贴图，图中红线为高度贴图中高度数值的可视化表达。也就是说，我们应该看到的是 B 点的纹理

*视差贴图的原理就是根据我们原本看到的点 A 信息结合高度贴图，计算得到 B 点的纹理贴图*

图中的方法是，从高度贴图中获取 A 点的高度值 H(A)，随后沿着黄线表示的视线方向，构建长度为 H(A)的向量 P，获取向量 P 在表面投影点位置在高度贴图中的数值（即实际上计算得到的是 P 点在平面上投影位置所对应的纹理，是 B 点的近似结果）

## 存在的问题

根据视差贴图的实现原理，可以发现

### 问题一：H(P)大概率不是 B 点真正的高度值

就和上图展示的一样，经过这种方法计算出来的 H(P)其实并不是 B 点真正的 H(B)，仅仅是一个近似

*当表面高度变化很快的时候，看起来就会很不真实，因为 P 点与 B 会相差很远*，如下图所示![](img/Pasted%20image%2020240110093303.png)

### 问题二：和法线贴图一样，需要考虑表面的具体朝向

这个问题很好解决，在法线贴图部分已经介绍了切线空间的概念，视差贴图也是同理，上述操作都定义在切线空间中，从而避免了在世界空间中表面朝向引起的复杂计算问题，切线空间固定了表面的朝向简化了计算。

- 切线空间的概念应该是直立空间的一种应用

### 问题三：当视线方向接近与表面平行时，显示上会出现问题

如图所示，当实现近乎于表面平行时，此时纹理表面的采样在比较陡峭的位置出现不自然的效果
![](img/Pasted%20image%2020240110152531.png)

## 视差贴图的应用

实际上，在具体应用中，位移贴图更多是如下图这种，深度图：![](img/Pasted%20image%2020240110094951.png)

*因为模拟深度比模拟高度更容易*

## 模拟深度的原理

参考下图加深理解：![](img/Pasted%20image%2020240110095037.png)

具体过程和之前描述的差不多，获得 A 点深度值 H(A)，对 viewDir 进行缩放，得到向量 P，*注意这里是深度值，此时向量 P 是和 viewDir 反向的*，所以经过偏移后的纹理坐标应该为：

$$texCoords(A) + (-P.xy*H(A)*heightScale)$$

- 一般会对向量 P 乘上一个系数 $heightScale$，用以控制偏差程度（本例中 0.1 左右较好）
- 有些时候会对 P 的坐标除以一个 $P.z$，目的是使得视线倾斜看向表面时能够产生更大的偏移营造深度感（**有偏移量限制的视差贴图 Parallax Mapping with Offset Limiting**）

## 视差贴图效果

应用深度贴图效果前后对比如图

| No Depth Mapping | Depth Mapping |
| ---- | ---- |
| ![](img/Pasted%20image%2020240110110112.png) | ![](img/Pasted%20image%2020240110110226.png) |

然而经过个人测验，目前这种方式，如果以倾斜角不太大的视角看向表面效果还算不错，但当视线方向接近平行于表面时效果就不尽人意了。

---
# 陡峭视差映射

## 什么是陡峭视差映射？

**陡峭视差映射 Steep Parallax Mapping**

是视差映射的一种拓展，不同的是，**陡峭视差映射**通过对深度贴图进行分层，多次采样从而提高了精确性

分层如图所示：![](img/Pasted%20image%2020240110145154.png)

具体思路是：
- 将总深度方向划分为同一个深度的多个层级
- 沿着 view 方向，遍历各个层可以得到 view 和不同层级之间的交点（图中紫色点）
- 同时根据该点对应的纹理坐标（图中蓝色点）查询对应的深度值
- *比较该层级深度与纹理中深度值*
	- 层级深度 < 纹理深度：继续迭代
	- 层级深度 > 纹理深度：*此时向量 P 位置*已经到达了表面之下，如图中 T 3 对应的紫色点和蓝色点

## 为什么需要陡峭视差映射？

在视差贴图中，当以一个接近表面平行方向的视角看过去时会在陡峭的位置出现不自然的效果，而**陡峭视差映射**则可以保证在这种陡峭的高度和比较极端的观察视角上仍然获得不错的凹凸效果，如下图：

| Layers=10 | Layers=50 |
| --- | --- |
| ![](img/Pasted%20image%2020240110152749.png) | ![](img/Pasted%20image%2020240110152807.png) |

可以看到当对深度方向分层比较少时（10 层），在深度方向上出现了明显的分层现象，这是因为采样的数量比较少，当我们提高分层数量时（50 层），深度方向的分层明显得到改善，并且呈现出的高低效果显然要比视差贴图更好

## 如何实现/应用陡峭视差映射？

原理同视差贴图，大概实现方式在“什么是陡峭视差映射”部分进行了阐述

这里提一下算法的一个优化：*在垂直看一个表面的时候纹理位移比以一定角度看时的小*

据此，可以根据视角方向与表面的法线夹角来动态计算需要对深度分层个数，利用到着色器中内置的 mix 函数

```c
float numLayers = mix(minLayers, maxLayers, abs(dot(vec3(0.0, 0.0, 0.1), viewDir)));
```

## 存在的问题

### 问题一：深度分层

如上边展示的不同分层数的对比图，这种方法是*基于有限的样本数量的*，当分层数越多，采样也就越多，效果也就越好，但相应的*性能开销也就越大*，图中实现比较简单所以帧率上相差无几

解决方法：**找到视线方向与凹凸表面的交点前后的两个深度层，进行插值计算**
- **浮雕视差映射 Relief Parallax Mapping**：更精确，但开销更大
- **视差遮挡映射 Parallax Occlusion Mapping**

# 视差遮蔽映射

## 什么是视差遮蔽映射？

**视差遮蔽映射 Parallax Occlusion Mapping**

原理和陡峭视差映射相同，但此时并不是使用触碰后的第一个深度层纹理坐标，而是*找到最接近触碰点的，触碰前和触碰后的深度层，根据深度层深度和对应纹理中映射的深度值的接近程度来对触碰点进行线性插值*

## 为什么需要视差遮蔽映射？

在陡峭视察映射中，我们对深度进行了分层采样，这种方法是基于*有限样本*的采样方法，当观察角度比较刁钻的时候就容易出现明显的分层问题，并且单纯增加分层层数还会增大性能开销

故**视差遮蔽映射**，提供了一种在给定有限的分层数条件下，仍然可以通过线性插值方法呈现出更好的凹凸质感。

## 如何实现/应用视差遮蔽映射？

可以继续参考下图：
![](img/Pasted%20image%2020240110145154.png)

即，此时寻找的是 T2 和 T3，根据各自深度映射的深度值（蓝色点）和深度层深度（紫色点）之间的深度之差，如果这个差越小，则表明蓝色点和紫色点越接近，根据这个差值大小来设置 T2 和 T3 在对触碰点线性插值时的权重大小

## 效果图对比

| 陡峭视差映射 | 视差遮蔽映射 |
| --- | --- |
| ![](img/Pasted%20image%2020240110162921.png) | ![](img/Pasted%20image%2020240110162940.png) |

对比上方两个图可以法线，分层同样为 10 层的情况下，开启视差遮蔽映射后，在同样的刁钻角度下分层现象得到了明显改善


