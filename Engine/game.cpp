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


    y1 = dice(250, 400);
    y2 = dice(250, 400);
    y3 = dice(250, 400);
    y4 = dice(250, 400);

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


void game::addAchievement(std::string achievement)
{
    if (!existAchievement(achievement)) {
        achievements.push_back(achievement);
        addNotification(achievement);
        Mix_PlayChannel(-1, win, 0);

        int achievementCounter = achievements.size();
        achievementsButton.setCaption("ACH " + std::to_string(achievementCounter) + " / " + std::to_string(achievementCounter * 100 / maxAchievements) + "%");
        //"Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60)
    }

}


bool game::existAchievement(std::string achievement)
{
    bool returnValue = false;

    for (std::string currentAchievement : achievements) {
        if (currentAchievement == achievement) {
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

    //baseMap.createSurface((gScreenSurface->w / 10) * 8, (gScreenSurface->h / 10) * 8);
    baseMap.createSurface();

    int width, height;
    width = 256;
    height = 256;
    baseMap.mymap.init();
    baseMap.mymap.generate(baseMap.mymap.seed_a.frequency, baseMap.mymap.seed_a.octave, baseMap.mymap.seed_a.persistance, 1, 1, width, height);
    baseMap.mymap.to_surface(baseMap.imageSurface);



    baseMap.imageSurface = SDL_ConvertSurface(baseMap.imageSurface, gScreenSurface->format, 0);

    //baseMap.SetSurface();

    updateMap();

 
    // baseMap.targetSurface = SDL_ConvertSurface(baseMap.targetSurface, gScreenSurface->format, 0);

    baseMap.blur();

    ticksPerFrame = (int)(1000 / desiredFPS);

    //Hide Window´s cursor
    SDL_ShowCursor(SDL_DISABLE);



    moveLeftButton.setButton(0, gScreenSurface->h / 2, 128, 128, "Left");
    moveLeftButton.setColor(0, 0, 200);
    moveRightButton.setButton(276 , gScreenSurface->h / 2, 128, 128, "Right");
    moveRightButton.setColor(0, 0, 200);
    moveUpButton.setButton(138, gScreenSurface->h / 2 -138, 128, 128, "Up");
    moveUpButton.setColor(0, 0, 200);
    moveDownButton.setButton(138, gScreenSurface->h / 2, 128, 128, "Down");
    moveDownButton.setColor(0, 0, 200);


    nextButton.setButton(gScreenSurface->w / 2 +128, gScreenSurface->h /2, 128, 128, "Siguiente");
    nextButton.setColor(128, 128, 128);

    prevButton.setButton(gScreenSurface->w / 2 - 256, gScreenSurface->h /2, 128, 128, "Anterior");
    prevButton.setColor(128, 128, 128);

    exitButton.setButton(gScreenSurface->w - 128, 0, 128, 128, "Salir");
    exitButton.setColor(200, 0, 0);

    //continueButton.setButton(gScreenSurface->w / 2, gScreenSurface->h / 2 - 64, 128, 128, "Continuar");
    continueButton.setButton(gScreenSurface->w / 2 -64, gScreenSurface->h - 128, 128, 128, "Continuar");
    continueButton.setColor(0, 0, 200);

    startButton.setButton(gScreenSurface->w / 2 -64, gScreenSurface->h / 2 -64, 128, 128, "Jugar");
    startButton.setColor(100, 100, 100);

    player1Button.setButton(gScreenSurface->w / 2, gScreenSurface->h / 2 + 100, 200, 50, "Nombre Jugador");
    player1Button.setColor(100, 100, 100);

    //configButton.setButton(gScreenSurface->w / 2, gScreenSurface->h / 2 + 150, 200, 50, "Configuración");
    configButton.setButton(1, gScreenSurface->h -128, 128, 128, "Configuración");
    configButton.setColor(100, 100, 100);

    musicButton.setButton(gScreenSurface->w / 2-100, gScreenSurface->h / 2 + 100, 200, 50, "Cambia canción");
    musicButton.setColor(100, 100, 100);

    muteButton.setButton(gScreenSurface->w / 2 -100, gScreenSurface->h / 2 + 150, 200, 50, "MUSIC ON");
    muteButton.setColor(100, 100, 100);

    volumeSoundUpButton.setButton(gScreenSurface->w / 2 -100, gScreenSurface->h / 2 + 200, 50, 50, "+");
    volumeSoundUpButton.setColor(100, 100, 100);

    volumeSoundButton.setButton(gScreenSurface->w / 2 + 50 - 100, gScreenSurface->h / 2 + 200, 100, 50, "SOUND 100%");
    volumeSoundButton.setColor(200, 200, 200);

    volumeSoundDownButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 2 + 200, 50, 50, "-");
    volumeSoundDownButton.setColor(100, 100, 100);

    volumeMusicUpButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 2 + 250, 50, 50, "+");
    volumeMusicUpButton.setColor(100, 100, 100);

    volumeMusicButton.setButton(gScreenSurface->w / 2 + 50 - 100, gScreenSurface->h / 2 + 250, 100, 50, "MUSIC 100%");
    volumeMusicButton.setColor(200, 200, 200);

    volumeMusicDownButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 2 + 250, 50, 50, "-");
    //volumeMusicDownButton.setButton(50, 100, 50, 50, "-");
    volumeMusicDownButton.setColor(100, 100, 100);

    mouseButton.setButton(0, 0, 200, 50, "X:" + std::to_string(mousex) + " Y:" + std::to_string(mousey));
    mouseButton.setColor(100, 100, 100);

    achievementsButton.setButton(gScreenSurface->w / 2 - 64, gScreenSurface->h - 128, 128, 128, "ACH 0 / 0%");
    achievementsButton.setColor(0, 0, 200);



    Mix_PlayMusic(musicINTRO, -1);
}

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


