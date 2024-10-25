#include "ShaderLoader.h"
#include "../debug/debug.h"

GLuint ShaderLoader::compileShader(GLenum targetShader, const char *source)
{   GLint success;
    GLuint shaderHandle = glCreateShader(targetShader); //get the shader handle from the target
    glShaderSource(shaderHandle, 1, &source, NULL);
    glCompileShader(shaderHandle);
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char buf[512];
        glGetShaderInfoLog(shaderHandle, sizeof(buf), nullptr, buf);
        debugLog("Shader compilation error: %s", buf);
        debugLog("Shader compilation failed");
        debugLog("Shader type: %s", (targetShader == GL_VERTEX_SHADER) ? "vertex" : "fragment");
        glDeleteShader(shaderHandle);
        return 0;
    }
    debugLog("Shader compiled %u", shaderHandle);
    return shaderHandle;
}

GLuint ShaderLoader::getProgramHandle()
{
    return _programHandle;
}

ShaderLoader::ShaderLoader(const char *sourceVs, const char *sourceFS)
{
    //create the shader program
    _programHandle = glCreateProgram();
    //compile vertex and fragment shaders seperately
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, sourceVs);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, sourceFS);
    //attach them to the shader program
    glAttachShader(_programHandle, vertexShader);
    glAttachShader(_programHandle, fragmentShader);

    glLinkProgram(_programHandle);
    GLint success;
    glGetProgramiv(_programHandle, GL_LINK_STATUS, &success);
    if (!success)
    {
        char buf[512];
        glGetProgramInfoLog(_programHandle, sizeof(buf), nullptr, buf);
        debugLog("Link error: %s", buf);
    }
    debugLog("Shader program created");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderLoader::~ShaderLoader()
{
    glDeleteProgram(_programHandle);
}
