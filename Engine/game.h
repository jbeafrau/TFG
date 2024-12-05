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
 

*/

struct NPC { //enemy definition
    int id;
    int x,y;
    my_enums::gameState map;
    string description;
    int skill;
    int stamina;
    int luck;
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

struct achievement { //Achievement definition
    string name;
    my_enums::Achievements type;
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
    SDL_Texture* loadTexture(std::string path);




    void start();
  //  int dice(int maxValue, int minValue);

    int getState();
    void setState(my_enums::gameState newState);

    std::string getBasePath();

    std::string getRaceName(my_enums::playerRaces race);
    std::string getArchetypeName(my_enums::playerArchetype archetype);






    void adjustFPS();

    void addNotification(std::string notification);
    void eventsNotifications();

    void addAchievement(std::string achievementName, my_enums::Achievements achievementType);
    bool existAchievement(std::string achievementName, my_enums::Achievements achievementType);

    void addItem(string name, string description, int count, int value, int tile);
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
  //  void paintFruit(int x, int y, Uint8 fruit);
    void drawMap();
    void updateMap();
    void drawPlayer();
    void drawNPCs();
    //void drawPlayerTileset(int x, int y, Uint8 player);
    void drawPlayerTileset(SDL_Rect target, int player);

    void drawBackground();
    

    void drawIMG(SDL_Surface* surface, int x, int y, int value);
    void drawIMGBox(int x, int y, int value, int max, SDL_Color color);


    void drawScreens();
    void screenClear();
    void screenIntro();
    void screenNotifications();
    void screenMain();
   // void screenName();
    void screenGameOver();
    void screenPlayerName();
    void screenPlayerAttributes();
    void screenPlayerRaces();
    void screenPlayerArchetypes();

    void screenInventory();
    void screenAchievements();

    //void screenGeneratePlayer();
    void screenConfigMenu();
    void screenHomeTown();
    void screenFight();

    void screenFlip();

    void events();
    void eventsIntro();
    void eventsMain();
    void eventsGameOver();
    void eventsPlayerName();
    void eventsPlayerAttributes();
    //void eventsGeneratePlayer();
    void eventsConfigMenu();
    void eventsHomeTown();

    void eventsPlayerRaces();
    void eventsPlayerArchetypes();

    void eventsInventory();
    void eventsAchievements();

    void eventsFight();

    void checkBoundaries();

    int dice(int maxValue, int minValue);
    void randomAttributes();
    
    void phaseNPCs();
    list<NPC> getNPCs(int x, int y);
    void deleteNPCs(int x, int y);

    classMap baseMap;

protected:
    int width = 600;
    int height = 700;
    int currentState = my_enums::_INTRO_;

    int desiredFPS = 60;

    SDL_Rect mouseRect;

    string basePath;


    std::list<achievement> achievements;
    std::list<std::string> notifications;
    
    std::list<NPC> NPCs; //All NPCs
    std::list<NPC> tmpNPCs; //NPCs found in our location

    std::list<item> items; //Player inventory
    int coins = 50; //Player currency
    int food = 10; //Player food rations 

    //Player attributes
    int skill = 1; //Combat Skill
    int stamina = 1; //Player hitpoins and posion defense
    int power = 1; //magic points and magic regeneration rate
    int luck = 1; //skill improves buy and sell values and posibility to discover how difficult are enemies
    int max_skill = 1;
    int max_stamina = 1;
    int max_power=1;
    int max_luck = 1;

    //combat information
    int playerDice = 0;
    int foeDice = 0;
    int turn = 0;

    int currentMusic = 1;
    bool musicON = true;


    std::string playerName = "Jugador"; //Player´s name
    my_enums::playerRaces currentRace = my_enums::_HUMAN_; //Player´s race
    my_enums::playerArchetype currentArchetype = my_enums::_FIGHTER_; //Player´s archetype
    int px=1, py=1; //Player location in the world
    int playerTile = 63; //Starting tile for player


    int cam_x = 1, cam_y = 1;//camera starting vertex (top-left)

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

    int mousex=1, mousey=1;

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

   /* SDL_Surface* buttonCloseSurface = NULL;
    SDL_Surface* buttonMusicSurface = NULL;
    SDL_Surface* buttonSocialSurface = NULL;
    SDL_Surface* buttonPauseSurface = NULL;
    SDL_Surface* buttonPlaySurface = NULL;
    SDL_Surface* buttonDangerSurface = NULL;
    */

    SDL_Surface* mouseSurface = NULL;

    SDL_Surface* playersSurface = NULL;

    //The final texture
    SDL_Texture* mouseTexture = NULL;
    SDL_Texture* playersTexture = NULL;

    SDL_Surface* bg1Surface = NULL;
    SDL_Texture* bg1Texture = NULL;

    SDL_Surface* bg2Surface = NULL;
    SDL_Texture* bg2Texture = NULL;


    //SDL_Texture* foodTexture = NULL;
    //SDL_Texture* buttonTexture = NULL;

    SDL_Texture* buttonCloseTexture = NULL;
    SDL_Texture* buttonNextTexture = NULL;
    SDL_Texture* buttonPrevTexture = NULL;
    SDL_Texture* buttonAcceptTexture = NULL;
    SDL_Texture* buttonSoundOffTexture = NULL;
    SDL_Texture* buttonSoundOnTexture = NULL;
    SDL_Texture* buttonConfigTexture = NULL;
    SDL_Texture* buttonRollDiceTexture = NULL;
    SDL_Texture* buttonMapTexture = NULL;
    SDL_Texture* buttonBackpackTexture = NULL;
    SDL_Texture* buttonPlayTexture = NULL;
    SDL_Texture* buttonCancelTexture = NULL;
    SDL_Texture* buttonStarsTexture = NULL;

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
    classButton backButton;

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

    classButton inventoryButton;
    classButton achievementsButton;


    classButton moveLeftButton;
    classButton moveRightButton;
    classButton moveUpButton;
    classButton moveDownButton;

    classButton playerUpButton;
    classButton playerDownButton;

    classButton rollButton;

    classButton newMapButton;

    classButton mouseButton;

    SDL_Rect tmpRect;

    //classMap mapTown;

private:
};

#endif // GAME_H
