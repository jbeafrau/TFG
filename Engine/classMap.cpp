#include "classMap.h"

classMap::classMap()
{
    //ctor

}

classMap::~classMap()
{
    //dtor
}

int classMap::get_cell(int x, int y)
{
    return map_cells[x][y];
}


/*


void map::load_tileset(SDL_Renderer* renderer2, const char* file)
{
    tileset = LoadSprite(file, renderer2);
}

void map::init_image(SDL_Renderer* renderer2, int startx2, int starty2, const char* file2)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    width = 256;
    height = 256;

    surface = SDL_CreateRGBSurface(0, width, height, 32,
        rmask, gmask, bmask, amask);
    if (surface == NULL)
    {
        fprintf(stderr, "CreateRGBSurface failed: %s", SDL_GetError());
        exit(1);
    }

    mymap.init();
    mymap.generate(mymap.seed_a.frequency, mymap.seed_a.octave, mymap.seed_a.persistance, 1, 1, width, height);
    //mymap.to_color_bmp();
    //mymap.to_greyscale_bmp();
    mymap.to_surface(surface);
    image.texture = SDL_CreateTextureFromSurface(renderer2, surface);

    miniorigin.x = 1;
    miniorigin.y = 1;
    miniorigin.w = image.w;
    miniorigin.h = image.h;

    minidest.x = 1;
    minidest.y = 1;
    minidest.w = 256;
    minidest.h = 256;
}



void map::init(SDL_Renderer* renderer2, int startx2, int starty2)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    width = 256;
    height = 256;

    surface = SDL_CreateRGBSurface(0, width, height, 32,
        rmask, gmask, bmask, amask);
    if (surface == NULL)
    {
        fprintf(stderr, "CreateRGBSurface failed: %s", SDL_GetError());
        exit(1);
    }

    mymap.init();
    mymap.generate(mymap.seed_a.frequency, mymap.seed_a.octave, mymap.seed_a.persistance, 1, 1, width, height);
    mymap.to_surface(surface);
    image.texture = SDL_CreateTextureFromSurface(renderer2, surface);
    miniorigin.x = 1;
    miniorigin.y = 1;
    miniorigin.w = 256; // image.w;
    miniorigin.h = 256; //image.h;
    minidest.x = 1;
    minidest.y = 1;
    minidest.w = 256;
    minidest.h = 256;
    int width = mymap.heightMap.GetWidth();
    int height = mymap.heightMap.GetHeight();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            const float* cell = mymap.heightMap.GetConstSlabPtr(x, y);
            map_cells[x][y] = 0;
            if (*cell >= -1.0000)  map_cells[x][y] = 201;
            if (*cell >= -0.2500)  map_cells[x][y] = 201;
            if (*cell >= 0.0000)  map_cells[x][y] = 0;


            //if ((*cell >= -0.2500 ) and ((x -y*2)% 5 == 0))


            //if ((*cell >=  0.0000 ) and ((x -y*2)% 5 == 0))map_cells[x][y] = 3;

            if (*cell >= 0.0625) {
                if ((x - y * 2) % 5 == 0)
                {
                    map_cells[x][y] = 4;
                }
                else {
                    map_cells[x][y] = 0;
                }
            }

            if (*cell >= 0.1250) {
                if ((x - y * 2) % 5 == 0)
                {
                    map_cells[x][y] = 5;
                }
                else {
                    map_cells[x][y] = 0;
                }
            }

            if (*cell >= 0.3750) {
                if ((x - y * 2) % 5 == 0)
                {
                    map_cells[x][y] = 6;
                }
                else {
                    map_cells[x][y] = 0;
                }
            }

            if (*cell >= 0.5750) {
                if ((x - y * 2) % 5 == 0)
                {
                    map_cells[x][y] = 7;
                }
                else {
                    map_cells[x][y] = 0;
                }
            }

            if (*cell >= 0.7500) {
                if ((x - y * 2) % 5 == 0)
                {
                    map_cells[x][y] = 8;
                }
                else {
                    map_cells[x][y] = 0;
                }
            }

            if (*cell >= 1.0000) {
                if ((x - y * 2) % 5 == 0)
                {
                    map_cells[x][y] = 9;
                }
                else {
                    map_cells[x][y] = 0;
                }
            }

        //    renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
        //      renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
        //      renderer.AddGradientPoint ( 0.0000, utils::Color (  0, 128, 255, 255)); // shore
        //      renderer.AddGradientPoint ( 0.0625, utils::Color (240, 240,  64, 255)); // sand
        //      renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
        //      renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
        //      renderer.AddGradientPoint ( 0.5750, utils::Color (152, 118,   84, 255)); // brown
        //      renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
        //      renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow 9
        }//for
    }//for

    mytileset.init(renderer2, 1, 1, 1, 1, 32, "images/tilesets/terrain_alpha.png");
}

void map::miniset(int x, int y, int w, int h)
{
    minidest.x = x;
    minidest.y = y;
    minidest.w = w;
    minidest.h = h;
}

void map::set(int x, int y, int w, int h)
{

    destrect.x = x;
    destrect.y = y;
    destrect.w = w;
    destrect.h = h;
}



void map::draw(SDL_Renderer* renderer2)
{
    SDL_RenderCopy(renderer2, image.texture, &originrect, &destrect);
}

void map::minidraw(SDL_Renderer* renderer2)
{
    SDL_RenderCopy(renderer2, image.texture, &miniorigin, &minidest);
}

void map::draw_tiles(SDL_Renderer* renderer2, int tilesize)
{
    int x, y;
    mytileset.resize(tilesize, tilesize);

    for (y = originrect.y; y < (originrect.y + originrect.h); y++)
    {
        for (x = originrect.x; x < (originrect.x + originrect.w); x++)
        {
            mytileset.settile(20, 20);//empty
            //if (map_cells[x][y]==1) mytileset.settile(7,7);//sea deep
            //if (map_cells[x][y]==2) mytileset.settile(7,8);//sea light
            //if (map_cells[x][y]==3) mytileset.settile(5,12);//shore *
            if (map_cells[x][y] == 4) mytileset.settile(5, 12);//sand
            if (map_cells[x][y] == 5) mytileset.settile(13, 12);//grass
            if (map_cells[x][y] == 6) mytileset.settile(14, 12);//dirty *
            if (map_cells[x][y] == 7) mytileset.settile(15, 12);//brown
            if (map_cells[x][y] == 8) mytileset.settile(16, 12);//rocks
            if (map_cells[x][y] == 9) mytileset.settile(17, 12);//snow

            //if (map_cells[x][y]==201) mytileset.settile(5,5);//snow

           // if (map_cells[x][y]==99) mytileset.settile(20,20);//empty
          //  if (map_cells[x][y]==101) mytileset.settile(20,20);//empty
           // if (map_cells[x][y]==102) mytileset.settile(20,20);//empty
            mytileset.move((x - originrect.x) * tilesize, (y - originrect.y) * tilesize);
            mytileset.draw(renderer2);
        }
    }

}


void map::update(int x, int y)
{
    originrect.x = x;
    originrect.y = y;
}

void map::setscale(int w, int h, int tilesize)
{
    originrect.w = w / tilesize + 1;
    originrect.h = h / tilesize + 1;
    minidest.h = h / 2;
    minidest.w = minidest.h;

    destrect.x = 1;
    destrect.y = 1;
    destrect.w = originrect.w * tilesize;
    destrect.h = originrect.h * tilesize;

}



*/

