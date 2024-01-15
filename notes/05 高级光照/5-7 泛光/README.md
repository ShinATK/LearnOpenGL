#cg #opengl 
- [泛光 - LearnOpenGL CN](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/07%20Bloom/)
---
# 泛光
## 什么是泛光？

**泛光 Bloom**：使明亮的物体/区域向四周发出光线，从而产生一种这个物体/区域是发光光源的感觉

如图所示，可以很容易从右图得到“那些地方是发光区域”的信息，而左图在同样位置，会让人怀疑到底是发光还是这个物体本身就这个颜色
![](img/Pasted%20image%2020240111112356.png)

## 为什么需要泛光？

因为显示器的亮度范围有限，所以很难展现出明亮的光源或区域。**泛光**是一种表现发光光源的手段

而且**泛光 Bloom**和**HDR**结合使用效果会更好
- 二者是不同的技术
- HDR 扩大了发光光源可以表示的亮度范围
- Bloom 则为这些发光光源附近加上光晕，加强这种发光的事实信息

## 如何实现/应用泛光？

一个简单的步骤：
- 按照 HDR 步骤对场景进行渲染
- 从这个 HDR 颜色缓冲纹理中提取*亮度超过一定阈值的区域*，设置为新的纹理，然后为这些区域加上光晕效果（模糊化）
- 最后让这个模糊后的纹理叠加到 HDR 纹理上
![](img/Pasted%20image%2020240111112956.png)

### 步骤一：提取亮色

按照正常步骤，首先要渲染一次场景存入 HDR 颜色缓冲

![](img/Pasted%20image%2020240111155522.png)

然后再渲染一次场景这次只把超过一定阈值的场景片段存入另一个颜色缓冲中

*但有一个技巧，可以实现一个像素着色器提供多个输出，从而使得一次就可以获得两个片段颜色输出*
- 这个技巧较多**MRT Multiple Render Targets，多渲染目标**
- 可以在一个着色器中实现多个输出，如下
```c
layout (location=0) out FragColor;
layout (location=1) out BrightColor;
```
- 和顶点着色器的输入语法类似

所以，可以通过 `FragColor` 记录场景片段颜色，通过 `BrightColor` 提取超过一定阈值的场景片段颜色

```c
#version 450 core
layout (location=0) out FragColor;
layout (location=1) out BrightColor;
/* 这里设置一些列变量 */
void main()
{
	/* 这里进行光照计算 */
	// 最终的片段颜色
	FragColor  = vec4(result, 1.0f);
	// 提取超阈值部分
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0)
		BrightColor = vec4(FragColor.rgb, 1.0);
}
```
- 注意这里的提取超过阈值部分的代码
- 通过 HDR 我们可以将颜色值设置在比 0~1 更大的范围，从而使得我们*对于图像中明亮区域有着更好的控制权*
- 没有 HDR 会导致不同区域之间的亮度差别不大，影响亮部提取的有效性


### 步骤二：多个颜色缓冲

**注意**：使用多个像素着色器输出的必要条件
- *有多个颜色缓冲附加到了当前绑定的帧缓冲对象上*

将颜色缓冲绑定到帧缓冲对象：
```cpp
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
```
- 注意参数中的 `GL_COLOR_ATTACHMENT0`
- 再使用一个 `GL_COLOR_ATTACHMENT1` 就可以绑定第二个颜色缓冲到帧缓冲对象上

这里我们有两个像素着色器输出，也就需要添加两个颜色缓冲到帧缓冲对象上：
```cpp
// floating point framebuffer to render scene to
unsigned int hdrFBO;
glGenFramebuffers(1, &hdrFBO);
glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
// two color buffers
unsigned int colorBuffers[2];
glGenTextures(2, colorBuffers);
for(GLuint i=0;i<2;++i)
{
	glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
// Attach texture to Framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
}
```

OpenGL 只会渲染到帧缓冲中的第一个颜色附件，并忽略其他附件，所以需要*显式设置 OpenGL 用 `glDrawBuffers()` 渲染到多个颜色缓冲*（通过传递多个颜色附件的枚举实现）：
```cpp
unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
glDrawBuffers(2, attachments);
```

### 步骤三：模糊过滤器

在帧缓冲部分提到过可以设置一系列核来影响最后呈现的效果，但这里使用一个更高级更好的模糊过滤器：**高斯模糊 Gaussian Blur**

![](img/Pasted%20image%2020240111144459.png)

