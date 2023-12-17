
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../include/shader.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //自定义Shader
    const char* vertexPath = "D:\\Programs\\msys2\\home\\Shin\\Obsidian\\Graphics\\LearnOpenGL\\src\\self-examples\\shader\\vs.shader";
    const char* fragmentPath = "D:\\Programs\\msys2\\home\\Shin\\Obsidian\\Graphics\\LearnOpenGL\\src\\self-examples\\shader\\fs.shader";
    Shader ourShader(vertexPath, fragmentPath);

    unsigned int VAO, VBO;

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float colorSin = sin(timeValue) / 2.0f + 0.5f;
        float colorCos = sin(timeValue) / 2.0f + 0.5f;
        // 设置顶点坐标、属性
        float triangle[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
            0.0f,  colorSin, 0.0f,  0.0f, 0.0f, 1.0f   // top 
        };
        // float triangle[] = {
        //     // positions       
        //      0.5f, -0.5f, 0.0f,  
        //     -0.5f, -0.5f, 0.0f,  
        //      0.0f,  0.5f, 0.0f
        // };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // 绑定第一个三角形
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        ourShader.use();
        // 设置uniform颜色变化
        // float timeValue = glfwGetTime();
        // float redValue = sin(timeValue) / 2.0f + 0.5f;
        // float greenValue = cos(timeValue) / 2.0f + 0.5f;
        // float blueValue = sin(timeValue) / 2.0f + 0.5f;
        // ourShader.setFloat4("vertexColor", redValue, greenValue, blueValue);
        // 绘制三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    // 删除着色器程序、释放 buffer 和 array
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
