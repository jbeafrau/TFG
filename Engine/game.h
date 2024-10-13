#ifndef GAME_H
#define GAME_H
#include <string>
#include <iostream>
#include "classButton.h"
#include "classTimer.h"
#include <cstdlib>
#include <ctime>       /* time */
#include<list>

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


enum gameState
{
    _NULL_ = -1,
    _INTRO_ = 1,
    _MAINMENU_ = 2,
    _NAME_ = 3,
    _GENERATEPLAYER_ = 4,
    _CONFIGMENU_ = 5,
    _HOMETOWN_ = 6,
    _GAMEOVER_ = 900,
    _GAMECLOSE_ = 999
};

enum playerRaces
{
    _HUMAN_ = 1,
    _ELF_ = 2,
    _DARF_ = 3,
    _HALFING_ = 4,
    _HALFORC_ = 5
};

enum playerArchetype
{
    _FIGHTER_ = 1,
    _ARCHER_ = 2,
    _THIEVE_ = 3,
    _MAGE_ = 4,
    _NECROMANCER_ = 5
};

class game
{
    public:
        game();
        virtual ~game();

        bool initSDL();
        //Loads media
        bool loadMedia(string base);

        void closeSDL();
        SDL_Surface* loadSurface( std::string path );

       void start();
       int dice(int maxValue, int minValue);

       int getState();
       void setState(gameState newState);

       std::string getBasePath();


       void events();

       void drawScreens();

       void screenFlip();

       void adjustFPS();

       void addNotification(std::string notification);
       void eventsNotifications();

       void addAchievement(std::string achievement);
       bool existAchievement(std::string achievement);


        void drawSquare(SDL_Rect rect,SDL_Color color);
        void drawTransparentSquare(SDL_Rect rect,SDL_Color color);
        void drawButton(classButton btn);
        void drawButtonSrc(classButton btn,SDL_Texture* texture);
        void drawText(string text, SDL_Rect rect);
        void drawTextL(string text, SDL_Rect rect);
        void drawTextBlock(string text, SDL_Rect rect);
        void drawTextResize(string text, SDL_Rect rect);
        void paintFruit(int x,int y, Uint8 fruit);
        
        void drawIMG(SDL_Surface* surface, int x, int y, int value);
        void screenClear();
        void screenIntro();
        void screenNotifications();
        void screenMain();
        void screenName();
        void screenGameOver();
        void screenPlayerName();

        void screenGeneratePlayer();
        void screenConfigMenu();
        void screenHomeTown();

        void eventsIntro();
        void eventsMain();
        void eventsGameOver();
        void eventsName();
        void eventsGeneratePlayer();
        void eventsConfigMenu();
        void eventsHomeTown();


    protected:
        int width = 600;
        int height = 700;
        int currentState = _INTRO_;

        int desiredFPS = 30;

        SDL_Rect mouseRect;

        string basePath;


        std::list<std::string> achievements;
        std::list<std::string> notifications;

        int y1,y2,y3,y4;


        //int players =1;
        int currentMusic =1;
        //int currentState = 0;
        int currentPlayer = 1;
       // int currentPhoto;
        bool musicON =true;
        bool squareDraw = true;
        /*currentstate:
          0=stop
          1-play
          2-pause
          */

        std::string playerName = "Jugador";

          //The application timer
			classTimer timer;
            classTimer timerGameOver;
            classTimer FPStimer;

            int ticksPerFrame = 0;
            int currentFrame = 0;
			int myTime = 0;
            int minutes = 0;
            int popupTime = 0;
            int maxAchievements = 7;
            int volumeSound = 128;
            int volumeMusic = 128;

            gameState previousScreen = _NULL_;


       // bool fight = false;

        int mousex,mousey;

        //The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

//Current displayed PNG image
SDL_Surface* gPNGSurface = NULL;

SDL_Surface* foodSurface = NULL;

SDL_Surface* buttonSurface = NULL;

SDL_Surface* buttonCloseSurface = NULL;
SDL_Surface* buttonMusicSurface = NULL;
SDL_Surface* buttonSocialSurface = NULL;
SDL_Surface* buttonPauseSurface = NULL;
SDL_Surface* buttonPlaySurface = NULL;
SDL_Surface* buttonDangerSurface = NULL;
SDL_Surface* mouseSurface = NULL;

//The final texture
SDL_Texture* mouseTexture = NULL;

SDL_Texture* foodTexture = NULL;
SDL_Texture* buttonTexture = NULL;

SDL_Texture* buttonCloseTexture = NULL;
SDL_Texture* buttonMusicTexture = NULL;
SDL_Texture* buttonSocialTexture = NULL;
SDL_Texture* buttonPauseTexture = NULL;
SDL_Texture* buttonPlayTexture = NULL;
SDL_Texture* buttonDangerTexture = NULL;

//Globally used font
TTF_Font *gFont = NULL;

Mix_Music *musicRetro = NULL;
Mix_Music *music8bit = NULL;
Mix_Music *musicFunky = NULL;

Mix_Music* musicGameOver = NULL;

Mix_Chunk *audioButton = NULL;
Mix_Chunk *line = NULL;
Mix_Chunk *block = NULL;
Mix_Chunk *loose = NULL;
Mix_Chunk *win = NULL;


classButton exitButton;
classButton startButton;
//classButton playersButton;
classButton musicButton;
classButton player1Button;
//classButton player2Button;
classButton continueButton;
//classButton rollButton;
classButton muteButton;
//classButton typeButton;

classButton configButton;
classButton volumeUpButton;
classButton volumeSoundUpButton;
classButton volumeMusicUpButton;
classButton volumeSoundDownButton;
classButton volumeMusicDownButton;
classButton volumeSoundButton;
classButton volumeMusicButton;
classButton achievementsButton;

classButton mouseButton;

SDL_Rect tmpRect;
    private:
};

#endif // GAME_H