利用高斯模糊，可以对 fragment 的周围，例如 32 x 32 大小的区域进行采样，不同位置的权重根据区域到 fragment 的距离按照高斯曲线变化

一个简化采样计算的方法：
- 将二维上的方程分成水平和垂直方向
- 交替在这两个方向上进行模糊
示意图如下：![](img/Pasted%20image%2020240111144856.png)
这种方法叫做**两步高斯模糊**
- 即对一个二维图片进行模糊最少要两步

这里将利用这种横纵交替模糊的方法对帧缓冲中的预渲染缓冲进行模糊：
- 建立专门渲染模糊效果的帧缓冲
- 交替进行横纵方向上的模糊

由于需要处理的只是*场景中发光区域*，所以这里使用的是步骤二中的第二个颜色缓冲，即 `colorBuffers[1]
`
接下来在模糊着色器中设置模糊操作：
- 对于模糊顶点着色器：实现接收 Position 和 TexCoords，并传递 TexCoords 到片段着色器即可
- 对于像素着色器：
```c
#version 450 core
out vec4 FragColor;
in vec2 TexCoords;

// 绑定在当前模糊帧缓冲上的纹理（第一次调用着色器是colorBuffers[1]）
uniform sampler2D image;

// horizontal交替取真假
uniform bool horizontal;
// 高斯模糊 5个离散值
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main() {
	vec2 tex_offset = 1.0 / textureSize(image, 0);
	// gets size of single texel
	vec3 result = texture(image, TexCoords).rgb * weight[0];
	if(horizontal) {
		for(int i = 1; i < 5; ++i)
		{
			result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
	} 
	else
	{
		for(int i = 1; i < 5; ++i)
		{
			result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}
	}
	FragColor = vec4(result, 1.0);
}
```

### 步骤四：模糊处理的帧缓冲

接下来设置建立帧缓冲用以存放 blur 效果：

注意这里使用了两个帧缓冲，假设分别为 A、B，首先将之前提取的亮色部分放入在 A 中进行一次模糊操作，随后将帧缓冲 A 中的颜色缓冲放入帧缓冲 B 中，得到第二次模糊操作结果，即帧缓冲 B 的颜色缓冲中；再次将帧缓冲 B 中的颜色缓冲放入 A 重复上述操作

- 之前提取的亮色部分：`colorBuffers[1]`，就是步骤二中添加到帧缓冲中的第二个颜色缓冲，作为第一次的 Texture 参与帧缓冲 A 的渲染
- 随后帧缓冲 A 和 B 的颜色缓冲分别作为对方的 Texture 参与到下一次模糊

所以综上，需要创建：
- 两个帧缓冲 `unsigned int pingpongFBO[2];`
- 每个帧缓冲中的颜色缓冲：`unsigned int pingpongColorbuffers[2];`
```cpp
// framebuffer for blurring
unsigned int pingpongFBO[2];
unsigned int pingpongColorbuffers[2];
glGenFramebuffers(2, pingpongFBO);
glGenTextures(2, pingpongColorbuffers);
for(unsigned int i = 0;i<2;i++)
{
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
}
glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

在渲染循环中，需要交替绑定两个帧缓冲以及*正确的*颜色缓冲
- 注意第一次循环需要绑定 `colorBuffers[1]`
- 并且绑定 A 时要用 B 的颜色缓冲，即 `pingpongFBO[horizontal]` 对应 `pingpongColorbuffers[!horizontal]`

```cpp
bool horizontal = true, first_iteration = true;
unsigned int amount = 10;
blurShader.use();
for (unsigned int i = 0; i < amount; i++)
{
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
	blurShader.setInt("horizontal", horizontal);
	glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
	RenderQuad();
	horizontal = !horizontal;
	if (first_iteration)
		first_iteration = false;
}
glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

此时我们可以绘制处对发亮区域的模糊效果：
![](img/Pasted%20image%2020240111155616.png)

### 步骤五：混合纹理

此时我们已经得到了场景的 HDR 纹理以及经过模糊处理的亮区纹理，接下来就是利用着色器将这两个纹理进行混合

- 对于顶点着色器，同样只需要 Position 和 TexCoords
- 对于片段着色器，则开始进行这两个纹理的混合
```c
#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
	if(bloom)
		hdrColor += bloomColor;
	vec3 result = vec3(1.0)-exp(-hdrColor * exposure);
	result = pow(result, vec3(1.0/gamma));
	FragColor = vec4(result, 1.0);
}
```

最后渲染结果：![](img/Pasted%20image%2020240111160151.png)


