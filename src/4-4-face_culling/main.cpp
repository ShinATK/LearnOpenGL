#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include <tools/stb_image.h>

#include <tools/my_camera.h>
#include <tools/my_window.h>

#include <tools/shader.h>

#include <tools/pathSettings.h>

using namespace std;

unsigned int loadTexture(const char *path);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Camera camera;
Window myWindow;

int main()
{
    myWindow.window_Init(800, 600, "4-4-face_culling");
    glfwSetInputMode(myWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(myWindow.window, mouse_callback);

    // shader for vertex and fragment
    Shader shader(
        relativePath(FILE_FOLDER_PATH, "shader/4-4-vertex.glsl"), 
        relativePath(FILE_FOLDER_PATH, "shader/4-4-frag.glsl"));

    // cube VAO
    // CCW order
    // float cubeVertices[] = {
    //     // Back face
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    //     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    //     // Front face
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    //     // Left face
    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    //     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    //     // Right face
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
    //     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
    //     // Bottom face
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    //     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    //     // Top face
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    //     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
    // };
    // CCW vertex order
    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // Right face
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
        // Bottom face          
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left              
    };

    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);     // vertex position
    glEnableVertexAttribArray(1);              
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // tex coords

    glBindVertexArray(0);

    unsigned int cubeTexture = loadTexture(relativePath(FILE_FOLDER_PATH, "texture/marble.jpg"));

    // activate shader
    shader.use();
    shader.setInt("texture1", 0);

    // activate depth-test
    glEnable(GL_DEPTH_TEST);
    // show things with a less z-value
    glDepthFunc(GL_LESS);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    // Culling which face: BACK
    glCullFace(GL_FRONT); // default: GL_BACK
    // Set front face vertex order: CW
    glFrontFace(GL_CW); // clock wise

    // Render Loop
    while (!glfwWindowShouldClose(myWindow.window))
    {
        
        myWindow.processInput();
        camera.processInput(myWindow.window);

        // Background Color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        // Clear color-buffer and depth-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // Draw cube
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // swap buffers, poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(myWindow.window);
        glfwPollEvents(); 
    }

    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &cubeVAO);

    glfwTerminate();
    return 0;
}

unsigned int loadTexture(const char *path)
{  
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format==GL_RGBA? GL_CLAMP_TO_EDGE:GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format==GL_RGBA? GL_CLAMP_TO_EDGE:GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.ProcessMouseMovement(xpos, ypos);
}