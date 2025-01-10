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


void classMap::getLocation(int *lx, int *ly, int tile)
{
    int return_x = 1;
    int return_y = 1;

    for (int sx = 1; sx <= 256; sx++) {
        for (int sy = 1; sy <= 256; sy++) {
            if (map_cells[sx][sy]== tile) {
                return_x = sx;
                return_y = sy;
                break;
            }
        }
    }
    *lx = return_x;
    *ly = return_y;

}

SDL_Rect classMap::getBuildingSpot(int w, int h)
{
    int x = dice(245 - w,5);
    int y = dice(245 - w, 5);
    bool spotFound = false;

    while (spotFound == false) {
        spotFound = true;
        for (int sx = x; sx <= x + w; sx++) {
            for (int sy = y; sy <= y+h; sy++) {
                const float* cell = mymap.heightMap.GetConstSlabPtr(sx, sy);
                if ((*cell < 0.0) ||(map_cells[sx][sy] == 9)){
                //if (*cell < 0.125){
                    spotFound = false;
                    break;
                }
            }
        }
        if (spotFound == false) {
            x = dice(250 - w, 1);
            y = dice(250 - w, 1);
        }

    }
   

    return {x,y,w,h};
}

void classMap::generateTiles(int currentState)
{
    int width = mymap.heightMap.GetWidth();
    int height = mymap.heightMap.GetHeight();


    //clean up first
    for (int x = 1; x < 299; x++) {
        for (int y = 1; y < 299; y++) {
            map_cells[x][y] = 0;
        }
    }

    switch (currentState)
    {

    case my_enums::S_FOREST_WORLD_: 
    {
        

        for (int y = 0; y < 256; y++)
        {
            for (int x = 0; x < 256; x++)
            {
                const float* cell = mymap.heightMap.GetConstSlabPtr(x, y);
                map_cells[x][y] = 0;

                if (*cell >= 0.1250) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 330;
                    }
                          }

                if (*cell >= 0.3750) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 333; //342
                    }
                        }

                if (*cell >= 0.5750) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 334;//344
                    }
                          }
                /*
                if (*cell >= 0.7500) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 345;
                    }
                        }

                if (*cell >= 1.0000) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 346;
                    }
                           }
                           */

                /*renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
                  renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
                  renderer.AddGradientPoint ( 0.0000, utils::Color (  0, 128, 255, 255)); // shore
                  renderer.AddGradientPoint ( 0.0625, utils::Color (240, 240,  64, 255)); // sand
                  renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
                  renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
                  renderer.AddGradientPoint ( 0.5750, utils::Color (152, 118,   84, 255)); // brown
                  renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
                  renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow 9*/
            }//for
        }//for



        //Draw  roads
        road({ 230,50,30,2 }, 90, 96);
       

        //Draw portal to necro world building
        building({ 221,47,8,9 }, 9, 104, 229, 51, 51);
        building({ 222,48,6,7 }, 9, 104, 228, 51, 51);
        building({ 223,49,4,5 }, 9, 104, 227, 51, 51);
        building({ 224,50,2,3 }, 9, 104, 226, 51, 51);


        map_cells[225][52] = 56;
       

        int w, h;
        
        //Shop building
        w = dice(3, 4);
        h = dice(3, 4);
        openBuilding({ dice(245 - w,5),dice(245 - w,5),w,h }, 9, 0, 0, 300);


        //Evil building
        w = dice(3, 4);
        h = dice(3, 4);
        openBuilding({ dice(245 - w,5),dice(245 - w,5),w,h }, 9, 0, 0, 573);

        //Clues building
        w = dice(3, 4);
        h = dice(3, 4);
        openBuilding({ dice(245 - w,5),dice(245 - w,5),w,h }, 9, 0, 0, 570);

        //lure buildings
        for (int x = 1; x < dice(4, 2); x++)
        {
            openBuilding({ dice(245 - w,5),dice(245 - w,5),w,h }, 9, 0, 0, 0);
        }



        break;
    }
    
    case my_enums::S_COAST_WORLD_:
    {
        for (int y = 0; y < 256; y++)
        {
            for (int x = 0; x < 256; x++)
            {
                const float* cell = mymap.heightMap.GetConstSlabPtr(x, y);
                map_cells[x][y] = 0;

                if (*cell >= 0.1250) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 330;
                    }
                    else {
                        map_cells[x][y] = 0;
                    }
                }

                if (*cell >= 0.3750) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 342;
                    }
                    else {
                        map_cells[x][y] = 0;
                    }
                }

                if (*cell >= 0.5750) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 344;
                    }
                    else {
                        map_cells[x][y] = 0;
                    }
                }

                if (*cell >= 0.7500) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 345;
                    }
                    else {
                        map_cells[x][y] = 0;
                    }
                }

                if (*cell >= 1.0000) {
                    if ((x - y * 2) % 5 == 0)
                    {
                        map_cells[x][y] = 346;
                    }
                    else {
                        map_cells[x][y] = 0;
                    }
                }

                /*renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
                  renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
                  renderer.AddGradientPoint ( 0.0000, utils::Color (  0, 128, 255, 255)); // shore
                  renderer.AddGradientPoint ( 0.0625, utils::Color (240, 240,  64, 255)); // sand
                  renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
                  renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
                  renderer.AddGradientPoint ( 0.5750, utils::Color (152, 118,   84, 255)); // brown
                  renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
                  renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow 9*/
            }//for
        }//for
        


        //Portal building
        openBuilding(getBuildingSpot(dice(3,4), dice(3, 4)), 9, 104, 38,56 );

        //Shop building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 9, 104, 38,300);

        //Evil building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 9, 104, 38, 573);

        //Clues building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 9, 104, 38, 570);

        //lure buildings
        for (int x = 1; x < dice(4, 2); x++)
        {
            openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 9, 104, 38, 0);
        }
                
        //coast world
        break;
    }

    case my_enums::S_ELEMENTAL_FIRE_WORLD_: 
    {
        //Portal building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 6, 157, 157, 56);

        //Shop building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 6, 157, 157, 300);

        //Evil building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 6, 157, 157, 573);

        //Clues building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 6, 157, 157, 570);

        //lure buildings
        for (int x = 1; x < dice(4, 2); x++)
        {
            openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 6, 157, 157, 0);
        }


            break;
        }
    case my_enums::S_ELEMENTAL_WATER_WORLD_: 
        {
       //9 104

        //Portal building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 21, 246, 246, 56);

        //Shop building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 21, 246, 246, 300);

        //Evil building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 21, 246, 246, 573);

        //Clues building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 21, 246, 246, 570);

        //lure buildings
        for (int x = 1; x < dice(4, 2); x++)
        {
            openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 21, 246, 246, 0);
        }

        break;
    }
    case my_enums::S_ELEMENTAL_EARTH_WORLD_: 
        {
      

        //Portal building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 154, 217, 217, 56);

        //Shop building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 154, 217, 217, 300);

        //Evil building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 154, 217, 217, 573);

        //Clues building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 154, 217, 217, 0);


        //lure buildings
        for (int x = 1; x < dice(4, 2); x++)
        {
            openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 154, 217, 217, 217);
        }
        break;
    }
    case my_enums::S_ELEMENTAL_WIND_WORLD_: 
        {
              //Portal building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 129, 216, 216, 56);

        //Shop building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 129, 216, 216, 300);

        //Evil building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 129, 216, 216, 573);

        //Clues building
        openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 129, 216, 216, 570);


        //lure buildings
        for (int x = 1; x < dice(4, 2); x++)
        {
            openBuilding(getBuildingSpot(dice(3, 4), dice(3, 4)), 129, 216, 216, 0);
        }
        break;
    }
    case my_enums::S_NECRO_WORLD_: 
        {

        for (int y = 0; y < 256; y++)
        {
            for (int x = 0; x < 256; x++)
            {
                map_cells[x][y] = 157;                
            }//for
        }//for

        //maps to final boss
        road({ 49,49,150,2 }, 20, 20);
        road({ 199,49,2,150 }, 20, 20);
        road({ 99,197,100,2 }, 20, 20);
        road({ 99,107,2,92 }, 20, 20);
        road({ 99,107,50,2 }, 20, 20);
        road({ 149,107,2,50 }, 20, 20);

        break;
    }
    case my_enums::S_HOMETOWN_:
    {
        //draw some random features
        for (int x = 1; x < 256; x++) {
            for (int y = 1; y < 256; y++) {
                int rFeature = dice(80, 1);
                if(rFeature == 1 )map_cells[x][y] = 341;
                if (rFeature == 2)map_cells[x][y] = 342;
                if (rFeature == 3)map_cells[x][y] = 343;
               // if (rFeature == 4)map_cells[x][y] = 344;
            }
        }

        //Draw town roads
        road({ 1,50,100,2 }, 90, 96);
        road({ 101,50,2,100 }, 90, 96);
        road({ 101,150,155,2 }, 90, 96);
        road({ 109,153,0,10 }, 90, 96);


        //Draw door to the Eastern coast
        building({ 251,149,4,4 }, 9, 104, 251, 151, 38);

        //Draw major building
        building({ 105,140,8,9 }, 9, 104, 109, 149, 38);

        //Draw elements building
        building({ 115,140,8,9 }, 9, 104, 119, 149, 54);

         //Draw pet´s hut
        building({ 116,120,4,5 }, 9, 104, 118, 120, 38);

        //Draw master´s building
        building({ 116,153,4,4 }, 9, 104, 118, 153, 38);

       

        //Draw portals
       // map_cells[1][143] = 56;
        map_cells[253][151] = 56;
        

        //Draw portals to elemental worlds
        map_cells[117][142] = 47;
        map_cells[121][142] = 48;
        map_cells[117][144] = 49;
        map_cells[121][144] = 50;


        //Draw fire pits
        map_cells[106][142] = 66;
        map_cells[112][142] = 66;
        map_cells[106][144] = 66;
        map_cells[112][144] = 66;
        map_cells[106][146] = 66;
        map_cells[112][146] = 66;

        //Draw fountain
        map_cells[109][151] = 579;

        //Draw town shops
        building({ 104,153,4,4 }, 9, 247, 108, 155, 37);
        building({ 104,159,4,4 }, 9, 247, 108, 161, 37);
        building({ 110,153,4,4 }, 9, 247, 110, 155, 37);
        building({ 110,159,4,4 }, 9, 247, 110, 161, 37);

        //draw road to cemetary
        road({ 139,153,0,10 }, 90, 96);

        //Draw cemetary
        building({ 134,153,4,4 }, 9, 247, 138, 155, 51);
        building({ 134,159,4,4 }, 9, 247, 138, 161, 51);
        building({ 140,153,4,4 }, 9, 247, 140, 155, 51);
        building({ 140,159,4,4 }, 9, 247, 140, 161, 51);

        //crypt
        building({ 137,164,4,7 }, 9, 247, 139, 165, 44);
        //stairs down
        map_cells[139][163] = 40;

        //tomstones
        map_cells[138][165] = 540;
        map_cells[140][165] = 540;
        map_cells[138][167] = 540;
        map_cells[140][167] = 540;
        map_cells[138][169] = 540;
        map_cells[140][169] = 540;

        //map_cells[136][172] = 540;

        //altar
        map_cells[139][170] = 68;



        //Draw bones
        map_cells[135][155] = 61;
        map_cells[135][161] = 61;

        map_cells[143][155] = 61;
        map_cells[143][161] = 61;

        break;
    }

    default:
    {
        // is likely to be an error
        break;
    }
    };




    

}//generate tiles

