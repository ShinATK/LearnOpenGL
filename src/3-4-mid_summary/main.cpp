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
    // ��������
    Window myWindow;
    myWindow.window_Init(800, 600, "LearnOpenGL");

    // �������
    Camera camera;

    // ������ɫ��
    Shader shader(
        relativePath(FILE_FOLDER_PATH, "shader/3-3-vertex.glsl"), 
        relativePath(FILE_FOLDER_PATH, "shader/3-3-frag.glsl")
    );
    
    // ����ģ��
    Model myModel(
        relativePath(FILE_FOLDER_PATH, "model/nanosuit/nanosuit.obj")
    );

    // ������Ȳ���
    glEnable(GL_DEPTH_TEST);

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(myWindow.window))
    {
        // ������루ESC�����ڴ�С��
        myWindow.processInput();
        // ����ƶ�
        camera.processInput(myWindow.window);

        // ������ɫ����
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        // �����ɫ�������Ȼ���
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ������ɫ��
        shader.use();

        // ������ɫ��
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

        // ����ģ��
        myModel.Draw(shader);

        // swap buffers, poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(myWindow.window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
