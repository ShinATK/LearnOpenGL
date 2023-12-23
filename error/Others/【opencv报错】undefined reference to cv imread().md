#error

# 代码

代码：

```cpp
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv; 

int main(){

    Mat image = cv::imread("D:/Programs/msys2/home/Shin/Code/Test/BackGround.png");
    imshow("image", image);
    waitKey(0);
    return 0;
}
```

# 报错

直接 g++ 编译，报错如下：

```shell
undefined reference to `cv::imread(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, int)'
undefined reference to `cv::imshow(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, cv::_InputArray const&)'
...
```

# 解决

建立 CMakeLists.txt 文件，写入以下内容：

```cmake
cmake_minimum_required(VERSION 3.10)
project(test_opencv)

find_package(OpenCV REQUIRED) # 直接搜索OpenCV目录即可

set(CMAKE_CXX_STANDARD 14)

add_executable(test_opencv test.cpp)

target_link_libraries(test_opencv ${OpenCV_LIBRARIES}) # 记得链接
```

```cmake
cmake_minimum_required(VERSION 3.10)
project(test)
set(CMAKE_CXX_STANDARD 14)
set(OpenCV_DIR "/ucrt64/include/") # 设置OpenCV目录位置
find_package(OpenCV REQUIRED)# 搜索OpenCV目录
include_directories(${OpenCV_INCLUDE_DIRS}) # 添加OpenCV头文件目录
link_directories(${OpenCV_LIBS}/ucrt64/libs/)
add_executable(test test.cpp)
target_link_libraries(test ${OpenCV_LIBS})
MESSAGE("OpenCV_LIBS:" ${OpenCV_LIBS}) #打印找到的opencv libs
```

利用 CMake 构建并运行 exe 成功


参考：
- [【报错】undefined reference to cv imread](https://jackilina.blog.csdn.net/article/details/126531276?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7EPayColumn-1-126531276-blog-126345418.235%5Ev39%5Epc_relevant_anti_vip&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7EPayColumn-1-126531276-blog-126345418.235%5Ev39%5Epc_relevant_anti_vip&utm_relevant_index=1)

