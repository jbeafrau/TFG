#ifndef GAME_H
#define GAME_H
#include <string>
#include <iostream>
#include "classButton.h"
#include "classTimer.h"
#include "classMap.h"
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

/*  GAME DESIGN NOTES
 
MELE
-------
ATACK: 1D10 + SKILL + STR + WEAPON
DEFENSE: 1D10 + SKILL + STR + WEAPON + SHIELD
DAMAGE: WEAPON + 1D STR

BOW
-----------
1D10 + SKILL + DEX + WEAPON
1D10 + DEX + SHIELD
DAMAGE: WEAPON +1D STR -1

MAGIC
-----------------
DAMAGE: SPELL + 1D INT

ATTRIBUTES 
----------------
(BASE=1 for all, MAX = 5)
STR
DEX
RES
INT
PER

LIFE POINTS = 10 + RES*5
MAGIC POINTS = 10 + ((INT + PER)/2) *5


LEVEL = 1 (MAX 20)
EXPERIENCE  = 0, required for next LEVEL = (Current level^2)*10 (100,400,900, etc)

LEVEL INCREASE BENEFITS:
-------------------------
MAX LIFE POINTS +5
SKILL POINTS +1

SKILL POINTS
-------------------
1 SKILL POINT = 1 NEW SKILL
UPGRADING ONE ATTRIBUTE COSTS Current attribute level +1 SKILL POINTS


*/


enum gameState
{
    _NULL_ = -1,
    _INTRO_ = 1,
    _MAINMENU_ = 2,
    _NAME_ = 3,
    _RACES_ = 4,
    _ARCHETYPES_ = 5,
    _CONFIGMENU_ = 6,
    _HOMETOWN_ = 10,
    _TOWN_SHOP_ = 20,
    _TOWN_ELDER_ = 30,
    _ELEMENTAL_TEMPLE_ = 40,
    _FOREST_WORLD_ = 110,
    _COAST_WORLD_ = 120,
    _ELEMENTAL_FIRE_WORLD_ = 130,
    _ELEMENTAL_WATER_WORLD_ = 140,
    _ELEMENTAL_EARTH_WORLD_ = 150,
    _ELEMENTAL_WIND_WORLD_ = 160,
    _NECRO_WORLD_ = 170,    
    _FINAL_ARENA_ = 200,
    _GAMEOVER_ = 900,
    _GAMECLOSE_ = 999
};




enum playerRaces
{
    _HUMAN_ = 1, //+1 STR
    _ELF_ = 2, // +1 PER
    _DARF_ = 3, //+1 RES
    _HALFING_ = 4, //+1 INT
    _HALFORC_ = 5, //+1 STR +1 RES -1 PER
    _BEHOLDER_ = 6//+1 INT +1 PER -1 RES
};

enum playerArchetype
{
    _FIGHTER_ = 1, // +1 STR MELE
    _ARCHER_ = 2, // +1 DEX ARCHERY
    _THIEVE_ = 3, // +1 PER SHADOWS
    _MAGE_ = 4, // +1 INT MAGIC
    _NECROMANCER_ = 5, // +1 INT RAISEDEAD
    _SUMMONER_ = 6 // +1 INT SUMMON
};

enum Skills
{
    _MELE_ = 1,
    _ARCHERY_ = 2,
    _SHADOWS_ = 3,
    _MAGIC_ = 4,
    _RAISEDEAD_ = 5,
    _SUMMON_ = 6,
    _SWIM_ = 7,
    _BARTERING_ = 8,
    _SHIELDS_ = 9,
    _FIRSTAID_ = 10

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
    SDL_Surface* loadSurface(std::string path);




    void start();
    int dice(int maxValue, int minValue);

    int getState();
    void setState(gameState newState);

    std::string getBasePath();

    std::string getRaceName(playerRaces race);
    std::string getArchetypeName(playerArchetype archetype);

    void events();

    void drawScreens();

    void screenFlip();

    void adjustFPS();

    void addNotification(std::string notification);
    void eventsNotifications();

    void addAchievement(std::string achievement);
    bool existAchievement(std::string achievement);


    void drawSquare(SDL_Rect rect, SDL_Color color);
    void drawTransparentSquare(SDL_Rect rect, SDL_Color color);
    void drawButton(classButton btn);
    void drawButtonSrc(classButton btn, SDL_Texture* texture);
    void drawText(string text, SDL_Rect rect);
    void drawTextL(string text, SDL_Rect rect);
    void drawTextBlock(string text, SDL_Rect rect);
    void drawTextResize(string text, SDL_Rect rect);
    void paintFruit(int x, int y, Uint8 fruit);

    void drawIMG(SDL_Surface* surface, int x, int y, int value);
    void screenClear();
    void screenIntro();
    void screenNotifications();
    void screenMain();
    void screenName();
    void screenGameOver();
    void screenPlayerName();

    void screenRaces();
    void screenArchetypes();

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

    void eventsRaces();
    void eventsArchetypes();

    classMap baseMap;

protected:
    int width = 600;
    int height = 700;
    int currentState = _INTRO_;

    int desiredFPS = 60;

    SDL_Rect mouseRect;

    string basePath;


    std::list<std::string> achievements;
    std::list<std::string> notifications;

    int y1, y2, y3, y4;


    //int players =1;
    int currentMusic = 1;
    //int currentState = 0;
    int currentPlayer = 1;
    // int currentPhoto;
    bool musicON = true;
    bool squareDraw = true;
    /*currentstate:
      0=stop
      1-play
      2-pause
      */

    std::string playerName = "Jugador";
    playerRaces currentRace = _HUMAN_;
    playerArchetype currentArchetype = _FIGHTER_;


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

    int mousex, mousey;

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
    TTF_Font* gFont = NULL;

    Mix_Music* musicINTRO = NULL;
    Mix_Music* musicDARK = NULL;
    Mix_Music* musicFOREST = NULL;
    Mix_Music* musicTOWN = NULL;
    Mix_Music* musicGameOver = NULL;

    Mix_Music* musicBATTLE = NULL;
    Mix_Music* musicBOSS = NULL;
    Mix_Music* musicCAVE = NULL;
    Mix_Music* musicHERO = NULL;
    Mix_Music* musicMYSTICAL = NULL;
    Mix_Music* musicTLE = NULL;


    Mix_Chunk* audioButton = NULL;
    //Mix_Chunk *line = NULL;
    //Mix_Chunk *block = NULL;
    Mix_Chunk* loose = NULL;
    Mix_Chunk* win = NULL;


    classButton nextButton;
    classButton prevButton;

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

    classMap mapTown;

private:
};

#endif // GAME_H
