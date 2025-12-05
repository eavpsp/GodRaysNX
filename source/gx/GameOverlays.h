
#ifndef GX_GAMEOVERLAYS_H
#define GX_GAMEOVERLAYS_H
//Drawables
#include<vector>
#include<string>
#include<raylib.h>
#include<ResourceManager.h>
//RAYGUI for ELEMENTS
extern Font gameFont;
extern std::map<int, std::string> RES_UI_Textures;
struct Drawable //Objects that can be drawn (Text, Images, etc)
{
    virtual void draw() = 0;
    Drawable() = default;
    virtual ~Drawable(){};
};
struct ProgressBar : public Drawable//add one that uses images
{
    float value = 0;
    Color fillColor = WHITE;
    Color bgColor = BLACK;
    float height = 10;
    float width = 100;
    Vector2 position;
    ProgressBar(float val, Color c, Color b , Vector2 p, float h, float w) : fillColor(c), bgColor(b), value(val), position(p), height(h), width(w) {}
    void draw() override
    {
        DrawRectangle(position.x, position.y, width, height, bgColor);
        DrawRectangle(position.x, position.y, (value) * width, height, fillColor);
    }
    void SetValue(float v) { value = v; }
    ~ProgressBar(){}
};
struct CrosshairDrawable : public Drawable
{
    //circle and 4 rects 
    /*
    
                   |
                ---.---
                   |
    
    
    */
   bool showCrosshair = true;
   Color crossHairColor = WHITE;
   void SetColor(Color c) { crossHairColor = c; }
   void draw() override
   {
    if(!showCrosshair)
    {
        return;
    }
       DrawCircle(640, 360, 10, crossHairColor); //center of screen
       //top
       DrawRectangle(640 - 10, 360 + 20, 20, 40, BLUE);
       //bottom
       DrawRectangle(640 - 10, 360 - (20 + 40), 20, 40, BLUE);
        //left
       DrawRectangle(640 - (20 + 40), 360 - 10, 40, 20, BLUE);
       //right
       DrawRectangle(640 + 20, 360 - 10, 40, 20, BLUE);
   }
   CrosshairDrawable(){};
   ~CrosshairDrawable(){};
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
        DrawTextEx(gameFont, text.c_str(), position, fontSize, fontSize/2, color);
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
        drawables.push_back(new TextDrawable("Loading", { 100, 250 }));
        drawables.push_back(new TextDrawable("Progress", { 100, 250 }));
    };
    ~LoadingOverlay() = default;
};
struct BootOverlay : public Overlay
{
    BootOverlay()
    {
        drawables.push_back(new TextDrawable("Welcome to GodRays Game Engine",{ 300, 100}, 25, BLACK));
        drawables.push_back(new TextDrawable("Booting Up...", {340, 150}, 25, BLACK));
        drawables.push_back(new Image2D((RES_UI_Textures[_RES::UI_ICONS::LOGO].c_str()), { 400, 205 }));
    };
    ~BootOverlay() = default;
};
#endif // GX_GAMEOVERLAYS_H
