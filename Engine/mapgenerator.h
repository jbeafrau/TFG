#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H


#ifdef linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <libnoise/noise.h>
#endif
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <noise.h>
#include "noiseutils.h"
#endif

#include "enum.h"

using namespace noise;

class mapgenerator
{
    public:
     module::Perlin myModule;
     utils::NoiseMap heightMap;
     utils::NoiseMapBuilderPlane heightMapBuilder;
        mapgenerator();
        virtual ~mapgenerator();
        void init ();
        void generate (int octave, int frequency, float persistance, int sizex, int sizey);
        void to_surface(SDL_Surface *surface, int currentState);
        void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );
    protected:
    private:
};

#endif // MAPGENERATOR_H
