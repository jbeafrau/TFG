#ifndef CLASSMAP_H
#define CLASSMAP_H

//#include "utils.h"
//#include "tileset.h"
#include "mapgenerator.h"



#include <stdio.h>

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

class classMap
{
public:
    classMap();
    virtual ~classMap();
    int get_cell(int x, int y);
    void blur();
    int getBlur();
    void setBlur(int blur);
    //void createSurface(int w,int h);
    void createSurface();
    void SetSurface();
    void put_pixel32(SDL_Surface* surface, int x, int y, Uint32 pixel);
    void generateTiles();


    int dice(int maxValue, int minValue);
    void building(SDL_Rect square, int wallTile, int floorTile);
    void road(SDL_Rect square, int floorTile1, int floorTile2);

    
    /*void init_image(SDL_Renderer* renderer2, int startx2, int starty2, const char* file2);
    void init(SDL_Renderer* renderer2, int startx2, int starty2);
   
    void draw(SDL_Renderer* renderer2);
    void draw2(SDL_Renderer* renderer2);
    void miniset(int x, int y, int w, int h);
    void set(int x, int y, int w, int h);
    void minidraw(SDL_Renderer* renderer2);

    void draw_tiles(SDL_Renderer* renderer2, int tilesize);

    void update(int x, int y);
    void setscale(int w, int h, int tilesize);

    void load_tileset(SDL_Renderer* renderer2, const char* file);

    ctileset mytileset;

    ;*/
    SDL_Surface* imageSurface = NULL;
    SDL_Surface* targetSurface = NULL;

    mapgenerator mymap;
protected:
private:
    int startx, starty, endx, endy; //,scale;
//    Sprite2 image;
 //   Sprite2 tileset;

    SDL_Rect  originrect;
    SDL_Rect  destrect;

    SDL_Rect  miniorigin;
    SDL_Rect  minidest;

  // SDL_Surface* surface;


   int blur_extent = 5;
  

    Uint32 rmask, gmask, bmask, amask;
    int width, height;
    int map_cells[256][256];

   // mapgenerator mymap;

};

#endif // CLASSMAP_H
