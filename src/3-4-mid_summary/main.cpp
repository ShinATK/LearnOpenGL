#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <tools/my_window.h>
#include <tools/my_camera.h>

#include <tools/shader.h>
#include <tools/model.h>
#include <tools/pathSettings.h>

int main()
{
    // 创建窗口
    Window myWindow;
    myWindow.window_Init(800, 600, "LearnOpenGL");

    // 创建相机
    Camera camera;

    // 创建着色器
    Shader shader(
        relativePath(FILE_FOLDER_PATH, "shader/3-3-vertex.glsl"), 
        relativePath(FILE_FOLDER_PATH, "shader/3-3-frag.glsl")
    );
    
    // 导入模型
    Model myModel(
        relativePath(FILE_FOLDER_PATH, "model/nanosuit/nanosuit.obj")
    );

    // 启动深度测试
    glEnable(GL_DEPTH_TEST);

    // 渲染循环
    while (!glfwWindowShouldClose(myWindow.window))
    {
        // 监控输入（ESC、窗口大小）
        myWindow.processInput();
        // 相机移动
        camera.processInput(myWindow.window);

        // 背景颜色设置
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        // 清除颜色缓冲和深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 激活着色器
        shader.use();

        // 设置着色器
        shader.setVec3("viewPos", camera.Position);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)myWindow.Width / myWindow.Height, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.14f));
        shader.setMat4("model", model);

        float angle = glfwGetTime();
        shader.setVec3("pointLight.position", glm::vec3(2*sin(angle), 0.0f, 2*cos(angle)));
        shader.setVec3("pointLight.ambient", glm::vec3(0.1f));
        shader.setVec3("pointLight.diffuse", glm::vec3(0.8f));
        shader.setVec3("pointLight.specular", glm::vec3(1.0f));

        // 绘制模型
        myModel.Draw(shader);

        // swap buffers, poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(myWindow.window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
