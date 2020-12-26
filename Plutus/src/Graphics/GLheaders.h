#ifndef _GLHEADERS_H
#define _GLHEADERS_H

#ifdef _WIN32
extern "C"
{
#include <windows.h>
}
#include <glad/glad.h>

#elif __ANDROID__

#include <GLES3/gl3.h>

#endif

#endif // !_GLHEADERS_H
