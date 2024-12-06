#include "game.h"
#include <exception>
#include <string>
#include <iostream>
#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

std::string GetExeFileName()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

std::string GetExePath()
{
    std::string f = GetExeFileName();
    return f.substr(0, f.find_last_of("\\/"));
}


game::game()
{
    /* initialize random seed: */
    srand((unsigned int)time(NULL));

#ifdef linux
    basePath = "/home/TFG/engine/Debug/";
#endif
#ifdef _WIN32

    basePath = GetExePath() + "\\";
#endif

}

game::~game()
{
    //dtor
}

int game::dice(int maxValue, int minValue)
{
    return rand() % maxValue + minValue;
}


void game::addNotification(std::string notification)
{
    if (notifications.size() == 0) {
        popupTime = timer.getTicks();

    }
    notifications.push_back(notification);
}


void game::addAchievement(std::string achievementName, my_enums::Achievements achievementType)
{
    if (!existAchievement(achievementName, achievementType )) {
        achievement newAchievement;
        newAchievement.name = achievementName;
        newAchievement.type = achievementType;

        achievements.push_back(newAchievement);
        addNotification(achievementName);
        Mix_PlayChannel(-1, win, 0);

        int achievementCounter = achievements.size();
      //  achievementsButton.setCaption("ACH " + std::to_string(achievementCounter) + " / " + std::to_string(achievementCounter * 100 / maxAchievements) + "%");
         achievementsButton.setCaption(std::to_string(achievementCounter*2) +" %");
        //"Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60)
    }
}


bool game::existAchievement(std::string achievementName, my_enums::Achievements achievementType)
{
    bool returnValue = false;

    for (achievement currentAchievement : achievements) {
        if ((currentAchievement.name == achievementName) && (currentAchievement.type == achievementType) ){
            returnValue = true;
            break;
        }
    }

    return returnValue;
}

void game::eventsNotifications()
{
    if (notifications.size() > 0) {
        int currentTicks = timer.getTicks();
        if ((currentTicks - popupTime) > 2000) {
            popupTime = currentTicks;
            notifications.pop_front();
        }
    }
}

void game::deleteNPCs(int x, int y)
{

        bool empty = false;
        while (empty != true)
        {
            empty = true;
            for (list<NPC>::iterator it = NPCs.begin(); it != NPCs.end(); it++)
            {
                if ((it->x == x) && (y == it->y)){
                    empty = false;
                    NPCs.erase(it);
                    break;

                }
            }
        }
    

}

list<NPC> game::getNPCs(int x, int y)
{
    list<NPC> tmp;
    for (list<NPC>::iterator it = NPCs.begin(); it != NPCs.end(); it++)
    {
        if ((x == it->x) && (y == it->y)) {
            NPC aNPC;

            aNPC.id = it->id;
            aNPC.x = it->x;
            aNPC.y = it->y;
            aNPC.map = it->map;
            aNPC.description = it->description;
            aNPC.skill = it->skill;
            aNPC.stamina = it->stamina;
            aNPC.exp = it->exp;
            aNPC.NPCAI = it->NPCAI;
            aNPC.tile = it->tile;

            /* s_Foe aFoe;
            aFoe.id = it->id;
            aFoe.description = it->description;
            aFoe.skill = it->skill;
            aFoe.stamina = it->stamina;
            */
           
            tmp.push_back(aNPC);
        }
    }

    return tmp;
}

void game::phaseNPCs()
{
    if (getState() != my_enums::_FIGHT_){
     tmpNPCs = getNPCs(px,py);
     if (tmpNPCs.size() > 0) {
         //addNotification("There are NPCS here...");
         //currentMusic = 5;
         Mix_PlayMusic(musicBATTLE, -1);
         previousScreen = my_enums::_HOMETOWN_;
         setState(my_enums::_FIGHT_);
        


     }
    }

}


void game::start()
{
    timer.start();
    FPStimer.start();
    timer.pause();
    timerGameOver.start();
    timerGameOver.pause();
    //initSDL();

    px = 75;
    py = 75;
    cam_x = 70;
    cam_y = 70;

    randomAttributes();


    NPC aNPC;
    aNPC.id = 1;
    aNPC.x = 78;
    aNPC.y = 75;
    aNPC.map = my_enums::_HOMETOWN_;
    aNPC.description = "NPC UNO";
    aNPC.skill = rand() % 10 + 1;
    aNPC.stamina = rand() % 10 + 5;
    aNPC.exp = 1;
    aNPC.NPCAI = my_enums::_FRIENDLY_STATIC_;
    aNPC.tile = rand() % 300 + 1;
    NPCs.push_back(aNPC);


    aNPC.description = "NPC DOS";
    aNPC.id = 2;
    aNPC.x = 80;
    aNPC.y = 75;
    aNPC.skill = rand() % 10 + 1;
    aNPC.stamina = rand() % 10 + 5;
    aNPC.tile = rand() % 300 + 1;
    NPCs.push_back(aNPC);

    aNPC.description = "NPC TRES";
    aNPC.id = 3;
    aNPC.x = 82;
    aNPC.y = 75;
    aNPC.skill = rand() % 10 + 10;
    aNPC.stamina = rand() % 10 + 11;
    aNPC.tile = rand() % 300 + 1;
    NPCs.push_back(aNPC);

    aNPC.description = "NPC CUATRO";
    aNPC.id = 4;
    aNPC.x = 78;
    aNPC.y = 75;
    aNPC.skill = rand() % 10 + 1;
    aNPC.stamina = rand() % 10 + 11;
    aNPC.tile = rand() % 300 + 1;
    NPCs.push_back(aNPC);

//Base map creation
    baseMap.createSurface();
    int width, height;
    width = 256;
    height = 256;
    baseMap.mymap.init();

    int octave = rand() % 4 + 3;
    float frequency = rand() % 8 + 1;
    //float frequency = 1.0f;
    float persistance = 0.5f;

    baseMap.mymap.generate(octave, frequency, persistance, 1, 1, width, height);
    baseMap.mymap.to_surface(baseMap.imageSurface, my_enums::_HOMETOWN_);

    baseMap.imageSurface = SDL_ConvertSurface(baseMap.imageSurface, gScreenSurface->format, 0);

    //baseMap.SetSurface();

    updateMap();

 
    // baseMap.targetSurface = SDL_ConvertSurface(baseMap.targetSurface, gScreenSurface->format, 0);

    baseMap.blur();

    //Set game framerate
    ticksPerFrame = (int)(1000 / desiredFPS);

    //Hide Window´s cursor
    SDL_ShowCursor(SDL_DISABLE);


    //Set button´s size, position and caption
    moveLeftButton.setButton(0, gScreenSurface->h / 2, 128, 128, "A");
    moveLeftButton.setColor(0, 0, 200);
    moveRightButton.setButton(276 , gScreenSurface->h / 2, 128, 128, "D");
    moveRightButton.setColor(0, 0, 200);
    moveUpButton.setButton(138, gScreenSurface->h / 2 -138, 128, 128, "W");
    moveUpButton.setColor(0, 0, 200);
    moveDownButton.setButton(138, gScreenSurface->h / 2, 128, 128, "S");
    moveDownButton.setColor(0, 0, 200);


    nextButton.setButton(gScreenSurface->w / 2 +128, gScreenSurface->h /2, 128, 128, "Siguiente");
    nextButton.setColor(128, 128, 128);

    prevButton.setButton(gScreenSurface->w / 2 - 256, gScreenSurface->h /2, 128, 128, "Anterior");
    prevButton.setColor(128, 128, 128);

    exitButton.setButton(gScreenSurface->w - 128, 0, 128, 128, "Salir");
    exitButton.setColor(200, 0, 0);

    continueButton.setButton(gScreenSurface->w / 2 +32 , gScreenSurface->h - 128, 128, 128, "Continuar");
    continueButton.setColor(0, 0, 200);

    backButton.setButton(gScreenSurface->w / 2 - 160 , gScreenSurface->h - 128, 128, 128, "Volver");
    backButton.setColor(0, 0, 200);

    rollButton.setButton(gScreenSurface->w / 2 - 64, gScreenSurface->h /2, 128, 128, "Volver a tirar");
    rollButton.setColor(0, 0, 200);

    startButton.setButton(gScreenSurface->w / 2 -64, gScreenSurface->h / 2 -64, 128, 128, "Jugar");
    startButton.setColor(100, 100, 100);

    player1Button.setButton(gScreenSurface->w / 2, gScreenSurface->h / 2 + 100, 200, 50, "Nombre Jugador");
    player1Button.setColor(100, 100, 100);


    configButton.setButton(1, gScreenSurface->h -128, 128, 128, "Configuración");
    configButton.setColor(100, 100, 100);
    inventoryButton.setButton(129, gScreenSurface->h - 128, 128, 128, "Inventario");
    inventoryButton.setColor(0, 0, 200);
    playerButton.setButton(257, gScreenSurface->h - 128, 128, 128, "Personaje");
    playerButton.setColor(0, 0, 200);
    newMapButton.setButton(385, gScreenSurface->h - 128, 128, 128, "New map");
    newMapButton.setColor(200, 200, 200);
    achievementsButton.setButton(513, gScreenSurface->h - 128, 128, 128, "0 %");
    achievementsButton.setColor(0, 0, 200);

    musicButton.setButton(gScreenSurface->w / 2-100, gScreenSurface->h / 2 + 100, 200, 50, "Cambia canción");
    musicButton.setColor(100, 100, 100);
    muteButton.setButton(gScreenSurface->w / 2 -100, gScreenSurface->h / 2 + 150, 200, 50, "MUSIC ON");
    muteButton.setColor(100, 100, 100);
    volumeSoundDownButton.setButton(gScreenSurface->w / 2 -100, gScreenSurface->h / 2 + 200, 50, 50, "+");
    volumeSoundDownButton.setColor(100, 100, 100);
    volumeSoundButton.setButton(gScreenSurface->w / 2 + 50 - 100, gScreenSurface->h / 2 + 200, 100, 50, "SOUND 100%");
    volumeSoundButton.setColor(200, 200, 200);
    volumeSoundUpButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 2 + 200, 50, 50, "-");
    volumeSoundUpButton.setColor(100, 100, 100);
    volumeMusicDownButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 2 + 250, 50, 50, "+");
    volumeMusicDownButton.setColor(100, 100, 100);
    volumeMusicButton.setButton(gScreenSurface->w / 2 + 50 - 100, gScreenSurface->h / 2 + 250, 100, 50, "MUSIC 100%");
    volumeMusicButton.setColor(200, 200, 200);
    volumeMusicUpButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 2 + 250, 50, 50, "-");
    volumeMusicUpButton.setColor(100, 100, 100);
    
    playerUpButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 2 + 250, 50, 50, "+");
    playerUpButton.setColor(100, 100, 100);
    playerDownButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 2 + 250, 50, 50, "-");
    playerDownButton.setColor(100, 100, 100);

    blurButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 2 + 300, 200, 50, "BLUR ON");
    blurButton.setColor(100, 100, 100);

    mouseButton.setButton(gScreenSurface->w / 2 - 100, 0, 200, 50, "X:" + std::to_string(mousex) + " Y:" + std::to_string(mousey));
    mouseButton.setColor(100, 100, 100);

    
    fightButton.setButton(1, gScreenSurface->h -128, 128, 128, "Atacar");
    fightButton.setColor(100, 100, 100);
    spellButton.setButton(129, gScreenSurface->h -128, 128, 128, "Magia");
    spellButton.setColor(100, 100, 100);

    potionHealthButton.setButton(257, gScreenSurface->h - 128, 128, 128, "+Salud");
    potionHealthButton.setColor(100, 100, 100);
    potionMagicButton.setButton(385, gScreenSurface->h - 128, 128, 128, "+Magia");
    potionMagicButton.setColor(100, 100, 100);

    
    blurButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 2 + 300, 200, 50, "BLUR ON");
    blurButton.setColor(100, 100, 100);


    //Play Intro music
    Mix_PlayMusic(musicINTRO, -1);
}

