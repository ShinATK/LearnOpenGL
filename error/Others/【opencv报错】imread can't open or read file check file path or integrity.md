#error

imread 无法读取相对路径下的图片

```cpp
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

int main(){
    Mat image = cv::imread("./BackGround.png");
    imshow("image", image);
    waitKey(0);
    return 0;
}
```

```shell
[ WARN:0@0.003] global loadsave.cpp:248 findDecoder imread_('/home/Shin/Code/Test/BackGround.png'): can't open/read file: check file path/integrity
OpenCV: terminate handler is called! The last OpenCV error is:
OpenCV(4.8.1) Error: Assertion failed (size.width>0 && size.height>0) in imshow, file C:/M/B/src/opencv-4.8.1/modules/highgui/src/window.cpp, line 971 
```

修改为图片的绝对路径后解决

```cpp
Mat image = cv::imread("D:/Programs/msys2/home/Shin/Code/Test/BackGround.png");
```



