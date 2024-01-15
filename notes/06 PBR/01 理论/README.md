#cg #opengl 
- [理论 - LearnOpenGL CN](https://learnopengl-cn.github.io/07%20PBR/01%20Theory/)
---
# PBR
## 什么是 PBR？

**PBR Physically Based Rendering**：在不同程度上基于与现实世界的物理原理更相符的基本理论所构成的渲染技术集合

*判断一种 PBR 光照技术是否是基于物理*，可以根据下面三个条件判断：
- **基于微平面 Microfacet 的表面模型**
- **能量守恒**
- **应用基于物理的 BRDF**

## 为什么需要 PBR？

**目的**：为了更真实/更符合物理规律地*模拟光线*

所以也可以称为**基于物理的着色 Physically Based Shading**，而不是物理着色 Physical Shading

**优点**：
- *更准确*
	- 比 Phong 模型、Blinn-Phong 模型的光照算法从渲染上来说更真实
- *编写材质更方便*
	- 可以直接根据物理参数为依据编写表面材质
	- 而不需要粗略的修改与调整来让光照效果更正常
- *材质更真实*
	- 不需要依赖于光照条件
	- 材质就可以展现出比非 PBR 渲染管线更为真实的效果

**缺点**：
- 仍然是对物理世界的近似
- 计算开销大
- 理论繁多复杂

## 如何实现 PBR？

> 这里的 PBR 方案为：最先由迪士尼 (Disney)提出探讨并被 Epic Games 首先应用于实时渲染的 PBR 方案
> **基于金属质地的工作流 Metallic Workflow**

- 前置知识要求：帧缓冲、[4-6 高级立方体贴图](../../04%20高级OpenGL/4-6%20高级立方体贴图.md)、[Gamma矫正](../../05%20高级光照/5-2%20Gamma矫正/README.md)、[HDR](../../05%20高级光照/5-6%20HDR/README.md)、[法线贴图](../../05%20高级光照/5-4%20法线贴图/README.md)

三个条件：
- **微平面模型**
- **能量守恒**
- **应用基于物理的 BRDF**

实现 PBR，其实就是实现这三个条件

---
# 微平面模型

微平面模型是 PBR 技术的基础，该模型理论认为：
- 微观尺度的任何平面都可以用**微平面 Microfacets**这种细小的镜面来进行描绘
- 并且根据平面的粗糙度不同，这些细小的镜面的取向与排列可以不一致，示意图如下
![](img/Pasted%20image%2020240114161007.png)

- 粗糙的平面，微平面排列混乱
	- 使得镜面光/镜面反射时光线趋向于向不同方向*发散 Scatter*开来
	- 产生分布更广泛的镜面反射
- 光滑的平面，微平面排列接近整齐
	- 光线趋向于向同一个方向反射
	- 产生分布更小更锐利的反射
![](img/Pasted%20image%2020240114161817.png)

然而实际上，微观尺度下，没有绝对光滑的平面，所以这里假设**粗糙度 Roughness**参数，再结合统计学方法来估计这些已经具有原子性的微平面所导致的粗糙程度
- 意义是：粗糙度描述了这些微平面所导致的光线反射，反射到某个向量方向的光线所占全部反射光线的比例

这里可以结合**半程向量**来讲描述改为，*影响一个平面粗糙度的众多为平面中，微平面朝向方向沿着半程向量方向的微平面比例*
- 二者越一致，相应的粗糙度就越小，镜面反射的效果越强烈，同时也就越集中
![](img/Pasted%20image%2020240114162348.png)

---
# 能量守恒

**能量守恒 Energy Conservation**规定除了发光面以外，
- *出射光线的能量永远不超过入射光线的能量*

正如上图粗糙度所展现的效果，粗糙度越大，发生镜面反射的区域就越大，但反射的亮度会下降，因为反射光线的能量被分散到了更大的区域

*这里将漫反射光和镜面反射光进行明确区分*：一束光碰到一个表面时，它就分离为两个部分：
- **反射部分**：直接反射为不进入平面的部分光线（镜面光照）
- **折射部分**：剩下的进入平面并被吸收的光线（漫反射光照）
	- *折射部分并不是立刻被全部吸收的*
	- 光线就是一束没有耗尽就一直向前运动的能量，而碰撞才能减少光束的能量
![](img/Pasted%20image%2020240114162947.png)

参考上述示意图，折射部分还会有光线经过与平面内部碰撞后反而射出了表面，*这部分脱离表面的光线协同构成该表面的漫反射颜色*
- 金属 Metallic 表面和非金属（介电质Dielectrics）表面遵循同样的反射折射原理，但是
	- 对于金属，所有折射光将被吸收而不会散开，即*金属只显示镜面反射的颜色*
- 所以这两种材质在 PBR 渲染管线中区别处理

> PBR 中对该问题进行了简化：*认为平面上每一点的折射光都被完全吸收而不会散开*
> **次表面散射 Subsurface Scattering**的着色器技术则考虑了这个问题
> 提升了皮肤、大理石或蜡质这样材质的效果，也相应的降低了性能

但有一条基于能量守恒的经验法则是确定的：**反射光与折射光之间是互斥的**
- 反射光无法再被表面吸收
- 即反射光后剩下的能量就是折射光的能量（方便计算）

```cpp
float kS = calculateSpecularComponent(...); // 反射/镜面 部分
float kD = 1.0 - ks; // 折射/漫反射 部分
```

同时也保证了反射和折射所占比例不会超过 1

## 反射率方程

**渲染方程 Render Equation**：是模拟光的视觉效果最好的模型

而这里的 PBR 使用的是一种**反射率方程 The Reflectance Equation**，它是渲染方程的一种特化版本：

$$
L_{out}(p,\omega_{out}) = \int\limits_{\Omega} f_r(p,\omega_{in},\omega_{out})\cdot L_{in}(p,\omega_{in})\cdot (\vec{n}\cdot\vec{w_{in}})\mathrm{d}w_{in}
$$

- 前置知识：**辐射度量学 Radiance**，一种度量电磁场辐射的手段
- 这里只用到其中一种辐射度量（radiometric quantities），**辐射率 Radiance**

**辐射率 Radiance**：量化来自单一方向上的光线的大小或强度
- 一个辐射强度为 $\Phi$ 的光源，在单位面积 $A$，单位立体角 $\omega$ 上辐射出的总能量 

$$
L = \frac{\mathrm{d^2}\Phi}{\mathrm{d}A\mathrm{d}\omega\cos{\theta}}
$$

示意图：![](img/Pasted%20image%2020240114165514.png)
- *表示了一个区域平面上光线总量的物理量*
- 当面积 $A$ 和立体角 $\omega$ 无穷小时，一个区域平面就变为了一个点
	- *即反射率方程中的 p*

 但由于这里需要考虑*所有入射辐射率的总和*，故需要对以点 p 为中心的，半球 $\Omega$ 区域上进行积分
- 程序中利用**黎曼和 Riemann sum**，计算积分

```c
int steps = 100;
float sum = 0.0f;

vec3 P = ...;
vec3 Wo = ...;
vec3 N = ...;
loat dW = 1.0f / steps;

for(int i = 0; i < steps; ++i)
{
	vec3 Wi = getNextIncomingLightDir(i);
	sum += Fr(p, Wi, Wo) * L(p, Wi) * dot(N, Wi) * dW;
}
```

至此，我们可以重新对反射率方程进行解读：
- 在半球领域 $\Omega$ 内，在平面上 p 点发生碰撞的所有入射方向 $\omega_{in}$ 上的光线的辐射率 $L_{in}$，经过平面自身性质即 $f_r$ 的约束，最终在观察方向上反射出的光线的辐射率 $L_{out}$

此时，反射率方程中，只剩下和表面相关的 $f_r$ 仍然是未知的，但根据反射率方程可以初步推断，这个位置的方程表示的是：
- 从对象上说，它与表面自身的性质有关
- 从物理上说，它反应入射方向 $\omega_{in}$ 上的光线，经由表面，最终“转换”到出射方向 $\omega_{out}$ 上的光线的过程
- 而根据能量守恒出射光线能量永远不超过入射光线能量，所以它应当是某种概率分布函数，反应了
	- *入射光线以方向 $\omega_{in}$ 入射，能在方向 $\omega_{out}$ 上贡献多少出射光线*

---
# 双向反射分布函数

至此，我们已经实现了 PBR 中三个条件其中两个，剩下一个“应用基于物理的 BRDF”

BRDF，即**Bidirectional Reflective Distribution Function**，双向反射分布函数

- Blinn-Phong 同样采用入射和出射方向作为输入参数，故也被认为是一个 BRDF
- *但不满足能量守恒定律，因此不认为是一个 PBR* 

这里将要介绍的 BRDF 为*实时渲染管线使用的一种*被称为**Cook-Torrance BRDF 模型**

$$
f_r = k_d\cdot f_{lambert} + k_s \cdot f_{cook-torrance}
$$

- $k_d$ for refraction part ratio
- $k_s$ for reflection part ratio

## 漫反射部分

**Lambertian 漫反射**：$f_{lambert}$

$$
f_{lambert} = \frac{color}{\pi}
$$

> 与之前光照计算中的漫反射计算公式的联系：
> 旧计算：表面法向量点乘光照方向，再乘以表面颜色
> Lambertian 漫反射：点乘在 $f_r$ 外部，这里只剩下了表面颜色项
> 按照Epic公司给出的结论，*Lambertian漫反射模型已经足够应付大多数实时渲染的用途了*

## 镜面反射部分

$$
f_{cook-torrance} = \frac{D\cdot F\cdot G}{4(\omega_{out}\cdot n)(\omega_{in}\cdot n)}
$$

Cook-Torrance BRDF 的镜面反射部分重点在于分子上的三个函数，分母是一个标准化因子

三个函数分别为（这里采用 Epic Games 在 Unreal Engine 4 中所使用的函数）：
- D：**法线分布函数 Normal Distribution Function**
	- 描述表面粗糙度影响下，微平面朝向方向与半程向量一致的微平面数量
	- *D 使用 Trowbridge-Reitz GGX*
- G：**几何函数 Geometry Function**
	- 描述微平面自成阴影的属性（微平面之间相互遮挡而形成的阴影）
	- *F 使用 Fresnel-Schlick 近似 (Fresnel-Schlick Approximation)*
- F：**菲涅尔方程 Fresnel Rquation**
	- 描述不同表面角下表面所反射的光线所占的比例
	- *G 使用 Smith’s Schlick-GGX*

> 用来实现相应物理机制的每种函数都有不止一种形式。它们有的非常真实，有的则性能高效，可以按照自己需求来选择实现函数

### D：法线分布函数（NDF）

- *表面粗糙度影响下，微平面朝向方向与半程向量一致的微平面数量*

**Trowbridge-Reitz GGX**：

$$
NDF_{GGXTR}(n,h,\alpha) = \frac{\alpha^2}{\pi((n\cdot h)^2(\alpha^2-1)+1)^2}
$$

- $\alpha$ 为表面粗糙度

效果如下：
![](img/Pasted%20image%2020240114162348.png)

结合分布函数进行解读：
- 当粗糙度增大时，微平面的取向即法线方向会变得更加随机，从而使得法线方向与半程向量方向一致的微平面分布在更大的范围->集中性低，光照效果也就越昏暗

函数实现如下：
```c
float D_GGX_TR(vec3 N, vec3 H, float a)
{
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdorH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}
```

### G：几何分布函数

- *统计学上近似微平面之间相互遮挡的比率*

![](img/Pasted%20image%2020240114182523.png)

GGX 与 Schilick-Beckmann 近似的结合体，即 Schlick-GGX：

$$
G_{SchlickGGX}(n,v,k) = \frac{n\cdot v}{(n\cdot v)(1-k) + k}
$$

- 同样采用材料的*粗糙度*作为输入参数，粗糙度越高，相互遮蔽的概率越高
- $k$ 作为 $\alpha$ 的重映射（Remapping），计算方法取决于*针对直接光照*，还是*针对 IBL 光照*的几何函数

$$\begin{align*}
k_{direct} &= \frac{(\alpha + 1)^2}{8} \\
k_{IBL} &= \frac{\alpha^2}{2}
\end{align*}$$

对于几何分布函数，利用史密斯（Smith's method）同时考虑
- **观察方向（几何遮蔽 Geometry Obstruction）**$G_{sub}(n, v, k)$
- **光线方向（几何阴影 Geometry Shadowing）**$G_{sub}(n, l, k)$

$$
G(n,v,l,k) = G_{sub}(n, v, k) \cdot G_{sub}(n, l, k)
$$

将 Schlick-GGX 作为 $G_{sub}$ 带入后可得到下图效果：![](img/Pasted%20image%2020240114183502.png)


```c
float GeometrySchlickGGX(float NdotV, float k)
{
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	
	return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);
	
	return ggx1 * ggx2;
}
```

### F：菲涅尔方程

**Freh-nel**方程：
- *描述一个入射光碰到平面后，在某个观察角度方向上反射光线所占的比例*，会随着观察角度不同而发生变化

当观察角度与表面法线垂直时，此时表面反映出的是其**基础反射率 Base Reflectivity**，如下图所示

![](img/Pasted%20image%2020240114184306.png)

- 可以看到，当观察视角近乎与球面平行，即与法线垂直时，如图中球的边缘，这些位置的反光会变得明显起来

*利用 Fresnel-Schlick 近似法得到的菲涅尔方程的近似解*：

$$
F_{Schlick}(h,v,F_0) = F_0 + (1-F_0)(1-(h\cdot v))^5
$$

- 这里的 $F_0$ 为表面的基础反射率（由折射指数 Indices of Refraction 计算得出）

结合方程理解菲涅尔现象：
- 当观察方向垂直于表面法线，说明此时半程向量可以认为就是法线方向
- 上述近似解近乎为 1，从而观察到更加明亮的反光

**缺点**：
- 该近似只对电介质或者说非金属表面有定义
- 对于导体表面或者说金属表面，利用折射指数无法得到正确的结果

> 这些基础反射率等参数可以在这个网站里找到 [RefractiveIndex.INFO - Refractive index database](https://refractiveindex.info/)

可以注意到这里边的基础反射率*使用 RGB 三原色来表示*，因为金属表面的基础反射率一般是有颜色的
- 因此就有了*金属工作流*的概念，需要额外引入**金属度 Metalness**参数来编写材质用以区分材质为金属还是非金属
- 金属度一般在 0~1 之间线性调配，用以模拟较为复杂的金属表面

*为了解决这个缺点，这里使用的方式是*：
- *预先*计算出金属的基础反射率，即平面相对于法向入射的结果
- *然后*利用相应观察角带入 Fresnel-Schlick 近似进行插值
从而使金属和非金属材质能够使用同一个公式进行计算

```c
vec3 F0 = vec3(0.04);
F0 = mix(F0, surfaceColor.rgb, metalness);

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
```
- 这里直接使用颜色纹理作为基础反射率


## Cook-Torrance 反射率方程

至此可以写出 Cook-Torrance 反射率方程的最终形式：

$$
L_{out} = \int\limits_{\Omega}(k_d\frac{c}{\pi}+k_s\frac{DFG}{4(\omega_{out}\cdot n)(\omega_i\cdot n)}L_i(p,\omega_i)n\cdot w_i\mathrm{d}\omega_i)
$$

---
# 编写 PBR 材质

![](img/Pasted%20image%2020240115091303.png)

- **反照率 Albedo**：给每一个纹素指定表面颜色或基础反射率
	- 与漫反射纹理不同：反照率纹理中没有阴影或裂纹
- **法线**
- **金属度**：用于逐纹素指定该纹素是否为金属质地
- **粗糙度**：逐纹素指定粗糙度（光滑度，光滑+粗糙=1）
- **AO**：环境光遮蔽 Ambient Occlusion 贴图，指定额外的阴影因子，如砖块裂缝