int classMap::dice(int maxValue, int minValue)
{
    return rand() % maxValue + minValue;
}

void classMap::road(SDL_Rect square, int floorTile1, int floorTile2) {
    for (int x = square.x; x <= (square.x + square.w); x++)
    {
        for (int y = square.y; y <= (square.y + square.h); y++)
        {
            if (dice(4, 1) < 4) {
                map_cells[x][y] = floorTile1;
            }
            else {
                map_cells[x][y] = floorTile2;
            }

        }
    }
}

void classMap::openBuilding(SDL_Rect square, int wallTile, int floorTile, int doorTile, int specialTile)
{

    for (int x = square.x; x <= (square.x + square.w); x++)
    {
        for (int y = square.y; y <= (square.y + square.h); y++)
        {
            map_cells[x][y] = floorTile;

        }
    }

    for (int x = square.x; x <= (square.x + square.w); x++)
    {
        map_cells[x][square.y] = wallTile;
        map_cells[x][(square.y + square.h)] = wallTile;
    }

    for (int y = square.y; y <= (square.y + square.h); y++)
    {
        map_cells[square.x][y] = wallTile;
        map_cells[(square.x + square.w)][y] = wallTile;
    }

    map_cells[square.x+ square.w/2][square.y] = doorTile;
    map_cells[square.x + square.w / 2][square.y+square.h] = doorTile;
    map_cells[square.x][square.y + square.h/2] = doorTile;
    map_cells[square.x+square.w][square.y + square.h/2] = doorTile;

    map_cells[square.x + square.w/2][square.y + square.h / 2] = specialTile;
}

