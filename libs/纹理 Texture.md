#cg #opengl 

---
**Why 纹理**：可以通过给每个顶点添加颜色来增加图形细节，但如果想要图形更真实，对顶点的数量和对应的颜色要求会很高，导致开销很大。解决方式：使用一个 2 D 图片，即**纹理 texture**来添加物体细节，从而避免了额外顶点

**How 如何引用纹理**，即如何将纹理**映射 map**到三角形上：指定三角形的每个顶点各自对应的纹理部分，即**顶点坐标**和**纹理坐标 texture coordinate**进行关联，从而指明该顶点应该从纹理图像的哪个部分开始采样（采集片段颜色），之后对图形的其他非定点部分进行对应的**片段插值 fragment interpolation**从而可以获得整个图形的纹理

## 纹理坐标

**采样 sampling**：使用纹理坐标获取纹理颜色

**纹理坐标**：x 和 y 轴的 0~1 之间（2 D 纹理图像）

*纹理坐标的字母表示为：s、t、r*，和 x、y、z 是等价的

![](img/Pasted%20image%2020231205104318.png)

纹理坐标代码如下：
```cpp
float texCoords[]={
	0.0f, 0.0f, // 左下角
	1.0f, 0.0f, // 右下角
	0.5f, 1.0f // 上顶点
}
```

至此，顶点坐标和纹理坐标的对应关系就确认好了。但是在图形顶点之外的地方还需要进行插值计算对应的纹理坐标，所以，我们还需要明确用以计算插值纹理坐标的**插值方法**，也即**OpenGL 如何对纹理进行采样**。

## 纹理环绕方式（Warpping）

目的：解决纹理坐标设置在 0~1 范围之外时的映射问题。
OpenGL 默认设置为：*重复这个纹理图像*（即*忽略浮点纹理坐标的整数部分*）

具体的环绕方式：

| 环绕方式             | 描述                                                         |
| -------------------- | ------------------------------------------------------------ |
| `GL_REPEAT`          | 默认行为，重复纹理图像                                       |
| `GL_MIRRORED_REPEAT` | 同上，但重复时图像为相邻轴的镜像                             |
| `GL_CLAMP_TO_EDGE`   | 约束纹理坐标在 0~1，超出部分则重复边缘的纹理（边缘拉伸效果） |
| `GL_CLAMP_TO_BORDER` | 超出部分为自定义的边缘颜色                                                             |

如图所示：

![](img/Pasted%20image%2020231205105125.png)

*设置环绕方式的函数*：`glTexParameter*`，不同后缀对应不同的设置方式
```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
```

- 参数 1：指定纹理目标为 2 D 纹理
- 参数 2：指定设置的选项为 WRAP 应用的纹理轴为 S 轴
- 参数 3：指定环绕方式为镜像重复

## 纹理过滤（Texture Filtering）

纹理坐标可以是任意浮点值，不依赖于分辨率（Resolution），所以 OpenGL 需要知道*怎样将纹理像素（Texture Pixel，Texel）映射到纹理坐标*。

两个问题：
- 物体很大，但纹理分辨率很低
- ...

**纹理过滤 Texture Filtering**：目前只讨论两种
- `GL_NEAREST`
- `GL_LINEAR`

### 临近过滤 Nearest Neighbor Filtering

OpenGL 的默认纹理过滤方式，即*选择像素中心点最接近纹理坐标的像素*

![](img/Pasted%20image%2020231205105948.png)

### 线性过滤 (Bi) Linear Filtering

基于纹理坐标附近的纹理像素计算一个插值，从而近似这些纹理像素之间的颜色。纹理像素中心到这个纹理坐标的距离反应了该像素对这个纹理坐标点的贡献。

![](img/Pasted%20image%2020231205110032.png)

### 两种过滤的效果

示例：物体很大，但纹理分辨率低

![](img/Pasted%20image%2020231205110148.png)

### 放大（Magnify）和缩小（Minify）操作

- 纹理被缩小，使用临近过滤
- 纹理被放大，使用线性过滤

利用 `glTexParameter*` 指定放大和缩小的过滤方式。具体的代码和纹理环绕部分类似：

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

### 多级渐远纹理（纹理缩小问题）

**目的**：解决远处物体的纹理映射问题，即**纹理缩小 Minify**问题
**描述**：一张图像上，远处的物体所占据的部分很小或者说显示该物体而使用的像素个数会很少，如果此时物体的纹理分辨率很高，就导致需要从大量的纹理数据中选出很少的几个数据来表示整体，从而导致*走样 aliasing*
**解决**：使用**多级渐远纹理 Mipmap**
**方式**：Mipmap 中存储同一个物体的一系列的纹理图像，区别在于后一个纹理图像大小是前一个的一半。每次映射纹理时，都会计算该物体到 viewer 的距离对其映射到纹理坐标后发生的变形程度的影响，去对应的纹理级别（Level）选取被合适缩放后的纹理数据，从而使纹理图像分辨率和远处物体的显示像素个数更匹配。

![Mipmap](img/Pasted%20image%2020231205111021.png)

OpenGL 中函数：`glGenerateMipmaps`

**问题**：由于不同层级之间互为固定的倍数关系，这就导致如果计算得到的层级在两层之间，但仍选取现有的固定层级的纹理，最后的图像会在应用了不同的渐远纹理层之间出现生硬的边界。
**解决**：参考前边的**纹理过滤**，对层级之间应用**临近过滤**或**线性过滤**

Mipmap 中的几种过滤方式：

