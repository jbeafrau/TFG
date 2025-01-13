#include "mapgenerator.h"

using namespace noise;
using namespace utils;

mapgenerator::mapgenerator()
{
    //ctor
}

mapgenerator::~mapgenerator()
{
    //dtor
}


void mapgenerator::init ()
{
heightMapBuilder.SetSourceModule (myModule);
heightMapBuilder.SetDestNoiseMap (heightMap);
}



void mapgenerator::generate (int octave, int frequency, float persistance, int sizex, int sizey)
{
	int bound_x = sizex;
	int bound_y = sizey;
        myModule.SetOctaveCount (octave);
   //(1-6) 6 default
   //the amount of detail increases when the number of octaves increases
    myModule.SetFrequency (frequency/2);
  //increasing the frequency will increase the number of terrain features
//(and also decrease the size of these features) in a terrain height map
    myModule.SetPersistence (persistance); //0.25-0.75 (0.5 default)+rougher frequency jump

 heightMapBuilder.SetDestSize (sizex, sizey);
 heightMapBuilder.SetBounds (bound_x, bound_x + 1, bound_y, bound_y + 1);
  heightMapBuilder.Build ();
}

void mapgenerator::to_surface(SDL_Surface *surface, int currentState)
{
	utils::RendererImage renderer;
  utils::Image image;
  utils::Color  color;
  renderer.SetSourceNoiseMap (heightMap);
  renderer.SetDestImage (image);
  renderer.ClearGradient ();

  switch (currentState)
  {


  case my_enums::S_ELEMENTAL_WATER_WORLD_:
  {

	  renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 0, 255)); // deeps
	  renderer.AddGradientPoint(0.0000, utils::Color(0, 0, 128, 255)); // deeps
	  renderer.AddGradientPoint(1.0000, utils::Color(0, 0, 255, 255)); // high
	  break;
  }

  case my_enums::S_ELEMENTAL_FIRE_WORLD_:
  {

	  renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 0, 255)); // deeps
	  renderer.AddGradientPoint(0.0000, utils::Color(255, 0, 0, 255)); // deeps
	  renderer.AddGradientPoint(1.0000, utils::Color(255, 0, 255, 255)); // high
	  break;
  }

  case my_enums::S_ELEMENTAL_WIND_WORLD_:
  {

	  renderer.AddGradientPoint(-1.0000, utils::Color(246, 246, 246, 255)); // deeps
	  renderer.AddGradientPoint(0.0000, utils::Color(204, 204, 204, 255)); // deeps
	  renderer.AddGradientPoint(1.0000, utils::Color(171, 171, 171, 255)); // high
	  break;
  }

  case my_enums::S_ELEMENTAL_EARTH_WORLD_:
  {

	  renderer.AddGradientPoint(-1.0000, utils::Color(205, 133, 63,255)); // deeps
	  renderer.AddGradientPoint(0.0000, utils::Color(222, 184, 135, 255)); // deeps
	  renderer.AddGradientPoint(1.0000, utils::Color(139, 69, 19, 255)); // high
	  break;
  }


  case my_enums::S_COAST_WORLD_:
  {

	  renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
	  renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); // shallow
	  renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); // shore
	  renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); // sand
	  renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); // grass
	  renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); // dirt
	  renderer.AddGradientPoint(0.5750, utils::Color(152, 118, 84, 255)); // brown
	  renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); // rock
	  renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); // snow
	  break;
  }


  case my_enums::S_FOREST_WORLD_:
  {
	  module::Billow baseFlatTerrain;
	  baseFlatTerrain.SetFrequency(2.0);

	  module::ScaleBias flatTerrain;
	  flatTerrain.SetSourceModule(0, baseFlatTerrain);
	  //flatTerrain.SetScale(0.125);
	  flatTerrain.SetScale(0.50);
	  flatTerrain.SetBias(+0.25);

	  heightMapBuilder.SetSourceModule(flatTerrain);
	  heightMapBuilder.SetDestNoiseMap(heightMap);
	  heightMapBuilder.SetDestSize(256, 256);
	  heightMapBuilder.SetBounds(1.0, 5.0, 1.0, 5.0);
	  heightMapBuilder.Build();


	  renderer.AddGradientPoint(-1.00, utils::Color(224, 224, 0, 255)); // dirt
	  renderer.AddGradientPoint(0.00, utils::Color(16, 120, 0, 255)); // grass
	  renderer.AddGradientPoint(1.00, utils::Color(32, 160, 0, 255)); // grass

	  break;
  }

  case my_enums::S_HOMETOWN_:
  {
	  module::Billow baseFlatTerrain;
	  baseFlatTerrain.SetFrequency(2.0);

	  module::ScaleBias flatTerrain;
	  flatTerrain.SetSourceModule(0, baseFlatTerrain);
	  flatTerrain.SetScale(0.125);
	  flatTerrain.SetBias(-0.75);

	  heightMapBuilder.SetSourceModule(flatTerrain);
	  heightMapBuilder.SetDestNoiseMap(heightMap);
	  heightMapBuilder.SetDestSize(256, 256);
	  heightMapBuilder.SetBounds(1.0, 5.0, 1.0, 5.0);
	  heightMapBuilder.Build();

	  renderer.AddGradientPoint(-1.00, utils::Color(32, 160, 0, 255)); // grass
	  renderer.AddGradientPoint(-0.25, utils::Color(224, 224, 0, 255)); // dirt
	  renderer.AddGradientPoint(0.25, utils::Color(128, 128, 128, 255)); // rock
	  renderer.AddGradientPoint(1.00, utils::Color(255, 255, 255, 255)); // snow


	  break;
  }

  case my_enums::S_NECRO_WORLD_:
  {
  module::Billow baseFlatTerrain;
  baseFlatTerrain.SetFrequency(2.0);

  module::ScaleBias flatTerrain;
  flatTerrain.SetSourceModule(0, baseFlatTerrain);
  flatTerrain.SetScale(0.125);
  //flatTerrain.SetScale(0.50);
  flatTerrain.SetBias(-0.5);

  heightMapBuilder.SetSourceModule(flatTerrain);
  heightMapBuilder.SetDestNoiseMap(heightMap);
  heightMapBuilder.SetDestSize(256, 256);
  heightMapBuilder.SetBounds(1.0, 5.0, 1.0, 5.0);
  heightMapBuilder.Build();


  renderer.AddGradientPoint(-1.00, utils::Color(224, 0, 0, 255)); // fire
  renderer.AddGradientPoint(0.00, utils::Color(224, 0, 0, 255)); // fire
  renderer.AddGradientPoint(1.00, utils::Color(100, 100, 100, 255)); // ash
  break;
  }


  default:
  {
	  // is likely to be an error
	  renderer.AddGradientPoint(-1.00, utils::Color(32, 160, 0, 255)); // grass
	  renderer.AddGradientPoint(-0.25, utils::Color(224, 224, 0, 255)); // dirt
	  renderer.AddGradientPoint(0.25, utils::Color(128, 128, 128, 255)); // rock
	  renderer.AddGradientPoint(1.00, utils::Color(255, 255, 255, 255)); // snow

	  break;
  }
  };

renderer.EnableLight();
renderer.SetLightContrast(3.0);
renderer.SetLightBrightness(2.0);
renderer.Render();

heightMapBuilder.SetSourceModule(myModule);
heightMapBuilder.SetDestNoiseMap(heightMap);

int width  = image.GetWidth  ();
int height = image.GetHeight ();

int x,y;
Uint32 pixel;
for (x=1;x<width;x++){
    for (y=1;y<height;y++){
        const Color* pSource = image.GetConstSlabPtr (x, y);
        pixel = SDL_MapRGBA(surface ->format, pSource ->red, pSource ->green, pSource ->blue, pSource ->alpha);
        put_pixel32( surface, x,  y, pixel ); // r g b  a

    }
}

}

void mapgenerator::put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}
