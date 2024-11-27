#ifndef GAME_H
#define GAME_H
#include <string>
#include <iostream>
#include "classButton.h"
#include "classTimer.h"
#include "classMap.h"
#include "enum.h"
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

struct NPC { //enemy definition
    int id;
    int x,y;
    my_enums::gameState map;
    string description;
    int skill;
    int stamina;
    int exp;
    my_enums::AItypes NPCAI;
    int tile;
};

struct item { //item definition
    string name;
    string description;
    int count;
    int value;
    int tile;
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
    void setState(my_enums::gameState newState);

    std::string getBasePath();

    std::string getRaceName(my_enums::playerRaces race);
    std::string getArchetypeName(my_enums::playerArchetype archetype);

    void events();

    void drawScreens();

    void screenFlip();

    void adjustFPS();

    void addNotification(std::string notification);
    void eventsNotifications();

    void addAchievement(std::string achievement);
    bool existAchievement(std::string achievement);

    void addItem(string name, int value);
    void updateItem(string name, int value);
    bool findItem(string name);

    void drawSquare(SDL_Rect rect, SDL_Color color);
    void drawTransparentSquare(SDL_Rect rect, SDL_Color color);
    void drawButton(classButton btn);
    void drawButtonSrc(classButton btn, SDL_Texture* texture);
    void drawText(string text, SDL_Rect rect);
    void drawTextL(string text, SDL_Rect rect);
    void drawTextBlock(string text, SDL_Rect rect);
    void drawTextResize(string text, SDL_Rect rect);
    void paintFruit(int x, int y, Uint8 fruit);
    void drawMap();
    void updateMap();
    void drawPlayer();
    void drawNPCs();
    //void drawPlayerTileset(int x, int y, Uint8 player);
    void drawPlayerTileset(SDL_Rect target, int player);

    

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

    void checkBoundaries();

    classMap baseMap;

protected:
    int width = 600;
    int height = 700;
    int currentState = my_enums::_INTRO_;

    int desiredFPS = 60;

    SDL_Rect mouseRect;

    string basePath;


    std::list<std::string> achievements;
    std::list<std::string> notifications;
    
    std::list<NPC> NPCs; //All NPCs
    std::list<NPC> tmpNPCs; //NPCs found in our location

    std::list<item> items; //Player inventory
    int coins = 0; //Player currency
    int food = 0; //Player food rations 



    int currentMusic = 1;
    bool musicON = true;
    //bool squareDraw = true;


    std::string playerName = "Jugador"; //Player´s name
    my_enums::playerRaces currentRace = my_enums::_HUMAN_; //Player´s race
    my_enums::playerArchetype currentArchetype = my_enums::_FIGHTER_; //Player´s archetype
    int px, py; //Player location in the world
    int playerTile = 63; //Starting tile for player


    int cam_x, cam_y;//camera starting vertex (top-left)

    //Number of tiles drawn on screen
    int cam_size_x = 16;
    int cam_size_y = 8;




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

    my_enums::gameState previousScreen = my_enums::_NULL_;


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

    SDL_Surface* playersSurface = NULL;

    //The final texture
    SDL_Texture* mouseTexture = NULL;
    SDL_Texture* playersTexture = NULL;


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

  //  classButton blurDownButton;
   // classButton blurUpButton;
    classButton blurButton;

    classButton achievementsButton;


    classButton moveLeftButton;
    classButton moveRightButton;
    classButton moveUpButton;
    classButton moveDownButton;

    classButton playerUpButton;
    classButton playerDownButton;

    classButton newMapButton;

    classButton mouseButton;

    SDL_Rect tmpRect;

    //classMap mapTown;

private:
};

#endif // GAME_H