| Mipmap 过滤方式             | 描述                                         |
| --------------------------- | -------------------------------------------- |
| `GL_NEAREST_MIPMAP_NEAREST` | 临近纹理（后一个后缀）来匹配像素，并用临近插值（前一个后缀）进行纹理采样 |
| `GL_LINEAR_MIPMAP_NEAREST`  | 临近匹配，线性插值采样                   |
| `GL_NEAREST_MIPMAP_LINEAR`  | 线性插值匹配，临近采样               |
| `GL_LINEAR_MIPMAP_LINEAR`   |        线性插值匹配，线性插值采样                                      |

同样使用 `glTexParameteri` 设置过滤方式：

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
```

- 注：纹理放大不会使用 Mipmap，如果强行设置会产生 `GL_INVALID_ENUM` 错误代码

## 加载和创建纹理

**目的**：将纹理加载到应用中
**问题**：纹理图像的存储格式多种多样（不同的数据结构和排列）
**解决方案一**：确定需要的文件格式如：`.png`，针对该格式编写**图像加载器**，将图像转化为字节序列，*但问题是需要解决多种格式支持问题*
**解决方案二**：使用支持多种流行格式的图像加载库，`stb_image.h`，下载链接：[stb\_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)

使用到的 [木箱图片]([container.jpg (512×512) (learnopengl-cn.github.io)](https://learnopengl-cn.github.io/img/01/06/container.jpg))

使用方式：

```cpp
// 修改头文件，使其只包含相关的函数定义源码，等价于hpp文件变为了cpp文件
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* 创建纹理ID */
unsigned int texture; // 纹理的ID引用
glGenTextures(1, &texture); // (生成纹理的数量，存储纹理)

/* 绑定纹理 */
glBindTexture(GL_TEXTURE_2D, texture);
/* 设置纹理环绕、过滤方式 */
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

/* 加载图片 */
int width, height, nrChannels
unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

/* 生成纹理 */
// 生成纹理：glTexImage2D(纹理目标，mipmap的级别，存储为RGB值，宽度，高度，0，源图的格式，源图的数据类型，图像数据)
if (data)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //给当前绑定的纹理对象附加纹理图像
	glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
	std::cout << "Failed to load texture" << std::endl;
}

/* 释放图像内存 */
stbi_image_free(data);
```

## 应用纹理

我们在绑定纹理 ID 的时候指定了采样方式，但还需要给顶点属性中增加纹理坐标属性，如下：

```cpp
float vertices[] = {
// ---- 位置 ----     ---- 颜色 ----      - 纹理坐标 - 
0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 右上 
0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 右下 
-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 左下 
-0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f // 左上 
};
```

可以看到顶点属性由三部分组成：**位置、颜色、和纹理坐标**

则此时的定点格式如图：

![](img/Pasted%20image%2020231205141246.png)

此时需要注意：各自属性需要调整的步长和偏差值，这里只给出材质属性指针的设置：

```cpp
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FLASE, 8*sizeof(float), (void*)(6*sizeof(float)));
glEnableVertexAttribArray(2);
```

*同时需要注意，修改顶点着色器中的顶点属性，从而才能将材质属性传递给顶点*

- **顶点着色器**
```cpp
#version 450 core
layout (location=0) in vec3 verPos;
layout (location=1) in vec3 verColor;
layout (location=2) in vec2 verTexCoord;

out vec3 VerColor;
out vec2 TexCoord;

void main()
{
	gl_Postion = vec4(verPos, 1.0f);
	VerColor = verColor;
	TexCoord = verTexCoord;
}
```

- **片段着色器**
```cpp
#version 450 core
out vec4 FragColor;

in vec3 VerColor;
in vec2 TexCoord;

// 采样器 sampler
uniform sampler2D uniTexture;

void main(){
	// 内置的texture函数，使用之前设置的纹理参数对相应位置的颜色值进行采样
	// 输出即为纹理的（插值）纹理坐标上的（过滤后的）颜色
	FragColor = texture(uniTexture, TexCoord);
	// FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}
```

最后在调用 `glDrawElements` 前绑定对应纹理将纹理赋值给片段着色器的采样器，剩下代码为：

```cpp
glBindTexture(GL_TEXTURE_2D, texture);
glBindVertexArray(VAO);//绑定后自动将纹理赋值给片段着色器的采样器
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

## 纹理单元

**疑问**：变量 `uniTexture` 为 uniform 类型，为什么不用 `glUniform` 进行赋值
**解答**：如果要在一个片段着色器中设置多个纹理，可以使用 `glUniform1i` 给纹理采样其分配位置值。这个位置值被称为一个**纹理单元 Texture Unit**。一个纹理默认纹理单元为 0，且默认激活。
**目的**：纹理单元允许我们在着色器中使用多于一个的纹理，只要激活对应的纹理单元。对应的函数为 `glActiveTexture`

```cpp
glActiveTexture(GL_TEXTURE0); // 绑定纹理之前先激活
glBindTexture(GL_TEXTURE_2D, texture); // 纹理texture被绑定为纹理单元0
```

> OpenGL 中至少保证有 16 个纹理单元可以使用（0~15），可以通过 `GL_TEXTURE0 + 8` 来获得 `GL_TEXTURE8`，这种方式在建立循环时很有用

*如果要使用多于一个的纹理，同样也要在片段着色器中，设置多个相对应的 uniform 类型的 texture 变量*

如下：

```cpp
#version 450 core 
... 

uniform sampler2D texture1; 
uniform sampler2D texture2;

void main() { 
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); // 输出两个texture的线性插值，后一个占比为0.2
}
```


- [04-hello-texture 练习题](../src/04-hello-texture/04-hello-texture%20练习题.md)
