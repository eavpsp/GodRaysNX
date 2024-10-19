
#ifndef GX_GAMEOVERLAYS_H
#define GX_GAMEOVERLAYS_H
//Drawables
#include<vector>
#include<string>
#include<raylib.h>
extern Font guiFont;
struct Drawable //Objects that can be drawn (Text, Images, etc)
{
    virtual void draw() = 0;
    Drawable() = default;
    virtual ~Drawable(){};
};
struct TextDrawable : public Drawable
{
    std::string text;
    Vector2 position;
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
    TextDrawable(std::string t, Vector2 p, Color c = BLACK) : text(t), position(p), color(c) {}    
    void draw() override
    {
        DrawTextEx(guiFont, text.c_str(), position, 20, 20/2, color);
    }

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

#endif // GX_GAMEOVERLAYS_H
