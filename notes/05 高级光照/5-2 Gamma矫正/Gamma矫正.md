#cg 

---


- gamma 值是什么？
	- 对颜色做出符合人类视觉感知的非线性幂指数映射
- 怎么做？
	- 对颜色进行指数运算，幂指数一般是 2.2
- 为什么？
	- 由于人眼对于黑色的感知更敏感
	- 在有限的计算机的颜色中将亮色与暗色均匀分布
		- 亮色精度过剩
		- 暗色精度不足
	- 为了获得符合人眼特性的可辨识精度而对图像的一种矫正方法
- Gamma 值取 2.2 原因在于最初的 CRT 显示器（阴极射线管显示器）
	- 阴极射线管的输入输出关系为 2.2 次方的幂指数关系

存在硬盘上的图像都是矫正过的（即，图像已经经过一次 1/2.2 幂指数的 gamma encoding，该数值对应 sRGB 颜色空间），当打开该图像显示在屏幕上时，屏幕显示过程中会对图像再进行一次 2.2 幂指数的 gamma 处理
- 由于存储时已经提前将图像修正过
- 因此再次显示时就是正确的颜色

游戏中一般是 DXT 压缩格式，精度为 6 bit，64 阶，会损失很多细节

可以通过 gamma 矫正来对这种被硬件的非线性空间处理过的图像修正回线性空间

*对于计算机图形学, 注重于对真实世界的模拟，其中的算法都处于线性空间，而读入图像确实矫正过的，所以对于这个问题一定要重视！*
- Gamma Corrected Pipeline




参考链接：
- [色彩校正中的 gamma 值是什么？ - Avatar Ye 的回答 - 知乎](https://www.zhihu.com/question/27467127/answer/37602200)
- [伽马校正笔记（Gamma Correction）\_利用gamma矫正曝光-CSDN博客](https://blog.csdn.net/vivo01/article/details/127968779)
- [【图形学】我理解的伽马校正（Gamma Correction）-CSDN博客](https://blog.csdn.net/candycat1992/article/details/46228771)