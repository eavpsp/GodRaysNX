#ifndef SKYBOXRENDERER_H
#define SKYBOXRENDERER_H
#include <raylib.h>
#include <ResourceManager.h>
#include <rlgl.h>
#include <raymath.h>
#include <map>
#include <string>
#include <vector>
#include <glad/glad.h>
extern std::map<int, std::string> RES_SkyBoxTextures;
extern std::map<int, std::pair<std::string, std::string>> RES_Shaders;

struct SkyBox//not working
{
    Model skyBoxModel;
    Shader cubeMapShader;
    Mesh cube;
    Image img;
    void DrawSkyBox()
    {
        rlEnableShader(skyBoxModel.materials[0].shader.id);
        rlDisableBackfaceCulling();
        rlDisableDepthMask();
            DrawModel(skyBoxModel, (Vector3){0, 0, 0}, 1.0f, WHITE);
        rlEnableBackfaceCulling();
        rlEnableDepthMask();
        rlDisableShader();
    }
    SkyBox(_RES::SkyBoxTextures skyboxTexture = _RES::SkyBoxTextures::DEFAULT_SKYBOX)
    {
        cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        skyBoxModel = LoadModelFromMesh(cube);
        skyBoxModel.materials[0].shader = LoadShader(RES_Shaders[_RES::ShaderFiles::SKYBOX].first.c_str(), RES_Shaders[_RES::ShaderFiles::SKYBOX].second.c_str());
        SetShaderValue(skyBoxModel.materials[0].shader, GetShaderLocation(skyBoxModel.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
        SetShaderValue(skyBoxModel.materials[0].shader, GetShaderLocation(skyBoxModel.materials[0].shader, "doGamma"), (int[1]) {  0 }, SHADER_UNIFORM_INT);
        SetShaderValue(skyBoxModel.materials[0].shader, GetShaderLocation(skyBoxModel.materials[0].shader, "vflipped"), (int[1]){  0 }, SHADER_UNIFORM_INT);
        cubeMapShader = LoadShader(RES_Shaders[_RES::ShaderFiles::CUBE_MAP].first.c_str(), RES_Shaders[_RES::ShaderFiles::CUBE_MAP].second.c_str());
        SetShaderValue(cubeMapShader, GetShaderLocation(cubeMapShader, "equirectangularMap"), (int[1]){ 0 }, SHADER_UNIFORM_INT);
        img = LoadImage(RES_SkyBoxTextures[skyboxTexture].c_str());
        skyBoxModel.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);    // CUBEMAP_LAYOUT_PANORAMA

    }
    ~SkyBox()
    {
        UnloadShader(skyBoxModel.materials[0].shader);
        UnloadTexture(skyBoxModel.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
        UnloadModel(skyBoxModel);  
        UnloadImage(img);
    }
  
};
static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format)
{
    TextureCubemap cubemap = { 0 };

    rlDisableBackfaceCulling();     // Disable backface culling to render inside the cube

    // STEP 1: Setup framebuffer
    //------------------------------------------------------------------------------------------
    unsigned int rbo = rlLoadTextureDepth(size, size, true);
    cubemap.id = rlLoadTextureCubemap(0, size, format);

    unsigned int fbo = rlLoadFramebuffer();
    rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
    rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

    // Check if framebuffer is complete with attachments (valid)
    if (rlFramebufferComplete(fbo)) TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", fbo);
    //------------------------------------------------------------------------------------------

    // STEP 2: Draw to framebuffer
    //------------------------------------------------------------------------------------------
    // NOTE: Shader is used to convert HDR equirectangular environment map to cubemap equivalent (6 faces)
    rlEnableShader(shader.id);

    // Define projection matrix and send it to shader
    Matrix matFboProjection = MatrixPerspective(90.0*DEG2RAD, 1.0, rlGetCullDistanceNear(), rlGetCullDistanceFar());
    rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matFboProjection);

    // Define view matrix for every side of the cubemap
    Matrix fboViews[6] = {
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  1.0f,  0.0f,  0.0f }, (Vector3){ 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ -1.0f,  0.0f,  0.0f }, (Vector3){ 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  0.0f,  1.0f,  0.0f }, (Vector3){ 0.0f,  0.0f,  1.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  0.0f, -1.0f,  0.0f }, (Vector3){ 0.0f,  0.0f, -1.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  0.0f,  0.0f,  1.0f }, (Vector3){ 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  0.0f,  0.0f, -1.0f }, (Vector3){ 0.0f, -1.0f,  0.0f })
    };

    rlViewport(0, 0, size, size);   // Set viewport to current fbo dimensions
    
    // Activate and enable texture for drawing to cubemap faces
    rlActiveTextureSlot(0);
    rlEnableTexture(panorama.id);

    for (int i = 0; i < 6; i++)
    {
        // Set the view matrix for the current cube face
        rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);
        
        // Select the current cubemap face attachment for the fbo
        // WARNING: This function by default enables->attach->disables fbo!!!
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
        rlEnableFramebuffer(fbo);

        // Load and draw a cube, it uses the current enabled texture
        rlClearScreenBuffers();
        rlLoadDrawCube();

        // ALTERNATIVE: Try to use internal batch system to draw the cube instead of rlLoadDrawCube
        // for some reason this method does not work, maybe due to cube triangles definition? normals pointing out?
        // TODO: Investigate this issue...
        //rlSetTexture(panorama.id); // WARNING: It must be called after enabling current framebuffer if using internal batch system!
        //rlClearScreenBuffers();
        //DrawCubeV(Vector3Zero(), Vector3One(), WHITE);
        //rlDrawRenderBatchActive();
    }
    //------------------------------------------------------------------------------------------

    // STEP 3: Unload framebuffer and reset state
    //------------------------------------------------------------------------------------------
    rlDisableShader();          // Unbind shader
    rlDisableTexture();         // Unbind texture
    rlDisableFramebuffer();     // Unbind framebuffer
    rlUnloadFramebuffer(fbo);   // Unload framebuffer (and automatically attached depth texture/renderbuffer)

    // Reset viewport dimensions to default
    rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
    rlEnableBackfaceCulling();
    //------------------------------------------------------------------------------------------

    cubemap.width = size;
    cubemap.height = size;
    cubemap.mipmaps = 1;
    cubemap.format = format;

    return cubemap;
};



#endif // SKYBOXRENDERER_H
