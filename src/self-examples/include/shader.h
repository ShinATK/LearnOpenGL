#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // program id
    unsigned int ID;

    // 构造器读取并创建着色器
    Shader(const char* vertexPath, const char* fragmentPath);

    // 激活shader程序
    void use();

    // uniform工具函数
    void setBool4(const std::string &name, bool* RGB) const;
    void setInt4(const std::string &name, int* RGB) const;
    void setFloat4(const std::string &name, float R, float G, float B) const;

private:
    // 检查着色器编译/程序链接问题
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
