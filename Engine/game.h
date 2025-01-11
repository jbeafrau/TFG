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
    int id=0;
    int x=0,y=0;
    my_enums::gameState map;
    string description="";
    int skill=0;
    int stamina=0;
    int power = 0;
    int luck=0;
    int damage = 0;
    int exp=0;
    my_enums::AItypes NPCAI;
    int tile=0;
    bool discovery = false;
    SDL_Rect boundaries;
};

struct item { //item definition
    string name;
    string description;
    int count;
    int value;
    int tile;
    my_enums::itemTypes type;
    int bonus;
};

struct achievement { //Achievement definition
    string name;
    my_enums::Achievements type;
    int achieved;
};

struct SHOP { // shop definition
    my_enums::gameState map;
    int id;
    int x, y;
    int option;
    string description;
    //LUCKROLL,SKILL,STAMINA,LUCK,GOLD,FOOD,SKILLPOTION,STAMINAPOTION,LUCKPOTION
    int value;
    string description2;
    int value2; 
    int tile;
    my_enums::itemTypes type;
    int bonus;
};

struct CHAT {
    my_enums::gameState map;
    int x;
    int y;
    int option;
    std::string question;
    std::string  answer;

};

struct animation {
    int startx;
    int starty;
    int endx;
    int endy;
    int w;
    int h;
    float incx;
    float incy;
    int seconds;
    int ticks;
    SDL_Texture* texture;
};

struct EVENT {
    int x;
    int y;
    my_enums::gameState map;
    string description;
    //GOLD,FOOD,TELEPORT,IF
    int value;
    int value2;
    int newx;
    int newy;
    my_enums::gameState newMap;
    int tile;
};

struct GLOBAL_EVENT {
    int id;
    my_enums::gameState map;
    SDL_Rect location;
    SDL_Rect newLocation;
    int distance;
    int NPCID;
    string description;
    //CHANGEAI/ADDMONSTER
};


class game
{
public:
    game();
    virtual ~game();

    bool initSDL();
    //Loads media
    bool loadMedia(string base);
    void loadNPCs();
    void loadShops();
    void loadChats();
    void loadEvents();

     void loadPlayerDefault();
    void setButtonDefaults();

    void locationEvents();

    void closeSDL();
    SDL_Surface* loadSurface(std::string path);
    SDL_Texture* loadTexture(std::string path);

    void changeMap();


    void start();
  //  int dice(int maxValue, int minValue);

    int getState();
    my_enums::gameState getStringState(int state);
    void setState(my_enums::gameState newState);


    std::string getBasePath();
    void adjustFPS();

    void addAnimation(int startx, int starty, int endx, int endy, int w, int h, int s,SDL_Texture* texture);
    void playAnimations();
    void clearAnimations();

    bool checkNPC(int x, int y);

    std::string getRaceName(my_enums::playerRaces race);
    std::string getArchetypeName(my_enums::playerArchetype archetype);
      

    void addSkill(std::string skill);
    bool hasSkill(std::string skill);

    //void addNotification(std::string notification);
    void addNotification(std::string notification, SDL_Color color);

    void eventsNotifications();
    

    void addAchievement(std::string achievementName, my_enums::Achievements achievementType);
    bool existAchievement(std::string achievementName, my_enums::Achievements achievementType);
    void drawAchievements();

    void addItem(string name, string description, int count, int value, int tile, my_enums::itemTypes type, int bonus);
    void updateItem(string name, int value);
    bool findItem(string name);
    item getItem(string name);

    bool findNPC(int id);


    void drawSquare(SDL_Rect rect, SDL_Color color);
    void drawTransparentSquare(SDL_Rect rect, SDL_Color color);
    void drawButton(classButton btn);
    void drawButtonSrc(classButton btn, SDL_Texture* texture);
    void drawText(string text, SDL_Rect rect);
    void drawTextL(string text, SDL_Rect rect);
    void drawTextBlock(string text, SDL_Rect rect);
    void drawTextResize(string text, SDL_Rect rect);
 
    void drawMap();
    void drawMiniMap();
    void drawEvents();
    void updateMap();
    void drawPlayer();
    void drawNPCs();

    void drawTileset(SDL_Rect target, SDL_Texture* texture, int player, int columns);

    bool collide(int x, int y);

    void drawBackground();

    bool isAround(int x, int y);
    bool insideBoundaries(int x, int y, SDL_Rect boundaries);
    int getDistance(int x1, int y1, int x2, int y2);

    int getEventTile(int x, int y);
    

    void drawIMG(SDL_Surface* surface, int x, int y, int value);
    void drawIMGBox(int x, int y, int value, int max, SDL_Color color);
    void drawIMGBoxSmall(int x, int y, int w, int h, int value, int max, SDL_Color color);

    void drawMouse();
    void drawScreens();
    void screenClear();
    void screenIntro();
    void screenNotifications();
    void screenMain();
  
    void screenGameOver();
    void screenPlayerName();
    void screenPlayerAttributes();
    void screenPlayerRaces();
    void screenPlayerArchetypes();

