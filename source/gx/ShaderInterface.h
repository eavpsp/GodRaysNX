#ifndef SHADERINTERFACE_H
#define SHADERINTERFACE_H
#include <stdlib.h>
#include <switch.h>
#include <ShaderLoader.h>
#include <glad/glad.h>  // glad library (OpenGL loader)
#include <string>

class ShaderInterface
{
    private:
        ShaderLoader *shader;
        char *_vertexShaderString;
        char *_fragmentShaderString;
        char *loadTextFromFile(const char *file);
    public:
        GLuint getProgramHandle();
        //uniform types
        void Bind() const;
        void UnBind() const;
        
        ShaderInterface(const char *VS, const char *FS);
        ~ShaderInterface();

};
#endif