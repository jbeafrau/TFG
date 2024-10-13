#ifndef CLASSBUTTON_H
#define CLASSBUTTON_H
#include <string>
#ifdef linux
        #include <SDL2/SDL.h>
        #include <SDL2/SDL_image.h>
        #include <SDL2/SDL_ttf.h>
        #include <SDL2/SDL_mixer.h>
#endif
#ifdef _WIN32
        #include <SDL.h>
        #include <SDL_image.h>
        #include <SDL_ttf.h>
        #include <SDL_mixer.h>
#endif

using namespace std;
class classButton
{
    public:
        classButton();
        virtual ~classButton();
        void setButton(int x,int y,int w,int h, string caption);
        void setColor(int r, int g, int b);
        SDL_Rect getRect();
        SDL_Color getColor();
        string getCaption();
        void setCaption(string text);
        bool clicked(int x, int y);

    protected:

    private:

        int x,y,w,h;
        string caption;
        SDL_Color color;
        SDL_Rect rect;
};

#endif // CLASSBUTTON_H