//Initialize SDL
bool game::initSDL()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        SDL_DisplayMode DM;
        //Get screen MAX size
        SDL_GetCurrentDisplayMode(0, &DM);
        auto Width = DM.w;
        auto Height = DM.h;
        Width = width;
        Height = height;


        //Create window
        gWindow = SDL_CreateWindow("Achievement Master", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_FULLSCREEN_DESKTOP);
        //gWindow = SDL_CreateWindow("Achievement Master", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_FULLSCREEN);

        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }
            else
            {
                //Get window surface
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }

            //Initialize SDL_ttf
            if (TTF_Init() == -1)
            {
                printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                success = false;
            }

            //Initialize SDL_mixer
            if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
            {
                return false;
            }

        }
    }

    return success;
}

//Close SDL
void game::closeSDL()
{

    //Free global font
    TTF_CloseFont(gFont);
    gFont = NULL;

    //Deallocate surface
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    //Free the sound effects
 /*Mix_FreeChunk( scratch );
 Mix_FreeChunk( audioSword );
 Mix_FreeChunk( audioFlesh );
 Mix_FreeChunk( audioMaleDeath );

 //Free the music
 Mix_FreeMusic( musicFight );*/
    Mix_FreeMusic(musicINTRO);
    Mix_FreeMusic(musicDARK);
    Mix_FreeMusic(musicFOREST);
    Mix_FreeMusic(musicTOWN);
    Mix_FreeMusic(musicGameOver);
    Mix_FreeMusic(musicBATTLE);
    Mix_FreeMusic(musicBOSS);
    Mix_FreeMusic(musicCAVE);
    Mix_FreeMusic(musicHERO);
    Mix_FreeMusic(musicMYSTICAL);


 //Quit SDL_mixer
    Mix_CloseAudio();

    SDL_DestroyRenderer(gRenderer);
    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();





    //Quit SDL subsystems
     //Quit SDL_mixer
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    exit(0);
}


//get Current game state
int game::getState()
{
    return currentState;
}

//Set Current game state
void game::setState(my_enums::gameState newState)
{
    currentState = newState;
}


std::string game::getBasePath()
{
    return basePath;
}

//Process game events for each state
void game::events()
{
    switch (getState())
    {
    case my_enums::_INTRO_:
    {
        eventsIntro();
        break;
    }
    case my_enums::_MAINMENU_:
    {
        eventsMain();
        break;
    }

    case my_enums::_HERO_:
    {
        eventsHero();
        break;
    }

    case my_enums::_NAME_:
    {
        eventsPlayerName();
        break;
    }

    case my_enums::_ATTRIBUTES_:
    {
        eventsPlayerAttributes();
        break;
    }

    case my_enums::_PLAYER_:
    {
        eventsPlayer();
        break;
    }

    case my_enums::_RACES_:
    {
        eventsPlayerRaces();
        break;
    }

    case my_enums::_ARCHETYPES_:
    {
        eventsPlayerArchetypes();
        break;
    }

    case my_enums::_INVENTORY_:
    {
        eventsInventory();
        break;
    }

    case my_enums::_ACHIEVEMENTS_:
    {
        eventsAchievements();
        break;
    }

    case my_enums::_FIGHT_:
    {
        eventsFight();
        break;
    }

    case my_enums::_GAMEOVER_:
    {
        eventsGameOver();
        break;
    }
   /* case _GENERATEPLAYER_:
    {
        eventsGeneratePlayer();
        break;
    }
    */

    case my_enums::_CONFIGMENU_:
    {
        eventsConfigMenu();
        break;
    }

    case my_enums::_FOREST_WORLD_: case my_enums::_COAST_WORLD_: case my_enums::_ELEMENTAL_FIRE_WORLD_: case my_enums::_ELEMENTAL_WATER_WORLD_: case my_enums::_ELEMENTAL_EARTH_WORLD_: case my_enums::_ELEMENTAL_WIND_WORLD_: case my_enums::_NECRO_WORLD_:
    case my_enums::_HOMETOWN_:

    {
        eventsHomeTown();
        break;
    }

    default:
    {
        // is likely to be an error
    }
    };
}

//Screens for each state
void game::drawScreens()
{
    switch (getState())
    {
    case my_enums::_INTRO_:
    {
        screenIntro();
        break;
    }
    case my_enums::_MAINMENU_:
    {
        screenMain();
        break;
    }

    case my_enums::_HERO_:
    {
        screenHero();
        break;
    }

    case my_enums::_NAME_:
    {
        screenPlayerName();
        break;
    }

    case my_enums::_ATTRIBUTES_:
    {
        screenPlayerAttributes();
        break;
    }

    case my_enums::_PLAYER_:
    {
        screenPlayer();
        break;
    }

    case my_enums::_GAMEOVER_:
    {
        screenGameOver();
        break;
    }

    case my_enums::_RACES_:
    {
        screenPlayerRaces();
        break;
    }

    case my_enums::_ARCHETYPES_:
    {
        screenPlayerArchetypes();
        break;
    }

    case my_enums::_INVENTORY_:
    {
        screenInventory();
        break;
    }

    case my_enums::_ACHIEVEMENTS_:
    {
        screenAchievements();
        break;
    }

    case my_enums::_FIGHT_:
    {
        screenFight();
        break;
    }
    case my_enums::_CONFIGMENU_:
    {
        screenConfigMenu();
        break;
    }

    case my_enums::_FOREST_WORLD_: case my_enums::_COAST_WORLD_: case my_enums::_ELEMENTAL_FIRE_WORLD_: case my_enums::_ELEMENTAL_WATER_WORLD_: case my_enums::_ELEMENTAL_EARTH_WORLD_: case my_enums::_ELEMENTAL_WIND_WORLD_: case my_enums::_NECRO_WORLD_:
    case my_enums::_HOMETOWN_:
    {
        screenHomeTown();
        break;
    }

    default:
    {
        // is likely to be an error
    }
    };


}

//Flip backscreen buffer to monitor
void game::screenFlip()
{
    //Paint Mouse Over everything
    SDL_PumpEvents();
    SDL_GetMouseState(&mousex, &mousey);
    mouseRect.x = mousex - 10;
    mouseRect.y = mousey;
    //mouseRect.w = mouseSurface->w * 3;
    //mouseRect.h = mouseSurface->h * 3;


    mouseRect.w = mouseSurface->w;
    mouseRect.h = mouseSurface->h;


    SDL_RenderCopy(gRenderer, mouseTexture, NULL, &mouseRect);

    //mouseButton.setCaption("X:" + std::to_string(mousex) + " Y:" + std::to_string(mousey));
    //drawButton(mouseButton);

    //Flip image to monitor
    SDL_RenderPresent(gRenderer);
}

//Adjust frame time to comply with desired framerate
void game::adjustFPS()
{
    currentFrame = FPStimer.getTicks();
    if (currentFrame < ticksPerFrame) {
            SDL_Delay(ticksPerFrame - currentFrame);
    }
    FPStimer.reset();

}

//Load image into SDL_Surface
SDL_Surface* game::loadSurface(std::string path)
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL)
        {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //  SDL_SetColorKey( optimizedSurface, SDL_TRUE, SDL_MapRGB( mouseSurface->format, 0xFF, 0xFF, 0xFF ) );
    return optimizedSurface;
}

//Load image into SDL_Texture
SDL_Texture* game::loadTexture(std::string path)
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;


    SDL_Texture* tmpTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        SDL_SetColorKey(optimizedSurface, SDL_TRUE, // enable color key (transparency)
            //SDL_MapRGB(playersSurface->format, 0xFF, 0, 0xFF)); // This is the color that should be taken as being the 'transparent' part of the image
            SDL_MapRGB(optimizedSurface->format, 0, 0, 0));
        if (optimizedSurface == NULL)
        {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    tmpTexture = SDL_CreateTextureFromSurface(gRenderer, optimizedSurface);
    SDL_FreeSurface(optimizedSurface);
    return tmpTexture;
}

