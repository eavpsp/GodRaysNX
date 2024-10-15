
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
};
struct TextDrawable : public Drawable
{
    std::string text;
    Vector2 position;
    Color color;
    TextDrawable() = default;
    TextDrawable(std::string t, Vector2 p, Color c = WHITE) : text(t), position(p), color(c) {}    
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
    ~Overlay()
    {
        for (size_t i = 0; i < drawables.size(); i++)
        {
            /* code */
            delete drawables[i];
        }
    }
};



#endif // GX_GAMEOVERLAYS_H
