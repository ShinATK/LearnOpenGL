#cg #opengl

**标准化设备坐标 (Normalized Device Coordinates, NDC)**

一旦你的顶点坐标已经在顶点着色器中处理过，它们就应该是**标准化设备坐标**了，标准化设备坐标是一个 x、y 和 z 值在 -1.0 到 1.0 的一小段空间。任何落在范围外的坐标都会被丢弃/裁剪，不会显示在你的屏幕上。下面你会看到我们定义的在标准化设备坐标中的三角形 (忽略 z 轴)：

![](img/Pasted%20image%2020231202213700.png)

与通常的屏幕坐标不同，y 轴正方向为向上，(0, 0) 坐标是这个图像的中心，而不是左上角。最终你希望所有 (变换过的) 坐标都在这个坐标空间中，否则它们就不可见了。


通过使用由 glViewport 函数提供的数据，进行<def>视口变换</def>(Viewport Transform)，<def>标准化设备坐标</def>(Normalized Device Coordinates) 会变换为<def>屏幕空间坐标</def>(Screen-space Coordinates)。所得的屏幕空间坐标又会被变换为片段输入到片段着色器中。