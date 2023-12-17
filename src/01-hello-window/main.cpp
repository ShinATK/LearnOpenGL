#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{    
    // ��ʼ�� GLFW
    glfwInit();

    // ʹ�� glfwWindowHint ������ GLFW
    // ��һ��������ѡ�����ƣ��� GLFW_��ͷ��ö��ֵ��ѡ��
    // �ڶ�������������ֵ
    // �±��Ǹ��� GLFW ����ʹ�õ� OpenGL �汾�� 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    // ���� GLFW ʹ�ú���ģʽ��Core-profile��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ������ Mac OS X ϵͳ��ʹ�ϱ����������ö�������Ҫ��
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // ����һ�����ڶ���
    // ���������ߡ����ڱ��⡢�ԡ���
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // TODO: GLAD
    // ��ʼ�� GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TODO: �ӿ�
    // ���� opengl ��Ⱦ���ڵĳߴ��С���ӿڣ�viewport��
    // ������ǰ�������ƴ������½�λ�ã�������Ϊ��Ⱦ���ڵĿ�ߣ����أ�������С�� GLFW �е�����
    glViewport(0, 0, 800, 600);

    // TODO: �ص�����
    // �û��ı䴰�ڴ�С
    // �Դ���ע��һ���ص�������callback function��
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // TODO: ��Ⱦѭ�� Render Loop
    // ÿ��ѭ����ʼǰ��� GLFW �Ƿ�Ҫ���˳������� true ����Ϊ��Ⱦ����
    while (!glfwWindowShouldClose(window))
    {
        // ����
        // ��ⰴ������
        processInput(window);
        

        // ��Ⱦָ��
        // ��ÿ��Ⱦɫ������ʼʱ��������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // �����Ļ��ʹ�õ���ɫ
        // GL_COLOR_BUFFER_BIT GL_DEPTH_BUFFER_BIT GL_STENCIL_BUFFER_BIT
        // ֻ����ɫ buffer ���
        glClear(GL_COLOR_BUFFER_BIT);
        

        // ��鲢�����¼�����������
        // ������ɫ����
        glfwSwapBuffers(window);
        // ��鴥��ʱ�䣨�����������ȣ������´���״̬
        // �����ö�Ӧ�ص������������ֶ����ûص�������
        glfwPollEvents();

    }

    // TODO: ��Ⱦѭ����������ȷ�ͷ�/ɾ��֮ǰ�����ȫ����Դ
    glfwTerminate();

    return 0;
}

// �ص����������ڴ�С�ı�ʱ�ͽ��е���
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//  GLFW �� glfwGetKey ����
// ����һ�����ں�һ�������������������Ƿ����ڱ�����
void processInput(GLFWwindow* window)
{
    // û���򷵻� GLFW_RELEASE
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // ������ WindowShouldClose ��������Ϊ true
        glfwSetWindowShouldClose(window, true);
}