//Load all media files (images, sounds and music)
bool game::loadMedia(string base)
{
    string images = base + "images\\";
    string sounds = base + "sounds\\";
    string fonts = base + "fonts\\";
    string aFile = images + "food.png";

    //Loading success flag
    bool success = true;

    string cursorFile = images + "cursor5b.png";
    mouseSurface = loadSurface(cursorFile);


    string tmpFile;

    SDL_SetColorKey(mouseSurface, SDL_TRUE, // enable color key (transparency)
        SDL_MapRGB(mouseSurface->format, 0x00, 0x00, 0x00)); // This is the color that should be taken as being the 'transparent' part of the image
    mouseTexture = SDL_CreateTextureFromSurface(gRenderer, mouseSurface);

    aFile = images + "creatures_alpha.png";
    playersSurface = loadSurface(aFile);
    SDL_SetColorKey(playersSurface, SDL_TRUE, // enable color key (transparency)
    //SDL_MapRGB(playersSurface->format, 0xFF, 0, 0xFF)); // This is the color that should be taken as being the 'transparent' part of the image
    SDL_MapRGB(playersSurface->format, 71, 108, 108)); // This is the color that should be taken as being the 'transparent' part of the image

    playersTexture = SDL_CreateTextureFromSurface(gRenderer, playersSurface);

   
    aFile = images + "15771169063_9f6d64ce74_b.jpg";

     // aFile = images + "12594319284_aac8307463_b.jpg";
bg1Surface = loadSurface(aFile);
bg1Texture = SDL_CreateTextureFromSurface(gRenderer, bg1Surface);

aFile = images + "334968999_40607c07ce_b.jpg";
bg2Surface = loadSurface(aFile);
bg2Texture = SDL_CreateTextureFromSurface(gRenderer, bg2Surface);


buttonCloseTexture    = loadTexture(images + "exit-door.png");
//buttonNextTexture     = loadTexture(images + "clockwise-rotation.png");
//buttonPrevTexture     = loadTexture(images + "anticlockwise-rotation.png");
buttonNextTexture = loadTexture(images + "next-button.png");
buttonPrevTexture     = loadTexture(images + "previous-button.png");

buttonAcceptTexture   = loadTexture(images + "check-mark.png");
buttonSoundOffTexture = loadTexture(images + "speaker-off.png");
buttonSoundOnTexture  = loadTexture(images + "speaker.png");
buttonConfigTexture   = loadTexture(images + "settings-knobs.png");
buttonRollDiceTexture = loadTexture(images + "rolling-dices.png");
buttonMapTexture      = loadTexture(images + "treasure-map.png");
buttonBackpackTexture = loadTexture(images + "backpack.png");
buttonPlayTexture     = loadTexture(images + "play-button.png");
buttonCancelTexture   = loadTexture(images + "cancel.png");
buttonStarsTexture    = loadTexture(images + "stars-stack.png");

buttonUpTexture = loadTexture(images + "up-button.png");
buttonDownTexture = loadTexture(images + "down-button.png");
buttonLeftTexture = loadTexture(images + "left-button.png");
buttonRightTexture = loadTexture(images + "right-button.png");

buttonPlayerTexture = loadTexture(images + "histogram.png");

buttonSwordTexture = loadTexture(images + "sword-brandish.png");
buttonSpellTexture = loadTexture(images + "spell-book.png");
buttonPotionHealthTexture = loadTexture(images + "potion-health.png");
buttonPotionMagicTexture = loadTexture(images + "potion-magic.png");


    string TTFFile = fonts + "712_serif.ttf";
    //Open the font
    gFont = TTF_OpenFont(TTFFile.c_str(), 28);
    if (gFont == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = { 0, 0, 0 };

    }

    string MUSIC = sounds + "Heroic_Demise.mp3";
    musicINTRO = Mix_LoadMUS(MUSIC.c_str());

    MUSIC = sounds + "The_Dark_Amulet.mp3";
    musicDARK = Mix_LoadMUS(MUSIC.c_str());

    MUSIC = sounds + "forest.ogg";
    musicFOREST = Mix_LoadMUS(MUSIC.c_str());

    MUSIC = sounds + "Town_Theme.mp3";
    musicTOWN = Mix_LoadMUS(MUSIC.c_str());

    MUSIC = sounds + "game_over_bad_chest.wav";
    musicGameOver = Mix_LoadMUS(MUSIC.c_str());

    MUSIC = sounds + "BattleThemeA.mp3";
    musicBATTLE = Mix_LoadMUS(MUSIC.c_str());

    MUSIC = sounds + "TLE_Digital_Loop_Long.wav";
    musicBOSS = Mix_LoadMUS(MUSIC.c_str());

    MUSIC = sounds + "Cave_Theme.ogg";
    musicCAVE = Mix_LoadMUS(MUSIC.c_str());

    MUSIC = sounds + "Hero_Immortal.mp3";
    musicHERO = Mix_LoadMUS(MUSIC.c_str());

    MUSIC = sounds + "Mystical_Theme.mp3";
    musicMYSTICAL = Mix_LoadMUS(MUSIC.c_str());

    // MUSIC = sounds + "";
     //music = Mix_LoadMUS(MUSIC.c_str());



    string Audio6 = sounds + "click.wav";
    audioButton = Mix_LoadWAV(Audio6.c_str());


    /* Audio6 = sounds + "line.wav";
    line = Mix_LoadWAV( Audio6.c_str());

     Audio6 = sounds + "block.wav";
     block = Mix_LoadWAV( Audio6.c_str());

     Audio6 = sounds + "loose.wav";
     loose = Mix_LoadWAV( Audio6.c_str());
     */

    Audio6 = sounds + "achievement.wav";
    win = Mix_LoadWAV(Audio6.c_str());



    return success;
}

//Draw a filled square
void game::drawSquare(SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderFillRect(gRenderer, &rect);
}

//Draw a square
void game::drawTransparentSquare(SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderDrawRect(gRenderer, &rect);
}

//Draw a button
void game::drawButton(classButton btn)
{
    drawSquare(btn.getRect(), btn.getColor());
    SDL_Color fg = { 0,0,0,0 };
    SDL_Surface* text = TTF_RenderUTF8_Solid(gFont, btn.getCaption().c_str(), fg);

    SDL_Rect target = btn.getRect();
    if (text->w <= target.w) {
        target.x = target.x + target.w / 2 - text->w / 2;
        target.w = text->w;
    }

    target.y = target.y + target.h / 2 - text->h / 2;
    target.h = text->h;
    SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, text);
    SDL_RenderCopy(gRenderer, txtTexture, NULL, &target);

    SDL_FreeSurface(text);
    text = NULL;
    SDL_DestroyTexture(txtTexture);
    txtTexture = NULL;

}

//Draw game map
void game::drawMap()
{
    SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, baseMap.targetSurface);
    SDL_RenderCopy(gRenderer, txtTexture, NULL, NULL);

    SDL_Rect destRect;
    destRect.x = 1;
    destRect.y = 1;
    destRect.w = 256;
    destRect.h = 256;

    SDL_Texture* miniMapTexture = SDL_CreateTextureFromSurface(gRenderer, baseMap.imageSurface);
    SDL_RenderCopy(gRenderer, miniMapTexture, NULL, &destRect);

    SDL_DestroyTexture(miniMapTexture);
    miniMapTexture = NULL;
    
    SDL_DestroyTexture(txtTexture);
    txtTexture = NULL;

    SDL_SetRenderDrawColor(gRenderer,200, 0, 0,0);
    destRect.x = px;
    destRect.y = py;
    destRect.w = cam_size_x;
    destRect.h = cam_size_y;
    SDL_RenderDrawRect(gRenderer, &destRect);
}

//Update section of map we display on screen
void game::updateMap()
{
    if (px <= cam_x) { cam_x--; }
    if (py <= cam_y) { cam_y --; }
    if (px >= (cam_x + cam_size_x -1)) { cam_x ++; }
    if (py >= (cam_y + cam_size_y -1)) { cam_y++; }


    SDL_Rect srcrect;
    srcrect.x = cam_x;
    srcrect.y = cam_y;

    srcrect.w = cam_size_x;
    srcrect.h = cam_size_y;



    SDL_BlitScaled(baseMap.imageSurface, &srcrect, baseMap.targetSurface, NULL);

    if (baseMap.getBlur() > 0) {
        baseMap.blur();
    }
}

void game::drawBackground()
{
    {
      /*  tmpRect.x = 1;
        tmpRect.y = 1;
        tmpRect.w = 400;
        tmpRect.h = 400;
        */
     //   SDL_Color fg = { 0,0,0,0 };

       // drawSquare(tmpRect, fg);
        if (getState() == my_enums::_INTRO_) {
        SDL_RenderCopy(gRenderer, bg1Texture, NULL, NULL);
        }
        else {
            SDL_RenderCopy(gRenderer, bg2Texture, NULL, NULL);

        }
      //  tmpRect.x = 401;
       // SDL_RenderCopy(gRenderer, playersTexture, NULL, &tmpRect);
    }

}


//Draw a button with a texture
void game::drawButtonSrc(classButton btn, SDL_Texture* texture)
{
    SDL_Rect target = btn.getRect();
    drawSquare(target, { 100, 100, 100, 0 });

    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0);
    SDL_RenderDrawLine(gRenderer,target.x,target.y, target.x+target.w, target.y);
    SDL_RenderDrawLine(gRenderer, target.x, target.y, target.x , target.y + target.h);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderDrawLine(gRenderer, target.x, target.y+target.h, target.x + target.w, target.y + target.h);
    SDL_RenderDrawLine(gRenderer, target.x+target.w, target.y, target.x+target.w, target.y + target.h);
    SDL_RenderCopy(gRenderer, texture, NULL, &target);

    SDL_Color fg = { 0,0,0,0 };
    SDL_Color bg = { 150,150,150,0 };
    SDL_Surface* text = TTF_RenderUTF8_Solid(gFont, btn.getCaption().c_str(), fg);

    if (text->w <= target.w) {
        target.x = target.x + target.w / 2 - text->w / 2;
        target.w = text->w;
    }

    target.y = target.y + target.h / 2 - text->h / 2;
    target.h = text->h;
    SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, text);
    SDL_RenderCopy(gRenderer, txtTexture, NULL, &target);
}