void classMap::put_pixel32(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    //Convert the pixels to 32 bit
    Uint32* pixels = (Uint32*)surface->pixels;
    //Set the pixel
    pixels[(y * surface->w) + x] = pixel;
}

void classMap::SetSurface()
{
    Uint32 pixel;
    for (int y = 0; y < imageSurface->h; y++)
    {
        for (int x = 0; x < (imageSurface->pitch / 4); x++)
        {
            if (y < 80) {
                pixel = SDL_MapRGBA(imageSurface->format, 255, 0, 0, 0);
            }else  {
                pixel = SDL_MapRGBA(imageSurface->format, 0, 255, 0, 0);

            }

            if (x > 80) {
                pixel = SDL_MapRGBA(imageSurface->format, 0, 0, 255, 0);
            }
            
            
         /*   int a;
            a = (y % 3);
            switch (a) {
            case 0: { pixel = SDL_MapRGBA(imageSurface->format, 255, 0, 0, 0); break; }
            case 1: { pixel = SDL_MapRGBA(imageSurface->format, 0, 255, 0, 0); break; }
            case 2: { pixel = SDL_MapRGBA(imageSurface->format, 0, 0, 255, 0); break; }
            }
            */
            

              //  pixel = SDL_MapRGBA(imageSurface->format, x, y, 0, 0);
          
            
            
            put_pixel32(imageSurface, x, y, pixel); // r g b  a

        }

    }

}

