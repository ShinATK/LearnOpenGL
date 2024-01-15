#cg #opengl 
- [HDR - LearnOpenGL CN](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/06%20HDR/)
---
# HDR

## 什么是 HDR？

**HDR High Dynamic Range 高动态范围**：像显示器被限制为只能显示 0~1 间的颜色，而光照方程中却没有这个限制，这使得我们可以得到*一个更大的颜色范围*

## 为什么需要 HDR？

假设一个特定区域，该区域内有多个亮光源，使得这些数值综合超过了 1.0，效果如下图：![](img/Pasted%20image%2020240110172406.png)

图中由于大量片段亮度都超过了 1.0，但是最终都会被限制在 1.0，从而导致场景无法分辨

*大量片段的颜色值都非常接近 1.0，在一个很大的区域内每一个亮的片段都有相同的白色，这损失了很多细节*

HDR 技术最开始被运用在摄影上：
- 摄影师对**同一个场景采取不同曝光拍摄多张照片**，捕捉大范围的色彩值
- 再将这些图片**合称为 HDR 图片**，综合了不同曝光等级使得大范围的细节可见
![](img/Pasted%20image%2020240110172609.png)

PS：HDR 渲染和人眼工作的原理非常相似，如人眼可以在弱光线环境下自动调整以适应过暗和过亮部分

## 如何实现 HDR？

**HDR 渲染**：
- 允许使用更大范围的颜色值渲染从而获取大范围的黑暗和明亮场景细节
- 将所有 HDR 值转换成在 0~1 范围的**LDR Low Dynamic Range 低动态范围**，这个转换过程叫做**色调映射 Tone Mapping**

现在有很多种**色调映射**算法，但都致力于**在转换过程中保留尽可能多的 HDR 细节**，一般会包含一个*选择性倾向黑暗或明亮区域的参数*

---
## OpenGL 实现 HDR：浮点帧缓冲

回想之前的帧缓冲，我们将场景渲染好后写入一个纹理，并将纹理添加到帧缓冲中

```cpp
glBindTexture(GL_TEXTURE_2D, colorBuffer);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
```

`glTexImage2D()` 参考链接：
- [OpenGL API 之 glTexImage2D\_opengl glteximage2d-CSDN博客](https://blog.csdn.net/csxiaoshui/article/details/27543615)
- [glTexImage2D.xhtml](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml)

注意上述代码中，第一个 `GL_RGB`，第二个 `GL_RGB`，以及 `GL_UNSIGNED_BYTE`，这些是我们之前设置帧缓冲时传入的实参

当帧缓冲使用了标准化的定点格式如 `GL_RGB`，OpenGL 会自动将这些值存入帧缓冲前约束在 0~1 之间

**浮点帧缓冲**：就是将帧缓冲中的颜色缓冲内部格式设定成 `GL_RGB16F` `GL_RGBA16F` `GL_RGB32F` `GL_RGBA32F` 时的帧缓冲，代码如下：

```cpp
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
```

**注意 `GL_FLOAT` 参数**
- 此时一个颜色分量为 32 位，占用的内存空间是默认情况下的四倍

最后将浮点帧缓冲中的内容输出到一个**铺屏四边形 Screen-filling Quad**上即可