void game::drawIMGBox(int x, int y, int value, int max, SDL_Color color)
{
    int tmp;
   // SDL_Texture* IMGTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_Rect target;
    target.x = x;
    target.y = y;
    target.w = 20;
    target.h = 20;
    //SDL_RenderCopy(gRenderer, IMGTexture, NULL, &target);
    //drawBo
   if (value > 0) drawSquare(target, color);
   // SDL_Color fg = { 0,0,0,0 };

    while (value > 0) {
        //SDL_RenderCopy(gRenderer, IMGTexture, NULL, &target);
        drawSquare(target, color);
        target.x += 20;
        value--;
    }

    color = { 0,0,0,0 };
    target.x = x;
    tmp = max;
    while (tmp > 0) {
        //drawTransparentSquare(&target ,color);
      //SDL_RenderCopy( gRenderer, surface, NULL, &target );
        drawTransparentSquare(target, color);
        target.x += 20;
        tmp--;
    }
}

//Draw image into screen
void game::drawIMG(SDL_Surface* surface, int x, int y, int value)
{
    SDL_Texture* IMGTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_Rect target;
    target.x = x;
    target.y = y;
    target.w = 20;
    target.h = 20;
    SDL_RenderCopy(gRenderer, IMGTexture, NULL, &target);

    while (value > 0) {
        SDL_RenderCopy(gRenderer, IMGTexture, NULL, &target);
        target.x += 20;
        value--;
    }
}

//Draw a text into screen
void game::drawText(string text, SDL_Rect rect)
{
    //drawSquare(btn.getRect(),btn.getColor());
    SDL_Color fg = { 0,0,0,0 };
    if (text.length() > 0) {
        // UTF8
         //TTF_RenderUTF8_Blended
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(gFont, text.c_str(), fg);
        // SDL_Surface* textSurface = TTF_RenderText_Solid(gFont,text.c_str(),fg);
        //SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(gFont, text.c_str(), fg, 80);
         /*SDL_Rect target = btn.getRect();
         target.x = target.x + target.w/2 - text->w/2;
         target.y = target.y + target.h/2 - text->h/2;*/
         // SDL_BlitSurface(textSurface,NULL,gScreenSurface,&rect);

        if (textSurface->w <= rect.w) { rect.w = textSurface->w; }
        rect.h = textSurface->h;
        SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, txtTexture, NULL, &rect);
        SDL_FreeSurface(textSurface);
        textSurface = NULL;
        SDL_DestroyTexture(txtTexture);
        txtTexture = NULL;
    }
}

//Draw a text into screen aligned
void game::drawTextL(string text, SDL_Rect rect)
{
    //drawSquare(btn.getRect(),btn.getColor());
    SDL_Color fg = { 0,0,0,0 };
    if (text.length() > 0) {
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(gFont, text.c_str(), fg);
        /*SDL_Rect target = btn.getRect();
        target.x = target.x + target.w/2 - text->w/2;
        target.y = target.y + target.h/2 - text->h/2;*/
        // SDL_BlitSurface(textSurface,NULL,gScreenSurface,&rect);

        rect.x = rect.x - rect.w;
        //rect.x = rect.x - textSurface->w;
        //rect.w = textSurface->w;

        if (textSurface->w <= rect.w) {
            rect.w = textSurface->w;
        }

        rect.h = textSurface->h;
        SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, txtTexture, NULL, &rect);

        SDL_FreeSurface(textSurface);
        textSurface = NULL;
        SDL_DestroyTexture(txtTexture);
        txtTexture = NULL;
    }
}

//Draw text block
void game::drawTextBlock(string text, SDL_Rect rect)
{
    //drawSquare(btn.getRect(),btn.getColor());
    SDL_Color fg = { 0,0,0,0 };
    if (text.length() > 0) {
        // SDL_Surface* textSurface = TTF_RenderText_Solid(gFont,text.c_str(),fg);
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(gFont, text.c_str(), fg, rect.w);

        /*SDL_Rect target = btn.getRect();
        target.x = target.x + target.w/2 - text->w/2;
        target.y = target.y + target.h/2 - text->h/2;*/
        // SDL_BlitSurface(textSurface,NULL,gScreenSurface,&rect);
        rect.w = textSurface->w;
        rect.h = textSurface->h;
        SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, txtTexture, NULL, &rect);

        SDL_FreeSurface(textSurface);
        textSurface = NULL;
        SDL_DestroyTexture(txtTexture);
        txtTexture = NULL;
    }
}

void game::drawTextResize(string text, SDL_Rect rect)
{
    
    if (text.length() > 0) {
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(gFont, text.c_str(), fg);
        SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, txtTexture, NULL, &rect);

        SDL_FreeSurface(textSurface);
        textSurface = NULL;
        SDL_DestroyTexture(txtTexture);
        txtTexture = NULL;
    }
}

void game::screenMain()
{
  /*  SDL_Rect dstrect;
    tmpRect.x = gScreenSurface->w / 8 - 1;
    tmpRect.y = 299;
    tmpRect.w = 322;
    tmpRect.h = 482;*/
    //SDL_Color square={0,0,255,0}; blue
    SDL_Color square = { 0,0,0,0 };

    tmpRect.w = 32;
    tmpRect.h = 32;
    tmpRect.x = gScreenSurface->w / 8;
    tmpRect.y = gScreenSurface->h / 8 - 50;
    tmpRect.w = 256;
    tmpRect.h = 50;
    tmpRect.x = gScreenSurface->w / 8;
    tmpRect.y = gScreenSurface->h / 8;
    tmpRect.x = gScreenSurface->w / 2 - 350;
    tmpRect.y = gScreenSurface->h / 8 - 50;
    tmpRect.w = 500;
    tmpRect.h = 100;
    drawTextResize("Menu Principal", tmpRect);

    tmpRect.x = gScreenSurface->w - 500;
    tmpRect.y = gScreenSurface->h - 50;
    tmpRect.w = 500;
    tmpRect.h = 50;

    drawText("Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60), tmpRect);

    //drawButton(exitButton);
    //drawButton(startButton);
    //drawButton(configButton);
    //drawButton(achievementsButton);

    drawButtonSrc(achievementsButton, buttonStarsTexture);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(startButton, buttonPlayTexture);

    drawButtonSrc(configButton, buttonConfigTexture);
    //drawButtonSrc(startButton, buttonAcceptTexture);
}



void game::screenPlayerName()
{

   // SDL_Rect dstrect;

   // drawButton(exitButton);
   // drawButton(continueButton);

   // drawButton(playerUpButton);
    //drawButton(playerDownButton);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);

    //drawButtonSrc(playerUpButton, buttonNextTexture);
    //drawButtonSrc(playerDownButton, buttonPrevTexture);

    drawButtonSrc(nextButton, buttonNextTexture);
    drawButtonSrc(prevButton, buttonPrevTexture);


    tmpRect.x = gScreenSurface->w / 2 - gScreenSurface->w / (cam_size_x*2);
    tmpRect.y = gScreenSurface->h / 2;
    tmpRect.w = gScreenSurface->w / cam_size_x;
    tmpRect.h = gScreenSurface->h / cam_size_y;

    drawPlayerTileset(tmpRect, playerTile);


    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;

    drawText("CAMBIAR NOMBRE E IMAGEN", tmpRect);
    tmpRect.y = 200;
    drawText("Introduce el nombre del jugador", tmpRect);

    tmpRect.y = 250;
    drawText(playerName, tmpRect);
    //tmpRect.y = 300;
    tmpRect.y = gScreenSurface->h / 2 - 50;
    drawText("Cambia la imagen y luego pulsa continuar...", tmpRect);
}


void game::screenPlayerAttributes()
{



   // drawButton(exitButton);
   // drawButton(continueButton);
   // drawButton(rollButton);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    drawButtonSrc(backButton, buttonCancelTexture);

    drawButtonSrc(rollButton, buttonRollDiceTexture);

    tmpRect.x = 100;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawText("Atributos del personaje", tmpRect);
    tmpRect.y = 150;
    drawText("Volver a tirar para nueva tirada o continuar", tmpRect);

    tmpRect.w = 100;
    tmpRect.y = 200;
    drawTextL("Combate", tmpRect);
    tmpRect.y = 250;
    drawTextL("Vitalidad", tmpRect);
    tmpRect.y = 300;
    drawTextL("Poder", tmpRect);
    tmpRect.y = 350;
    drawTextL("Suerte", tmpRect);

   // drawIMG(shieldTexture, 100, 200, skill, 18);
  //  drawIMG(heartTexture, 100, 250, stamina, 30);
  //  drawIMG(starTexture, 100, 300, power, 18);
  //  drawIMG(starTexture, 100, 300, luck, 18);
    //, SDL_Color color

    drawIMGBox(100, 200, skill, 18, {0,0,200,0});
  drawIMGBox(100, 250, stamina, 30, { 200,0,0,0 });
  drawIMGBox(100, 300, power, 18, { 0,200,0,0 });
  drawIMGBox(100, 350, luck, 18, { 0,200,200,0 });
}


void game::screenPlayer()
{


    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
   // drawButtonSrc(backButton, buttonCancelTexture);

   // drawButtonSrc(rollButton, buttonRollDiceTexture);

    tmpRect.x = 100;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawText("Ficha del personaje", tmpRect);
    //tmpRect.y = 150;
    //drawText("Volver a tirar para nueva tirada o continuar", tmpRect);

    tmpRect.w = 100;
    tmpRect.y = 200;
    drawTextL("Combate", tmpRect);
    tmpRect.y = 250;
    drawTextL("Vitalidad", tmpRect);
    tmpRect.y = 300;
    drawTextL("Poder", tmpRect);
    tmpRect.y = 350;
    drawTextL("Suerte", tmpRect);

    drawIMGBox(100, 200, skill, 18, { 0,0,200,0 });
    drawIMGBox(100, 250, stamina, 30, { 200,0,0,0 });
    drawIMGBox(100, 300, power, 18, { 0,200,0,0 });
    drawIMGBox(100, 350, luck, 18, { 0,200,200,0 });

    tmpRect.y = 500;
    drawTextL("Nivel:" + to_string(level), tmpRect);
    tmpRect.y = 550;
    drawTextL("Experiencia:" + to_string(exp) + "/" + to_string((level*level)*100), tmpRect);
}



void game::screenClear()
{
    SDL_SetRenderDrawColor(gRenderer, 0x99, 0x99, 0x99, 0x99);
    SDL_RenderClear(gRenderer);
}

void game::screenIntro()
{

    drawButtonSrc(exitButton,buttonCloseTexture);
    //drawButton(exitButton);
    drawButtonSrc(continueButton,buttonPlayTexture);
    //drawButton(continueButton);
    tmpRect.x = gScreenSurface->w / 2 - 350;
    tmpRect.y = gScreenSurface->h / 4;
    tmpRect.w = 700;
    tmpRect.h = 100;


    tmpRect.x = gScreenSurface->w / 2 - 450;
    tmpRect.w = 900;
    fg = { 128, 0, 128, 0 };
    drawTextResize("Achievement master", tmpRect);
    fg = { 0, 0, 0, 0 };
}


void game::screenNotifications()
{
    tmpRect.h = 30;
    //tmpRect.x = 0;
    tmpRect.x = gScreenSurface->w / 2 - 250;

    tmpRect.w = 500;
    int tmpY = 0;
    if (notifications.size() > 0) {

        for (std::string notification : notifications) {

            //tmpRect.y = ((gScreenSurface->h / 4) * 3) + (tmpY * 30);
            tmpRect.y = gScreenSurface->h - (notifications.size()*30 +30) + (tmpY * 30);
            drawTextResize(notification, tmpRect);
            tmpY++;
        }
    }
}

void game::screenGameOver()
{
    tmpRect.y = gScreenSurface->h / 4;
    tmpRect.h = 100;

    tmpRect.x = gScreenSurface->w / 2 - 450;
    tmpRect.w = 900;
    drawTextResize("Game Over", tmpRect);
}

void game::eventsIntro()
{
    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            // closeSDL();
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.reset();
            timerGameOver.start();
        }
        //Special key input
        else if (e.type == SDL_MOUSEMOTION)
        {
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timer.start();
                timer.reset();
            }
            if (continueButton.clicked(mousex, mousey)) {
                setState(my_enums::_MAINMENU_);
                //quit =true;
                Mix_PlayChannel(-1, audioButton, 0);
                //Mix_PlayMusic(music8bit, -1);
                timer.start();
            }
        }

    }


    //}
}

