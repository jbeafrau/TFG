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
    void createSurface();
    void SetSurface();
    void put_pixel32(SDL_Surface* surface, int x, int y, Uint32 pixel);
    void generateTiles(int currentState);

    int dice(int maxValue, int minValue);
    SDL_Rect getBuildingSpot(int w, int h);
    void building(SDL_Rect square, int wallTile, int floorTile,int x, int y, int doorTile);
    void road(SDL_Rect square, int floorTile1, int floorTile2);
    void openBuilding(SDL_Rect square, int wallTile, int floorTile, int doorTile, int specialTile);
    void getLocation(int *lx, int *ly, int tile);

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
    int map_cells[300][300];

   // mapgenerator mymap;

};

#endif // CLASSMAP_H