//void classMap::createSurface(int w, int h)
void classMap::createSurface()
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    width = 256;
    height = 256;

    imageSurface = SDL_CreateRGBSurface(0, width, height, 32,rmask, gmask, bmask, amask);
    if (imageSurface == NULL)
    {
        fprintf(stderr, "CreateRGBSurface failed: %s", SDL_GetError());
        exit(1);
    }

    //targetSurface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
    //targetSurface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    targetSurface = SDL_CreateRGBSurface(0, 600, 400, 32, rmask, gmask, bmask, amask);

    if (targetSurface == NULL)
    {
        fprintf(stderr, "CreateRGBSurface failed: %s", SDL_GetError());
        exit(1);
    }

}

void classMap::blur() //This manipulates with SDL_Surface and gives it box blur effect
{
    for (int y = 0; y < targetSurface->h; y++)
    {
        for (int x = 0; x < (targetSurface->pitch / 4); x++)
        {
            Uint32 color = ((Uint32*)targetSurface->pixels)[(y * (targetSurface->pitch / 4)) + x];

            //SDL_GetRGBA() is a method for getting color
            //components from a 32 bit color
            Uint8 r = 0, g = 0, b = 0, a = 0;
            SDL_GetRGBA(color, targetSurface->format, &r, &g, &b, &a);

            Uint32 rb = 0, gb = 0, bb = 0, ab = 0;

            //Within the two for-loops below, colors of adjacent pixels are added up

            for (int yo = -blur_extent; yo <= blur_extent; yo++)
            {
                for (int xo = -blur_extent; xo <= blur_extent; xo++) {
                    if (y + yo >= 0 && x + xo >= 0
                        && y + yo < targetSurface->h && x + xo < (targetSurface->pitch / 4)
                        )
                    {
                        Uint32 colOth = ((Uint32*)targetSurface->pixels)[((y + yo)
                            * (targetSurface->pitch / 4)) + (x + xo)];

                        Uint8 ro = 0, go = 0, bo = 0, ao = 0;
                        SDL_GetRGBA(colOth, targetSurface->format, &ro, &go, &bo, &ao);

                        rb += ro;
                        gb += go;
                        bb += bo;
                        ab += ao;
                    }
                }
            }

            //The sum is then, divided by the total number of
            //pixels present in a block of blur radius

            //For blur_extent 1, it will be 9
            //For blur_extent 2, it will be 25
            //and so on...

            //In this way, we are getting the average of
            //all the pixels in a block of blur radius

            //(((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)) calculates
            //the total number of pixels present in a block of blur radius

            r = (Uint8)(rb / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));
            g = (Uint8)(gb / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));
            b = (Uint8)(bb / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));
            a = (Uint8)(ab / (((blur_extent * 2) + 1) * ((blur_extent * 2) + 1)));

            //Bit shifting color bits to form a 32 bit proper colour
            color = (r) | (g << 8) | (b << 16) | (a << 24);           ((Uint32*)targetSurface->pixels)[(y * (targetSurface->pitch / 4)) + x] = color;
        }
    }
}