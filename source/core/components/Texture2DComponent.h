#ifndef TEXTURE2DCOMPONENT_H
#define TEXTURE2DCOMPONENT_H

#include <GameObject.h>
#include <raylib.h>
#include <RenderSystem.h>
extern RenderSystem *renderSystem;
struct TextureDrawable : public ObjectDrawable
{
    Texture2D texture;
    TextureDrawable() { }
    TextureDrawable(Texture2D _texture) { texture = _texture; }
    void draw() override
    {
        DrawTexture(texture, componentParent->parentObject->position.x, componentParent->parentObject->position.y, WHITE);
    }
};
struct Texture2DComponent : public GameComponent
{
    int texture2DIndex;
    void SetTexture(Texture2D _texture) { textureRenderer.texture = _texture; }
    TextureDrawable textureRenderer;
    Texture2DComponent() { }
    Texture2DComponent(const char *_texturePath) { textureRenderer.texture = LoadTexture(_texturePath); }
    void ComponentAddedCallback() override { textureRenderer.componentParent = this; renderSystem->drawable2D.push_back(&textureRenderer);
     };
    void OnUpdate() override { return; }
    ~Texture2DComponent() { parentObject->objectDrawables.erase(std::remove(parentObject->objectDrawables.begin(), parentObject->objectDrawables.end(), &textureRenderer), parentObject->objectDrawables.end()); renderSystem->drawable2D.erase(std::remove(renderSystem->drawable2D.begin(), renderSystem->drawable2D.end(), &textureRenderer), renderSystem->drawable2D.end()); UnloadTexture(textureRenderer.texture); }
};



#endif /* TEXTURE2DCOMPONENT_H_ */