void classMap::building(SDL_Rect square, int wallTile, int floorTile, int x, int y, int doorTile)
{
    for (int x = square.x; x <= (square.x+square.w); x++)
    {
        for (int y = square.y; y <= (square.y + square.h); y++)
        {
            map_cells[x][y] = floorTile;
            
        }
    }

    for (int x = square.x; x <= (square.x + square.w); x++)
    {
        map_cells[x][square.y] = 9;
        map_cells[x][(square.y + square.h)] = 9;
    }

    for (int y = square.y; y <= (square.y + square.h); y++)
    {
        map_cells[square.x][y] = 9;
        map_cells[(square.x + square.w)][y] = 9;
    }

    map_cells[x][y] = doorTile;

}

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
          /*  if (y < 80) {
                pixel = SDL_MapRGBA(imageSurface->format, 255, 0, 0, 0);
            }else  {
                pixel = SDL_MapRGBA(imageSurface->format, 0, 255, 0, 0);

            }

            if (x > 80) {
                pixel = SDL_MapRGBA(imageSurface->format, 0, 0, 255, 0);
            }
            */
            
            
            int a;
            a = ((x+(y*256)) % 3);
            switch (a) {
            case 0: { pixel = SDL_MapRGBA(imageSurface->format, 255, 0, 0, 0); break; }
            case 1: { pixel = SDL_MapRGBA(imageSurface->format, 0, 255, 0, 0); break; }
            case 2: { pixel = SDL_MapRGBA(imageSurface->format, 0, 0, 255, 0); break; }
            }
            
            

              //  pixel = SDL_MapRGBA(imageSurface->format, x, y, 0, 0);
          
            
            
            put_pixel32(imageSurface, x, y, pixel); // r g b  a

        }

    }

}

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
    //targetSurface = SDL_CreateRGBSurface(0, 600, 400, 32, rmask, gmask, bmask, amask);
    targetSurface = SDL_CreateRGBSurface(0, 200, 100, 32, rmask, gmask, bmask, amask);

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


int classMap::getBlur()
{
    return blur_extent;
}

void classMap::setBlur(int blur)
{
    blur_extent = blur;

}