int game::getState()
{
    return currentState;
}

void game::setState(gameState newState)
{
    currentState = newState;
}


std::string game::getBasePath()
{
    return basePath;
}

void game::events()
{
    switch (getState())
    {
    case _INTRO_:
    {
        eventsIntro();
        break;
    }
    case _MAINMENU_:
    {
        eventsMain();
        break;
    }

    case _NAME_:
    {
        eventsName();
        break;
    }

    case _RACES_:
    {
        eventsRaces();
        break;
    }

    case _ARCHETYPES_:
    {
        eventsArchetypes();
        break;
    }



    case _GAMEOVER_:
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

    case _CONFIGMENU_:
    {
        eventsConfigMenu();
        break;
    }

    case _HOMETOWN_:
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


void game::drawScreens()
{
    switch (getState())
    {
    case _INTRO_:
    {
        screenIntro();
        break;
    }
    case _MAINMENU_:
    {
        screenMain();
        break;
    }

    case _NAME_:
    {
        screenPlayerName();
        break;
    }

    case _GAMEOVER_:
    {
        screenGameOver();
        break;
    }

    case _RACES_:
    {
        screenRaces();
        break;
    }

    case _ARCHETYPES_:
    {
        screenArchetypes();
        break;
    }

   /* case _GENERATEPLAYER_:
    {
        screenGeneratePlayer();
        break;
    }
    */

    case _CONFIGMENU_:
    {
        screenConfigMenu();
        break;
    }

    case _HOMETOWN_:
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

    mouseButton.setCaption("X:" + std::to_string(mousex) + " Y:" + std::to_string(mousey));
    drawButton(mouseButton);

    //Flip image to monitor
    SDL_RenderPresent(gRenderer);
}

void game::adjustFPS()
{
    currentFrame = FPStimer.getTicks();
    if (currentFrame < ticksPerFrame) {
            SDL_Delay(ticksPerFrame - currentFrame);
    }
    FPStimer.reset();

}

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

void game::drawSquare(SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderFillRect(gRenderer, &rect);
}

void game::drawTransparentSquare(SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderDrawRect(gRenderer, &rect);
}

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

}

void game::drawMap()
{
    SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, baseMap.targetSurface);
    SDL_RenderCopy(gRenderer, txtTexture, NULL, NULL);

}

void game::updateMap()
{
    SDL_Rect srcrect;
    srcrect.x = px;
    srcrect.y = py;
    //srcrect.w = 40;
    //srcrect.h = 20;

    srcrect.w = 16;
    srcrect.h = 8;

    /*SDL_Rect  dstrect;
     dstrect.x = 0;
     dstrect.y = 0;
     dstrect.w = 256;
     dstrect.h = 256;*/


    SDL_BlitScaled(baseMap.imageSurface, &srcrect, baseMap.targetSurface, NULL);
}

void game::drawButtonSrc(classButton btn, SDL_Texture* texture)
{
    SDL_Rect target = btn.getRect();
    SDL_RenderCopy(gRenderer, texture, NULL, &target);

    SDL_Color fg = { 0,0,0,0 };
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
    }
}

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
    }
}

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
    }
}

