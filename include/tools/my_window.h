#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// 图像大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Window
{
public:
    GLFWwindow *window;
    unsigned int Width;
    unsigned int Height;
    Window()
    {
        // 初始化 GLFW
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
        {
            std::cout << "Failed to initialize glfw!" << std::endl;
            return ;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        std::cout << "Initialize GLFW done" << std::endl;
    }

    void window_Init(unsigned int width, unsigned int height, const char *window_name)
    {
        // 创建GLFW window
        Width = width;
        Height = height;
        window = glfwCreateWindow(width, height, window_name, NULL, NULL);
        // 检查 window 是否创建成功
        if(!window_CheckError())
        {
            return ;
        }
        // window 设置
        window_Settings();
    }

    void processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

private:
    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    int window_CheckError()
    {
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            return 0;
        }
        std::cout << "Create GLFW window done" << std::endl;
        return 1;
    }

    void window_Settings()
    {
        // glfw设置
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // 垂直同步 vsync
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return ;
        }
    }


};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#endif