void game::eventsMain()
{
    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
                //addNotification("Saliendo del juego");
                addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
            }

            if (configButton.clicked(mousex, mousey)) {
                setState(my_enums::_CONFIGMENU_);
                previousScreen = my_enums::_MAINMENU_;

            }

            if (achievementsButton.clicked(mousex, mousey)) {
                setState(my_enums::_ACHIEVEMENTS_);
                previousScreen = my_enums::_MAINMENU_;

            }


            if (startButton.clicked(mousex, mousey)) {
                //addNotification("Comenzando el juego");
                addAchievement("Comenzando el juego", my_enums::_OPTIONS_);
                SDL_StartTextInput();
                setState(my_enums::_NAME_);
            }



        }
  

        //******************
    }

    myTime = (int)(timer.getTicks() / 1000);
}

void game::eventsGameOver()
{
    if (timerGameOver.getTicks() > 3000) {
        setState(my_enums::_GAMECLOSE_);
    }

    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //Special text input event

        if (e.type == SDL_MOUSEMOTION)
        {
        }
    }
}



void game::eventsPlayerName()
{

    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            //closeSDL();
            SDL_StopTextInput();
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        //Special key input
        else if (e.type == SDL_KEYDOWN)
        {
            //Handle backspace
            if (e.key.keysym.sym == SDLK_BACKSPACE && playerName.length() > 0) { playerName.pop_back(); }

        }
        //Special text input event
        else if (e.type == SDL_TEXTINPUT)
        {
            playerName += e.text.text;
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                //closeSDL();
                SDL_StopTextInput();
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
            }
            if (continueButton.clicked(mousex, mousey)) {
                // quit =true;
                Mix_PlayChannel(-1, audioButton, 0);
                SDL_StopTextInput();
                //setState(_MAINMENU_);
                //setState(my_enums::_RACES_);
                setState(my_enums::_ATTRIBUTES_);
                //Mix_PlayMusic(musicGameOver, -1);
                //timerGameOver.start();
                //timerGameOver.reset();

            }


            if (nextButton.clicked(mousex, mousey)) {
                playerTile++;
               // if (((playerTile +1) % 32) > 20) { playerTile += 12; }
                if (playerTile > 96) { playerTile = 96; }
            }

            if (prevButton.clicked(mousex, mousey)) {
                playerTile--;
              //  if (((playerTile+1) % 32) > 20) { playerTile -= 12; }
                if (playerTile < 63) { playerTile = 63; }

            }

            
        }

    }
}

void game::screenConfigMenu()
{
   // SDL_Rect dstrect;
    tmpRect.x = gScreenSurface->w / 8 - 1;
    tmpRect.y = 299;
    tmpRect.w = 322;
    tmpRect.h = 482;
    //SDL_Color square={0,0,255,0}; blue
    SDL_Color square = { 0,0,0,0 };

    tmpRect.w = 32;
    tmpRect.h = 32;
    tmpRect.x = gScreenSurface->w / 8;
    tmpRect.y = gScreenSurface->h / 8 - 50;
    tmpRect.w = 256;
    tmpRect.h = 50;
    tmpRect.x = gScreenSurface->w / 8;
    tmpRect.y = gScreenSurface->h / 8;
    tmpRect.x = gScreenSurface->w / 2 - 350;
    tmpRect.y = gScreenSurface->h / 8 - 50;
    tmpRect.w = 500;
    tmpRect.h = 100;
    drawTextResize("Menu Configuración", tmpRect);

    tmpRect.x = gScreenSurface->w - 500;
    tmpRect.y = gScreenSurface->h - 50;
    tmpRect.w = 500;
    tmpRect.h = 50;

    //drawText("Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60), tmpRect);


    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(startButton, buttonAcceptTexture);

    drawButtonSrc(volumeSoundUpButton, buttonNextTexture);
    drawButtonSrc(volumeSoundDownButton, buttonPrevTexture);

    drawButtonSrc(volumeMusicUpButton, buttonNextTexture);
    drawButtonSrc(volumeMusicDownButton, buttonPrevTexture);



//    drawButton(exitButton);
//    drawButton(startButton);
    drawButton(musicButton);
    drawButton(muteButton);
  //  drawButton(volumeSoundUpButton);
    drawButton(volumeSoundButton);
  //  drawButton(volumeSoundDownButton);

    //drawButton(volumeMusicUpButton);
    drawButton(volumeMusicButton);
    //drawButton(volumeMusicDownButton);

    drawButton(blurButton);
}

