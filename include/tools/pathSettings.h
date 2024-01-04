#ifndef __DEFAULT_SETTINGS_H_
#define __DEFAULT_SETTINGS_H_

// #define IMG_PATH ""
// #define SHADER_PATH ""
// #define TEXTURE_PATH ""

#define WORK_FOLDER_PATH "D:/Documents/VSCode/LearnOpenGL/"
#define FILE_FOLDER_PATH "D:/Documents/VSCode/LearnOpenGL/src/4-6-cubemaps/"
// #define SHADER_RELATIVE_PATH ""
#define TEXTURE_RELATIVE_PATH "D:/Documents/VSCode/LearnOpenGL/src/4-6-cubemaps/texture/"

#include <iostream>
using namespace std;

const char * relativePath(string path, string name)
{
    return (path + name).c_str();
}


#endif //__DEFAULT_SETTINGS_H_