    void screenInventory();
    void screenAchievements();

    void screenChat();

    void screenConfigMenu();
    void screenHomeTown();
    void screenFight();
    void screenPlayer();
    void screenHero();
    void screenShops();
    void screenMaster();

    void screenFlip();

    void events();
    void eventsIntro();
    void eventsMain();
    void eventsGameOver();
    void eventsPlayerName();
    void eventsPlayerAttributes();
   
    void eventsConfigMenu();
    void eventsHomeTown();

    void eventsPlayerRaces();
    void eventsPlayerArchetypes();

    void eventsInventory();
    void eventsAchievements();

    void eventsFight();
    void eventsHero();

    void eventsPlayer();

    void eventsShops();

    void eventsChat();

    void eventsMaster();

    void checkBoundaries();

    int dice(int maxValue, int minValue);
    void randomAttributes();
    
    void dropLoot(int x, int y, int value);

    void phaseNPCs();
    void addNPC(int id, int x, int y, my_enums::gameState map, std::string description, int skill, int stamina, int power, int luck, int damage, my_enums::AItypes NPCAI, int tile, SDL_Rect boundaries);
    list<NPC> getNPCs(int x, int y);
    void deleteNPCs(int x, int y);

    list<SHOP> getShops(int x, int y);
    void cleanShop(int x, int y, int option);
    void addShop(my_enums::gameState map, int id, int x, int y, int option, std::string description, int value, std::string  description2, int value2, int tile, my_enums::itemTypes type, int bonus);

    list<CHAT> getChat(int x, int y);
    //void cleanShop(int x, int y, int option);
    void addChat(my_enums::gameState map, int x, int y, int option, std::string question, std::string  answer);
    void removeChat(int x, int y, int option);

    list<EVENT> getEvents(int x, int y);
    void addEvent(int x, int y, my_enums::gameState map, std::string description, int value, int value2, int newx, int newy, my_enums::gameState newMap, int tile);
    void cleanEvents(int x, int y);

    void updateNPCandEVENTS(my_enums::gameState state);
    
    
    void addGlobalEvent(int id, my_enums::gameState map, SDL_Rect location, SDL_Rect newLocation, int distance, int NPCID, std::string description);
    void cleanGlobalEvent(int id);
    
    void addExp(int xp);

    void processAI();

    void timeEvents();

    void playTutorial();
    void playTutorialHomeTown();


    int countMonsters();
    void monsterGenerator();
    int getMonsterID();

    classMap baseMap;
    bool debugMode = false;

    

protected:
    int width = 600;
    int height = 700;
    int currentState = my_enums::S_INTRO_;

    int desiredFPS = 60;

    SDL_Rect mouseRect;

    string basePath;

    //Used to identify random monsters
    int monsterID = 20000;

    //USed to display controls tutorial
    bool firstRun = true;
    bool firstRunHomeTown = true;

    //limit amount of monsters per map
    int maxMonsters = 50;


    SDL_Color lightGreyColor = { 200,200,200,0 };
    SDL_Color greyColor = { 100,100,100,0 };
    SDL_Color brownColor = { 139,69,19,0 };

    int killCount = 0;
    int magicKill = 0;
    int achievementGroup = 0;

    bool growing = true;
    int eyesize = 100;
    int introCounter = 0;
    int introTile = 1;

    int btnx = 0;
    int btny = 0;

    std::string currentChat = "";

    std::string fightPlayer="";
    std::string fightFoe="";
    int lastTurn = 0;

    std::list<achievement> achievements; //Player큦 achievements 
    std::list<std::string> skills; //Player큦 skills
    std::list<std::string> notifications; //Te,porary on screen notifications

    std::list<animation> animations; //Animations
    
    std::list<NPC> NPCs; //All NPCs
    std::list<NPC> tmpNPCs; //NPCs found in our location

    std::list<SHOP> SHOPs; // All Shops
    std::list<SHOP> tmpSHOPs; // Shops Found in our location

    std::list<CHAT> CHATs; // All chat options
    std::list<CHAT> tmpCHATs; // chat options Found in our location

    std::list<EVENT> EVENTs; // All local events
    std::list<EVENT> tmpEVENTs; // Events Found in our location

    std::list<GLOBAL_EVENT> GLOBAL_EVENTs; // Global Events

    std::list<item> items; //Player inventory
    int coins = 50; //Player currency
    int food = 10; //Player food rations 
    int potions_health = 1;
    int potions_power = 1;
    int powerRegeneration = 0;

    //Player attributes
    int skill = 1; //Combat Skill
    int stamina = 1; //Player hitpoins and posion defense
    int power = 1; //magic points and magic regeneration rate
    int luck = 1; //skill improves buy and sell values and posibility to discover how difficult are enemies
    int max_skill = 1;
    int max_stamina = 1;
    int max_power=1;
    int max_luck = 1;

    int skillBonus = 0;
    int staminaBonus = 0;
    int powerBonus = 0;
    int luckBonus = 0;