void game::screenHomeTown()
{

    SDL_Rect target;
     target.x = 0;
    target.y = 0;

    target.w = gScreenSurface->w;
    target.h = gScreenSurface->h;

    drawMap();

    drawPlayer();

    drawNPCs();

    tmpRect.x = gScreenSurface->w / 2 - 400;
    tmpRect.y = gScreenSurface->h / 12;
    
    tmpRect.w = 800;
    tmpRect.h = 100;

    string townName = "";
    switch (getState())
    {


    case my_enums::_ELEMENTAL_WATER_WORLD_:
    {
        townName = "MUNDO ELEMENTAL DE AGUA";
        break;
    }

    case my_enums::_ELEMENTAL_FIRE_WORLD_:
    {
        townName = "MUNDO ELEMENTAL DE FUEGO";
        break;
    }


    case my_enums::_COAST_WORLD_:
    {
        townName = "ISLAS";
        break;
    }

    case my_enums::_HOMETOWN_:
    {
        townName = "POBLADO INICIAL";
        break;
    }

    default:
    {
        townName = "";
        break;
    }
    };
    drawTextResize(townName, tmpRect);
  
    tmpRect.x = gScreenSurface->w - 500;
    tmpRect.y = gScreenSurface->h - 50;
    tmpRect.w = 500;
    tmpRect.h = 50;

    drawText("Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60), tmpRect);

   // drawButton(exitButton);
   // drawButton(achievementsButton);
   // drawButton(configButton);
    //drawButton(inventoryButton);

  /*  drawButton(moveLeftButton);
    drawButton(moveRightButton);
    drawButton(moveUpButton);
    drawButton(moveDownButton);
    */

    drawButtonSrc(moveLeftButton, buttonLeftTexture);
    drawButtonSrc(moveRightButton, buttonRightTexture);
    drawButtonSrc(moveUpButton, buttonUpTexture);
    drawButtonSrc(moveDownButton, buttonDownTexture);
    
    drawButton(newMapButton);

    drawButtonSrc(achievementsButton, buttonStarsTexture);

    drawButtonSrc(playerButton, buttonPlayerTexture);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(configButton, buttonConfigTexture);
    drawButtonSrc(newMapButton, buttonMapTexture);
    drawButtonSrc(inventoryButton, buttonBackpackTexture);
    //drawButtonSrc(prevButton, buttonPrevTexture);

}

std::string game::getRaceName(my_enums::playerRaces race)
{

    switch (race)
    {
    case my_enums::_HUMAN_:
    {
        return "Humano";
        break;
    }
    case my_enums::_ELF_:
    {
        return "Elfo";
        break;
    }

    case my_enums::_DARF_:
    {
        return "Enano";
        break;
    }

    case my_enums::_HALFING_:
    {
        return "Mediano";
        break;
    }

    case my_enums::_HALFORC_:
    {
        return "Medio Orco";
        break;
    }

    case my_enums::_BEHOLDER_:
    {
        return "Contemplador";
        break;
    }
    
    default:
    {
        // is likely to be an error
        return "";
    }
    };


}


std::string game::getArchetypeName(my_enums::playerArchetype archetype)
{

    switch (archetype)
    {
    case my_enums::_FIGHTER_:
    {
        return "Guerrero";
        break;
    }
    case my_enums::_ARCHER_:
    {
        return "Arquero";
        break;
    }

    case my_enums::_THIEVE_:
    {
        return "Ladrón";
        break;
    }

    case my_enums::_MAGE_:
    {
        return "Mago";
        break;
    }

    case my_enums::_NECROMANCER_:
    {
        return "Necromante";
        break;
    }

    case my_enums::_SUMMONER_:
    {
        return "Invocador";
        break;
    }

    default:
    {
        // is likely to be an error
        return "";
    }
    };


}



void game::screenPlayerRaces()
{

   // SDL_Rect dstrect;

    //drawButton(exitButton);
    //drawButton(continueButton);
    //drawButton(nextButton);
    //drawButton(prevButton);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    drawButtonSrc(backButton, buttonCancelTexture);
    drawButtonSrc(nextButton, buttonNextTexture);
    drawButtonSrc(prevButton, buttonPrevTexture);


    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;

    drawText("RAZA DEL JUGADOR", tmpRect);
    tmpRect.y = 200;
    drawText("Selecciona la raza del personaje", tmpRect);

    //tmpRect.y = 250;
    //drawText(playerName, tmpRect);
    tmpRect.y = 300;
    drawText("Luego pulsa continuar...", tmpRect);

    //tmpRect.y = 400;
    tmpRect.y = gScreenSurface->h / 2 +64;
    tmpRect.x = gScreenSurface->w / 2 -100;
    drawText(getRaceName(currentRace), tmpRect);


}

void game::screenPlayerArchetypes()
{
    //drawButton(exitButton);
    //drawButton(continueButton);
    //drawButton(nextButton);
    //drawButton(prevButton);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonPlayTexture);
    drawButtonSrc(backButton, buttonCancelTexture);
    drawButtonSrc(nextButton, buttonNextTexture);
    drawButtonSrc(prevButton, buttonPrevTexture);


    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;

    drawText("ARQUETIPO DEL JUGADOR", tmpRect);
    tmpRect.y = 200;
    drawText("Selecciona el arquetipo del personaje", tmpRect);

    tmpRect.y = 300;
    drawText("Luego pulsa continuar...", tmpRect);

   // tmpRect.y = 400;
    tmpRect.y = gScreenSurface->h / 2 + 64;
    tmpRect.x = gScreenSurface->w / 2 - 100;
    drawText(getArchetypeName(currentArchetype), tmpRect);
}


void game::screenInventory()
{
    //quit = false;
    //while (!quit) {
        //screenClear();
       // drawButtonSrc(continueButton, buttonTexture);
        //drawButton(continueButton);
        //drawButton(exitButton);


        drawButtonSrc(exitButton, buttonCloseTexture);
        drawButtonSrc(continueButton, buttonAcceptTexture);

        int tmpy = 100;
        tmpRect.x = gScreenSurface->w / 2 - 200;
        tmpRect.y = 0;
        tmpRect.w = 400;
        tmpRect.h = 50;
        SDL_Rect target;
        target.x = tmpRect.x - 50;
        target.w = tmpRect.w + 40;
        target.h = tmpRect.h + 25;

        string tmpString = "";
        tmpString = "Monedas de oro: " + to_string(coins);
        tmpy += 50;
        tmpRect.y = tmpy;
        target.y = tmpRect.y - 25;
        //SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
        drawText(tmpString, tmpRect);


        tmpString = "Raciones de comida: " + to_string(food);
        tmpy += 50;
        tmpRect.y = tmpy;
        target.y = tmpRect.y - 25;
        //SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
        drawText(tmpString, tmpRect);

        for (list<item>::iterator it = items.begin(); it != items.end(); it++)
        {
            tmpy += 50;
            tmpRect.y = tmpy;
            target.y = tmpRect.y - 25;
            if (it->name.substr(0, 2) == "**") {
                tmpString = it->name.substr(2, it->name.length() - 2) + " (" + to_string(it->count) + ")";
                // cout << it->name.substr(2,it->name.length() -2) << " (" << it->count << ")" << endl;
            }
            else {
                tmpString = it->name + " (" + to_string(it->count) + ")";
                // cout << it->name << " (" << it->count << ")" << endl;
            }
            //SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
            drawText(tmpString, tmpRect);

        }
}


void game::screenHero()
{
    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);

    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 0;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawText("Heroe inmortal", tmpRect);

    tmpRect.y = gScreenSurface->h / 2;
    drawText("¿Te rindes o vas a seguir luchando?", tmpRect);
 
}


void game::screenAchievements()
{
    
    //drawButton(continueButton);
    //drawButton(exitButton);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    int combat = 0, missions = 0, npcs = 0, options = 0, hidden = 0;

    for (achievement currentAchievement : achievements) {

        switch (currentAchievement.type)
        {
        case my_enums::_COMBAT_:
        {
            combat++;
            break;
        }
        case my_enums::_MISSIONS_:
        {
            missions++;
            break;
        }

        case my_enums::_NPCS_:
        {
            npcs++;
            break;
        }

        case my_enums::_OPTIONS_:
        {
            options++;
            break;
        }

        case my_enums::_HIDDEN_:
        {
            hidden++;
            break;
        }

        default:
        {
            // is likely to be an error
        }
        };
    }

    tmpRect.x = gScreenSurface->w / 2 - 250;
    tmpRect.y = gScreenSurface->h / 8 - 50;
    tmpRect.w = 500;
    tmpRect.h = 100;
    drawTextResize("LOGROS", tmpRect);


    tmpRect.x = 100;
    tmpRect.y = 100;
    /*tmpRect.w = 400;
    tmpRect.h = 50;
    drawText("NUEVO PERSONAJE", tmpRect);
    tmpRect.y = 150;
    drawText("Volver a tirar para nueva tirada o continuar", tmpRect);
    */
    tmpRect.w = 100;
    tmpRect.y = 200;
    drawTextL("Combate", tmpRect);
    tmpRect.y = 250;
    drawTextL("Missiones", tmpRect);
    tmpRect.y = 300;
    drawTextL("NPCs", tmpRect);
    tmpRect.y = 350;
    drawTextL("Opciones", tmpRect);
    tmpRect.y = 400;
    drawTextL("Ocultos", tmpRect);

    //combat = 0, missions = 0, npcs = 0, options = 0, hidden = 0;

    drawIMGBox(100, 200, combat, 10, { 200,0,0,0 });
    drawIMGBox(100, 250, missions, 10, { 200,0,0,0 });
    drawIMGBox(100, 300, npcs, 10, { 200,0,0,0 });
    drawIMGBox(100, 350, options, 10, { 200,0,0,0 });
    drawIMGBox(100, 400, hidden, 10, { 200,0,0,0 });


}



void game::screenFight()
{
    
    drawButtonSrc(exitButton, buttonCloseTexture);
    //drawButtonSrc(continueButton, buttonAcceptTexture);

    drawButtonSrc(fightButton, buttonSwordTexture);
    drawButtonSrc(spellButton, buttonSpellTexture);
    drawButtonSrc(potionHealthButton, buttonPotionHealthTexture);
    drawButtonSrc(potionMagicButton, buttonPotionMagicTexture);

    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 0;
    tmpRect.w = 400;
    tmpRect.h = 50;
   
    drawText("¡¡COMBATE!!", tmpRect);


    int tmp = 0;

    //cout << "<<Combate!!>>" << endl;

    /*tmpRect.x = 150;
    tmpRect.y = 50;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawText("<<Combate!!>>", tmpRect);
    */
    tmpRect.x = 100;
    for (list<NPC>::iterator it = tmpNPCs.begin(); it != tmpNPCs.end(); it++)
    {
        tmp++;

        tmpRect.w = 100;
        tmpRect.h = 100;
        tmpRect.x = 1;
        tmpRect.y = tmp * 100;
        drawPlayerTileset(tmpRect, it->tile);

        tmpRect.x = 101;
        //tmpRect.y = tmp * 100;
        tmpRect.h = 50;
        drawText("Enemigo", tmpRect);
        tmpRect.w = 200;
        tmpRect.x = 201;
        drawText(it->description, tmpRect);
        tmpRect.w = 100;
        tmpRect.x = 101;
        tmpRect.y = 25 + tmp * 100;
        drawText("Habilidad", tmpRect);
        tmpRect.y = 50 + tmp * 100;
        drawText("Vitalidad", tmpRect);
       
        drawIMGBox(200, 25 + tmp * 100, it->skill, it->skill, { 0,0,200,0 });
        drawIMGBox(200, 50 + tmp * 100, it->stamina, it->stamina, { 200,0,0,0 });


    }

    tmpRect.w = 100;
    tmpRect.h = 100;
    tmpRect.x = 100;
    tmpRect.y = gScreenSurface->h - 250;
  //  drawTextL("Tu ataque", tmpRect);
    drawTextL("Vitalidad:", tmpRect);
    drawIMGBox(100, gScreenSurface->h - 250, stamina, 18, { 200,0,0,0 });

    tmpRect.y = gScreenSurface->h - 200;
    drawTextL("Poder:", tmpRect);
    drawIMGBox(100, gScreenSurface->h - 200, power, 18, { 128,0,128,0 });
 //   drawTextL("Ataque enemigo", tmpRect);


//    drawIMGBox(100, gScreenSurface->h - 250, playerDice, 18, { 0,0,200,0 });
 //   drawIMGBox(100, gScreenSurface->h - 200, foeDice, 18, { 200,0,0,0 });

    tmpRect.y = gScreenSurface->h - 150;
    drawTextL("Turno:", tmpRect);
    drawText(to_string(turn), tmpRect);
    //tmpRect.y = gScreenSurface->h - 100;
    
   // drawIMG(redSurface, 100, gScreenSurface->h - 100, Stamina.getCurrent());

   

   // drawButton(buttonAttack);


}


