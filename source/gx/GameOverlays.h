
#ifndef GX_GAMEOVERLAYS_H
#define GX_GAMEOVERLAYS_H
//Drawables
#include<vector>
#include<string>
#include<raylib.h>
#include<ResourceManager.h>
#include <raygui.h>
//RAYGUI for ELEMENTS
extern Font guiFont;
extern std::map<int, std::string> RES_UI_Textures;
struct Drawable //Objects that can be drawn (Text, Images, etc)
{
    virtual void draw() = 0;
    Drawable() = default;
    virtual ~Drawable(){};
};
struct Image2D : public Drawable
{
    Texture2D texture;
    Vector2 position;
    Image2D() = default;
    Image2D(const char* filename, Vector2 p) : position(p) 
    {
        texture = LoadTexture(filename);
    }
    void draw() override
    {
        DrawTexture(texture, position.x, position.y, WHITE);
    }
    ~Image2D()
    {
        UnloadTexture(texture);
    }
};

struct TextDrawable : public Drawable
{
    std::string text;
    Vector2 position;
    float fontSize = 20;
    Color color;
    void SetText(std::string t) { text = t; }
    void SetText(const char* format, ...) {
        va_list args;
        va_start(args, format);
        int len = vsnprintf(nullptr, 0, format, args) + 1;
        char* buffer = new char[len];
        va_start(args, format);
        vsnprintf(buffer, len, format, args);
        text = buffer;
        delete[] buffer;
    }
    TextDrawable() = default;
    TextDrawable(std::string t, Vector2 p, float fontSize = 20, Color c = BLACK) : text(t), position(p), color(c), fontSize(fontSize) {}    
    void draw() override
    {
        DrawTextEx(guiFont, text.c_str(), position, fontSize, fontSize/2, color);
    }
    ~TextDrawable()
    {}

};
struct Overlay//Game Overlays
{
    std::vector<Drawable*> drawables;
    void Draw()
    {
        for (size_t i = 0; i < drawables.size(); i++)
        {
            /* code */
            drawables[i]->draw();
        }
        
    }
    template<typename T>
    T* GetDrawable()
    {
        for (size_t i = 0; i < drawables.size(); i++)
        {
            if (dynamic_cast<T*>(drawables[i]))
                return dynamic_cast<T*>(drawables[i]);
        }
        return nullptr;
    }
    void AddDrawable(Drawable* d)
    {
        drawables.push_back(d);
    }

    void RemoveDrawable(Drawable* d)
    {
        for (size_t i = 0; i < drawables.size(); i++)
        {
            /* code */
            if (drawables[i] == d)
            {
                drawables.erase(drawables.begin() + i);
                break;
            }
        }
    }
    Overlay() = default;
  virtual ~Overlay()
    {
        for (size_t i = 0; i < drawables.size(); i++)
        {
            /* code */
            delete drawables[i];
        }
    }
};

struct LoadingOverlay : public Overlay
{
    TextDrawable* GetProgress()
    {
        return (TextDrawable*)drawables.at(1);
    };
    LoadingOverlay()
    {
        drawables.push_back(new TextDrawable("Loading", { 10, 10 }));
        drawables.push_back(new TextDrawable("Progress", { 10, 40 }));
    };
    ~LoadingOverlay() = default;
};
struct BootOverlay : public Overlay
{
    BootOverlay()
    {
        drawables.push_back(new TextDrawable("Welcome to GodRays Game Engine",{ 400, 100}, 20, WHITE));
        drawables.push_back(new TextDrawable("Booting Up...", {440, 150}, 20, WHITE));
        drawables.push_back(new Image2D((RES_UI_Textures[_RES::UI_ICONS::LOGO].c_str()), { 400, 205 }));
    };
    ~BootOverlay() = default;
};
#endif // GX_GAMEOVERLAYS_H
