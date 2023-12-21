下载地址：https://github.com/assimp/assimp

# 配置 Assimp

- 解压
- cmake，选择vs17 2020，configure+generate到build文件夹
- 打开Assimp.sln
- 选择Assimp项目，右键->生成解决方案
- 获得文件：assimp-vc140-mtd.lib、assimp-vc140-mtd.dll 、config.h文件
- config.h 文件在assimp-master\build\include\assimp 目录下，放到assimp-master\include\assimp 目录下
否则项目中使用assimp时会提示找不到 assimp\config.h 文件
- **配置vs2020**：
  a. 项目属性 ----> C/C++ —> 附加包含目录 —> your_path\assimp-master\include
  b. 项目属性 ----> 链接器 —> 常规 —> 附加库目录 —> your_path\lib
  c. 项目属性 ----> 链接器 —> 输入 —> 附加依赖项 —> assimp-vc140-mtd.lib

# 报错

**问题**：fatal error LNK1104: 无法打开文件“..\lib\Debug\assimp-vc143-mtd.lib”

- 右键Assimp，选择 属性 -> C/C++ -> 常规 -> “将警告视为错误”修改为：“否（/WX-）”
- 重新生成解决方案
- 成功
- 参考：https://blog.csdn.net/u012278016/article/details/105788231




