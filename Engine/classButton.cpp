#include "classButton.h"

classButton::classButton()
{
    //ctor
    x=0;
    y=0;
    w=0;
    h=0;
    caption = "N/A";
}

classButton::~classButton()
{
    //dtor
}

void classButton::setButton(int x,int y,int w,int h, string caption)
{
    rect.x=x;
    rect.y=y;
    rect.w=w;
    rect.h=h;
    this->caption = caption;
}

void classButton::setColor(int r, int g, int b)
{
    color.r =r;
    color.g = g;
    color.b = b;
}

SDL_Rect classButton::getRect()
{
   return rect;
}

SDL_Color classButton::getColor()
{
   return color;
}

string classButton::getCaption()
{
    return caption;
}

void classButton::setCaption(string text)
{
    caption = text;
}

bool classButton::clicked(int x, int y)
{
if(x>= rect.x && x<= (rect.x +rect.w) && y>= rect.y && y<= (rect.y+ rect.h)){
    return true;
}else{
    return false;
}
}