void game::eventsPlayerRaces()
{
    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                //closeSDL();
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
                
            }
            if (continueButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                setState(my_enums::_ARCHETYPES_);

            }

            if (backButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                //SDL_StartTextInput();
                // setState(my_enums::_NAME_);
                setState(my_enums::_ATTRIBUTES_);

            }//back

            if (nextButton.clicked(mousex, mousey)) {
                switch (currentRace)
                {
                case my_enums::_HUMAN_:
                {
                    currentRace = my_enums::_ELF_;
                    break;
                }
                case my_enums::_ELF_:
                {
                    currentRace = my_enums::_DARF_;
                    break;
                }

                case my_enums::_DARF_:
                {
                    currentRace = my_enums::_HALFING_;
                    break;
                }

                case my_enums::_HALFING_:
                {
                    currentRace = my_enums::_HALFORC_;
                    break;
                }

                case my_enums::_HALFORC_:
                {
                    currentRace = my_enums::_BEHOLDER_;
                    break;
                }

                case my_enums::_BEHOLDER_:
                {
                    currentRace = my_enums::_HUMAN_;
                    break;
                }

                default:
                {
                    // is likely to be an error
                }
                };

            }//nextbutton

            if (prevButton.clicked(mousex, mousey)) {
                switch (currentRace)
                {
                case my_enums::_HUMAN_:
                {
                    currentRace = my_enums::_BEHOLDER_;
                    
                   
                    break;
                }
                case my_enums::_ELF_:
                {
                    currentRace = my_enums::_HUMAN_;
                   
                    break;
                }

                case my_enums::_DARF_:
                {
                    currentRace = my_enums::_ELF_;
                   
                    break;
                }

                case my_enums::_HALFING_:
                {
                    currentRace = my_enums::_DARF_;
                   
                    break;
                }

                case my_enums::_HALFORC_:
                {
                    
                    
                    currentRace = my_enums::_HALFING_;
                    break;
                }

                case my_enums::_BEHOLDER_:
                {
                    currentRace = my_enums::_HALFORC_;
                    break;
                }

                default:
                {
                    // is likely to be an error
                }
                };

            }//prevbutton
        }

    }
}

void game::eventsPlayerArchetypes()
{
    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
               


                Mix_PlayChannel(-1, audioButton, 0);
                //closeSDL();
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();

            }
            if (continueButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                Mix_PlayMusic(musicTOWN, -1);
                setState(my_enums::_HOMETOWN_);

                addItem("ESPADA", "Una espada mellada", 1, 0, 1);
                addItem("ARMADURA CUERO", "Armadura ligera de cuero", 1, 0, 1);

            }//continue

            if (backButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                //closeSDL();
                SDL_StartTextInput();
                setState(my_enums::_RACES_);

            }//back


            if (nextButton.clicked(mousex, mousey)) {
                switch (currentArchetype)
                {
                case my_enums::_FIGHTER_:
                {
                    currentArchetype = my_enums::_ARCHER_;
                    break;
                }
                case my_enums::_ARCHER_:
                {
                    currentArchetype = my_enums::_THIEVE_;
                    break;
                }

                case my_enums::_THIEVE_:
                {
                    currentArchetype = my_enums::_MAGE_;
                    break;
                }

                case my_enums::_MAGE_:
                {
                    currentArchetype = my_enums::_NECROMANCER_;
                    break;
                }

                case my_enums::_NECROMANCER_:
                {
                    currentArchetype = my_enums::_SUMMONER_;
                    break;
                }

                case my_enums::_SUMMONER_:
                {
                    currentArchetype = my_enums::_FIGHTER_;
                    break;
                }

                default:
                {
                    // is likely to be an error
                }
                };

            }//nextbutton


            if (prevButton.clicked(mousex, mousey)) {
                switch (currentArchetype)
                {
                case my_enums::_FIGHTER_:
                {
                    currentArchetype = my_enums::_SUMMONER_;
                    break;
                }
                case my_enums::_ARCHER_:
                {
                    currentArchetype = my_enums::_FIGHTER_;
                    break;
                }

                case my_enums::_THIEVE_:
                {
                    currentArchetype = my_enums::_ARCHER_;
                    break;
                }

                case my_enums::_MAGE_:
                {
                    currentArchetype = my_enums::_THIEVE_;
                    break;
                }

                case my_enums::_NECROMANCER_:
                {
                    currentArchetype = my_enums::_MAGE_;
                    break;
                }

                case my_enums::_SUMMONER_:
                {
                    currentArchetype = my_enums::_NECROMANCER_;
                    break;
                }

                default:
                {
                    // is likely to be an error
                }
                };

            }//prevbutton
        }

    }
}



void game::eventsPlayerAttributes()
{
    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                

                Mix_PlayChannel(-1, audioButton, 0);
                //closeSDL();
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();

            }
            if (continueButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                //Mix_PlayMusic(musicTOWN, -1);
                setState(my_enums::_RACES_);

             
            }//continue

            if (backButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                //closeSDL();
                SDL_StartTextInput();
                //setState(my_enums::_RACES_);
                setState(my_enums::_NAME_);


            }//back

            if (rollButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                //Mix_PlayMusic(musicTOWN, -1);
                randomAttributes();


            }//continue
           

            
        }

    }
}


void game::eventsPlayer()
{
    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {


                Mix_PlayChannel(-1, audioButton, 0);
                //closeSDL();
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();

            }
            if (continueButton.clicked(mousex, mousey)) {
                setState(previousScreen);


            }//continue



        }

    }
}


void game::randomAttributes()
{
    skill= dice(6, 1) + 6;
    stamina = dice(6, 1) + dice(6, 1) + 12;
    power = dice(6, 1) + 6;
    luck = dice(6, 1) + 6;
}



void game::eventsConfigMenu()
{
    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
                //addNotification("Saliendo del juego");
                addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
            }

            if (muteButton.clicked(mousex, mousey)) {
                if (musicON == true) {
                    musicON = false;
                    muteButton.setCaption("MUSIC OFF");
                    Mix_Volume(-1, 0);
                    Mix_VolumeMusic(0);
                    //addNotification("Sonido apagado");
                    addAchievement("Sonido apagado", my_enums::_OPTIONS_);
                    volumeSoundButton.setCaption("SOUND " + std::to_string(volumeSound * 100 / 128) + "%");
                    volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
                }
                else {
                    musicON = true;
                    muteButton.setCaption("MUSIC ON");

                    Mix_Volume(-1, volumeSound);
                    Mix_VolumeMusic(volumeMusic);
                    //addNotification("Sonido activado");
                    addAchievement("Sonido activado", my_enums::_OPTIONS_);
                    volumeSoundButton.setCaption("SOUND " + std::to_string(volumeSound * 100 / 128) + "%");
                    volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
                }
            }

            if (musicButton.clicked(mousex, mousey)) {
                switch (currentMusic) {

                case 1:
                    currentMusic = 2;
                    Mix_PlayMusic(musicDARK, -1);
                    addAchievement("Musica DARK", my_enums::_OPTIONS_);
                    break;
                case 2:
                    currentMusic = 3;
                    Mix_PlayMusic(musicFOREST, -1);
                    addAchievement("Musica FOREST", my_enums::_OPTIONS_);
                    break;
                case 3:
                    currentMusic = 4;
                    Mix_PlayMusic(musicTOWN, -1);
                    addAchievement("Musica TOWN", my_enums::_OPTIONS_);
                    break;

                case 4:
                    currentMusic = 5;
                    Mix_PlayMusic(musicBATTLE, -1);
                    addAchievement("Musica BATTLE", my_enums::_OPTIONS_);
                    break;

                case 5:
                    currentMusic = 6;
                    Mix_PlayMusic(musicBOSS, -1);
                    addAchievement("Musica BOSS", my_enums::_OPTIONS_);
                    break;

                case 6:
                    currentMusic = 7;
                    Mix_PlayMusic(musicCAVE, -1);
                    addAchievement("Musica CAVE", my_enums::_OPTIONS_);
                    break;

                case 7:
                    currentMusic = 8;
                    Mix_PlayMusic(musicHERO, -1);
                    addAchievement("Musica HERO", my_enums::_OPTIONS_);
                    break;

                case 8:
                    currentMusic = 9;
                    Mix_PlayMusic(musicMYSTICAL, -1);
                    addAchievement("Musica MYSTICAL", my_enums::_OPTIONS_);
                    break;

                case 9:
                    currentMusic = 1;
                    Mix_PlayMusic(musicINTRO, -1);
                    addAchievement("Musica INTRO", my_enums::_OPTIONS_);
                    break;

                }
            }


            if (startButton.clicked(mousex, mousey)) {
                setState(previousScreen);

            }

            if (volumeSoundUpButton.clicked(mousex, mousey)) {
                volumeSound += 8;
                if (volumeSound > 128) { volumeSound = 128; }
                Mix_Volume(-1, volumeSound);
                Mix_VolumeMusic(volumeMusic);

                volumeSoundButton.setCaption("SOUND " + std::to_string(volumeSound * 100 / 128) + "%");
            }

            if (volumeSoundDownButton.clicked(mousex, mousey)) {
                volumeSound -= 8;
                if (volumeSound < 0) { volumeSound = 0; }
                Mix_Volume(-1, volumeSound);
                Mix_VolumeMusic(volumeMusic);
                volumeSoundButton.setCaption("SOUND " + std::to_string(volumeSound * 100 / 128) + "%");
            }

            if (volumeMusicUpButton.clicked(mousex, mousey)) {
                volumeMusic += 8;
                if (volumeMusic > 128) { volumeMusic = 128; }
                Mix_Volume(-1, volumeSound);
                Mix_VolumeMusic(volumeMusic);
                volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
            }

            if (volumeMusicDownButton.clicked(mousex, mousey)) {
                volumeMusic -= 8;
                if (volumeMusic < 0) { volumeMusic = 0; }
                Mix_Volume(-1, volumeSound);
                Mix_VolumeMusic(volumeMusic);
                volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
            }

            if (blurButton.clicked(mousex, mousey)) {
                int blur = baseMap.getBlur();
                if (blur == 5) {
                    blur = 0;
                    blurButton.setCaption("BLUR OFF");                  
                }
                else {
                    blur = 5;
                    blurButton.setCaption("BLUR ON");
                }
                baseMap.setBlur(blur);
                updateMap();
            }


        }
 

        //******************
    }

    myTime = (int)(timer.getTicks() / 1000);

}


