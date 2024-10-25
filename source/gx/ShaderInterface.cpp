#include <ShaderInterface.h>
#include "../debug/debug.h"

char *ShaderInterface::loadTextFromFile(const char *file)
{
    debugLog("Loading: %s", file);
    
    FILE *currentFile = fopen(file, "rt");
    fseek(currentFile, 0, SEEK_END);
    int count = (int)ftell(currentFile);
    rewind(currentFile);

    char *data = (char *)malloc(sizeof(char)* (count +1));
    count = (int)fread(data, sizeof(char), count, currentFile);
    data[count] = '\0';

    return data;
}

GLuint ShaderInterface::getProgramHandle()
{
    return shader->getProgramHandle();
}




void ShaderInterface::Bind() const
{
    glUseProgram(shader->getProgramHandle());
}
void ShaderInterface::UnBind() const
{
    glUseProgram(0);
}

ShaderInterface::ShaderInterface(const char *VS, const char *FS)
{
    _vertexShaderString = loadTextFromFile(VS);
    _fragmentShaderString = loadTextFromFile(FS);
    shader = new ShaderLoader(_vertexShaderString, _fragmentShaderString);//gives an ID and if var doesnt exist returns -1
  }

ShaderInterface::~ShaderInterface()
{
    delete shader;
}