    //default inventory

   // std::string alternate3 = "";
    std::string  itemArmor = "";
    std::string itemHelmet = "";
    std::string shield = "";
    std::string itemRing = "";
    std::string alternate = "";

    std::string alternate2 = "";
    std::string alternate3 = "";

    // movement attributes
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

    //combat information
    int playerDice = 0;
    int foeDice = 0;
    int turn = 0;

    int ticksAI = 0;

    int currentMusic = 1;
    bool musicON = true;


    std::string playerName = "Jugador"; //Player큦 name
    my_enums::playerRaces currentRace = my_enums::_HUMAN_; //Player큦 race
    my_enums::playerArchetype currentArchetype = my_enums::_FIGHTER_; //Player큦 archetype
    int px=1, py=1; //Player location in the world

    int tmpx = 1;
    int tmpy = 1;

    int playerTile = 63; //Starting tile for player
    int exp = 0; //current experience
    int level = 1; // current player level
    int skillPoints = 0; //Skill points won after leveling up

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
    int prevTime = 0;
    int minutes = 0;
    int popupTime = 0;
    int maxAchievements = 7;
    int volumeSound = 128;
    int volumeMusic = 128;

    my_enums::gameState previousScreen = my_enums::S_NULL_;


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

    SDL_Surface* mouseSurface = NULL;

    SDL_Surface* playersSurface = NULL;
    SDL_Surface* itemsSurface = NULL;
    SDL_Surface* locationsSurface = NULL;

    //The final texture
    SDL_Texture* mouseTexture = NULL;
    SDL_Texture* playersTexture = NULL;
    SDL_Texture* itemsTexture = NULL;
    SDL_Texture* locationsTexture = NULL;


    SDL_Surface* bg1Surface = NULL;
    SDL_Texture* bg1Texture = NULL;

    SDL_Surface* bg2Surface = NULL;
    SDL_Texture* bg2Texture = NULL;

    SDL_Texture* talkTexture = NULL;

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

    SDL_Texture* buttonUpTexture = NULL;
    SDL_Texture* buttonDownTexture = NULL;
    SDL_Texture* buttonLeftTexture = NULL;
    SDL_Texture* buttonRightTexture = NULL;
    SDL_Texture* buttonPlayerTexture = NULL;

    SDL_Texture* buttonSwordTexture = NULL;
    SDL_Texture* buttonBowTexture = NULL;
    SDL_Texture* buttonSpellTexture = NULL;
    SDL_Texture* buttonDrainTexture = NULL;
    SDL_Texture* buttonHideTexture = NULL;
    SDL_Texture* buttonSummonTexture = NULL;

    SDL_Texture* buttonPotionHealthTexture = NULL;
    SDL_Texture* buttonPotionMagicTexture = NULL;
    SDL_Texture* buttonFoodTexture = NULL;

    SDL_Texture* buttonBackTexture = NULL;

    //Globally used font
    TTF_Font* gFont = NULL;
    TTF_Font* smallFont = NULL;

    //drawing collor
    SDL_Color fg = { 0,0,0,0 };

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

    Mix_Chunk* sword = NULL;
    Mix_Chunk* bow = NULL;
    Mix_Chunk* dagger = NULL;
    Mix_Chunk* magic = NULL;
    Mix_Chunk* drain = NULL;
    Mix_Chunk* invoke = NULL;

    Mix_Chunk* buysell = NULL;
    Mix_Chunk* heal = NULL;
    Mix_Chunk* ring = NULL;
    Mix_Chunk* clothes = NULL;
    Mix_Chunk* weapon = NULL;


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

    classButton skillButton;
    classButton staminaButton;
    classButton powerButton;
    classButton luckButton;

  
    classButton blurButton;

    classButton inventoryButton;
    classButton achievementsButton;
    classButton playerButton;

    classButton moveLeftButton;
    classButton moveRightButton;
    classButton moveUpButton;
    classButton moveDownButton;

    classButton playerUpButton;
    classButton playerDownButton;

    classButton rollButton;

    classButton fightButton;
    classButton bowButton;
    classButton spellButton;
    classButton drainButton;
    classButton hideButton;
    classButton summonButton;

    classButton potionHealthButton;
    classButton potionMagicButton;
    classButton foodButton;

    classButton titleButton;


    classButton shop0;
    classButton shop1;
    classButton shop2;
    classButton shop3;
    classButton shop4;
    classButton shop5;

    classButton chat0;
    classButton chat1;
    classButton chat2;
    classButton chat3;
    classButton chat4;
    classButton chat5;

    classButton newMapButton;
    classButton mapButton;

    classButton mouseButton;

    classButton achievementGroup0Button;
    classButton achievementGroup1Button;
    classButton achievementGroup2Button;
    classButton achievementGroup3Button;
    classButton achievementGroup4Button;

    classButton itemSelectedButton;

    SDL_Rect tmpRect;

    //classMap mapTown;

private:
};

#endif // GAME_H