void game::drawTextResize(string text, SDL_Rect rect)
{
    SDL_Color fg = { 0,0,0,0 };
    if (text.length() > 0) {
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(gFont, text.c_str(), fg);
        SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, txtTexture, NULL, &rect);
    }
}

void game::paintFruit(int x, int y, Uint8 fruit)
{
    if (fruit != 0) {
        int sx;
        int sy;
        sy = fruit / 8;
        sx = fruit % 8;

        SDL_Rect foodSrc, fooddstrect;
        foodSrc.x = sx * 32 + 1;
        foodSrc.y = sy * 32 + 1;
        foodSrc.w = 32;
        foodSrc.h = 32;
        fooddstrect.x = x;
        fooddstrect.y = y;
        fooddstrect.w = 32;
        fooddstrect.h = 32;

        if (squareDraw == true) {
            //drawButtonSrc()
            SDL_Color fg = { 0,0,0,0 };
            int color = fruit % 6;
            switch (color) {
            case 0:
                // fg={ fruit*4,0,0,0};
                fg = { 255,0,0,0 };
                break;
            case 1:
                //fg={0,fruit*4,0,0};
                fg = { 0,255,0,0 };
                break;
            case 2:
                //fg={0,0,fruit*4,0};
                fg = { 0,0,255,0 };
                break;
            case 3:
                //        fg={fruit*4,fruit*4,0,0};
                fg = { 255,255,0,0 };
                break;
            case 4:
                //      fg={0,fruit*4,fruit*4,0};
                fg = { 0,255,255,0 };
                break;
            case 5:
                //fg={fruit*4,0,fruit*4,0};
                fg = { 255,0,255,0 };
                break;
            }
            drawSquare(fooddstrect, fg);
            fg = { 0,0,0,0 };
            drawTransparentSquare(fooddstrect, fg);
        }
        else {
            SDL_RenderCopy(gRenderer, foodTexture, &foodSrc, &fooddstrect);
        }

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

    drawButton(exitButton);
    drawButton(startButton);
    //drawButton(musicButton);
    //drawButton(muteButton);
    //drawButton(player1Button);

    drawButton(configButton);

    drawButton(achievementsButton);
}

void game::screenPlayerName()
{

    SDL_Rect snow0, snow1;
    snow0.x = 1;
    snow0.y = 1;
    snow0.w = gScreenSurface->w;
    snow0.h = gScreenSurface->h;
    snow1.x = 1;
    snow1.y = 1;
    snow1.w = gScreenSurface->w;
    snow1.h = gScreenSurface->h;
   // SDL_Rect dstrect;

    drawButton(exitButton);
    drawButton(continueButton);


    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;

    drawText("CAMBIAR NOMBRE", tmpRect);
    tmpRect.y = 200;
    drawText("Introduce el nombre del jugador", tmpRect);

    tmpRect.y = 250;
    drawText(playerName, tmpRect);
    tmpRect.y = 300;
    drawText("Luego pulsa continuar...", tmpRect);
}

void game::screenClear()
{
    SDL_SetRenderDrawColor(gRenderer, 0x99, 0x99, 0x99, 0x99);
    SDL_RenderClear(gRenderer);
}

void game::screenIntro()
{

    drawButton(exitButton);
    drawButton(continueButton);

    tmpRect.x = gScreenSurface->w / 2 - 350;
    tmpRect.y = gScreenSurface->h / 4;
    tmpRect.w = 700;
    tmpRect.h = 100;


    tmpRect.x = gScreenSurface->w / 2 - 450;
    tmpRect.w = 900;
    drawTextResize("Achievement master", tmpRect);
}


void game::screenNotifications()
{
    // tmpRect.y = (gScreenSurface->h / 4)*3;
    tmpRect.h = 30;


    tmpRect.x = 0;
    tmpRect.w = 500;
    int tmpY = 0;
    if (notifications.size() > 0) {

        for (std::string notification : notifications) {

            tmpRect.y = ((gScreenSurface->h / 4) * 3) + (tmpY * 30);
            drawTextResize(notification, tmpRect);
            tmpY++;
        }

        //drawTextResize(notifications.front(), tmpRect);
    }
}

void game::screenGameOver()
{
    tmpRect.x = gScreenSurface->w / 2 - 350;
    tmpRect.y = gScreenSurface->h / 4;
    tmpRect.w = 700;
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
            setState(_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.reset();
            timerGameOver.start();
        }
        //Special key input
                          //Special text input event

        else if (e.type == SDL_MOUSEMOTION)
        {
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                setState(_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timer.start();
                timer.reset();
            }
            if (continueButton.clicked(mousex, mousey)) {
                setState(_MAINMENU_);
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
            setState(_GAMEOVER_);
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
                setState(_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
                //addNotification("Saliendo del juego");
                addAchievement("Saliendo del juego");
            }

            if (configButton.clicked(mousex, mousey)) {
                setState(_CONFIGMENU_);
                previousScreen = _MAINMENU_;

            }


            if (startButton.clicked(mousex, mousey)) {
                //addNotification("Comenzando el juego");
                addAchievement("Comenzando el juego");
                SDL_StartTextInput();
                setState(_NAME_);
            }



        }
  

        //******************
    }

    myTime = (int)(timer.getTicks() / 1000);
}

void game::eventsGameOver()
{
    if (timerGameOver.getTicks() > 3000) {
        setState(_GAMECLOSE_);
        /*  Mix_FadeOutMusic(3000);

          while (!Mix_FadeOutMusic(3000) && Mix_PlayingMusic()) {
              // wait for any fades to complete
              SDL_Delay(100);
          }*/
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



void game::eventsName()
{
    //bool quit = false;

    //Event handler
    SDL_Event e;
    //quit = false;
   // SDL_StartTextInput();
    //While application is running
    //while( !quit )
      //  {
                    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            //closeSDL();
            SDL_StopTextInput();
            setState(_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver.start();
            timerGameOver.reset();
        }
        //Special key input
        else if (e.type == SDL_KEYDOWN)
        {
            //Handle backspace
            if (e.key.keysym.sym == SDLK_BACKSPACE && playerName.length() > 0) { playerName.pop_back(); }
            /* if (currentPlayer == 1) {
             if( e.key.keysym.sym == SDLK_BACKSPACE && player1.name.length() > 0 ){ player1.name.pop_back();}
             }else{
                 if( e.key.keysym.sym == SDLK_BACKSPACE && player2.name.length() > 0 ){ player2.name.pop_back();}
             }*/


        }
        //Special text input event
        else if (e.type == SDL_TEXTINPUT)
        {
            playerName += e.text.text;
            /* if (currentPlayer == 1) {
                 player1.name += e.text.text;
             }else{
                 player2.name += e.text.text;
             }*/

             //renderText = true;
         //}
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
                setState(_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
            }
            if (continueButton.clicked(mousex, mousey)) {
                // quit =true;
                Mix_PlayChannel(-1, audioButton, 0);
                SDL_StopTextInput();
                //setState(_MAINMENU_);
                setState(_RACES_);
                //Mix_PlayMusic(musicGameOver, -1);
                //timerGameOver.start();
                //timerGameOver.reset();

            }
        }

    }
    //screenPlayerName();
    //SDL_Delay(50);
//	}
//Disable text input
  //  SDL_StopTextInput();
}


void game::screenGeneratePlayer()
{

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

    drawButton(exitButton);
    drawButton(startButton);
    drawButton(musicButton);
    drawButton(muteButton);
    drawButton(volumeSoundUpButton);
    drawButton(volumeSoundButton);
    drawButton(volumeSoundDownButton);

    drawButton(volumeMusicUpButton);
    drawButton(volumeMusicButton);
    drawButton(volumeMusicDownButton);

    // 
    //drawButton(configButton);
//    drawButton(player1Button);

  //  drawButton(achievementsButton);

}

void game::screenHomeTown()
{

    SDL_Rect target;
     target.x = 0;
    target.y = 0;

    //target.x = gScreenSurface->w / 10;
    //target.y = gScreenSurface->h / 10;
    //target.w = 256;
    //target.h = 256;

    //target.w = (gScreenSurface->w / 10) * 8;
    //target.h = (gScreenSurface->h / 10) * 8;

    target.w = gScreenSurface->w;
    target.h = gScreenSurface->h;

    //SDL_Color mapColor = { 120,120,120,0 };

    //drawSquare(target, mapColor);

    //SDL_Texture* mapTexture = SDL_CreateTextureFromSurface(gRenderer, baseMap.imageSurface);
   // SDL_Texture* mapTexture = SDL_CreateTextureFromSurface(gRenderer, baseMap.targetSurface);
    //SDL_RenderCopy(gRenderer, mapTexture, NULL, &target);
    //SDL_RenderCopy(gRenderer, mapTexture, NULL, NULL);
    drawMap();

    //SDL_Rect dstrect;


  // drawButton(nextButton);
  //  drawButton(prevButton);


    tmpRect.x = gScreenSurface->w / 2 - 200;
    //tmpRect.y = 100;
    tmpRect.y = gScreenSurface->h / 12;
    

    tmpRect.w = 400;
    tmpRect.h = 50;

    drawText("POBLADO INICIAL", tmpRect);
  
    tmpRect.x = gScreenSurface->w - 500;
    tmpRect.y = gScreenSurface->h - 50;
    tmpRect.w = 500;
    tmpRect.h = 50;

    drawText("Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60), tmpRect);


  


    drawButton(exitButton);
    drawButton(achievementsButton);
    drawButton(configButton);

    drawButton(moveLeftButton);
    drawButton(moveRightButton);
    drawButton(moveUpButton);
    drawButton(moveDownButton);
    

}

void game::eventsGeneratePlayer()
{

}

std::string game::getRaceName(playerRaces race)
{

    switch (race)
    {
    case _HUMAN_:
    {
        return "Humano";
        break;
    }
    case _ELF_:
    {
        return "Elfo";
        break;
    }

    case _DARF_:
    {
        return "Enano";
        break;
    }

    case _HALFING_:
    {
        return "Mediano";
        break;
    }

    case _HALFORC_:
    {
        return "Medio Orco";
        break;
    }

    case _BEHOLDER_:
    {
        return "Contemplador";
        break;
    }
    
    default:
    {
        // is likely to be an error
    }
    };


}


std::string game::getArchetypeName(playerArchetype archetype)
{

    switch (archetype)
    {
    case _FIGHTER_:
    {
        return "Guerrero";
        break;
    }
    case _ARCHER_:
    {
        return "Arquero";
        break;
    }

    case _THIEVE_:
    {
        return "Ladrón";
        break;
    }

    case _MAGE_:
    {
        return "Mago";
        break;
    }

    case _NECROMANCER_:
    {
        return "Necromante";
        break;
    }

    case _SUMMONER_:
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



void game::screenRaces()
{
    string tmpRace;
    SDL_Rect snow0, snow1;
    snow0.x = 1;
    snow0.y = 1;
    snow0.w = gScreenSurface->w;
    snow0.h = gScreenSurface->h;
    snow1.x = 1;
    snow1.y = 1;
    snow1.w = gScreenSurface->w;
    snow1.h = gScreenSurface->h;
   // SDL_Rect dstrect;

    drawButton(exitButton);
    drawButton(continueButton);
    drawButton(nextButton);
    drawButton(prevButton);


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

    tmpRect.y = 400;
    drawText(getRaceName(currentRace), tmpRect);


}

void game::screenArchetypes()
{
    SDL_Rect snow0, snow1;
    snow0.x = 1;
    snow0.y = 1;
    snow0.w = gScreenSurface->w;
    snow0.h = gScreenSurface->h;
    snow1.x = 1;
    snow1.y = 1;
    snow1.w = gScreenSurface->w;
    snow1.h = gScreenSurface->h;
    //SDL_Rect dstrect;

    drawButton(exitButton);
    drawButton(continueButton);
    drawButton(nextButton);
    drawButton(prevButton);


    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;

    drawText("ARQUETIPO DEL JUGADOR", tmpRect);
    tmpRect.y = 200;
    drawText("Selecciona el arquetipo del personaje", tmpRect);

    //tmpRect.y = 250;
    //drawText(playerName, tmpRect);
    tmpRect.y = 300;
    drawText("Luego pulsa continuar...", tmpRect);

    tmpRect.y = 400;
    drawText(getArchetypeName(currentArchetype), tmpRect);
}


void game::eventsRaces()
{
    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            //closeSDL();
            //SDL_StopTextInput();
            setState(_GAMEOVER_);
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
               SDL_StartTextInput();
                setState(_NAME_);
                
            }
            if (continueButton.clicked(mousex, mousey)) {
                // quit =true;
                Mix_PlayChannel(-1, audioButton, 0);
               // SDL_StopTextInput();
                setState(_ARCHETYPES_);
                //Mix_PlayMusic(musicGameOver, -1);
                //timerGameOver.start();
                //timerGameOver.reset();

            }

            if (nextButton.clicked(mousex, mousey)) {
                switch (currentRace)
                {
                case _HUMAN_:
                {
                    currentRace = _ELF_;
                    break;
                }
                case _ELF_:
                {
                    currentRace = _DARF_;
                    break;
                }

                case _DARF_:
                {
                    currentRace = _HALFING_;
                    break;
                }

                case _HALFING_:
                {
                    currentRace = _HALFORC_;
                    break;
                }

                case _HALFORC_:
                {
                    currentRace = _BEHOLDER_;
                    break;
                }

                case _BEHOLDER_:
                {
                    currentRace = _HUMAN_;
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
                case _HUMAN_:
                {
                    currentRace = _BEHOLDER_;
                    
                   
                    break;
                }
                case _ELF_:
                {
                    currentRace = _HUMAN_;
                   
                    break;
                }

                case _DARF_:
                {
                    currentRace = _ELF_;
                   
                    break;
                }

                case _HALFING_:
                {
                    currentRace = _DARF_;
                   
                    break;
                }

                case _HALFORC_:
                {
                    
                    
                    currentRace = _HALFING_;
                    break;
                }

                case _BEHOLDER_:
                {
                    currentRace = _HALFORC_;
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

void game::eventsArchetypes()
{
    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            //closeSDL();
            //SDL_StopTextInput();
            setState(_GAMEOVER_);
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
                SDL_StartTextInput();
                setState(_RACES_);

            }
            if (continueButton.clicked(mousex, mousey)) {
                // quit =true;
                Mix_PlayChannel(-1, audioButton, 0);
               // SDL_StopTextInput();
                Mix_PlayMusic(musicTOWN, -1);
                setState(_HOMETOWN_);
                //Mix_PlayMusic(musicGameOver, -1);
                //timerGameOver.start();
                //timerGameOver.reset();

            }//continue


            if (nextButton.clicked(mousex, mousey)) {
                switch (currentArchetype)
                {
                case _FIGHTER_:
                {
                    currentArchetype = _ARCHER_;
                    break;
                }
                case _ARCHER_:
                {
                    currentArchetype = _THIEVE_;
                    break;
                }

                case _THIEVE_:
                {
                    currentArchetype = _MAGE_;
                    break;
                }

                case _MAGE_:
                {
                    currentArchetype = _NECROMANCER_;
                    break;
                }

                case _NECROMANCER_:
                {
                    currentArchetype = _SUMMONER_;
                    break;
                }

                case _SUMMONER_:
                {
                    currentArchetype = _FIGHTER_;
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
                case _FIGHTER_:
                {
                    currentArchetype = _SUMMONER_;
                    break;
                }
                case _ARCHER_:
                {
                    currentArchetype = _FIGHTER_;
                    break;
                }

                case _THIEVE_:
                {
                    currentArchetype = _ARCHER_;
                    break;
                }

                case _MAGE_:
                {
                    currentArchetype = _THIEVE_;
                    break;
                }

                case _NECROMANCER_:
                {
                    currentArchetype = _MAGE_;
                    break;
                }

                case _SUMMONER_:
                {
                    currentArchetype = _NECROMANCER_;
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
            setState(_GAMEOVER_);
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
                setState(_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();
                //addNotification("Saliendo del juego");
                addAchievement("Saliendo del juego");
            }

            if (muteButton.clicked(mousex, mousey)) {
                if (musicON == true) {
                    musicON = false;
                    muteButton.setCaption("MUSIC OFF");
                    Mix_Volume(-1, 0);
                    Mix_VolumeMusic(0);
                    //addNotification("Sonido apagado");
                    addAchievement("Sonido apagado");
                    volumeSoundButton.setCaption("SOUND " + std::to_string(volumeSound * 100 / 128) + "%");
                    volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
                }
                else {
                    musicON = true;
                    muteButton.setCaption("MUSIC ON");
                    //Mix_Volume(-1, MIX_MAX_VOLUME);
                    //Mix_VolumeMusic(MIX_MAX_VOLUME);

                    Mix_Volume(-1, volumeSound);
                    Mix_VolumeMusic(volumeMusic);
                    //addNotification("Sonido activado");
                    addAchievement("Sonido activado");
                    volumeSoundButton.setCaption("SOUND " + std::to_string(volumeSound * 100 / 128) + "%");
                    volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
                }
            }

            if (musicButton.clicked(mousex, mousey)) {
                switch (currentMusic) {

                case 1:
                    currentMusic = 2;
                    Mix_PlayMusic(musicDARK, -1);
                    addAchievement("Musica DARK");
                    break;
                case 2:
                    currentMusic = 3;
                    Mix_PlayMusic(musicFOREST, -1);
                    addAchievement("Musica FOREST");
                    break;
                case 3:
                    currentMusic = 4;
                    Mix_PlayMusic(musicTOWN, -1);
                    addAchievement("Musica TOWN");
                    break;

                case 4:
                    currentMusic = 5;
                    Mix_PlayMusic(musicBATTLE, -1);
                    addAchievement("Musica BATTLE");
                    break;

                case 5:
                    currentMusic = 6;
                    Mix_PlayMusic(musicBOSS, -1);
                    addAchievement("Musica BOSS");
                    break;

                case 6:
                    currentMusic = 7;
                    Mix_PlayMusic(musicCAVE, -1);
                    addAchievement("Musica CAVE");
                    break;

                case 7:
                    currentMusic = 8;
                    Mix_PlayMusic(musicHERO, -1);
                    addAchievement("Musica HERO");
                    break;

                case 8:
                    currentMusic = 9;
                    Mix_PlayMusic(musicMYSTICAL, -1);
                    addAchievement("Musica MYSTICAL");
                    break;

                case 9:
                    currentMusic = 1;
                    Mix_PlayMusic(musicINTRO, -1);
                    addAchievement("Musica INTRO");
                    break;

                }
            }


            if (startButton.clicked(mousex, mousey)) {
                //addNotification("Comenzando el juego");
                //addAchievement("Comenzando el juego");

               // setState(previousScreen);
                //setState(_MAINMENU_);
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



        }
        else if (e.type == SDL_KEYDOWN)
        {
            //Select surfaces based on key press
            switch (e.key.keysym.sym)
            {
            case SDLK_a:
                /*  if (!player1.gameOver) {
                  if (!player1.collide(-1,0))player1.chipX--;
                  }*/
                break;

            case SDLK_w:
                /*   if (!player1.gameOver) {
                       player1.rotateChip();
                       if (player1.collide(0,0)){
                           player1.rotateChip();
                           player1.rotateChip();
                           player1.rotateChip();
                           }
                   }*/

                break;

            case SDLK_s:
                /*  if (!player1.gameOver) {
              if (!player1.collide(0,1)){player1.chipY++;}
                  }*/
                break;

            case SDLK_d:
                /*   if (!player1.gameOver) {
                   if (!player1.collide(1,0))player1.chipX++;
                   }*/
                break;

            case SDLK_UP:
                /*      if (!player2.gameOver) {
                       player2.rotateChip();
                       if (player2.collide(0,0)){
                              player2.rotateChip();
                              player2.rotateChip();
                              player2.rotateChip();
                              }
                      }*/

                break;

            case SDLK_DOWN:
                /*if (!player2.gameOver) {
            if (!player2.collide(0,1)){player2.chipY++;}
                }*/
                break;

            case SDLK_LEFT:
                /*if (!player2.gameOver) {
                if (!player2.collide(-1,0))player2.chipX--;
                }*/

                break;

            case SDLK_RIGHT:
                /*                                if (!player2.gameOver) {
                                                if (!player2.collide(1,0))player2.chipX++;
                                                }*/

                break;

            default:

                break;
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
            setState(_GAMEOVER_);
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
                setState(_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver.start();
                timerGameOver.reset();

            }//exit button
            

            if (configButton.clicked(mousex, mousey)) {
                setState(_CONFIGMENU_);
                previousScreen = _HOMETOWN_;
            }//config button

            if (moveRightButton.clicked(mousex, mousey)) {
                px++;
                updateMap();
                baseMap.blur();
            }//config button

            if (moveLeftButton.clicked(mousex, mousey)) {
                px--;
                updateMap();
                baseMap.blur();
            }//config button

            if (moveDownButton.clicked(mousex, mousey)) {
                py++;
                updateMap();
                baseMap.blur();
            }//config button

            if (moveUpButton.clicked(mousex, mousey)) {
                py--;
                updateMap();
                baseMap.blur();
            }//config button

        }

    }
    myTime = (int)(timer.getTicks() / 1000);
}