#ifndef TEXTURE2DCOMPONENT_H
#define TEXTURE2DCOMPONENT_H

#include <GameObject.h>
#include <raylib.h>
struct TextureDrawable : public ObjectDrawable
{
    Texture2D* texture;
    TextureDrawable() { texture = nullptr; }
    TextureDrawable(Texture2D* _texture) { texture = _texture; }
    void draw() override
    {
        DrawTexture(*texture, componentParent->parentObject->position.x, componentParent->parentObject->position.y, WHITE);
    }
};
struct Texture2DComponent : public GameComponent
{
    TextureDrawable textureRenderer;
    Texture2DComponent() { }
    Texture2DComponent(Texture2D* _texture) { textureRenderer.texture = _texture; }
    void ComponentAddedCallback() override { textureRenderer.componentParent = this; parentObject->objectDrawables.push_back(&textureRenderer); };
    void OnUpdate() override { return; }
};



#endif /* TEXTURE2DCOMPONENT_H_ */
