#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{    
    // 初始化 GLFW
    glfwInit();

    // 使用 glfwWindowHint 来配置 GLFW
    // 第一个参数：选项名称，以 GLFW_开头的枚举值中选择
    // 第二个参数：整型值
    // 下边是告诉 GLFW 这里使用的 OpenGL 版本是 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    // 告诉 GLFW 使用核心模式（Core-profile）
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 下面是 Mac OS X 系统中使上边配置起作用而额外需要的
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // 创建一个窗口对象
    // 参数：宽、高、窗口标题、略、略
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // TODO: GLAD
    // 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TODO: 视口
    // 设置 opengl 渲染窗口的尺寸大小，视口（viewport）
    // 参数：前两个控制窗口左下角位置，后两个为渲染窗口的宽高（像素），可以小于 GLFW 中的设置
    glViewport(0, 0, 800, 600);

    // TODO: 回调函数
    // 用户改变窗口大小
    // 对窗口注册一个回调函数（callback function）
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // TODO: 渲染循环 Render Loop
    // 每次循环开始前检查 GLFW 是否被要求退出，返回 true 则认为渲染结束
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        // 检测按键输入
        processInput(window);
        

        // 渲染指令
        // 在每个染色迭代开始时进行清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 清空屏幕所使用的颜色
        // GL_COLOR_BUFFER_BIT GL_DEPTH_BUFFER_BIT GL_STENCIL_BUFFER_BIT
        // 只对颜色 buffer 清空
        glClear(GL_COLOR_BUFFER_BIT);
        

        // 检查并调用事件，交换缓冲
        // 交换颜色缓冲
        glfwSwapBuffers(window);
        // 检查触发时间（键盘鼠标输入等），更新窗口状态
        // 并调用对应回调函数（可以手动设置回调方法）
        glfwPollEvents();

    }

    // TODO: 渲染循环结束后，正确释放/删除之前分配的全部资源
    glfwTerminate();

    return 0;
}

// 回调函数：窗口大小改变时就进行调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//  GLFW 的 glfwGetKey 函数
// 接收一个窗口和一个按键，检测这个按键是否正在被按下
void processInput(GLFWwindow* window)
{
    // 没有则返回 GLFW_RELEASE
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // 按下则将 WindowShouldClose 属性设置为 true
        glfwSetWindowShouldClose(window, true);
}

