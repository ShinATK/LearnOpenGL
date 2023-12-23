
- texture加载问题：`Texture failed to load at path: @`
  - 前一天编译运行没有出现纹理的加载问题
  - 第二天没有进行改动的情况下重新进行编译运行，多次出现`Texture failed to load at path: @`情况，地面的纹理没有正常加载
  - **暂时解决**：出错就重新编译运行，直到正常加载
    - 因为之前莫名其妙遇到过着色器里`gl_Position`无法访问的问题，以及`error C5145: must write to gl_Position`
    - 当时又重新编译运行了几次
    - **目前还不清楚原因，多次编译运行时正常加载和加载失败的情况都有出现**