void game::eventsInventory()
{
    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
                //addNotification("Saliendo del juego");
                addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
            }

            if (continueButton.clicked(mousex, mousey)) {
                setState(previousScreen);

            }

        }

        //******************
    }

    myTime = (int)(timer.getTicks() / 1000);

}

void game::eventsHero()
{
    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
                //addNotification("Saliendo del juego");
                addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
            }

            if (continueButton.clicked(mousex, mousey)) {
                Mix_PlayMusic(musicINTRO, -1);
                setState(my_enums::_MAINMENU_);

            }

        }

        //******************
    }

    myTime = (int)(timer.getTicks() / 1000);

}

void game::eventsFight()
{
    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
                //addNotification("Saliendo del juego");
                deleteNPCs(px, py);
                addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
            }

           /* if (continueButton.clicked(mousex, mousey)) {
                Mix_PlayMusic(musicTOWN, -1);
                deleteNPCs(px, py);
                setState(previousScreen);

            }*/

            if (fightButton.clicked(mousex, mousey)) {
                int good = dice(10, 1) + skill;
                int bad = dice(10, 1) + tmpNPCs.begin()->skill;
               // playerDice = good;
              //  foeDice = bad;
                int damage = 1;

                if (good > bad) {
                    

                    NPC tmpNPC = tmpNPCs.front();
                    addNotification("Has herido a "+ tmpNPC.description +"!!");
                    tmpNPC.stamina -= damage;
                    if (tmpNPC.stamina <= 0) {
                        //   cout << "Has derrotado a " << tmpFoe.description << endl;
                         addNotification("Has derrotado a "+ tmpNPC.description +"!!");
                        //Mix_PlayChannel(-1, audioMaleDeath, 0);
                        tmpNPCs.pop_front();
                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }
                }

                if (tmpNPCs.size() > 0){
                    int good = dice(10, 1) + skill;
                    int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                    if (bad > good) {
                        stamina -= damage;
                        addNotification(tmpNPCs.begin()->description + " te ha herido!!");
                        if (stamina <= 0) {
                            addNotification("Has sido derrotado por " + tmpNPCs.begin()->description + "!!");
                            setState(my_enums::_HERO_);
                            deleteNPCs(px, py);
                            //Mix_PlayMusic(musicGameOver, -1);
                            Mix_PlayMusic(musicHERO, -1);
                            timerGameOver.start();
                            timerGameOver.reset();
                            //addNotification("Saliendo del juego");
                            addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
                        }

                    }

                }

                turn++;
            }//fightbutton click


            if (tmpNPCs.size() == 0) {
                Mix_PlayMusic(musicTOWN, -1);
                deleteNPCs(px, py);
                setState(previousScreen);

            }


            //    drawIMGBox(100, gScreenSurface->h - 250, playerDice, 18, { 0,0,200,0 });
 //   drawIMGBox(100, gScreenSurface->h - 200, foeDice, 18, { 200,0,0,0 });

        }

        //******************
    }

    myTime = (int)(timer.getTicks() / 1000);

}


void game::eventsAchievements()
{
    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
                //addNotification("Saliendo del juego");
                addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
            }

            if (continueButton.clicked(mousex, mousey)) {
                setState(previousScreen);

            }

        }

        //******************
    }

    myTime = (int)(timer.getTicks() / 1000);

}



void game::eventsHomeTown()
{

    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            setState(my_enums::_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                setState(my_enums::_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();

            }//exit button
            

            if (configButton.clicked(mousex, mousey)) {
                setState(my_enums::_CONFIGMENU_);
                previousScreen = my_enums::_HOMETOWN_;
            }//config button

            if (achievementsButton.clicked(mousex, mousey)) {
                setState(my_enums::_ACHIEVEMENTS_);
                previousScreen = my_enums::_HOMETOWN_;

            }

            if (inventoryButton.clicked(mousex, mousey)) {
                setState(my_enums::_INVENTORY_);
                previousScreen = my_enums::_HOMETOWN_;
            }//inventory button

            if (playerButton.clicked(mousex, mousey)) {
                setState(my_enums::_PLAYER_);
                previousScreen = my_enums::_HOMETOWN_;
            }//inventory button

            if (moveRightButton.clicked(mousex, mousey)) {
                px++;
                updateMap();
                
                
            }//config button

            if (moveLeftButton.clicked(mousex, mousey)) {
                px--;
                updateMap();
                //baseMap.blur();
            }//config button

            if (moveDownButton.clicked(mousex, mousey)) {
                py++;
                updateMap();
                //baseMap.blur();
            }//config button

            if (moveUpButton.clicked(mousex, mousey)) {
                py--;
                updateMap();
               // baseMap.blur();
            }//config button


            if (newMapButton.clicked(mousex, mousey)) {
                
                int width, height;
                width = 256;
                height = 256;
               // baseMap.mymap.init();
                baseMap.mymap.generate(rand() % 6 + 1, rand() % 6 + 1, 0.5f, 1, 1, width, height);

                switch (getState())
                {
                
                case my_enums::_ELEMENTAL_WATER_WORLD_:
                {
                    currentState = my_enums::_HOMETOWN_;
                    break;
                }

                case my_enums::_ELEMENTAL_FIRE_WORLD_:
                {
                    currentState = my_enums::_ELEMENTAL_WATER_WORLD_;
                    break;
                }

                case my_enums::_COAST_WORLD_:
                {
                    currentState = my_enums::_ELEMENTAL_FIRE_WORLD_;
                    break;
                }
                case my_enums::_HOMETOWN_:
                {
                    currentState = my_enums::_COAST_WORLD_;
                    break;
                }

                default:
                {
                    // is likely to be an error
                }
                };


                baseMap.mymap.to_surface(baseMap.imageSurface,getState());
                updateMap();
                baseMap.blur();

            }//config button

        }
        else if (e.type == SDL_KEYDOWN)
        {
            //Select surfaces based on key press
            switch (e.key.keysym.sym)
            {
            case SDLK_a:
                px--;
                updateMap();
              //  baseMap.blur();
                break;

            case SDLK_w:
                py--;
                updateMap();
               // baseMap.blur();
                break;

            case SDLK_s:
                py++;
                updateMap();
              //  baseMap.blur();
                break;

            case SDLK_d:
                px++;
                updateMap();
              //  baseMap.blur();
                break;


            default:
                  break;
            }
        }

    }
    myTime = (int)(timer.getTicks() / 1000);
}

void game::drawPlayer()
{
    SDL_Color player = { 0,0,200,0 };
    
    int tx =  px - cam_x;
    int ty =  py - cam_y;

    SDL_Rect target;
    target.x = (gScreenSurface->w / cam_size_x) * tx;
    target.y = (gScreenSurface->h / cam_size_y) * ty;
    target.w = gScreenSurface->w / cam_size_x;
    target.h = gScreenSurface->h / cam_size_y;

    

    //drawSquare(target,player);
    drawPlayerTileset(target, playerTile);
    


}


void game::drawNPCs()
{
    SDL_Color NPCColor = { 200,0,0,0 };
    int tx, ty;
    SDL_Rect target;
//    NPC aNPC;

    //list<s_Foe> tmp;
    for (list<NPC>::iterator it = NPCs.begin(); it != NPCs.end(); it++)
    {
        if (currentState == it->map) {
           // s_Foe aFoe;
            tx = it->x - cam_x;
            ty = it->y - cam_y;


            target.x = (gScreenSurface->w / cam_size_x) * tx;
            target.y = (gScreenSurface->h / cam_size_y) * ty;
            target.w = gScreenSurface->w / cam_size_x;
            target.h = gScreenSurface->h / cam_size_y;



           // drawSquare(target, NPCColor);
            drawPlayerTileset(target, it->tile);
           // tmp.push_back(aFoe);
        }
    }
}//end


//void game::drawPlayerTileset(int x, int y, Uint8 player)
void game::drawPlayerTileset(SDL_Rect target, int player)
{
    if (player != 0) {
        int sx;
        int sy;
        sy = player / 20;
        sx = player % 20;

        SDL_Rect playerSrc, playerDest;
        playerSrc.x = sx * 32 + 1;
        playerSrc.y = sy * 32 + 1;
        playerSrc.w = 32;
        playerSrc.h = 32;
        //playerDest.x = x;
        //playerDest.y = y;
       // playerDest.w = 32;
       // playerDest.h = 32;
            SDL_RenderCopy(gRenderer, playersTexture, &playerSrc, &target);
    }
}//End 

void game::checkBoundaries()
{
    if (px < 1) { px = 1; }
    if (py < 1) { py = 1; }
    if (px > 256) { px = 256; }
    if (py > 256) { py = 256; }

}


void game::addItem(string name, string description, int count, int value, int tile)
{
    item aItem;
    aItem.name = name;
    aItem.description = description;
    aItem.count = count;
    aItem.value = value;
    aItem.tile = tile;
    items.push_back(aItem);
    addNotification("Has conseguido:" + name);
}


void game::updateItem(string name, int value)
{
    for (list<item>::iterator it = items.begin(); it != items.end(); it++)
    {
        if (it->name == name) {
            item aItem;
            aItem.name = name;
            aItem.count = it->count + value;
            items.erase(it);
            if (aItem.count > 0) { items.push_back(aItem); }
            break;
        }
    }
}

bool game::findItem(string name)
{
    bool found = false;
    for (list<item>::iterator it = items.begin(); it != items.end(); it++)
    {
        if (it->name == name)
        {
            found = true;
            break;
        }
    }
    return found;
}
