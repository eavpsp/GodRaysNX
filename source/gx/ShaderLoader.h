#ifndef SHADERLOADER_H
#define SHADERLOADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <glad/glad.h>  // glad library (OpenGL loader)


class ShaderLoader
{
    private:
        GLuint _programHandle;
        GLuint compileShader(GLenum targetShader, const char *source);
    public:
        GLuint getProgramHandle();
        ShaderLoader(const char *sourceVs, const char *sourceFS);//get shader program vertex and fragment
        ~ShaderLoader();
};
#endif