#include <DefaultSettings/DefaultSettings.h>
#include <iostream>

using namespace std;

const char * relativePath(string path, string name)
{
    return (path + "/" + name).c_str();
}