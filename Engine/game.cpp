#include "game.h"
#include <exception>
#include <stdexcept>
#include <string>
#include <iostream>
#ifdef _WIN32
#include "windows.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

using namespace std;

std::string GetExeFileName()
{
    #if _WIN32
        char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
    #else
        throw std::runtime_error("Trying to call a Windows function from the wrong platform");
    #endif
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
    basePath = "./Engine/";
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
    int value = 0;
    if(maxValue > 0){
    value =  rand() % maxValue + minValue;
   }
   return value;
}

void game::addNotification(std::string notification, SDL_Color color)
{
    if (notifications.size() == 0) {
        popupTime = SDL_GetTicks();

    }
    notifications.push_back(notification);
}

void game::clearAnimations()
{
    animations.clear();
}

void game::addAchievement(std::string achievementName, my_enums::Achievements achievementType)
{
    if (!existAchievement(achievementName, achievementType )) {
        achievement newAchievement;
        newAchievement.name = achievementName;
        newAchievement.type = achievementType;
        newAchievement.achieved = SDL_GetTicks();

        if (achievementType == my_enums::_MISSIONS_)addExp(100);

        achievements.push_back(newAchievement);
        Mix_PlayChannel(-1, win, 0);

        int achievementCounter = achievements.size();
         achievementsButton.setCaption(std::to_string(static_cast<int>(round(achievementCounter*2.5))) +" %");
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
        int currentTicks = SDL_GetTicks();
        if ((currentTicks - popupTime) > 2000) {
            popupTime = currentTicks;
            notifications.pop_front();
        }
    }
}

void game::cleanEvents(int x, int y)
{
    bool empty = false;
    while (empty != true)
    {
        empty = true;
        for (list<EVENT>::iterator it = EVENTs.begin(); it != EVENTs.end(); it++)
        {
            if ((it->x == x) && (y == it->y)) {
                empty = false;
                EVENTs.erase(it);
                break;

            }
        }
    }
}

void game::addGlobalEvent(int id, my_enums::gameState map, SDL_Rect location, SDL_Rect newLocation, int distance, int NPCID, std::string description)
{
    GLOBAL_EVENT aEVENT;
    aEVENT.id = id;
    aEVENT.map = map;
    aEVENT.location = location;
    aEVENT.newLocation = newLocation;
    aEVENT.description = description;
    aEVENT.NPCID = NPCID;
    aEVENT.distance = distance;
    GLOBAL_EVENTs.push_back(aEVENT);
}

void game::cleanGlobalEvent(int id)
{
    bool empty = false;
    while (empty != true)
    {
        empty = true;
        for (list<GLOBAL_EVENT>::iterator it = GLOBAL_EVENTs.begin(); it != GLOBAL_EVENTs.end(); it++)
        {
            if ((it->id == id)&& (it->map == currentState) ){
                empty = false;
                GLOBAL_EVENTs.erase(it);
                break;

            }
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
        if ((x == it->x) && (y == it->y) && (it->map == currentState)) {
            NPC aNPC;

            aNPC.id = it->id;
            aNPC.x = it->x;
            aNPC.y = it->y;
            aNPC.map = it->map;
            aNPC.description = it->description;
            aNPC.skill = it->skill;
            aNPC.stamina = it->stamina;
            aNPC.power = it->power;
            aNPC.luck = it->luck;
            aNPC.damage = it->damage;
            aNPC.exp = it->exp;
            aNPC.NPCAI = it->NPCAI;
            aNPC.tile = it->tile;
            aNPC.boundaries = it->boundaries;
           
            tmp.push_back(aNPC);
        }
    }

    return tmp;
}

void game::phaseNPCs()
{
    
     tmpNPCs = getNPCs(tmpx,tmpy);
     
     if (tmpNPCs.size() > 0) {
         my_enums::AItypes NPCAI;
         NPCAI = tmpNPCs.begin()->NPCAI;
          if (NPCAI == my_enums::_FRIENDLY_CHAT_) {
             tmpCHATs = getChat(tmpx, tmpy);
             previousScreen = getStringState(currentState);
             setState(my_enums::S_CHAT_);

         }
         else if (NPCAI == my_enums::_FRIENDLY_SHOP_) {
             tmpSHOPs = getShops(tmpx, tmpy);
             previousScreen = getStringState(currentState);
             setState(my_enums::S_SHOP_);

         }
         else if (NPCAI == my_enums::_FRIENDLY_MASTER_) {
             //tmpSHOPs = getShops(tmpx, tmpy);
             previousScreen = getStringState(currentState);
             setState(my_enums::S_MASTER_);

         }         
         else if (NPCAI == my_enums::_ENEMY_STATIC_ || NPCAI == my_enums::_ENEMY_STATIC_MAGE_ || NPCAI == my_enums::_ENEMY_RANDOM_ || NPCAI == my_enums::_ENEMY_FOLLOW_ || NPCAI == my_enums::_ENEMY_FOLLOW_MAGE_) {
             if (getState() != my_enums::S_FIGHT_) {
                 fightPlayer = "";
                 fightFoe = "";
                 px = tmpx;
                 py = tmpy;
                 checkBoundaries();
                 updateMap();
                 if ((tmpNPCs.begin()->id == 4000) || (tmpNPCs.begin()->id == 5000)) {
                     Mix_PlayMusic(musicBOSS, -1);
                 }
                 else {
                     Mix_PlayMusic(musicBATTLE, -1);
                 }
                 
                 previousScreen = getStringState(currentState);
                 setState(my_enums::S_FIGHT_);
             }
         }
        


     }
     else {
         if(!collide(tmpx,tmpy,true)){
         if (tmpx>= 5)px = tmpx;
         if (tmpy >= 5)py = tmpy;
         checkBoundaries();
         updateMap();
         }
     }
    

}


void game::addNPC(int id, int x, int y, my_enums::gameState map , std::string description, int skill, int stamina, int power, int luck, int damage, my_enums::AItypes NPCAI, int tile, SDL_Rect boundaries)
{

    NPC aNPC;
    aNPC.id = id;
    aNPC.x = x;
    aNPC.y = y;
    aNPC.map = map;
    aNPC.description = description;
    aNPC.skill = skill;
    aNPC.stamina = stamina;
    aNPC.power = power;
    aNPC.luck = luck;
    aNPC.damage = damage;
    aNPC.exp = (((aNPC.skill + aNPC.power + aNPC.luck + aNPC.stamina) / 10) + 1)*5;
    aNPC.NPCAI = NPCAI;
    aNPC.tile = tile;
    aNPC.boundaries = boundaries;
    NPCs.push_back(aNPC);

}

void game::addShop(my_enums::gameState map, int id, int x, int y, int option, std::string description, int value, std::string  description2, int value2, int tile, my_enums::itemTypes type, int bonus)
{
    SHOP aShop;
    aShop.map = map;
    aShop.id = id;
    aShop.x = x;
    aShop.y = y;
    aShop.option = option;
    aShop.description = description;
    aShop.value = value;
    aShop.description2 = description2;
    aShop.value2 = value2;
    aShop.tile = tile;
    aShop.type = type;
    aShop.bonus = bonus;
    SHOPs.push_back(aShop);

}

bool game::insideBoundaries(int x, int y, SDL_Rect boundaries)
{
    bool value = false;
    if ((x >= boundaries.x) && (x <= boundaries.w) && (y >= boundaries.y) && (y <= boundaries.h)) {
        value = true;
    }
    return value;

}

bool game::isAround(int x, int y)
{
    bool value = false;
    int tmpx = px - x;
    int tmpy = py - y;
    if ((tmpx >= -1) && (tmpx <= 1) && (tmpy >= -1) && (tmpy <= 1)) {
        value = true;
    }
    return value;
}

int game::getDistance(int x1, int y1, int x2, int y2)
{
    int value = 0;
    int tmpx = abs(x1 - x2);
    int tmpy = abs(y1 - y2);
    value = sqrt(tmpx * tmpx + tmpy * tmpy);
    return value;
}




list<EVENT> game::getEvents(int x, int y)
{
    list<EVENT> tmp;
    for (list<EVENT>::iterator it = EVENTs.begin(); it != EVENTs.end(); it++)
    {
        if ((x == it->x) && (y == it->y) && ( it->map == currentState)) {

            EVENT aEVENT;
            aEVENT.x = it->x;
            aEVENT.y = it->y;
            aEVENT.map = it->map;
            aEVENT.description = it->description;
            aEVENT.value = it->value;
            aEVENT.value2 = it->value2;
            aEVENT.newx = it->newx;
            aEVENT.newy = it->newy;
            aEVENT.newMap = it->newMap;
            aEVENT.tile = it->tile;
            tmp.push_back(aEVENT);
        }
    }
    return tmp;
}

void game::addEvent(int x, int y, my_enums::gameState map, std::string description, int value, int value2, int newx, int newy, my_enums::gameState newMap, int tile)
{
    EVENT aEVENT;
    aEVENT.x = x;
    aEVENT.y = y;
    aEVENT.map = map;
    aEVENT.description = description;
    aEVENT.value = value;
    aEVENT.value2 = value2;
    aEVENT.newx = newx;
    aEVENT.newy = newy;
    aEVENT.newMap = newMap;
    aEVENT.tile = tile;
    EVENTs.push_back(aEVENT);
}


void game::addChat(my_enums::gameState map, int x, int y, int option, std::string question, std::string  answer)
{
    CHAT achat;
    achat.map = map;
    achat.x = x;
    achat.y = y;
    achat.option = option;
    achat.question = question;
    achat.answer = answer;
    
    CHATs.push_back(achat);

}


list<SHOP> game::getShops(int x, int y)
{
    list<SHOP> tmp;
    for (list<SHOP>::iterator it = SHOPs.begin(); it != SHOPs.end(); it++)
    {
        if ((x == it->x) && (y == it->y)) {
            SHOP aShop;
            aShop.id = it->id;
            aShop.x = it->x;
            aShop.y = it->y;
            aShop.option = it->option;
            aShop.description = it->description;
            aShop.value = it->value;
            aShop.description2 = it->description2;
            aShop.value2 = it->value2;
            aShop.tile = it->tile;
            aShop.type = it->type;
            aShop.bonus = it->bonus;

            tmp.push_back(aShop);
        }
    }
    return tmp;
}


list<CHAT> game::getChat(int x, int y)
{
    list<CHAT> tmp;
    for (list<CHAT>::iterator it = CHATs.begin(); it != CHATs.end(); it++)
    {
        if ((x == it->x) && (y == it->y)) {
            CHAT aCHAT;
            //aShop.id = it->id;
            aCHAT.x = it->x;
            aCHAT.y = it->y;
            aCHAT.option = it->option;
            aCHAT.question = it->question;
            aCHAT.answer = it->answer;
          

            tmp.push_back(aCHAT);
        }
    }
    return tmp;
}

void game::eventsShops()
{
    //Event handler
    SDL_Event e;

    //Info to cleanup shop options
    int shop_x = 0;
    int shop_y = 0;
    int shop_option = 0;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
          //  timerGameOver.reset();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                    //addNotification("Saliendo del juego");
                    addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
                }
            }

            if (continueButton.clicked(mousex, mousey)) {
                setState(previousScreen);
                tmpx = px;
                tmpy = py;
                left = false;
                right = false;
                up = false;
                down = false;

            }

            string tmpStr = "";
            int itC = -1;
            int tmpShop = -1;
            int option = -1;
          

            for (list<SHOP>::iterator it = tmpSHOPs.begin(); it != tmpSHOPs.end(); it++) {
                    itC++;
                    switch (itC) {
                    case 0:
                        if (shop0.clicked(mousex, mousey)) {
                            tmpShop = itC;
                            Mix_PlayChannel(-1, buysell, 0);

                        }
                        break;
                    case 1:
                        if (shop1.clicked(mousex, mousey)) {
                            Mix_PlayChannel(-1, buysell, 0);
                            tmpShop = itC;
                        }
                        break;
                    case 2:
                        if (shop2.clicked(mousex, mousey)) {
                            Mix_PlayChannel(-1, buysell, 0);
                            tmpShop = itC;
                        }
                        break;
                    case 3:
                        if (shop3.clicked(mousex, mousey)) {
                            Mix_PlayChannel(-1, buysell, 0);
                            tmpShop = itC;
                        }
                        break;
                    case 4:
                        if (shop4.clicked(mousex, mousey)) {

                            Mix_PlayChannel(-1, buysell, 0);
                            tmpShop = itC;
                        }
                        break;
                    case 5:
                        if (shop5.clicked(mousex, mousey)) {

                            Mix_PlayChannel(-1, buysell, 0);
                            tmpShop = itC;
                        }
                        break;
                    }

                }//for tmpshops

                if (tmpShop > -1) {
                    for (list<SHOP>::iterator it = tmpSHOPs.begin(); it != tmpSHOPs.end(); it++) {
                        option++;
                        if (option == tmpShop) {//clicked this item...
                            if (it->value < 0) {
                                //venta
                                if (findItem(it->description) == true) {
                                    //tenemos el objeto
                                    updateItem(it->description, it->value);
                                    coins += it->value2;
                                    tmpStr = "Vendes ";
                                    if (it->description == "FOOD") {
                                        tmpStr = tmpStr + " raciones de comida";
                                    }
                                    else {
                                        tmpStr = tmpStr + it->description;
                                    }
                                    addNotification(tmpStr, { 0,0,0 });
                                }
                                else {
                                    tmpStr = "No tienes eso..";
                                    addNotification(tmpStr, { 0,0,0 });
                                }//fin venta
                            }
                            else {
                                //compra
                                   //if (it->)

                                if (it->description2 == "GOLD" and coins >= it->value2) {
                                    coins -= it->value2;
                                    tmpStr = "Compras ";
                                    if (it->description == "FOOD") {
                                        tmpStr = tmpStr + " raciones de comida";
                                        addNotification(tmpStr, { 0,0,0 });
                                        food += it->value;
                                    }
                                    else  if (it->description == "POCION VITALIDAD") {
                                        tmpStr = tmpStr + it->description;
                                        addNotification(tmpStr, { 0,0,0 });
                                        potions_health += it->value;
                                    }else  if (it->description == "POCION PODER") {
                                        tmpStr = tmpStr + it->description;
                                        potions_power += it->value;
                                    }else                               
                                    {
                                        tmpStr = tmpStr + it->description;
                                        addNotification(tmpStr, { 0,0,0 });


                                        addItem(it->description, it->description, it->value, it->value2, it->tile, it->type, it->bonus);

                                        shop_x = tmpx;
                                        shop_y = tmpy;
                                        shop_option = it->option;
                                    }

                                }
                                else {
                                    addNotification("No tienes dinero suficiente", { 0,0,0 });
                                }//gold
                                if (it->description2 == "FOOD" and food >= it->value2) {
                                    tmpStr = "Compras ";
                                    if (it->description == "FOOD") {
                                        tmpStr = tmpStr + " raciones de comida";
                                        addNotification(tmpStr, { 0,0,0 });
                                        food += it->value;
                                    }
                                    else {
                                        addItem(it->description, it->description, it->value, it->value2, it->tile, it->type, it->bonus);
                                        tmpStr = tmpStr + it->description;
                                    }
                                    addNotification(tmpStr, { 0,0,0 });
                                    food -= it->value2;
                                    shop_x = tmpx;
                                    shop_y = tmpy;
                                    shop_option = it->option;

                                }//food
                            }//compra / venta
                        }  //found option
                    }//for shops

                    if (shop_x !=0){
                         cleanShop(shop_x, shop_y, shop_option); 
                    }

                }//which button?
               


        }//mouse events

        //******************
    }//SDL events

}//events shops



void game::eventsChat()
{
    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
            
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
            }

            if (continueButton.clicked(mousex, mousey)) {
                setState(previousScreen);
                tmpx = px;
                tmpy = py;
                left = false;
                right = false;
                up = false;
                down = false;
                currentChat = " " ;
            }

            string tmpStr = "";
            int itC = -1;
            int tmpChat = -1;
            int option = -1;


            for (list<CHAT>::iterator it = tmpCHATs.begin(); it != tmpCHATs.end(); it++) {
                itC++;
                switch (itC) {
                case 0:
                    if (chat0.clicked(mousex, mousey)) {
                        tmpChat = itC;
                        Mix_PlayChannel(-1, audioButton, 0);

                    }
                    break;
                case 1:
                    if (chat1.clicked(mousex, mousey)) {
                        Mix_PlayChannel(-1, audioButton, 0);
                        tmpChat = itC;
                    }
                    break;
                case 2:
                    if (chat2.clicked(mousex, mousey)) {
                        Mix_PlayChannel(-1, audioButton, 0);
                        tmpChat = itC;
                    }
                    break;
                case 3:
                    if (chat3.clicked(mousex, mousey)) {
                        Mix_PlayChannel(-1, audioButton, 0);
                        tmpChat = itC;
                    }
                    break;
                case 4:
                    if (chat4.clicked(mousex, mousey)) {

                        Mix_PlayChannel(-1, audioButton, 0);
                        tmpChat = itC;
                    }
                    break;
                case 5:
                    if (chat5.clicked(mousex, mousey)) {

                        Mix_PlayChannel(-1, audioButton, 0);
                        tmpChat = itC;
                    }
                    break;
                }

            }//for tmpshops

            if (tmpChat > -1) {
                for (list<CHAT>::iterator it = tmpCHATs.begin(); it != tmpCHATs.end(); it++) {
                    option++;
                    if (option == tmpChat) {//clicked this item...
                        currentChat = it->answer;
                        
                      
                    }  //found option
                }//for shops

            }//which button?



        }//mouse events

        //******************
    }//SDL events

}//events chat

void game::removeChat(int x, int y, int option)
{
    bool empty = false;
    while (empty != true)
    {
        empty = true;
        for (list<CHAT>::iterator it = CHATs.begin(); it != CHATs.end(); it++)
        {
            if (it->x == x && it->y == y && it->option == option) {
                empty = false;
                CHATs.erase(it);
                break;

            }
        }
    }
}

void game::removeItem(string name)
{
    bool empty = false;
    while (empty != true)
    {
        empty = true;
        for (list<item>::iterator it = items.begin(); it != items.end(); it++)
        {
            if (it->name == name ) {
                empty = false;
                items.erase(it);
                break;

            }
        }
    }
}


void game::cleanShop(int x, int y, int option)
{
    bool empty = false;
    while (empty != true)
    {
        empty = true;
        for (list<SHOP>::iterator it = SHOPs.begin(); it != SHOPs.end(); it++)
        {
            if (it->x == x && it->y == y && it->option == option) {
                empty = false;
                SHOPs.erase(it);
                break;

            }
        }
    }


    empty = false;
    while (empty != true)
    {
        empty = true;
        for (list<SHOP>::iterator it = tmpSHOPs.begin(); it != tmpSHOPs.end(); it++)
        {
            if (it->x == x && it->y == y && it->option == option) {
                empty = false;
                tmpSHOPs.erase(it);
                break;

            }
        }
    }
}

void game::loadPlayerDefault()
{
    px = 109;
    py = 146;
    cam_x = 100;
    cam_y = 140;

    randomAttributes();


    alternate3 = "";
     alternate2 = "";
    itemHelmet = "";
    shield = "";
    itemRing = "";
    alternate = "";

    coins = 50; //Player currency
    food = 10; //Player food rations 
    potions_health = 1;
    potions_power = 1;
    powerRegeneration = 0;

    skillBonus = 0;
    staminaBonus = 0;
    powerBonus = 0;
    luckBonus = 0;


    int playerDice = 0;
    int foeDice = 0;
    int turn = 0;

    int ticksAI = 0;

    int currentMusic = 1;
    bool musicON = true;


   playerName = "Jugador"; //Player´s name
   currentRace = my_enums::_HUMAN_; //Player´s race
   currentArchetype = my_enums::_FIGHTER_; //Player´s archetype
  
    tmpx = 1;
    tmpy = 1;

    playerTile = 63; //Starting tile for player
    exp = 0; //current experience
    level = 1; // current player level
    skillPoints = 0; //Skill points won after leveling up

    //Number of tiles drawn on screen
    cam_size_x = 16;
   cam_size_y = 8;

}


bool game::collide(int x, int y, bool player)
{
    bool tmp = false;

    int c = baseMap.get_cell(x, y);
    if ((c== 9)|| (c == 579) || (c == 6) || (c == 21) || (c == 129) || (c == 154) || (c == 341) || (c == 343) || (c == 344) || (c == 345) || (c == 346) || (c == 157))tmp = true;


    if(player){
    std::list<EVENT>::iterator it = EVENTs.begin();
    while (it != EVENTs.end())
    {
        bool erased = false;
        if (it->map == currentState) {//only process events in current map
            if ((it->description.substr(0, 4) == "IF**") && (it->x == x) && (it->y == y)) {


               // if (it->map == currentState) {//on the current map
                    if (findItem(it->description.substr(4, it->description.length() - 4))) {
                        addNotification("Se abre el camino", { 0,0,0 });
                        //GLOBAL_EVENTs.erase(it);
                        it = EVENTs.erase(it);
                        erased = true;
                        tmp = false;
                    }
                    else {
                        tmp = true;

                        addNotification("No puedes pasar, te falta "+ it->description.substr(4, it->description.length() - 4), { 0,0,0 });
                    }
               // }
            }         

        }//currentstate
        if ((it != EVENTs.end()) && (erased == false))it++;
    }
    }

    return tmp;
}


void game::changeMusic()
{

    switch (currentState)
    {

    case my_enums::S_FOREST_WORLD_:
    {
        Mix_PlayMusic(musicFOREST, -1);
        break;
    }

    case my_enums::S_COAST_WORLD_:
    {
        Mix_PlayMusic(musicTOWN, -1);
        break;
    }

    case my_enums::S_ELEMENTAL_FIRE_WORLD_:
    {
        Mix_PlayMusic(musicMYSTICAL, -1);
        break;
    }
    case my_enums::S_ELEMENTAL_WATER_WORLD_:
    {
        Mix_PlayMusic(musicMYSTICAL, -1);
        break;
    }
    case my_enums::S_ELEMENTAL_EARTH_WORLD_:
    {
        Mix_PlayMusic(musicMYSTICAL, -1);
        break;
    }
    case my_enums::S_ELEMENTAL_WIND_WORLD_:
    {
        Mix_PlayMusic(musicMYSTICAL, -1);
        break;
    }
    case my_enums::S_NECRO_WORLD_:
    {
        Mix_PlayMusic(musicDARK, -1);
        addAchievement("Necromundo", my_enums::_HIDDEN_);
        break;
    }
    case my_enums::S_HOMETOWN_:
    {
        Mix_PlayMusic(musicTOWN, -1);
        break;
    }

    case my_enums::S_FIGHT_:
    {
        Mix_PlayMusic(musicBATTLE, -1);
        break;
    }
    
    case my_enums::S_HERO_:
    {
        Mix_PlayMusic(musicHERO, -1);
        break;
    }

    default:
    {
        // is likely to be an error
        break;
    }
    };

}

void game::loadNPCs()
{
    NPCs.clear();

    //Add some monster on initial map

    //east
    addNPC(10, 136,155, my_enums::S_HOMETOWN_, "Esqueleto", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, 219, { 130,150,150,170 });
    addNPC(10, 136,161, my_enums::S_HOMETOWN_, "Esqueleto", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, 219, { 130,150,150,170 });
    addNPC(10, 142,155, my_enums::S_HOMETOWN_, "Esqueleto", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, 219, { 130,150,150,170 });
    addNPC(10, 142,161, my_enums::S_HOMETOWN_, "Esqueleto", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, 219, { 130,150,150,170 });
    addNPC(3000, 139, 167, my_enums::S_HOMETOWN_, "Nigromante", dice(5, 10), dice(5, 15), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_FOLLOW_MAGE_, 160, { 130,150,150,170 });

    //Add a dog
    addNPC(10001, 118, 122, my_enums::S_HOMETOWN_, "Tor el perro", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_FRIENDLY_STATIC_, 269, { 1,1,255,255 });

    //Add 30 peasants in the town
    for (int x = 0; x < 30;x++)addNPC(30+x, dice(30, 90), dice(50, 120), my_enums::S_HOMETOWN_, "Campesino", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_FRIENDLY_RANDOM_, 74, { 90,130,120,170 });
    
    //Add main friendly NPCs shops
    addNPC(19, 109, 142, my_enums::S_HOMETOWN_, "Sabio del pueblo", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(20, 119, 142, my_enums::S_HOMETOWN_, "Mago del templo elemental", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 83, { 1,1,255,255 });
    addNPC(21, 107, 143, my_enums::S_HOMETOWN_, "Tutorial equipo", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(21, 111, 143, my_enums::S_HOMETOWN_, "Tutorial magia", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(22, 107, 145, my_enums::S_HOMETOWN_, "Tutorial combate", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(23, 111, 145, my_enums::S_HOMETOWN_, "Tutorial tiendas", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(24, 118, 155, my_enums::S_HOMETOWN_, "Maestro", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_MASTER_, 77, { 1,1,255,255 });

    //Add main friendly shops
    addNPC(5, 106, 155, my_enums::S_HOMETOWN_, "Tienda de comida", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(6, 112, 155, my_enums::S_HOMETOWN_, "Tienda de armas", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(7, 106, 161, my_enums::S_HOMETOWN_, "Tienda de armaduras", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(8, 112, 161, my_enums::S_HOMETOWN_, "Tienda de pociones", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 79, { 1,1,255,255 });

   
    addNPC(1000, 1, 1, my_enums::S_COAST_WORLD_, "Tienda de la costa", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(2000, 1, 1, my_enums::S_COAST_WORLD_, "Sabio de la costa", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(3000, 1, 1, my_enums::S_COAST_WORLD_, "Nigromante", dice(5, 10), dice(5, 20), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_FOLLOW_MAGE_, 160, { 1,1,250,250 });

    addNPC(1000, 1, 1, my_enums::S_ELEMENTAL_FIRE_WORLD_, "Tienda del elemento fuego", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(2000, 1, 1, my_enums::S_ELEMENTAL_FIRE_WORLD_, "Sabio del elemento fuego", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(3000, 1, 1, my_enums::S_ELEMENTAL_FIRE_WORLD_, "Nigromante", dice(5, 10), dice(5, 20), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_FOLLOW_MAGE_, 160, { 1,1,250,250 });

    addNPC(1000, 1, 1, my_enums::S_ELEMENTAL_WATER_WORLD_, "Tienda del elemento agua", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(2000, 1, 1, my_enums::S_ELEMENTAL_WATER_WORLD_, "Sabio del elemento agua", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(3000, 1, 1, my_enums::S_ELEMENTAL_WATER_WORLD_, "Nigromante", dice(5, 10), dice(5, 20), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_FOLLOW_MAGE_, 160, { 1,1,250,250 });

    addNPC(1000, 1, 1, my_enums::S_ELEMENTAL_EARTH_WORLD_, "Tienda del elemento tierra", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(2000, 1, 1, my_enums::S_ELEMENTAL_EARTH_WORLD_, "Sabio del elemento tierra", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(3000, 1, 1, my_enums::S_ELEMENTAL_EARTH_WORLD_, "Nigromante", dice(5, 10), dice(5, 20), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_FOLLOW_MAGE_, 160, { 1,1,250,250 });

    addNPC(1000, 1, 1, my_enums::S_ELEMENTAL_WIND_WORLD_, "Tienda del elemento aire", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(2000, 1, 1, my_enums::S_ELEMENTAL_WIND_WORLD_, "Sabio del elemento aire", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(3000, 1, 1, my_enums::S_ELEMENTAL_WIND_WORLD_, "Nigromante", dice(5, 10), dice(5, 20), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_FOLLOW_MAGE_, 160, { 1,1,250,250 });


    addNPC(1000, 1, 1, my_enums::S_FOREST_WORLD_, "Tienda del bosque oscuro", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(2000, 230, 50, my_enums::S_FOREST_WORLD_, "Figura oscura", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 198, { 1,1,255,255 });
    addNPC(3000, 1, 1, my_enums::S_FOREST_WORLD_, "Nigromante", dice(5, 10), dice(5, 20), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_STATIC_MAGE_, 160, { 1,1,250,250 });

    addNPC(4000, 129, 158, my_enums::S_NECRO_WORLD_, "GOLEM", 15, 50, 15, 15, 4, my_enums::_FRIENDLY_STATIC_, 55, { 1,1,250,250 });
    addNPC(5000, 123, 158, my_enums::S_NECRO_WORLD_, "SEÑOR OSCURO", 16, 60, 15, 15, 4, my_enums::_ENEMY_STATIC_MAGE_, 32, { 1,1,250,250 });
   
}


void game::loadEvents()
{
 
    addGlobalEvent(1, my_enums::S_HOMETOWN_, { 117,121,119,123 }, { 0,0,0,0 }, 0, 10001, "CHANGE_AI_FRIENDLY_FOLLOW");
    addGlobalEvent(2, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 10001, "CHANGE_AI_FRIENDLY_STATIC");
    addGlobalEvent(3, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 10001, "REMOVE_CHAT");
    addGlobalEvent(4, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 10001, "ADD_CHAT");
    addGlobalEvent(5, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 10001, "ADD_ITEM");

    addGlobalEvent(6, my_enums::S_HOMETOWN_, { 0,0,0,0 }, { 0,0,0,0 }, 0, 3000, "NPC_DEFEATED");

    addGlobalEvent(7, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 100, "HAVE_ITEM GAFAS DE SOL");
    addGlobalEvent(8, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 200, "HAVE_ITEM ANILLO VUELO");
    addGlobalEvent(9, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 300, "HAVE_ITEM ANILLO RESISTENCIA FUEGO");
    addGlobalEvent(10, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 400, "HAVE_ITEM ANILLO RESPIRAR AGUA");
    addGlobalEvent(11, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 500, "HAVE_ITEM ANILLO DE LA TIERRA");
    addGlobalEvent(12, my_enums::S_HOMETOWN_, { 111, 160,113, 162 }, { 0,0,0,0 }, 0, 500, "HAVE_ITEM LIBRO RECETAS");
    addGlobalEvent(13, my_enums::S_HOMETOWN_, { 118,141,120,144 }, { 0,0,0,0 }, 0, 500, "HAVE_ITEM POCION NEGRA");

    addGlobalEvent(13, my_enums::S_NECRO_WORLD_, { 128, 157,130, 159 }, { 0,0,0,0 }, 0, 4000, "HAVE_ITEM POCION ROJA");

    
   
    addGlobalEvent(20, my_enums::S_COAST_WORLD_, { 0,0,0,0 }, { 0,0,0,0 }, 0, 3000, "NPC_DEFEATED");
    addGlobalEvent(21, my_enums::S_FOREST_WORLD_, { 0,0,0,0 }, { 0,0,0,0 }, 0, 3000, "NPC_DEFEATED");
    addGlobalEvent(22, my_enums::S_ELEMENTAL_WIND_WORLD_, { 0,0,0,0 }, { 0,0,0,0 }, 0, 3000, "NPC_DEFEATED");
    addGlobalEvent(23, my_enums::S_ELEMENTAL_FIRE_WORLD_, { 0,0,0,0 }, { 0,0,0,0 }, 0, 3000, "NPC_DEFEATED");
    addGlobalEvent(24, my_enums::S_ELEMENTAL_WATER_WORLD_, { 0,0,0,0 }, { 0,0,0,0 }, 0, 3000, "NPC_DEFEATED");
    addGlobalEvent(25, my_enums::S_ELEMENTAL_EARTH_WORLD_, { 0,0,0,0 }, { 0,0,0,0 }, 0, 3000, "NPC_DEFEATED");
    addGlobalEvent(26, my_enums::S_NECRO_WORLD_, { 0,0,0,0 }, { 0,0,0,0 }, 0, 5000, "NPC_DEFEATED");
  
   
    addEvent(229, 51, my_enums::S_FOREST_WORLD_, "IF**ANILLO VUELO", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);
    addEvent(228, 51, my_enums::S_FOREST_WORLD_, "IF**ANILLO RESISTENCIA FUEGO", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);
    addEvent(227, 51, my_enums::S_FOREST_WORLD_, "IF**ANILLO RESPIRAR AGUA", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);
    addEvent(226, 51, my_enums::S_FOREST_WORLD_, "IF**MEDALLON OSCURO", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);

    addEvent(1, 1, my_enums::S_FOREST_WORLD_, "IF**ANILLO VUELO", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);

    addEvent(117, 142, my_enums::S_HOMETOWN_, "IF**ANILLO VUELO", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);
    addEvent(121, 142, my_enums::S_HOMETOWN_, "IF**ANILLO RESISTENCIA FUEGO", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);
    addEvent(117, 144, my_enums::S_HOMETOWN_, "IF**ANILLO RESPIRAR AGUA", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);
    addEvent(121, 144, my_enums::S_HOMETOWN_, "IF**ANILLO DE LA TIERRA", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);
    addEvent(139, 163, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 139, 166, my_enums::S_HOMETOWN_, 0);
    addEvent(139, 165, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 139, 162, my_enums::S_HOMETOWN_, 0);

    addEvent(251, 151, my_enums::S_HOMETOWN_, "IF**LLAVE PUERTA ESTE", 50, 0, 0, 0, my_enums::S_HOMETOWN_, 0);



    addEvent(225, 52, my_enums::S_FOREST_WORLD_, "TELEPORT", 0, 0, 50, 50, my_enums::S_NECRO_WORLD_, 0);

    addEvent(5, 50, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 249, 50, my_enums::S_FOREST_WORLD_, 0);
    addEvent(5, 51, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 249, 51, my_enums::S_FOREST_WORLD_, 0);
    addEvent(5, 52, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 249, 52, my_enums::S_FOREST_WORLD_, 0);

    addEvent(254, 50, my_enums::S_FOREST_WORLD_, "TELEPORT", 0, 0,6, 50, my_enums::S_HOMETOWN_ , 0);
    addEvent(254, 51, my_enums::S_FOREST_WORLD_, "TELEPORT", 0, 0,6, 51, my_enums::S_HOMETOWN_ , 0);
    addEvent(254, 52, my_enums::S_FOREST_WORLD_, "TELEPORT", 0, 0,6, 52, my_enums::S_HOMETOWN_ , 0);

    addEvent(253, 151, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 120, 120, my_enums::S_COAST_WORLD_, 0);
    addEvent(117, 142, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 120, 120, my_enums::S_ELEMENTAL_WIND_WORLD_, 0);
    addEvent(121, 142, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 120, 120, my_enums::S_ELEMENTAL_FIRE_WORLD_, 0);
    addEvent(117, 144, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 120, 120, my_enums::S_ELEMENTAL_WATER_WORLD_, 0);
    addEvent(121, 144, my_enums::S_HOMETOWN_, "TELEPORT", 0, 0, 120, 120, my_enums::S_ELEMENTAL_EARTH_WORLD_, 0);


    addEvent(1, 1, my_enums::S_COAST_WORLD_, "TELEPORT", 0, 0, 252, 151, my_enums::S_HOMETOWN_, 0);
    addEvent(1, 1, my_enums::S_ELEMENTAL_WIND_WORLD_, "TELEPORT", 0, 0, 118, 142, my_enums::S_HOMETOWN_, 0);
    addEvent(1, 1, my_enums::S_ELEMENTAL_FIRE_WORLD_, "TELEPORT", 0, 0, 120, 142, my_enums::S_HOMETOWN_, 0);
    addEvent(1, 1, my_enums::S_ELEMENTAL_WATER_WORLD_, "TELEPORT", 0, 0, 118, 144, my_enums::S_HOMETOWN_, 0);
    addEvent(1, 1, my_enums::S_ELEMENTAL_EARTH_WORLD_, "TELEPORT", 0, 0, 120, 144, my_enums::S_HOMETOWN_, 0);

}


void game::dropLoot(int x, int y, int value)
{
    addEvent(x, y, previousScreen, "GOLD", value, 0, 0, 0, my_enums::S_HOMETOWN_, 344);
}


void game::loadChats()
{
    CHATs.clear();

    //wise man
    addChat(my_enums::S_HOMETOWN_,109,142, 1, "¿Que está pasando en el pueblo?", "Un mal ha venido de otro plano y esta generando caos en el pueblo, no sabemos quien exactamente ha sido, pero los muertos se levantan, necesitas abrir los portales a los otros planos para poder descubrir donde se esconde el mal...");
    addChat(my_enums::S_HOMETOWN_,109, 142, 2, "¿Que puedo hacer?", "Equipate y preparate para luchar contra el mal, compra equipo en las tiendas y pregunta en el pueblo primero");
    addChat(my_enums::S_HOMETOWN_,109, 142, 3, "Cuentame algo del pueblo", "Al sur encontraras varias tiendas, al este la costa y siguiendo el camino del noroeste, el bosque oscuro");
    addChat(my_enums::S_HOMETOWN_,109, 142, 4, "<<MISSION>>¿Por donde comienzo?", "En el cementerio del pueblo hay enemigos que no permiten salir a los ciudadanos...");
    addChat(my_enums::S_HOMETOWN_,109, 142, 5, "¿Puedo ayudar a alguien mas?", "El mago del templo elemental al este necesita ayuda con su mascota ");
    addChat(my_enums::S_HOMETOWN_,109, 142, 6, "¿Quien es el maestro?", "Al este del pueblo esta el maestro, cuando mejores de nivel puedes mejorar tus atributos allí");
    
    //elements
    addChat(my_enums::S_HOMETOWN_,119, 142, 1, "¿Que es esto?", "Esta es la sala de los elementos, pero aun no estas preparado para luchar al mal");
    addChat(my_enums::S_HOMETOWN_,119, 142, 2, "<<MISSION>>¿Puedo ayudar?", "Tor, mi perro esta en una choza al norte, con tantos monstruos no me atrevo a ir a por el, ¿Podrias traermelo por favor??, si lo haces te daré la llave que abre la puerta del este");
    
    //tutos
    addChat(my_enums::S_HOMETOWN_,107, 143, 1, "¿Como accedo a mi equipo?", "Pulsa el icono de la mochila en la pantalla de juego, está en la parte de arriba de la pantalla");
    addChat(my_enums::S_HOMETOWN_,107, 143, 2, "¿Que quiere decir EQUIPADO?", "Hay piezas de equipo que puedes usar para mejorar tus habilidades como armas, armaduras u anillos mágicos, para equiparla, tan solo tienes que pinchar sobre el equipo, no puedes usar escudos cuando usas armas de dos manos o de ataques a distancia");
    addChat(my_enums::S_HOMETOWN_,107, 143, 3, "¿Como puedo obtener equipo?", "De tres maneras diferentes: comprandolo en las tiendas, derrotando enemigos y a veces también lo encontrarás en el mapa");   
    
    addChat(my_enums::S_HOMETOWN_,111, 143, 1, "¿Como se gasta la magia?", "La magia engloba varias habilidades que suponen ventajas en el combate, tanto los magos, necromantes e invocadores pueen gastar esas habilidades");
    addChat(my_enums::S_HOMETOWN_,111, 143, 2, "¿Como saber lo poderoso que soy?", "La magia depende de un atributo: Poder, cuanto más tengas más fuerte seran tus ataques mágicos y más ataques podras realizar antes de agotar tu reserva");
    addChat(my_enums::S_HOMETOWN_,111, 143, 3, "¿Tiene límite la magia?", "Lamentablemente la magia consume tu poder a grán velocidad y cuando se agota, no podrás usarla");
    addChat(my_enums::S_HOMETOWN_,111, 143, 4, "¿Se puede recuperar el poder?", "Cuando no estás en combate la mágia se regenera a un ritmo de un punto cada 30 segundos (- el poder máximo), durante el combate si usas una poción de poder recuperas instantáneamente 10 puntos");
    
    addChat(my_enums::S_HOMETOWN_,107, 145, 1, "¿Como funciona el combate?", "En el mapa verás enemigos, si intentas entrar en su ubicación u ellos te alcanzan entrarás en combate, una vez entres en combate en la parte izquierda veras los enemigos con sus habilidades, el combate es por turnos y solo termina cuando terminas con tus enemigos u ellos contigo");
    addChat(my_enums::S_HOMETOWN_,107, 145, 2, "¿Como ataco?", "Todos los arquetipos pueden realizar ataques de melee, para hacerlo tan solo hay que pulsar el boton de la espada, además otros personajes tienen otro tipos de ataques, como los ataques a distancia, con subterfugio o magia ");
    addChat(my_enums::S_HOMETOWN_,107, 145, 3, "¿Cuando derroto a los enemigos?", "Cuando la barra roja de los enemigos se consume los derrotas, lo mismo sirve para tu personaje");
    addChat(my_enums::S_HOMETOWN_,107, 145, 4, "¡Ayuda!, ¡Me han herido!", "Hay dos maneras de recuperar vitalidad, fuera de combate consumiendo raciones de comida (el icono del pán que recupera 4 puntos de vida por ración) y dentro del combate las pociones de vitalidad que recuperan 10 puntos de vida");
    
    addChat(my_enums::S_HOMETOWN_,111, 145, 1, "¿Como funcionan las tiendas?", "En el mapa verás personajes amigos, si intentas desplazarte en su dirección la pantalla de la tienda se abre");
    addChat(my_enums::S_HOMETOWN_,111, 145, 2, "¿Como compro el quipo?", "Es tan sencillo como pulsar sobre el objeto deseado, si tienes suficiente dinero perderas dicho dinero pero se añade a tu equipo el objeto");
    addChat(my_enums::S_HOMETOWN_,111, 145, 3, "¿Solo se puede pagar con dinero?", "A veces podrás cambiar un objeto por otro");
    addChat(my_enums::S_HOMETOWN_,111, 145, 4, "¿Se puede vender equipo?", "A veces los comerciantes te compraran equipo especial, como pueden ser gemas");

    //wise coast
    addChat(my_enums::S_COAST_WORLD_, 1,1, 1, "Logros de combate", "Los logros de combate requieren derrotar enemigos usando las habilidades específicas de cada arquetipo...");
    addChat(my_enums::S_COAST_WORLD_, 1, 1, 2, "Logros de misiones", "Para desbloquear la história tan solo tienes que cumpplir las peticiones que te dan los personajes no jugadores");

    //wise elemental water
    addChat(my_enums::S_ELEMENTAL_WATER_WORLD_, 1, 1, 1, "Logros de Opciones", "¿Has probado todas las opciones del juego?, ¿Todas las opciones?, ¿Todas las configuraciones?, ¿Usado todos los tipos de consumibles?");
    //addChat(my_enums::S_ELEMENTAL_WATER_WORLD_, 1, 1, 2, "Pistas2", "bla bla");

    //wise elemental fire
    addChat(my_enums::S_ELEMENTAL_FIRE_WORLD_, 1, 1, 1, "Logros ocultos", "¿Te has enfrentado al señor oscuro?");
    addChat(my_enums::S_ELEMENTAL_FIRE_WORLD_, 1, 1, 2, "Más logros ocultos", "¿Has muerto y renacido?");

    //wise elemental air
    addChat(my_enums::S_ELEMENTAL_WIND_WORLD_, 1, 1, 1, "Logros ocultos", "¿Y si los nombre del todo además de verse se pudiera tocar?");
    //addChat(my_enums::S_ELEMENTAL_WIND_WORLD_, 1, 1, 2, "Pistas2", "bla bla");

    //wise elemental earth
    addChat(my_enums::S_ELEMENTAL_EARTH_WORLD_, 1, 1, 1, "Más pistas sobre los logros de combate", "Algunos logros requieren derrotar múltiples enemigos con las habilidades de cuerpo a cuerpo (Se sumán los enemigos derrotados con Mele, Arqueria o Subertugio) o mágicas (Se sumán los enemigos derrotados con Magia o Drenar)");
    

    //wise dark forest
    addChat(my_enums::S_FOREST_WORLD_, 230, 50, 1, "¿Quien eres?", "Hace mucho tiempo mi morada estaba dentro de este edificio, pero fuí desterrado...");
    addChat(my_enums::S_FOREST_WORLD_, 230, 50, 2, "¿Que es este edificio?", "Es un portal a otro plano, oscuro, ardiente y peligroso, no intentes entrar a menos que mejores tus habilidades y te carges de suministros");
    addChat(my_enums::S_FOREST_WORLD_, 230, 50, 3, "¿Como puedo entrar?", "Hacen falta 4 anillos de 4 dimensiones diferentes, pero es solo una parte");
    addChat(my_enums::S_FOREST_WORLD_, 230, 50, 4, "<<MISIÓN>>Después del portal...", "Cruzar el portal es solo una parte porqué despúes necesitarás una poción especial para abrir el paso... y aún no la tienes");
}

void game::loadShops()
{
    SHOPs.clear();

    addShop(my_enums::S_HOMETOWN_,1, 106, 155, 1, "FOOD", 1, "GOLD", 4, 207,my_enums::_OTHER_,0);
    addShop(my_enums::S_HOMETOWN_,2, 112, 155, 1, "ESPADA", 1, "GOLD", 10, 44, my_enums::_WEAPON_1H_, 1);
    addShop(my_enums::S_HOMETOWN_,2, 112, 155, 2, "ESPADA 2M", 1, "GOLD", 25, 40, my_enums::_WEAPON_2H_, 2);
    addShop(my_enums::S_HOMETOWN_,2, 112, 155, 3, "DAGA", 1, "GOLD", 5, 30, my_enums::_WEAPON_1H_, 0);
    addShop(my_enums::S_HOMETOWN_,2, 112, 155, 4, "ARCO", 1, "GOLD", 10, 59, my_enums::_WEAPON_RANGED_, 1);
    addShop(my_enums::S_HOMETOWN_,2, 112, 155, 5, "BALLESTA", 1, "GOLD", 25, 60, my_enums::_WEAPON_RANGED_, 2);
    addShop(my_enums::S_HOMETOWN_,3, 106, 161, 1, "ARMADURA CUERO", 1, "GOLD", 10, 114, my_enums::_ARMOR_, 1);
    addShop(my_enums::S_HOMETOWN_,3, 106, 161, 2, "CASCO CUERO", 1, "GOLD", 5, 160, my_enums::_HELMET_, 1);
    addShop(my_enums::S_HOMETOWN_,3, 106, 161, 3, "ESCUDO CUERO", 1, "GOLD", 5, 151, my_enums::_SHIELD_, 1);
    addShop(my_enums::S_HOMETOWN_,4, 112, 161, 1, "POCION VITALIDAD", 1, "GOLD", 25, 276, my_enums::_OTHER_, 0);
    addShop(my_enums::S_HOMETOWN_,4, 112, 161, 2, "POCION PODER", 1, "GOLD", 25, 273, my_enums::_OTHER_, 0);
    addShop(my_enums::S_HOMETOWN_,4, 112, 161, 3, "ANILLO HABILIDAD+1", 1, "GOLD", 50, 220, my_enums::_RING_, 1);

    //outside hometown
    addShop(my_enums::S_COAST_WORLD_, 1, 1, 1, 1, "FOOD", 1, "GOLD", 4, 207, my_enums::_OTHER_, 0);
    addShop(my_enums::S_COAST_WORLD_, 2, 1, 1, 2, "ESPADA+1", 1, "GOLD", 25, 44, my_enums::_WEAPON_1H_, 2);
    addShop(my_enums::S_COAST_WORLD_, 2, 1, 1, 3, "ARCO+1", 1, "GOLD", 25, 59, my_enums::_WEAPON_RANGED_, 2);
    addShop(my_enums::S_COAST_WORLD_, 3, 1, 1, 4, "ARMADURA CUERO+1", 1, "GOLD", 25, 114, my_enums::_ARMOR_, 2);
    addShop(my_enums::S_COAST_WORLD_, 3, 1, 1, 5, "CASCO CUERO+1", 1, "GOLD", 10, 160, my_enums::_HELMET_, 2);
    addShop(my_enums::S_COAST_WORLD_, 4, 1, 1, 3, "ANILLO VITALIDAD+2", 1, "GOLD", 50, 220, my_enums::_RING_, 2);

    addShop(my_enums::S_FOREST_WORLD_, 1, 1, 1, 1, "FOOD", 1, "GOLD", 4, 207, my_enums::_OTHER_, 0);
    addShop(my_enums::S_FOREST_WORLD_, 2, 1, 1, 2, "ESPADA 2M+1", 1, "GOLD", 50, 40, my_enums::_WEAPON_2H_, 3);
    addShop(my_enums::S_FOREST_WORLD_, 2, 1, 1, 3, "DAGA+1", 1, "GOLD", 10, 30, my_enums::_WEAPON_1H_, 1);
    addShop(my_enums::S_FOREST_WORLD_, 3, 1, 1, 4, "ESCUDO CUERO+1", 1, "GOLD", 10, 151, my_enums::_SHIELD_, 2);
    addShop(my_enums::S_FOREST_WORLD_, 3, 1, 1, 5, "ARMADURA ACERO", 1, "GOLD", 50, 114, my_enums::_ARMOR_, 3);
    addShop(my_enums::S_FOREST_WORLD_, 4, 1, 1, 6, "ANILLO SUERTE+2", 1, "GOLD", 100, 220, my_enums::_RING_, 2);
 

    addShop(my_enums::S_ELEMENTAL_EARTH_WORLD_, 1, 1, 1, 1, "FOOD", 1, "GOLD", 4, 207, my_enums::_OTHER_, 0);
    addShop(my_enums::S_ELEMENTAL_EARTH_WORLD_, 2, 1, 1, 2, "ESPADA+2", 1, "GOLD", 50, 44, my_enums::_WEAPON_1H_, 3);
    addShop(my_enums::S_ELEMENTAL_EARTH_WORLD_, 2, 1, 1, 3, "BALLESTA+2", 1, "GOLD", 100, 60, my_enums::_WEAPON_RANGED_, 4);
    addShop(my_enums::S_ELEMENTAL_EARTH_WORLD_, 3, 1, 1, 4, "CASCO ACERO", 1, "GOLD", 25, 160, my_enums::_HELMET_, 2);
    addShop(my_enums::S_ELEMENTAL_EARTH_WORLD_, 3, 1, 1, 5, "ESCUDO ACERO", 1, "GOLD", 25, 151, my_enums::_SHIELD_, 2);
    addShop(my_enums::S_ELEMENTAL_EARTH_WORLD_, 4, 1, 1, 6, "ANILLO PODER+2", 1, "GOLD", 100, 220, my_enums::_RING_, 2);

    addShop(my_enums::S_ELEMENTAL_WATER_WORLD_, 1, 1, 1, 1, "FOOD", 1, "GOLD", 4, 207, my_enums::_OTHER_, 0);
    addShop(my_enums::S_ELEMENTAL_WATER_WORLD_, 2, 1, 1, 2, "ESPADA 2M+3", 1, "GOLD", 200, 40, my_enums::_WEAPON_2H_, 5);
    addShop(my_enums::S_ELEMENTAL_WATER_WORLD_, 2, 1, 1, 3, "ARCO+3", 1, "GOLD", 100, 59, my_enums::_WEAPON_RANGED_, 4);
    addShop(my_enums::S_ELEMENTAL_WATER_WORLD_, 3, 1, 1, 4, "ARMADURA ACERO+1", 1, "GOLD", 100, 114, my_enums::_ARMOR_, 4);
    addShop(my_enums::S_ELEMENTAL_WATER_WORLD_, 3, 1, 1, 5, "CASCO ACERO+1", 1, "GOLD", 50, 160, my_enums::_HELMET_, 3);
    addShop(my_enums::S_ELEMENTAL_WATER_WORLD_, 4, 1, 1, 6, "ANILLO HABILIDAD+3", 1, "GOLD", 200, 220, my_enums::_RING_, 3);
    

    addShop(my_enums::S_ELEMENTAL_WIND_WORLD_, 1, 1, 1, 1, "FOOD", 1, "GOLD", 4, 207, my_enums::_OTHER_, 0);
    addShop(my_enums::S_ELEMENTAL_WIND_WORLD_, 2, 1, 1, 2, "DAGA+4", 1, "GOLD", 100, 30, my_enums::_WEAPON_1H_, 4);
    addShop(my_enums::S_ELEMENTAL_WIND_WORLD_, 2, 1, 1, 3, "BALLESTA+4", 1, "GOLD", 400, 60, my_enums::_WEAPON_RANGED_, 6);
    addShop(my_enums::S_ELEMENTAL_WIND_WORLD_, 3, 1, 1, 4, "ESCUDO ACERO+1", 1, "GOLD", 50, 151, my_enums::_SHIELD_, 3);
    addShop(my_enums::S_ELEMENTAL_WIND_WORLD_, 3, 1, 1, 5, "ARMADURA TITANIUM", 1, "GOLD", 200, 114, my_enums::_ARMOR_, 5);
    addShop(my_enums::S_ELEMENTAL_WIND_WORLD_, 4, 1, 1, 6, "ANILLO VITALIDAD+4", 1, "GOLD", 200, 220, my_enums::_RING_, 5);

    addShop(my_enums::S_ELEMENTAL_FIRE_WORLD_, 1, 1, 1, 1, "FOOD", 1, "GOLD", 4, 207, my_enums::_OTHER_, 0);
    addShop(my_enums::S_ELEMENTAL_FIRE_WORLD_, 2, 1, 1, 2, "ESPADA+5", 1, "GOLD", 400, 44, my_enums::_WEAPON_1H_, 6);
    addShop(my_enums::S_ELEMENTAL_FIRE_WORLD_, 2, 1, 1, 3, "ARCO+5", 1, "GOLD", 400, 59, my_enums::_WEAPON_RANGED_, 6);
    addShop(my_enums::S_ELEMENTAL_FIRE_WORLD_, 3, 1, 1, 4, "CASCO TITANIUM", 1, "GOLD", 100, 160, my_enums::_HELMET_, 4);
    addShop(my_enums::S_ELEMENTAL_FIRE_WORLD_, 3, 1, 1, 5, "ESCUDO TITANIUM", 1, "GOLD", 100, 151, my_enums::_SHIELD_, 4);
    addShop(my_enums::S_ELEMENTAL_FIRE_WORLD_, 4, 1, 1, 6, "ANILLO SUERTE+4", 1, "GOLD", 400, 220, my_enums::_RING_, 4);

}

void game::setButtonDefaults()
{
    //Set button´s size, position and caption
    titleButton.setButton(gScreenSurface->w / 2 - 450, gScreenSurface->h / 4, 900, 100, "");

    itemSelectedButton.setButton(gScreenSurface->w / 2 - 200, 350, 850, gScreenSurface->h - 350, " ");

    achievementGroup0Button.setButton(gScreenSurface->w / 2 - 250, 200, 100, 50, "");
    achievementGroup1Button.setButton(gScreenSurface->w / 2 - 250, 250, 100, 50, "");
    achievementGroup2Button.setButton(gScreenSurface->w / 2 - 250, 300, 100, 50, "");
    achievementGroup3Button.setButton(gScreenSurface->w / 2 - 250, 350, 100, 50, "");
    achievementGroup4Button.setButton(gScreenSurface->w / 2 - 250, 400, 100, 50, "");

    moveLeftButton.setButton(gScreenSurface->w - btnx * 3, gScreenSurface->h / 4 * 3, btnx, btny, "A");
    moveLeftButton.setColor(0, 0, 200);
    moveDownButton.setButton(gScreenSurface->w - btnx * 2, gScreenSurface->h / 4 * 3, btnx, btny, "S");
    moveDownButton.setColor(0, 0, 200);
    moveRightButton.setButton(gScreenSurface->w - btnx, gScreenSurface->h / 4 * 3, btnx, btny, "D");
    moveRightButton.setColor(0, 0, 200);
    moveUpButton.setButton(gScreenSurface->w - btnx * 2, gScreenSurface->h / 4 * 3 - btny, btnx, btny, "W");
    moveUpButton.setColor(0, 0, 200);

     nextButton.setButton(gScreenSurface->w / 2 + 128, gScreenSurface->h / 2, btnx * 2, btny * 2, "Siguiente");
    nextButton.setColor(128, 128, 128);
    prevButton.setButton(gScreenSurface->w / 2 - 256, gScreenSurface->h / 2, btnx * 2, btny * 2, "Anterior");
    prevButton.setColor(128, 128, 128);

     exitButton.setButton(gScreenSurface->w - btnx, 0, btnx, btny, " ");
    exitButton.setColor(200, 0, 0);

    continueButton.setButton(gScreenSurface->w / 2 - btnx, gScreenSurface->h / 4 * 3, btnx * 2, btny * 2, "Continuar");
    continueButton.setColor(0, 0, 200);

     backButton.setButton(gScreenSurface->w / 2 - btnx * 3, gScreenSurface->h / 4 * 3, btnx * 2, btny * 2, "Volver");
    backButton.setColor(0, 0, 200);

     rollButton.setButton(gScreenSurface->w / 2 - btnx, gScreenSurface->h / 2, btnx * 2, btny * 2, "Volver a tirar");
    rollButton.setColor(0, 0, 200);

     startButton.setButton(gScreenSurface->w / 2 - btnx, gScreenSurface->h / 4 * 3, btnx * 2, btny * 2, "Jugar");

    startButton.setColor(100, 100, 100);

    player1Button.setButton(gScreenSurface->w / 2, gScreenSurface->h / 2 + 100, 200, 50, "Nombre Jugador");
    player1Button.setColor(100, 100, 100);

    achievementsButton.setButton(1, 1, btnx, btny, "0 %");
    achievementsButton.setColor(0, 0, 200);

    configButton.setButton(1 + btnx, 1, btnx, btny, " ");
    configButton.setColor(100, 100, 100);
    inventoryButton.setButton(1 + btnx * 2, 1, btnx, btny, " ");
    inventoryButton.setColor(0, 0, 200);
    playerButton.setButton(1 + btnx * 3, 1, btnx, btny, " ");
    playerButton.setColor(0, 0, 200);
    mapButton.setButton(1 + btnx * 4, 1, btnx, btny, " ");
    mapButton.setColor(200, 200, 200);
    foodButton.setButton(1 + btnx * 5, 1, btnx, btny, " ");

    skillButton.setButton(gScreenSurface->w / 2 - 250 - 100, 200, 50, 50, "+");
    staminaButton.setButton(gScreenSurface->w / 2 - 250 - 100, 250, 50, 50, "+");
    powerButton.setButton(gScreenSurface->w / 2 - 250 - 100, 300, 50, 50, "+");
    luckButton.setButton(gScreenSurface->w / 2 - 250 - 100, 350, 50, 50, "+");

      skillButton.setColor(200, 200, 200);
    staminaButton.setColor(200, 200, 200);
      powerButton.setColor(200, 200, 200);
       luckButton.setColor(200, 200, 200);

    newMapButton.setButton(gScreenSurface->w - 128, gScreenSurface->h / 2, 128, 128, "New map");
    newMapButton.setColor(200, 200, 200);

    musicButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 100, 200, 50, "Cambia canción");
    musicButton.setColor(100, 100, 100);
    muteButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 150, 200, 50, "MUSIC ON");
    muteButton.setColor(100, 100, 100);
    volumeSoundDownButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 200, 50, 50, "-");
    volumeSoundDownButton.setColor(100, 100, 100);
    volumeSoundButton.setButton(gScreenSurface->w / 2 + 50 - 100, gScreenSurface->h / 3 + 200, 100, 50, "SOUND 100%");
    volumeSoundButton.setColor(200, 200, 200);
    volumeSoundUpButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 3 + 200, 50, 50, "+");
    volumeSoundUpButton.setColor(100, 100, 100);
    volumeMusicDownButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 250, 50, 50, "-");
    volumeMusicDownButton.setColor(100, 100, 100);
    volumeMusicButton.setButton(gScreenSurface->w / 2 + 50 - 100, gScreenSurface->h / 3 + 250, 100, 50, "MUSIC 100%");
    volumeMusicButton.setColor(200, 200, 200);
    volumeMusicUpButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 3 + 250, 50, 50, "+");
    volumeMusicUpButton.setColor(100, 100, 100);
    blurButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 300, 200, 50, "BLUR ON");
    blurButton.setColor(100, 100, 100);


    tuto1Button.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 350, 200, 50, "Tutorial Controles");
    tuto1Button.setColor(100, 100, 100);
    tuto2Button.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 400, 200, 50, "Tutorial Pantalla Principal");
    tuto2Button.setColor(100, 100, 100);

    playerUpButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 2 + 250, 50, 50, "+");
    playerUpButton.setColor(100, 100, 100);
    playerDownButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 2 + 250, 50, 50, "-");
    playerDownButton.setColor(100, 100, 100);



    mouseButton.setButton(gScreenSurface->w / 2 - 100, 0, 200, 50, "X:" + std::to_string(mousex) + " Y:" + std::to_string(mousey));
    mouseButton.setColor(100, 100, 100);

    fightButton.setButton(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 50, 128, 128, "Atacar");
    bowButton.setButton(gScreenSurface->w / 2 - 200 + 128, gScreenSurface->h / 2 - 50, 128, 128, "Arco");
    spellButton.setButton(gScreenSurface->w / 2 - 200 + 128 * 2, gScreenSurface->h / 2 - 50, 128, 128, "Magia");
    drainButton.setButton(gScreenSurface->w / 2 - 200 + 128 * 3, gScreenSurface->h / 2 - 50, 128, 128, "Drenar");
    hideButton.setButton(gScreenSurface->w / 2 - 200 + 128 * 4, gScreenSurface->h / 2 - 50, 128, 128, "Subterfugio");
    summonButton.setButton(gScreenSurface->w / 2 - 200 + 128 * 5, gScreenSurface->h / 2 - 50, 128, 128, "Invocar");

    potionHealthButton.setButton(gScreenSurface->w - 256, gScreenSurface->h - 128, 128, 128, "+Salud");
    potionHealthButton.setColor(100, 100, 100);
    potionMagicButton.setButton(gScreenSurface->w - 128, gScreenSurface->h - 128, 128, 128, "+Magia");
    potionMagicButton.setColor(100, 100, 100);

    shop0.setButton(50, gScreenSurface->h / 2, gScreenSurface->w - 100, 50, "0");
    shop1.setButton(50, gScreenSurface->h / 2 + 50, gScreenSurface->w - 100, 50, "1");
    shop2.setButton(50, gScreenSurface->h / 2 + 100, gScreenSurface->w - 100, 50, "2");
    shop3.setButton(50, gScreenSurface->h / 2 + 150, gScreenSurface->w - 100, 50, "3");
    shop4.setButton(50, gScreenSurface->h / 2 + 200, gScreenSurface->w - 100, 50, "4");
    shop5.setButton(50, gScreenSurface->h / 2 + 250, gScreenSurface->w - 100, 50, "5");

    chat0.setButton(50, gScreenSurface->h / 2-100,  gScreenSurface->w - 100, 50, "0");
    chat1.setButton(50, gScreenSurface->h / 2 -100 + 50, gScreenSurface->w - 100, 50, "1");
    chat2.setButton(50, gScreenSurface->h / 2 -100 + 100, gScreenSurface->w - 100, 50, "2");
    chat3.setButton(50, gScreenSurface->h / 2 -100 + 150, gScreenSurface->w - 100, 50, "3");
    chat4.setButton(50, gScreenSurface->h / 2 -100 + 200, gScreenSurface->w - 100, 50, "4");
    chat5.setButton(50, gScreenSurface->h / 2 -100 + 250, gScreenSurface->w - 100, 50, "5");

    confirmExitButton.setButton(gScreenSurface->w/2 - 400, gScreenSurface->h/4*3, 200, 100, "Salir");
    cancelExitButton.setButton(gScreenSurface->w / 2 + 200, gScreenSurface->h / 4 * 3, 200, 100, "Cancelar");

    
                                              
}

void game::start()
{
    
    btnx = gScreenSurface->w / 32;
    btny = gScreenSurface->h / 16;


    //Initialize timers
    ticksAI = SDL_GetTicks();

    loadPlayerDefault();

    loadNPCs();

    loadShops();

    loadChats();

    loadEvents();

    int width, height;
    width = 256;
    height = 256;
    baseMap.mymap.init();
    baseMap.createSurface();


    int octave = rand() % 3 + 4;
    float frequency = (rand() % 6) / 2 + 1;
    float persistance = 0.5f;


    baseMap.blur();

    //Set game framerate
    ticksPerFrame = (int)(1000 / desiredFPS);

    //Hide Window´s cursor
    SDL_ShowCursor(SDL_DISABLE);

    setButtonDefaults();

    
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


//getStringState(currentstate);
my_enums::gameState game::getStringState(int state)
{
    switch (state)
    {
    case my_enums::S_INTRO_: return my_enums::S_INTRO_;
    case my_enums::S_MAINMENU_:return my_enums::S_MAINMENU_;
    case my_enums::S_HERO_:return my_enums::S_HERO_;
    case my_enums::S_SHOP_:return my_enums::S_SHOP_;
    case my_enums::S_CHAT_:return my_enums::S_CHAT_;
    case my_enums::S_MASTER_:return my_enums::S_MASTER_;
    case my_enums::S_NAME_:return my_enums::S_NAME_;
    case my_enums::S_ATTRIBUTES_:return my_enums::S_ATTRIBUTES_;
    case my_enums::S_PLAYER_:return my_enums::S_PLAYER_;
    case my_enums::S_GAMEOVER_:return my_enums::S_GAMEOVER_;
    case my_enums::S_RACES_:return my_enums::S_RACES_;
    case my_enums::S_ARCHETYPES_:return my_enums::S_ARCHETYPES_;
    case my_enums::S_INVENTORY_:return my_enums::S_INVENTORY_;
    case my_enums::S_ACHIEVEMENTS_:return my_enums::S_ACHIEVEMENTS_;
    case my_enums::S_FIGHT_:return my_enums::S_FIGHT_;
    case my_enums::S_CONFIGMENU_:return my_enums::S_CONFIGMENU_;
    case my_enums::S_FOREST_WORLD_: return my_enums::S_FOREST_WORLD_;
    case my_enums::S_COAST_WORLD_: return my_enums::S_COAST_WORLD_;
    case my_enums::S_ELEMENTAL_FIRE_WORLD_: return my_enums::S_ELEMENTAL_FIRE_WORLD_;
    case my_enums::S_ELEMENTAL_WATER_WORLD_: return my_enums::S_ELEMENTAL_WATER_WORLD_;
    case my_enums::S_ELEMENTAL_EARTH_WORLD_: return my_enums::S_ELEMENTAL_EARTH_WORLD_;
    case my_enums::S_ELEMENTAL_WIND_WORLD_: return my_enums::S_ELEMENTAL_WIND_WORLD_;
    case my_enums::S_NECRO_WORLD_:return my_enums::S_NECRO_WORLD_;
    case my_enums::S_HOMETOWN_:return my_enums::S_HOMETOWN_;
    default: return my_enums::S_HOMETOWN_;
    };
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
    case my_enums::S_INTRO_:
    {
        eventsIntro();
        break;
    }
    case my_enums::S_MAINMENU_:
    {
        eventsMain();
        break;
    }

    case my_enums::S_HERO_:
    {
        eventsHero();
        break;
    }

    case my_enums::S_SHOP_:
    {
        eventsShops();
        break;
    }

    case my_enums::S_CHAT_:
    {
        eventsChat();
        break;
    }

    case my_enums::S_NAME_:
    {
        eventsPlayerName();
        break;
    }

    case my_enums::S_ATTRIBUTES_:
    {
        eventsPlayerAttributes();
        break;
    }

    case my_enums::S_PLAYER_:
    {
        eventsPlayer();
        break;
    }

    case my_enums::S_RACES_:
    {
        eventsPlayerRaces();
        break;
    }

    case my_enums::S_ARCHETYPES_:
    {
        eventsPlayerArchetypes();
        break;
    }

    case my_enums::S_INVENTORY_:
    {
        eventsInventory();
        break;
    }

    case my_enums::S_ACHIEVEMENTS_:
    {
        eventsAchievements();
        break;
    }

    case my_enums::S_FIGHT_:
    {
        eventsFight();
        break;
    }

    case my_enums::S_GAMEOVER_:
    {
        eventsGameOver();
        break;
    }

    case my_enums::S_MASTER_:
    {
        eventsMaster();
        break;
    }
   

    case my_enums::S_CONFIGMENU_:
    {
        eventsConfigMenu();
        break;
    }

    case my_enums::S_FOREST_WORLD_: case my_enums::S_COAST_WORLD_: case my_enums::S_ELEMENTAL_FIRE_WORLD_: case my_enums::S_ELEMENTAL_WATER_WORLD_: case my_enums::S_ELEMENTAL_EARTH_WORLD_: case my_enums::S_ELEMENTAL_WIND_WORLD_: case my_enums::S_NECRO_WORLD_:
    case my_enums::S_HOMETOWN_:

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
    case my_enums::S_INTRO_:
    {
        screenIntro();
        break;
    }
    case my_enums::S_MAINMENU_:
    {
        screenMain();
        break;
    }

    case my_enums::S_HERO_:
    {
        screenHero();
        break;
    }

    case my_enums::S_SHOP_:
    {
        screenShops();
        break;
    }

    case my_enums::S_CHAT_:
    {
        screenChat();
        break;
    }

    case my_enums::S_MASTER_:
    {
        screenMaster();
        break;
    }

    case my_enums::S_NAME_:
    {
        screenPlayerName();
        break;
    }

    case my_enums::S_ATTRIBUTES_:
    {
        screenPlayerAttributes();
        break;
    }

    case my_enums::S_PLAYER_:
    {
        screenPlayer();
        break;
    }

    case my_enums::S_GAMEOVER_:
    {
        screenGameOver();
        break;
    }

    case my_enums::S_RACES_:
    {
        screenPlayerRaces();
        break;
    }

    case my_enums::S_ARCHETYPES_:
    {
        screenPlayerArchetypes();
        break;
    }

    case my_enums::S_INVENTORY_:
    {
        screenInventory();
        break;
    }

    case my_enums::S_ACHIEVEMENTS_:
    {
        screenAchievements();
        break;
    }

    case my_enums::S_FIGHT_:
    {
        screenFight();
        break;
    }

 
    case my_enums::S_CONFIGMENU_:
    {
        screenConfigMenu();
        break;
    }

      case my_enums::S_FOREST_WORLD_: case my_enums::S_COAST_WORLD_: case my_enums::S_ELEMENTAL_FIRE_WORLD_: case my_enums::S_ELEMENTAL_WATER_WORLD_: case my_enums::S_ELEMENTAL_EARTH_WORLD_: case my_enums::S_ELEMENTAL_WIND_WORLD_: case my_enums::S_NECRO_WORLD_:
    case my_enums::S_HOMETOWN_:
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

void game::screenMaster() {
    screenPlayer();

    tmpRect.x = 1;
    tmpRect.y = 1;
    tmpRect.w = 500;
    tmpRect.h = 100;
    drawSquare(tmpRect, { 200,200,200 });
    drawTextResize("MASTER", tmpRect);

    tmpRect.x = gScreenSurface->w -600;
    tmpRect.h = gScreenSurface->h;
    tmpRect.w = 600;
    drawSquare(tmpRect, { 200,200,200 });
    
    tmpRect.h = 50;
    drawTextBlock("Hola, "+ playerName + " cada vez que subes de nivel consigues puntos de habilidad, yo puedo mejorar tus habilidades gastando esos puntos y a continuación te explico cada atributo", tmpRect);

    tmpRect.y = 300;
    drawTextBlock("COMBATE: Habilidad para luchar y defenderte, mejoras un punto por punto de habilidad", tmpRect);
    tmpRect.y = 500;
    drawTextBlock("VITALIDAD: Puntos de vidad, mejoras dos punto por punto de habilidad", tmpRect);
    tmpRect.y = 700;
    drawTextBlock("PODER: Habilidad que te permite usar la magia , mejoras un punto por punto de habilidad", tmpRect);
    tmpRect.y = 900;
    drawTextBlock("SUERTE: Habilidad que te permite tener hacer ataques furtivos y posibilidad de esquivar ataques usando armas a distancia, mejoras un punto por punto de habilidad", tmpRect);

    if (skillPoints > 0) {
        drawButton(skillButton);
        drawButton(staminaButton);
        drawButton(powerButton);
        drawButton(luckButton);
    }
}

void game::drawMouse()
{
    //Paint Mouse Over everything
    SDL_PumpEvents();
    SDL_GetMouseState(&mousex, &mousey);
    mouseRect.x = mousex - 10;
    mouseRect.y = mousey;
    mouseRect.w = mouseSurface->w;
    mouseRect.h = mouseSurface->h;
    SDL_RenderCopy(gRenderer, mouseTexture, NULL, &mouseRect);
}

//Flip backscreen buffer to monitor
void game::screenFlip()
{
    //Flip image to monitor
    SDL_RenderPresent(gRenderer);
}

//Adjust frame time to comply with desired framerate
void game::adjustFPS()
{
    currentFrame =  SDL_GetTicks()- FPStimer;
    if (currentFrame < ticksPerFrame) {
            SDL_Delay(ticksPerFrame - currentFrame);
    }
    FPStimer = SDL_GetTicks();

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


std::string game::slash(){
    #ifdef WIN_32
    return "\";
    #else
        return "/";
    #endif
 }


//Load all media files (images, sounds and music)
bool game::loadMedia(string base)
{
    string images = base + "images" + std::string(slash());
    string sounds = base + "sounds" + std::string(slash());
    string fonts = base + "fonts" + std::string(slash());


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
     SDL_MapRGB(playersSurface->format, 71, 108, 108)); // This is the color that should be taken as being the 'transparent' part of the image

    playersTexture = SDL_CreateTextureFromSurface(gRenderer, playersSurface);


    aFile = images + "items_alpha.png";
    itemsSurface = loadSurface(aFile);
    SDL_SetColorKey(itemsSurface, SDL_TRUE, // enable color key (transparency)
        SDL_MapRGB(itemsSurface->format, 71, 108, 108)); // This is the color that should be taken as being the 'transparent' part of the image

    itemsTexture = SDL_CreateTextureFromSurface(gRenderer, itemsSurface);


    aFile = images + "terrain_alpha.png";
    locationsSurface = loadSurface(aFile);
    SDL_SetColorKey(locationsSurface, SDL_TRUE, // enable color key (transparency)
        SDL_MapRGB(locationsSurface->format, 71, 108, 108)); // This is the color that should be taken as being the 'transparent' part of the image

    locationsTexture = SDL_CreateTextureFromSurface(gRenderer, locationsSurface);
   
    aFile = images + "15771169063_9f6d64ce74_b.jpg";

bg1Surface = loadSurface(aFile);
bg1Texture = SDL_CreateTextureFromSurface(gRenderer, bg1Surface);

aFile = images + "334968999_40607c07ce_b.jpg";
bg2Surface = loadSurface(aFile);
bg2Texture = SDL_CreateTextureFromSurface(gRenderer, bg2Surface);


talkTexture = loadTexture(images + "talk.png");

buttonCloseTexture    = loadTexture(images + "exit-door.png");
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
buttonBackTexture     = loadTexture(images + "anticlockwise-rotation.png");


buttonSwordTexture = loadTexture(images + "sword-brandish.png");
buttonBowTexture = loadTexture(images + "archer.png");
buttonSpellTexture = loadTexture(images + "spell-book.png");
buttonDrainTexture = loadTexture(images + "dead-head.png");
buttonHideTexture = loadTexture(images + "invisible.png");
buttonSummonTexture = loadTexture(images + "magic-hat.png");


buttonSoundOnTexture  = loadTexture(images + "speaker.png");
buttonSoundOffTexture = loadTexture(images + "speaker-off.png");

buttonUpTexture = loadTexture(images + "up-button.png");
buttonDownTexture = loadTexture(images + "down-button.png");
buttonLeftTexture = loadTexture(images + "left-button.png");
buttonRightTexture = loadTexture(images + "right-button.png");

buttonPlayerTexture = loadTexture(images + "histogram.png");


buttonPotionHealthTexture = loadTexture(images + "potion-health.png");
buttonPotionMagicTexture = loadTexture(images + "potion-magic.png");
buttonFoodTexture = loadTexture(images + "bread.png");

string TTFFile = fonts + "PressStart2P.ttf";
    
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



    //Open small font
    smallFont = TTF_OpenFont(TTFFile.c_str(), 14);
    if (smallFont == NULL)
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

    string Audio6 = sounds + "click.wav";
    audioButton = Mix_LoadWAV(Audio6.c_str());
   
    Audio6 = sounds + "swish_2.wav";
    sword  = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "bow.wav";
    bow    = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "swish_2.wav";
    dagger = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "explode.ogg";
    magic  = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "click.wav";
    drain  = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "click.wav";
    invoke = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "sell_buy_item.wav";
    buysell = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "heal.ogg";
    heal    = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "ring_inventory.wav";
    ring    = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "click.wav";
    clothes = Mix_LoadWAV(Audio6.c_str());

    Audio6 = sounds + "metal-clash.wav";
    weapon  = Mix_LoadWAV(Audio6.c_str());
    
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
    SDL_Rect target = btn.getRect();
    drawSquare(btn.getRect(), btn.getColor());
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0);
    SDL_RenderDrawLine(gRenderer, target.x, target.y, target.x + target.w, target.y);
    SDL_RenderDrawLine(gRenderer, target.x, target.y, target.x, target.y + target.h);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderDrawLine(gRenderer, target.x, target.y + target.h, target.x + target.w, target.y + target.h);
    SDL_RenderDrawLine(gRenderer, target.x + target.w, target.y, target.x + target.w, target.y + target.h);

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

    SDL_FreeSurface(text);
    text = NULL;
    SDL_DestroyTexture(txtTexture);
    txtTexture = NULL;

}

int game::getEventTile(int x, int y)
{
    int tile = 0;

    for (list<EVENT>::iterator it = EVENTs.begin(); it != EVENTs.end(); it++)
    {
        if ((x == it->x) && (y == it->y) && (it->map == currentState)) {

            tile = it->tile;
        }
    }


    return tile;

}

//Draw events which have a tile associated
void game::drawEvents()
{
    SDL_Rect srcrect;

    //draw second layer tiles
    int size_x = gScreenSurface->w / cam_size_x;
    int size_y = gScreenSurface->h / cam_size_y;

    for (int x = cam_x; x < cam_x + cam_size_x; x++) {
        for (int y = cam_y; y < cam_y + cam_size_y; y++) {
            int ax = (x - cam_x) * size_x;
            int ay = (y - cam_y) * size_y;
            srcrect.x = ax;
            srcrect.y = ay;
            srcrect.w = size_x;
            srcrect.h = size_y;

            int tile = getEventTile(x, y);
            if (tile > 0) {
                drawTileset(srcrect, itemsTexture, tile, 20);
            }

        }

    }

}

//Draw game map
void game::drawMap()
{
    //Draw base map
    SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, baseMap.targetSurface);
    SDL_RenderCopy(gRenderer, txtTexture, NULL, NULL);

    //draw tiles
    SDL_Rect srcrect;


    //draw second layer tiles
    int size_x = gScreenSurface->w / cam_size_x;
    int size_y = gScreenSurface->h / cam_size_y;

    for (int x = cam_x; x < cam_x + cam_size_x; x++) {
        for (int y = cam_y; y < cam_y + cam_size_y; y++) {
            int ax = (x - cam_x)* size_x;
            int ay = (y - cam_y)* size_y;
            srcrect.x = ax;
            srcrect.y = ay;
            srcrect.w = size_x;
            srcrect.h = size_y;
            int tile = baseMap.get_cell(x, y);
            if (tile > 0) {
                drawTileset(srcrect, locationsTexture, tile,30);
            }

        }

    }



    SDL_Rect destRect;
    
    SDL_DestroyTexture(txtTexture);
    txtTexture = NULL;


}

void game::drawMiniMap()
{
    SDL_Rect destRect;
    int cell = 0;

    destRect.x = gScreenSurface->w / 2 - 256;
    destRect.y = gScreenSurface->h / 2 - 256;
    destRect.w = 512;
    destRect.h = 512;


    SDL_Texture* miniMapTexture = SDL_CreateTextureFromSurface(gRenderer, baseMap.imageSurface);
    SDL_RenderCopy(gRenderer, miniMapTexture, NULL, &destRect);

    SDL_DestroyTexture(miniMapTexture);
    miniMapTexture = NULL;




    SDL_SetRenderDrawColor(gRenderer, 200, 0, 0, 0);
    destRect.x += (cam_x - 1) * 2;
    destRect.y += (cam_y - 1) * 2;
    destRect.w = cam_size_x * 2;
    destRect.h = cam_size_y * 2;
    SDL_RenderDrawRect(gRenderer, &destRect);
  

    for (int x = 1; x < 256; x++) {
        for (int y = 1; y < 256; y++) {
            cell = baseMap.get_cell(x, y);
            if (cell > 0) {
                destRect.x = gScreenSurface->w / 2 - 256 + x * 2;
                destRect.y = gScreenSurface->h / 2 - 256 + y * 2;
                destRect.w = 2;
                destRect.h = 2;
                //walls and mountain tops
                if((cell==9)|| (cell == 579)||(cell == 345) || (cell == 346) || (cell == 6) || (cell == 21) || (cell == 129) || (cell == 154)) drawSquare(destRect, greyColor);
                //plants
                if ((cell == 330) || (cell == 341) || (cell == 342) || (cell == 343) || (cell == 344)) drawSquare(destRect, { 0,200,0,0 });
                //roads
                if ((cell == 90) || (cell == 96) ) drawSquare(destRect, brownColor);
                //building floors
                if ((cell == 104) || (cell == 247) || (cell == 20)) drawSquare(destRect, lightGreyColor);
                //firepits
                if (cell == 66) drawSquare(destRect, {200,0,0,0});                                      
            }
        }

    }
    destRect.x = gScreenSurface->w / 2 - 256;
    destRect.y = gScreenSurface->h / 2 + 256;
    destRect.w = 512;
    destRect.h = 50;
    drawSquare(destRect, lightGreyColor);
    drawTextResize("Pulsa cualquier tecla para continuar...", destRect);


    destRect.x = gScreenSurface->w / 2 - 256 +px*2;
    destRect.y = gScreenSurface->h / 2 - 256 + py*2;
    destRect.w = 2;
    destRect.h = 2;
    drawSquare(destRect, {250,250,250,0});
    bool tick = false;

    destRect.x = gScreenSurface->w / 2 -20;
    destRect.y = gScreenSurface->h / 2 - 256;
    destRect.w = 40;
    destRect.h = 40;
    drawTextResize("N", destRect);
    destRect.y = gScreenSurface->h / 2 + 226;
    drawTextResize("S", destRect);
    destRect.x = gScreenSurface->w / 2 - 256;
    destRect.y = gScreenSurface->h / 2 - 20;
    drawTextResize("O", destRect);
    destRect.x = gScreenSurface->w / 2 + 226;
    drawTextResize("E", destRect);
    screenFlip();

    bool quit = false;
    while (!quit)
    {
        if (tick) {
            tick = false;
            destRect.x = gScreenSurface->w / 2 - 257 + px * 2;
            destRect.y = gScreenSurface->h / 2 - 257 + py * 2;
            destRect.w = 4;
            destRect.h = 4;
            drawSquare(destRect, { 250,250,250,0 });
            screenFlip();
        }
        else {
            tick = true;
            destRect.x = gScreenSurface->w / 2 - 257 + px * 2;
            destRect.y = gScreenSurface->h / 2 - 257 + py * 2;
            destRect.w = 4;
            destRect.h = 4;
            drawSquare(destRect, { 0,0,0,0 });
            screenFlip();
        }
        
        //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
            
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            quit = true;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            quit = true;
        }

        //******************
    }//events
}//while not quite

    
}


bool game::confirmExit()
{
   // int startTime = SDL_GetTicks();
    //int elapsed = 0;

    bool quit = false;
    bool confirm = false;
    SDL_Rect destRect;
    while (!quit)
    {        
        screenClear();
        drawBackground();

        destRect.x = gScreenSurface->w / 2 - 256;
        destRect.y = gScreenSurface->h / 8;
        destRect.w = 512;
        destRect.h = 50;
        drawSquare(destRect, lightGreyColor);
        drawTextResize("CONFIRMAR SALIR", destRect);


        destRect.x = gScreenSurface->w / 4;
        destRect.y = gScreenSurface->h / 4;
        destRect.w = gScreenSurface->w / 2;
        destRect.h = 100;
        drawSquare(destRect, lightGreyColor);
        drawTextBlock("¿Estas seguro que quieres salir del juego?", destRect);


        drawButtonSrc(confirmExitButton, buttonAcceptTexture);
        drawButtonSrc(cancelExitButton, buttonCancelTexture);

        /*
        if (elapsed > 1000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h / 4;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 100;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Achievement master se controla de dos maneras, con el teclado o bien con el ratón", destRect);

        }

        if (elapsed > 3000) {

            drawButtonSrc(moveLeftButton, buttonLeftTexture);
            drawButtonSrc(moveRightButton, buttonRightTexture);
            drawButtonSrc(moveUpButton, buttonUpTexture);
            drawButtonSrc(moveDownButton, buttonDownTexture);


            destRect.x = gScreenSurface->w / 2;
            destRect.y = gScreenSurface->h - 100;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 100;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Te desplazas pulsando las teclas WASD, o bien pulsando sobre las flechas de dirección", destRect);

        }

        if (elapsed > 5000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h / 2;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 150;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Para interactuar con los personajes debes desplazarte en su dirección con el teclado o bién pulsar con el ratón sobre ellos", destRect);

            destRect.y = gScreenSurface->h / 2 - 100;
            destRect.w = 100;
            destRect.h = 100;
            destRect.x = gScreenSurface->w / 4 + 101;
            drawTileset(destRect, playersTexture, 74, 20);
            destRect.x = gScreenSurface->w / 4 + 201;
            drawTileset(destRect, playersTexture, 77, 20);
            destRect.x = gScreenSurface->w / 4 + 301;
            drawTileset(destRect, playersTexture, 79, 20);
            destRect.x = gScreenSurface->w / 4 + 401;
            drawTileset(destRect, playersTexture, 83, 20);

        }

        if (elapsed > 7000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h * 3 / 4;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 150;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("para seleccionar opciones, simplemente desplazate con el ratón sobre la opcíon (icono) deseado y haz click", destRect);

            classButton demoButton;
            demoButton.setButton(gScreenSurface->w / 4 + 200, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonStarsTexture);
            demoButton.setButton(gScreenSurface->w / 4 + 300, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonConfigTexture);
            demoButton.setButton(gScreenSurface->w / 4 + 400, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonBackpackTexture);
            demoButton.setButton(gScreenSurface->w / 4 + 500, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonPlayerTexture);
            demoButton.setButton(gScreenSurface->w / 4 + 600, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonMapTexture);



        }

        if (elapsed > 9000) {
            destRect.x = 50;
            destRect.y = gScreenSurface->h - 100;
            destRect.w = 512;
            destRect.h = 50;
            drawSquare(destRect, lightGreyColor);
            drawTextResize("Pulsa cualquier tecla para continuar...", destRect);
        }

        */

        //Event handler
        SDL_Event e;

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                setState(my_enums::S_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver = SDL_GetTicks();
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                //******
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
            //    quit = true;

                if (confirmExitButton.clicked(mousex, mousey)) {
                    quit = true;
                    confirm = true;
                }

                if (cancelExitButton.clicked(mousex, mousey)) {
                    quit = true;
                    confirm = false;
                }

            }
            else if (e.type == SDL_KEYDOWN)
            {
              //  quit = true;
            }

            //******************
        }//events
        drawMouse();
        screenFlip();
        SDL_Delay(50);
    }//while not quit
    return confirm;
}



void game::screenDarkFate()
{
    int startTime = SDL_GetTicks();
    int elapsed = 0;

    bool quit = false;
    SDL_Rect destRect;
    while (!quit)
    {
        elapsed = SDL_GetTicks() - startTime;
        screenClear();
        drawBackground();

        destRect.x = gScreenSurface->w / 2 - 256;
        destRect.y = gScreenSurface->h / 8;
        destRect.w = 512;
        destRect.h = 50;
        drawSquare(destRect, lightGreyColor);
        drawTextResize("Los muertos no mueren", destRect);

        if (elapsed > 1000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h / 4;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 100;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Con un último ataque el señor oscuro cae...", destRect);

        }

        if (elapsed > 3000) {



          //  destRect.x = gScreenSurface->w / 2;
            destRect.y = gScreenSurface->h /2;
        //    destRect.w = gScreenSurface->w / 2;
            destRect.h = 100;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Pero cuando crees que ya has derrotado al señor oscuro, contemplas horrorizado como su cuerpo se reforma...", destRect);

        }

        if (elapsed > 5000) {
           // destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h / 4*3;
         //   destRect.w = gScreenSurface->w / 2;
            destRect.h = 150;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("SEÑOR OSCURO: ¡¡Solo los seres puros que han conseguido todos los logros en esta vida pueden derrotarme!, ¡¡Ahora muere!!, te lanza un rayo que te drena tu vitalidad, cierras los ojos y....", destRect);

        }

       /* if (elapsed > 7000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h * 3 / 4;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 150;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("para seleccionar opciones, simplemente desplazate con el ratón sobre la opcíon (icono) deseado y haz click", destRect);

            classButton demoButton;
            demoButton.setButton(gScreenSurface->w / 4 + 200, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonStarsTexture);
            demoButton.setButton(gScreenSurface->w / 4 + 300, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonConfigTexture);
            demoButton.setButton(gScreenSurface->w / 4 + 400, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonBackpackTexture);
            demoButton.setButton(gScreenSurface->w / 4 + 500, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonPlayerTexture);
            demoButton.setButton(gScreenSurface->w / 4 + 600, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonMapTexture);



        }*/



        if (elapsed > 9000) {
            destRect.x = gScreenSurface->w/2-256;
            destRect.y = gScreenSurface->h - 100;
            destRect.w = 512;
            destRect.h = 50;
            drawSquare(destRect, lightGreyColor);
            drawTextResize("Pulsa cualquier tecla para continuar...", destRect);
        }

        //Event handler
        SDL_Event e;

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                setState(my_enums::S_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver = SDL_GetTicks();
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                //******
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                quit = true;
            }

            //******************
        }//events
        drawMouse();
        screenFlip();
        SDL_Delay(50);
    }//while not quit
}

void game::screenWIN()
{
    int startTime = SDL_GetTicks();
    int elapsed = 0;

    bool quit = false;
    SDL_Rect destRect;
    while (!quit)
    {
        elapsed = SDL_GetTicks() - startTime;
        screenClear();
        drawBackground();

        destRect.x = gScreenSurface->w / 2 - 256;
        destRect.y = gScreenSurface->h / 8;
        destRect.w = 512;
        destRect.h = 50;
        drawSquare(destRect, lightGreyColor);
        drawTextResize("ACHIEVEMENT MASTER", destRect);

        if (elapsed > 1000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h / 4;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 100;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Con un último ataque el señor oscuro cae...", destRect);

        }

        if (elapsed > 3000) {
            destRect.y = gScreenSurface->h / 2;
            destRect.h = 100;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Observas como el señor oscuro se retuerze en el suelo y te habla por última vez...", destRect);
        }

        if (elapsed > 5000) {
            destRect.y = gScreenSurface->h / 4 * 3;
            destRect.h = 150;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("SEÑOR OSCURO: Solo un ser puro que ha conseguido todos los logros en esta vida podía derrotarme..., Tu.. eres.. ", destRect);
        }

         if (elapsed > 7000) {
             tmpRect.y = gScreenSurface->h / 4;
             tmpRect.h = 100;

             tmpRect.x = gScreenSurface->w / 2 - 450;
             tmpRect.w = 900;
             fg = { 128, 0, 128, 0 };
             drawTextResize("¡¡Achievement master!!", tmpRect);
             fg = { 0, 0, 0, 0 };
         }



        if (elapsed > 9000) {
            destRect.x = gScreenSurface->w / 2 - 256;
            destRect.y = gScreenSurface->h - 100;
            destRect.w = 512;
            destRect.h = 50;
            drawSquare(destRect, lightGreyColor);
            drawTextResize("Pulsa cualquier tecla para continuar...", destRect);
        }

        //Event handler
        SDL_Event e;

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                setState(my_enums::S_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver = SDL_GetTicks();
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                //******
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                quit = true;
            }

            //******************
        }//events
        drawMouse();
        screenFlip();
        SDL_Delay(50);
    }//while not quit

}


void game::playTutorial()

{
    int startTime = SDL_GetTicks();
    int elapsed = 0;

    bool quit = false;
    SDL_Rect destRect;
    while (!quit)
    {
        elapsed = SDL_GetTicks() - startTime;
        screenClear();
        drawBackground();
        
        destRect.x = gScreenSurface->w / 2 - 256;
        destRect.y = gScreenSurface->h / 8;
        destRect.w = 512;
        destRect.h = 50;
        drawSquare(destRect, lightGreyColor);
        drawTextResize("TUTORIAL CONTROLES", destRect);

        if (elapsed > 1000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h / 4;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 100;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Achievement master se controla de dos maneras, con el teclado o bien con el ratón", destRect);

        }

        if (elapsed > 3000) {

            drawButtonSrc(moveLeftButton, buttonLeftTexture);
            drawButtonSrc(moveRightButton, buttonRightTexture);
            drawButtonSrc(moveUpButton, buttonUpTexture);
            drawButtonSrc(moveDownButton, buttonDownTexture);

          
            destRect.x = gScreenSurface->w /2;
            destRect.y = gScreenSurface->h -100;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 100;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Te desplazas pulsando las teclas WASD, o bien pulsando sobre las flechas de dirección", destRect);

        }

        if (elapsed > 5000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h / 2;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 150;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Para interactuar con los personajes debes desplazarte en su dirección con el teclado o bién pulsar con el ratón sobre ellos", destRect);
            
            destRect.y = gScreenSurface->h / 2  -100;
            destRect.w = 100;
            destRect.h = 100;
            destRect.x = gScreenSurface->w / 4+101;
            drawTileset(destRect, playersTexture, 74, 20);
            destRect.x = gScreenSurface->w / 4+201;
            drawTileset(destRect, playersTexture, 77, 20);
            destRect.x = gScreenSurface->w / 4+301;
            drawTileset(destRect, playersTexture, 79, 20);
            destRect.x = gScreenSurface->w / 4+401;
            drawTileset(destRect, playersTexture, 83, 20);

        }

        if (elapsed > 7000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h *3/4;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 150;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("para seleccionar opciones, simplemente desplazate con el ratón sobre la opcíon (icono) deseado y haz click", destRect);

            classButton demoButton;
            demoButton.setButton(gScreenSurface->w / 4 +200, (gScreenSurface->h * 3 / 4)-100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonStarsTexture);
            demoButton.setButton(gScreenSurface->w / 4 +300, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonConfigTexture);
            demoButton.setButton(gScreenSurface->w / 4+400, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonBackpackTexture);   
            demoButton.setButton(gScreenSurface->w / 4+500, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonPlayerTexture);
            demoButton.setButton(gScreenSurface->w / 4+600, (gScreenSurface->h * 3 / 4) - 100, 100, 100, " ");
            drawButtonSrc(demoButton, buttonMapTexture);
            
            

        }


    
        if (elapsed > 9000) {
            destRect.x = 50;
            destRect.y = gScreenSurface->h -100;
            destRect.w = 512;
            destRect.h = 50;
            drawSquare(destRect, lightGreyColor);
            drawTextResize("Pulsa cualquier tecla para continuar...", destRect);
        }

           //Event handler
        SDL_Event e;

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                setState(my_enums::S_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver = SDL_GetTicks();
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                //******
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                quit = true;
            }

            //******************
        }//events
    drawMouse();
    screenFlip();
    SDL_Delay(50);
    }//while not quit
}


void game::playTutorialHomeTown()

{
    int startTime = SDL_GetTicks();
    int elapsed = 0;

    bool quit = false;
    SDL_Rect destRect;
    while (!quit)
    {
        elapsed = SDL_GetTicks() - startTime;
        screenClear();
        drawBackground();

        destRect.x = gScreenSurface->w / 2 - 256;
        destRect.y = gScreenSurface->h / 8;
        destRect.w = 512;
        destRect.h = 50;
        drawSquare(destRect, lightGreyColor);
        drawTextResize("TUTORIAL PANTALLA PRINCIPAL", destRect);

        if (elapsed > 1000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h / 4;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 100;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("En la pantalla principal veras múltiples personajes", destRect);
        }

        if (elapsed > 3000) {

            drawButtonSrc(moveLeftButton, buttonLeftTexture);
            drawButtonSrc(moveRightButton, buttonRightTexture);
            drawButtonSrc(moveUpButton, buttonUpTexture);
            drawButtonSrc(moveDownButton, buttonDownTexture);


           
              

                destRect.w = 100;
                destRect.h = 100;
                destRect.y = gScreenSurface->h - 250;
                destRect.x = gScreenSurface->w -300;
                drawTileset(destRect, playersTexture, 74, 20);
                drawTransparentSquare(destRect, { 200,0,0,0 });
                drawTransparentSquare({ destRect.x + 1,destRect.y + 1,destRect.w - 2,destRect.h - 2, }, { 200,0,0,0 });
            

            destRect.x = gScreenSurface->w / 2;
            destRect.y = gScreenSurface->h - 150;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 150;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Los enemigos tiene un recuadro rojo, para atacarlos desplazate en su dirección con las teclas WASD, o bien pulsando sobre las flechas de dirección", destRect);

        }

        if (elapsed > 5000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h / 2-50;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 200;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Los personajes amigos que puedes interactuar tienen un recuadro verde, Para interactuar con los personajes debes desplazarte en su dirección con el teclado o bién pulsar con el ratón sobre ellos", destRect);

            destRect.w = 100;
            destRect.h = 100;
            destRect.y = gScreenSurface->h / 2 -150;
            destRect.x = gScreenSurface->w / 4;
            drawTileset(destRect, playersTexture, 74, 20);
            drawTransparentSquare(destRect, { 0,200,0,0 });
            drawTransparentSquare({ destRect.x + 1,destRect.y + 1,destRect.w - 2,destRect.h - 2, }, { 0,200,0,0 });

        }

        if (elapsed > 7000) {
            destRect.x = gScreenSurface->w / 4;
            destRect.y = gScreenSurface->h * 3 / 4 - 100;
            destRect.w = gScreenSurface->w / 2;
            destRect.h = 200;
            drawSquare(destRect, lightGreyColor);
            drawTextBlock("Habla con todos los personajes que veas, ellos tienen misiones y pistas para tí, en cada mapa hay al menos un personaje que te puede ayudar, recuerda que para ganar el juego tienes que conseguir todos los logros", destRect);

          }



        if (elapsed > 9000) {
            destRect.x = 50;
            destRect.y = gScreenSurface->h - 100;
            destRect.w = 512;
            destRect.h = 50;
            drawSquare(destRect, lightGreyColor);
            drawTextResize("Pulsa cualquier tecla para continuar...", destRect);
        }

        //Event handler
        SDL_Event e;

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                setState(my_enums::S_GAMEOVER_);
                Mix_PlayMusic(musicGameOver, -1);
                timerGameOver = SDL_GetTicks();
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                //******
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                quit = true;
            }

            //******************
        }//events
        drawMouse();
        screenFlip();
        SDL_Delay(50);
    }//while not quit
}


//Used to get auto-incermental ids
int game::getMonsterID()
{
    monsterID++;
    return monsterID;
}

//Count enemy NPCs on current map
int game::countMonsters()
{
    int mCounter = 0;
    for (list<NPC>::iterator it = NPCs.begin(); it != NPCs.end(); it++)
    {
        if ((it->map == currentState) && ((it->NPCAI == my_enums::_ENEMY_STATIC_) || (it->NPCAI == my_enums::_ENEMY_RANDOM_) || (it->NPCAI == my_enums::_ENEMY_FOLLOW_)  ))mCounter++;
    }
    return mCounter;
}

//if we have less than 50 monsters, add one
void game::monsterGenerator()
{
    if (countMonsters() < maxMonsters) {

        int tile = dice(300, 2);
        int roll = dice(4, 1);

        if (getStringState(currentState) == my_enums::S_ELEMENTAL_EARTH_WORLD_) {
            switch (roll) {
            case 1: tile = 1; break;
            case 2: tile = 103; break;
            case 3: tile = 254; break;
            case 4: tile = 53; break;
            }
        }

        if (getStringState(currentState) == my_enums::S_ELEMENTAL_WIND_WORLD_) {
            switch (roll) {
            case 1: tile = 120; break;
            case 2: tile = 39; break;
            case 3: tile = 316; break;
            case 4: tile = 328; break;
            }
        }

        if (getStringState(currentState) == my_enums::S_ELEMENTAL_WATER_WORLD_) {
            switch (roll) {
            case 1: tile = 80; break;
            case 2: tile = 81; break;
            case 3: tile = 82; break;
            case 4: tile = 150; break;
            }
        }

        if (getStringState(currentState) == my_enums::S_ELEMENTAL_FIRE_WORLD_) {
            switch (roll) {
            case 1: tile = 2; break;
            case 2: tile = 15; break;
            case 3: tile = 121; break;
            case 4: tile = 133; break;
            }
        }

        if (getStringState(currentState) == my_enums::S_FOREST_WORLD_) {
            switch (roll) {
            case 1: tile = 58; break;
            case 2: tile = 185; break;
            case 3: tile = 221; break;
            case 4: tile = 224; break;
            }
        }

        if (getStringState(currentState) == my_enums::S_NECRO_WORLD_) {
            switch (roll) {
            case 1: tile = 19; break;
            case 2: tile = 26; break;
            case 3: tile = 27; break;
            case 4: tile = 123; break;
            }
        }

        int mx = dice(245, 5);
        int my = dice(245, 5);
        bool mcollide = true;
        while(mcollide){
        
        mcollide = collide(mx, my,false);
        if (mcollide) {
            mx = dice(245, 5);
            my = dice(245, 5);
        }
        else {
            addNPC(getMonsterID(), mx, my, getStringState(currentState), "Monstruo", dice(10, 1)+ (level - 1), dice(10, 5) + ((level - 1) * 3), dice(10, 5)+(level - 1), dice(10, 5)+ (level - 1), dice(3, 1), my_enums::_ENEMY_FOLLOW_, tile, { 1,1,255,255 });
        }
        }

    }
}


//Update section of map we display on screen
void game::updateMap()
{



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
        if (getState() == my_enums::S_INTRO_) {
        SDL_RenderCopy(gRenderer, bg1Texture, NULL, NULL);
        }
        else {
            SDL_RenderCopy(gRenderer, bg2Texture, NULL, NULL);
        }
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


    SDL_FreeSurface(text);
    SDL_DestroyTexture(txtTexture);
    text = NULL;
    txtTexture = NULL;
}

void game::drawIMGBox(int x, int y, int value, int max, SDL_Color color)
{
    int tmp;
    SDL_Rect target;
    target.x = x;
    target.y = y;
    target.w = 20;
    target.h = 20;

   if (value > 0) drawSquare(target, color);

    while (value > 0) {
        drawSquare(target, color);
        target.x += 20;
        value--;
    }

    color = { 0,0,0,0 };
    target.x = x;
    tmp = max;
    while (tmp > 0) {
        drawTransparentSquare(target, color);
        target.x += 20;
        tmp--;
    }
}

void game::drawIMGBoxSmall(int x, int y, int w, int h, int value, int max, SDL_Color color)
{
    int tmp;
    SDL_Rect target;
    target.x = x;
    target.y = y;
    target.w = w / max;

    target.h = h;
    if (value > 0) drawSquare(target, color);
    
    while (value > 0) {
        drawSquare(target, color);
        target.x += w / max;
        value--;
    }

    color = { 0,0,0,0 };
    target.x = x;
    tmp = max;
    while (tmp > 0) {
        drawTransparentSquare(target, color);
        target.x += w/max;
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
    SDL_Color fg = { 0,0,0,0 };
    if (text.length() > 0) {
        // UTF8
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(gFont, text.c_str(), fg);
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
    SDL_Color fg = { 0,0,0,0 };
    if (text.length() > 0) {
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(gFont, text.c_str(), fg);
        rect.x = rect.x - rect.w;

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
    SDL_Color fg = { 0,0,0,0 };
    if (text.length() > 0) {
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(gFont, text.c_str(), fg, rect.w);

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


    if (firstRun) {
        firstRun = false;
        playTutorial();
    }

    SDL_Color square = { 0,0,0,0 };


    tmpRect.x = gScreenSurface->w / 2 - 250;
    tmpRect.y = gScreenSurface->h / 4;
    tmpRect.w = 500;
    tmpRect.h = 100;
    drawTextResize("Menu Principal", tmpRect);


    //tmpRect.x = 1;
    tmpRect.x = gScreenSurface->w / 2 - 350;
    tmpRect.y = gScreenSurface->h / 4 + 100;
    tmpRect.w = 700;
    tmpRect.h = 200;
    drawSquare(tmpRect, { 200,200,200 });
    drawTextBlock("¡Derrota enemigos, consigue tesoros, mejora tu personaje, cumple misiones, obtén todos los logros y gana el juego!", tmpRect);


    tmpRect.x = gScreenSurface->w - 500;
    tmpRect.y = gScreenSurface->h - 50;
    tmpRect.w = 500;
    tmpRect.h = 50;

    if (debugMode) {
        drawText("X:" + std::to_string(px) + " Y:" + std::to_string(py), tmpRect);
    }
    else {
        drawText("Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60), tmpRect);
    }
    

    drawButtonSrc(achievementsButton, buttonStarsTexture);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(startButton, buttonPlayTexture);

    drawButtonSrc(configButton, buttonConfigTexture);
}



void game::screenPlayerName()
{
    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    drawButtonSrc(nextButton, buttonNextTexture);
    drawButtonSrc(prevButton, buttonPrevTexture);

    tmpRect.x = gScreenSurface->w / 2 - gScreenSurface->w / (cam_size_x*2);
    tmpRect.y = gScreenSurface->h / 2;
    tmpRect.w = gScreenSurface->w / cam_size_x;
    tmpRect.h = gScreenSurface->h / cam_size_y;

    drawTileset(tmpRect, playersTexture, playerTile,20);

    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;

    drawTextResize("CAMBIAR NOMBRE E IMAGEN", tmpRect);
    tmpRect.y = 200;
    drawText("Introduce el nombre del jugador", tmpRect);

    tmpRect.y = 250;
    drawSquare(tmpRect, lightGreyColor);
    drawText(playerName, tmpRect);


    tmpRect.y = gScreenSurface->h / 2 - 200;
    tmpRect.h = 150;
    tmpRect.w = gScreenSurface->w / 2+200;
    tmpRect.x = gScreenSurface->w / 4-100;
    drawSquare(tmpRect, lightGreyColor);
    drawTextBlock("Cambia la imagen y luego pulsa continuar, ten en cuenta que la imagen es solo un elemento cosmético y no afecta a las habilidades del personaje...", tmpRect);
}


void game::screenPlayerAttributes()
{

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    drawButtonSrc(backButton, buttonBackTexture);

    drawButtonSrc(rollButton, buttonRollDiceTexture);

    //tmpRect.x = 100;
    tmpRect.x = gScreenSurface->w/2 -200;

    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawTextResize("Atributos del personaje", tmpRect);
    tmpRect.y = 150;
    drawText("Volver a tirar para nueva tirada o continuar", tmpRect);

    tmpRect.w = 100;
    tmpRect.y = 200;
    drawTextL("Combate(" + to_string(max_skill) + ")", tmpRect);
    tmpRect.y = 250;
    drawTextL("Vitalidad(" + to_string(max_stamina) + ")", tmpRect);
    tmpRect.y = 300;
    drawTextL("Poder(" + to_string(max_power) + ")", tmpRect);
    tmpRect.y = 350;
    drawTextL("Suerte(" + to_string(max_luck) + ")", tmpRect);

  drawIMGBox(gScreenSurface->w / 2 - 200, 200, skill, max_skill, { 0,0,200,0 });
  drawIMGBox(gScreenSurface->w / 2 - 200, 250, stamina, max_stamina, { 200,0,0,0 });
  drawIMGBox(gScreenSurface->w / 2 - 200, 300, power, max_power, { 128,0,128,0 });
  drawIMGBox(gScreenSurface->w / 2 - 200, 350, luck, max_luck, { 0,200,200,0 });


 
  tmpRect.x = gScreenSurface->w - 600;
  tmpRect.y = 150;
  tmpRect.h = 700;
  tmpRect.w = 450;
  //drawSquare(tmpRect, { 200,200,200 });

  //tmpRect.h = 50;
  tmpRect.h = 100;

  tmpRect.y = 150;
  
  tmpRect.x = 1;
  drawSquare(tmpRect, { 200,200,200 });
  drawTextBlock("COMBATE: Habilidad para luchar y defenderte", tmpRect);
  
 
  tmpRect.x = gScreenSurface->w - 450;
  drawSquare(tmpRect, { 200,200,200 });
  drawTextBlock("VITALIDAD: Puntos de vidad", tmpRect);
  tmpRect.y = 300;
  //tmpRect.y = 450;
  
  tmpRect.x = 1;
  drawSquare(tmpRect, { 200,200,200 });
  drawTextBlock("PODER: Habilidad que te permite usar la magia", tmpRect);
  //tmpRect.y = 500;
  tmpRect.x = gScreenSurface->w - 450;
  tmpRect.h = 300;
  drawSquare(tmpRect, { 200,200,200 });
  drawTextBlock("SUERTE: Habilidad que te permite hacer ataques furtivos y posibilidad de esquivar ataques usando armas a distancia", tmpRect);
}


void game::screenPlayer()
{
    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);

    int tmpy = 100;
    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 0;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawTextResize("Ficha del personaje: " +playerName, tmpRect);

    tmpRect.x = gScreenSurface->w/2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawTextResize(getRaceName(currentRace) + " / "+ getArchetypeName(currentArchetype), tmpRect);
 
    tmpRect.w = 100;
    tmpRect.y = 200;
    drawTextL("Combate(" + to_string(max_skill) + ")", tmpRect);
    tmpRect.y = 250;
    drawTextL("Vitalidad(" + to_string(max_stamina) + ")", tmpRect);
    tmpRect.y = 300;
    drawTextL("Poder(" + to_string(max_power) + ")", tmpRect);
    tmpRect.y = 350;
    drawTextL("Suerte(" + to_string(max_luck) + ")", tmpRect);


    drawIMGBox(gScreenSurface->w / 2 - 200, 200, skill, max_skill, { 0,0,200,0 });
    drawIMGBox(gScreenSurface->w / 2 - 200, 250, stamina, max_stamina, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2 - 200, 300, power, max_power, { 128,0,128,0 });
    drawIMGBox(gScreenSurface->w / 2 - 200, 350, luck, max_luck, { 0,200,200,0 });


    tmpRect.y = 500;
    drawText("Nivel:" + to_string(level), tmpRect);
    tmpRect.y = 550;
    tmpRect.w = 200;
    drawText("Experiencia:" + to_string(exp) + "/" + to_string((level*level)*100), tmpRect);
    tmpRect.y = 600;
    drawText("Puntos de habilidad: " + to_string(skillPoints) , tmpRect);


    SDL_Rect target;
    target.x = 1;
    target.y = 200;
    target.w = 500;
    target.h = 300;
    drawSquare(target, { 200,200,200 });

    tmpRect.y = 200;
    tmpRect.x = 1;
    drawTextResize("HABILIDADES: ", tmpRect);
    tmpy = 200;
    for (std::string skill : skills)
    {
        tmpy += 50;
        tmpRect.y = tmpy;
        drawText(skill, tmpRect);

    }


}

void game::screenShops()
{

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);

    std::string tmpStr;



   

    SDL_Rect target;
    tmpRect.x = gScreenSurface->w /2 -200;
    tmpRect.y = 50;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawTextResize(tmpNPCs.begin()->description, tmpRect);

    string tmpString = "";
    tmpString = "Monedas de oro: " + to_string(coins);
    tmpy += 50;
    tmpRect.y = gScreenSurface->h / 4;
    drawText(tmpString, tmpRect);


    
    target.x = tmpRect.x - 50;
    target.y = tmpRect.y - 25;
    target.w = tmpRect.w + 40;
    target.h = tmpRect.h + 25;
    tmpRect.y = 100;
    target.x = tmpRect.x - 25;
    target.y = tmpRect.y - 25;
    target.w = tmpRect.w + 100;
    target.h = tmpRect.h + 50;

    int itC = 0;
    for (list<SHOP>::iterator it = tmpSHOPs.begin() ; it != tmpSHOPs.end(); it++) {

        if (it->value > 0) { tmpStr = "comprar " + to_string(it->value); }
        else {
            tmpStr = "vender " + to_string(it->value);
        }
        tmpStr = tmpStr + " unidades de ";
        if (it->description == "FOOD") {
            tmpStr = tmpStr + " raciones de comida";
        }
        else {

            if (it->description.substr(0, 2) == "**") {
                tmpStr = tmpStr + it->description.substr(2, it->description.length() - 2);
            }
            else {
                tmpStr = tmpStr + it->description;
            }

        }
        tmpStr = tmpStr + " por " + to_string(it->value2);
        if (it->description2 == "GOLD") { tmpStr = tmpStr + " monedas de oro"; }
        if (it->description2 == "FOOD") { tmpStr = tmpStr + " raciones de comida"; }
        // cout << endl;

        switch (itC) {
        case 0:
            shop0.setCaption(tmpStr);
            shop0.setColor(100, 100, 100);
            drawButton(shop0);
            drawTileset({ 0, gScreenSurface->h / 2,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 1:
            shop1.setCaption(tmpStr);
            shop1.setColor(100, 100, 100);
            drawButton(shop1);
            drawTileset({ 0, gScreenSurface->h / 2 +50 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 2:
            shop2.setCaption(tmpStr);
            shop2.setColor(100, 100, 100);
            drawButton(shop2);
            drawTileset({ 0, gScreenSurface->h / 2 + 100 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 3:
            shop3.setCaption(tmpStr);
            shop3.setColor(100, 100, 100);
            drawButton(shop3);
            drawTileset({ 0, gScreenSurface->h / 2 + 150 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 4:
            shop4.setCaption(tmpStr);
            shop4.setColor(100, 100, 100);
            drawButton(shop4);
            drawTileset({ 0, gScreenSurface->h / 2 + 200 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 5:
            shop5.setCaption(tmpStr);
            shop5.setColor(100, 100, 100);
            drawButton(shop5);
            drawTileset({ 0, gScreenSurface->h / 2 + 250 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        }
        itC++;
    }
}

void game::screenChat()
{

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);

    std::string tmpStr;

    SDL_Rect target;
    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 50;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawTextResize(tmpNPCs.begin()->description, tmpRect);


    target.x = tmpRect.x - 50;
    target.y = tmpRect.y - 25;
    target.w = tmpRect.w + 40;
    target.h = tmpRect.h + 25;
    tmpRect.y = 100;
    target.x = tmpRect.x - 25;
    target.y = tmpRect.y - 25;
    target.w = tmpRect.w + 100;
    target.h = tmpRect.h + 50;

    int itC = 0;
    for (list<CHAT>::iterator it = tmpCHATs.begin(); it != tmpCHATs.end(); it++) {

        tmpStr = it->question;

        switch (itC) {
        case 0:
            chat0.setCaption(tmpStr);
            chat0.setColor( 100, 100, 100);
            drawButton(chat0);
         
            break;
        case 1:
            chat1.setCaption(tmpStr);
            chat1.setColor(100, 100, 100);
            drawButton(chat1);
         
            break;
        case 2:
            chat2.setCaption(tmpStr);
            chat2.setColor(100, 100, 100);
            drawButton(chat2);
         
            break;
        case 3:
            chat3.setCaption(tmpStr);
            chat3.setColor(100, 100, 100);
            drawButton(chat3);
         
            break;
        case 4:
            chat4.setCaption(tmpStr);
            chat4.setColor(100, 100, 100);
            drawButton(chat4);
         
            break;
        case 5:
            chat5.setCaption(tmpStr);
            chat5.setColor(100, 100, 100);
            drawButton(chat5);
         
            break;
        }
        itC++;
    }

    target.x = 50;
    target.y = gScreenSurface->h / 8;
    target.w = gScreenSurface->w - 100;
    target.h = 200;
    drawSquare(target, { 200,200,200 });
    drawTextBlock(currentChat, target);
}




void game::screenClear()
{
    SDL_SetRenderDrawColor(gRenderer, 0x99, 0x99, 0x99, 0x99);
    SDL_RenderClear(gRenderer);
}

void game::screenIntro()
{
    drawButtonSrc(exitButton,buttonCloseTexture);
    drawButtonSrc(continueButton,buttonPlayTexture);
    tmpRect.y = gScreenSurface->h / 4;
    tmpRect.h = 100;

    tmpRect.x = gScreenSurface->w / 2 - 450;
    tmpRect.w = 900;
    fg = { 128, 0, 128, 0 };
    drawTextResize("Achievement master", tmpRect);
    fg = { 0, 0, 0, 0 };

    tmpRect.x = gScreenSurface->w / 2 - (248 - eyesize)/2;
    tmpRect.y = gScreenSurface->h / 2 - (248 - eyesize) / 2;
    tmpRect.w = 248 - eyesize;
    tmpRect.h = 248 - eyesize;

    for(int x = 0; x < (gScreenSurface->w / 100) + 2; x++)
    {
        for(int y = 0; y < (gScreenSurface->h / 100) + 2; y++)
        {
            drawTileset(tmpRect, playersTexture, introTile, 20);
        }
    }
    if (growing)
    {
        eyesize++;
    }
    else
    {
        eyesize--;
    }

    if (eyesize > 200) growing = false;
    if (eyesize < 48) growing = true;
    introCounter++;
    if (introCounter == 15) {
        introCounter = 0;
        introTile++;
        if (introTile > 300)introTile = 1;
    }
}


void game::drawAchievements()
{
    int currentTime = SDL_GetTicks();
    int aTime = 0;
    SDL_Rect target;

    if (achievements.size() > 0) {

        for (list<achievement>::iterator it = achievements.begin(); it != achievements.end(); it++)
        {
            aTime = (currentTime - it->achieved);
            if(it->type == my_enums::_COMBAT_)fg = { 200,0,0 };
            if (it->type == my_enums::_HIDDEN_)fg = { 200,0,200 };
            if (it->type == my_enums::_MISSIONS_)fg = { 0,200,0 };
            if (it->type == my_enums::_OPTIONS_)fg = { 0,0,200 };


            if (aTime < 5000) {
                target.x = gScreenSurface->w/2 -250  - aTime / 20;
                target.y = gScreenSurface->h  - 100 - aTime / 12;
                target.w = 500 + aTime/10;
                target.h = 100 + aTime/25;


                drawTextResize(it->name, target);
            }
        }
    }
    fg = { 0,0,0 };
}

void game::screenNotifications()
{
    tmpRect.h = 30;
    tmpRect.x = 1;

    tmpRect.w = 500;
    int tmpY = 0;
    if (notifications.size() > 0) {

        int base_y = gScreenSurface->h - (notifications.size() * 30 + 30);
        if (base_y < gScreenSurface->h /2) { base_y = gScreenSurface->h  /2; }
        for (std::string notification : notifications) {
            tmpRect.y = base_y + (tmpY * 30);
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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        //Special key input
        else if (e.type == SDL_MOUSEMOTION)
        {
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    Mix_PlayChannel(-1, audioButton, 0);
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timer = SDL_GetTicks();
                }
            }
            if (continueButton.clicked(mousex, mousey)) {
                setState(my_enums::S_MAINMENU_);
                Mix_PlayChannel(-1, audioButton, 0);
                timer = SDL_GetTicks();
            }

            if (titleButton.clicked(mousex, mousey)) {
                
                    addAchievement("El nombre del juego", my_enums::_HIDDEN_);
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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
            }

            if (configButton.clicked(mousex, mousey)) {
                setState(my_enums::S_CONFIGMENU_);
                previousScreen = my_enums::S_MAINMENU_;

            }

            if (achievementsButton.clicked(mousex, mousey)) {
                setState(my_enums::S_ACHIEVEMENTS_);
                previousScreen = my_enums::S_MAINMENU_;

            }


            if (startButton.clicked(mousex, mousey)) {
                addAchievement("Comenzando el juego", my_enums::_HIDDEN_);
                SDL_StartTextInput();
                setState(my_enums::S_NAME_);
            }



        }
  

        //******************
    }

}

void game::eventsGameOver()
{
    if ( SDL_GetTicks()- timerGameOver > 3000) {
        setState(my_enums::S_GAMECLOSE_);
    }

    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
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
                if (confirmExit() == true) {
                    Mix_PlayChannel(-1, audioButton, 0);
                    //closeSDL();
                    SDL_StopTextInput();
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
            }
            if (continueButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                SDL_StopTextInput();

                setState(my_enums::S_ATTRIBUTES_);

            }


            if (nextButton.clicked(mousex, mousey)) {
                playerTile++;
                if (playerTile > 96) { playerTile = 96; }
            }

            if (prevButton.clicked(mousex, mousey)) {
                playerTile--;
                if (playerTile < 63) { playerTile = 63; }

            }

            
        }

    }
}

void game::screenConfigMenu()
{
    tmpRect.y = gScreenSurface->h / 4;
    tmpRect.h = 100;

    tmpRect.x = gScreenSurface->w / 2 - 450;
    tmpRect.w = 900;
    fg = { 128, 0, 128, 0 };
    drawTextResize("Achievement master", tmpRect);
    fg = { 0, 0, 0, 0 };


    tmpRect.x = gScreenSurface->w / 2 - 250;
    tmpRect.y = gScreenSurface->h / 8 - 50;
    tmpRect.w = 500;
    tmpRect.h = 100;
    drawTextResize("Menu Configuración", tmpRect);
    drawButtonSrc(exitButton, buttonCloseTexture);   

    drawButtonSrc(volumeSoundUpButton, buttonNextTexture);
    drawButtonSrc(volumeSoundDownButton, buttonPrevTexture);
    drawButton(volumeSoundButton);

    drawButtonSrc(volumeMusicUpButton, buttonNextTexture);
    drawButtonSrc(volumeMusicDownButton, buttonPrevTexture);
    drawButton(volumeMusicButton);
    

    if (debugMode) {
        drawButton(musicButton);
    }

    if (musicON == false) {
        drawButtonSrc(muteButton, buttonSoundOffTexture);
    }
    else {
        drawButtonSrc(muteButton, buttonSoundOnTexture);
    }

    drawButton(blurButton);
    drawButton(tuto1Button);
    drawButton(tuto2Button);
    drawButtonSrc(startButton, buttonAcceptTexture);
}

void game::screenHomeTown()
{

    if (firstRunHomeTown) {
        firstRunHomeTown = false;
        playTutorialHomeTown();
    }

    SDL_Rect target;
     target.x = 0;
    target.y = 0;

    target.w = gScreenSurface->w;
    target.h = gScreenSurface->h;

    drawMap();
    drawEvents();
    drawPlayer();
    drawNPCs();


    tmpRect.x = gScreenSurface->w / 2 - 400;
    tmpRect.y = gScreenSurface->h / 12;
    
    tmpRect.w = 800;
    tmpRect.h = 100;

    string townName = "";
    switch (getState())
    {

    case my_enums::S_NECRO_WORLD_:
    {
        townName = "NECROMUNDO";
        break;
    }

    case my_enums::S_FOREST_WORLD_:
    {
        townName = "EL BOSQUE OSCURO";
        break;
    }

    case my_enums::S_ELEMENTAL_WATER_WORLD_:
    {
        townName = "MUNDO ELEMENTAL DE AGUA";
        break;
    }

    case my_enums::S_ELEMENTAL_FIRE_WORLD_:
    {
        townName = "MUNDO ELEMENTAL DE FUEGO";
        break;
    }


    case my_enums::S_COAST_WORLD_:
    {
        townName = "ISLAS";
        break;
    }

    case my_enums::S_HOMETOWN_:
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

    //drawText("Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60), tmpRect);
    if (debugMode) {
        drawText("X:" + std::to_string(px) + " Y:" + std::to_string(py), tmpRect);
    }
    else {
        drawText("Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60), tmpRect);
    }


    drawButtonSrc(moveLeftButton, buttonLeftTexture);
    drawButtonSrc(moveRightButton, buttonRightTexture);
    drawButtonSrc(moveUpButton, buttonUpTexture);
    drawButtonSrc(moveDownButton, buttonDownTexture);
    
    drawButtonSrc(achievementsButton, buttonStarsTexture);

    drawButtonSrc(playerButton, buttonPlayerTexture);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(configButton, buttonConfigTexture);
    if(debugMode)drawButtonSrc(newMapButton, buttonMapTexture);
    drawButtonSrc(mapButton, buttonMapTexture);
    drawButtonSrc(inventoryButton, buttonBackpackTexture);


    if (food > 0) {
        drawButtonSrc(foodButton, buttonFoodTexture);
    }

}

std::string game::getRaceDescription(my_enums::playerRaces race)
{

    switch (race)
    {
    case my_enums::_HUMAN_:
    {
        return "HUMANO: +1 a Combate";
        break;
    }
    case my_enums::_ELF_:
    {
        return "ELFO: +1 a Poder";
        break;
    }

    case my_enums::_DARF_:
    {
        return "ENANO: +2 a Vitalidad";
        break;
    }

    case my_enums::_HALFING_:
    {
        return "MEDIANO: +2 a suerte";
        break;
    }

    case my_enums::_HALFORC_:
    {
        return "MEDIO ORCO: +2 a Combate, -1 a Poder";
        break;
    }

    case my_enums::_BEHOLDER_:
    {
        return "CONTEMPLADOR: +2 a poder , -1 a combate";
        break;
    }

    default:
    {
        // is likely to be an error
        return "";
    }
    };


}


std::string game::getArchetypeDescription(my_enums::playerArchetype archetype)
{

    switch (archetype)
    {
    case my_enums::_FIGHTER_:
    {
        return "GUERRERO: Comienzas con la habilidad de MELE (+1 a Combate)";
        break;
    }
    case my_enums::_ARCHER_:
    {
        return "ARQUERO:  Comienzas con la habilidad de ARQUERO (Ataques a distancia)";
        break;
    }

    case my_enums::_THIEVE_:
    {
        return "LADRON:  Comienzas con la habilidad de SUBTERFUGIO (Ataques con subterfugio)";
        break;
    }

    case my_enums::_MAGE_:
    {
        return "MAGO: Comienzas con la habilidad de MAGIA (Ataques con magia)";
        break;
    }

    case my_enums::_NECROMANCER_:
    {
        return "NECROMANTE:  Comienzas con la habilidad de DRENAR  (Ataques con magia que drenan vida)";
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

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    drawButtonSrc(backButton, buttonBackTexture);
    drawButtonSrc(nextButton, buttonNextTexture);
    drawButtonSrc(prevButton, buttonPrevTexture);


    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;

    drawTextResize("RAZA DEL JUGADOR", tmpRect);
    tmpRect.y = 200;
    drawText("Selecciona la raza del personaje", tmpRect);

    tmpRect.y = gScreenSurface->h / 4 * 3 - 50;
    drawText("Luego pulsa continuar...", tmpRect);

    tmpRect.y = gScreenSurface->h / 2 - 100;
    tmpRect.x = gScreenSurface->w / 2 - 300;
    tmpRect.w = 600;
    drawSquare(tmpRect, { 200,200,200 });
    drawTextBlock(getRaceDescription(currentRace), tmpRect);


    /*
    tmpRect.y = gScreenSurface->h / 2 +64;
    tmpRect.x = gScreenSurface->w / 2 -100;
    drawText(getRaceName(currentRace), tmpRect);

    
    tmpRect.x = gScreenSurface->w - 600;
    tmpRect.y = 150;
    tmpRect.h = 700;
    tmpRect.w = 600;
    drawSquare(tmpRect, { 200,200,200 });
    
    tmpRect.h = 50;

    tmpRect.y = 150;
    drawTextBlock("HUMANO: +1 a Combate", tmpRect);
    tmpRect.y = 250;
    drawTextBlock("ELFO: +1 a Poder", tmpRect);
    tmpRect.y = 350;
    drawTextBlock("ENANO: +2 a Vitalidad", tmpRect);
    tmpRect.y = 450;
    drawTextBlock("MEDIANO: +2 a suerte ", tmpRect);
    tmpRect.y = 550;
    drawTextBlock("MEDIO ORCO: +2 a Combate, -1 a Poder ", tmpRect);
    tmpRect.y = 650;
    drawTextBlock("CONTEMPLADOR: +2 a poder , -1 a combate", tmpRect);
    */
}

void game::screenPlayerArchetypes()
{

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonPlayTexture);
    drawButtonSrc(backButton, buttonBackTexture);
    drawButtonSrc(nextButton, buttonNextTexture);
    drawButtonSrc(prevButton, buttonPrevTexture);


    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;

    drawTextResize("ARQUETIPO DEL JUGADOR", tmpRect);
    tmpRect.y = 200;
    drawText("Selecciona el arquetipo del personaje", tmpRect);

    tmpRect.y = gScreenSurface->h / 4 * 3 - 50;
    drawText("Luego pulsa continuar...", tmpRect);

    tmpRect.y = gScreenSurface->h / 2 - 150;
    tmpRect.x = gScreenSurface->w / 2 - 400;
    tmpRect.w = 800;
    tmpRect.h = 100;

    drawSquare(tmpRect, { 200,200,200 });
    drawTextBlock(getArchetypeDescription(currentArchetype), tmpRect);

    
    /*
    tmpRect.y = gScreenSurface->h / 2 + 64;
    tmpRect.x = gScreenSurface->w / 2 - 100;
    drawText(getArchetypeName(currentArchetype), tmpRect);

    tmpRect.x = gScreenSurface->w - 600;
    tmpRect.y = 150;
    tmpRect.h = 850;
    tmpRect.w = 600;
    drawSquare(tmpRect, { 200,200,200 });

    tmpRect.h = 50;

    tmpRect.y = 150;
    drawTextBlock("GUERRERO: Comienzas con la habilidad de MELE (+1 a Combate)", tmpRect);
    tmpRect.y = 300;
    drawTextBlock("ARQUERO:  Comienzas con la habilidad de ARQUERO (Ataques a distancia)", tmpRect);
    tmpRect.y = 450;
    drawTextBlock("LADRON:  Comienzas con la habilidad de SUBTERFUGIO (Ataques con subterfugio)", tmpRect);
    tmpRect.y = 600;
    drawTextBlock("MAGO: Comienzas con la habilidad de MAGIA (Ataques con magia)", tmpRect);
    tmpRect.y = 750;
    drawTextBlock("NECROMANTE:  Comienzas con la habilidad de DRENAR  (Ataques con magia que drenan vida)", tmpRect);

    */
}


void game::screenInventory()
{
        drawButtonSrc(exitButton, buttonCloseTexture);
        drawButtonSrc(continueButton, buttonAcceptTexture);

        int tmpy = 100;
        tmpRect.x = gScreenSurface->w / 2 - 200;
        tmpRect.y = 0;
        tmpRect.w = 400;
        tmpRect.h = 50;

        drawTextResize("Inventario", tmpRect);

        string tmpString = "";
        tmpString = "Monedas de oro: " + to_string(coins);
        tmpy += 50;
        tmpRect.y = tmpy;
        drawText(tmpString, tmpRect);


        tmpString = "Raciones de comida: " + to_string(food);
        tmpy += 50;
        tmpRect.y = tmpy;
        drawText(tmpString, tmpRect);
        drawTileset({ gScreenSurface->w / 2 - 250,tmpy,50,50 }, itemsTexture, 207, 20);

        tmpString = "Pociones de vitalidad: " + to_string(potions_health);
        tmpy += 50;
        tmpRect.y = tmpy;
        drawText(tmpString, tmpRect);
        drawTileset({ gScreenSurface->w / 2 - 250,tmpy,50,50 }, itemsTexture, 276, 20);

        tmpString = "Pociones de poder: " + to_string(potions_power);
        tmpy += 50;
        tmpRect.y = tmpy;
        drawText(tmpString, tmpRect);
        drawTileset({ gScreenSurface->w / 2 - 250,tmpy,50,50 }, itemsTexture, 273, 20);  
        

        int itemCounter = 0;
        int x_offset = 0;
        for (list<item>::iterator it = items.begin(); it != items.end(); it++)
        {
            
            itemCounter++;
            if (itemCounter == 11) {
                tmpy -= 500;
                x_offset = 450;
            }
            tmpy += 50;
            tmpRect.y = tmpy;

            tmpRect.x = gScreenSurface->w / 2 - 250 + x_offset;
            tmpRect.w = 50;
            drawTileset(tmpRect, itemsTexture, it->tile,20);

            if (it->name.substr(0, 2) == "**") {
                tmpString = it->name.substr(2, it->name.length() - 2) + " (" + to_string(it->count) + ") valor:" + to_string(it->value);
            }
            else {
                tmpString = it->name + " (" + to_string(it->count) + ") valor:" + to_string(it->value);
            }
            tmpRect.w = 400;
            tmpRect.x = gScreenSurface->w / 2 - 200 + x_offset;
            drawText(tmpString, tmpRect);

        }

        SDL_Rect target;
        target.x = 1;
        target.y = 300;
        target.w = 500;
        target.h = 300;
        drawSquare(target, { 200,200,200 });

        tmpy = 300;
        tmpRect.x = 51;
        tmpRect.y = tmpy;
        drawTextResize("ITEMS EQUIPADOS", tmpRect);

        tmpRect.x = 1;
        tmpRect.w = 150;
        tmpRect.y += 50;
        drawTextResize("ARMA:", tmpRect);
        tmpRect.y += 50;
        drawTextResize("ARMADURA:", tmpRect);
        tmpRect.y += 50;
        drawTextResize("CASCO:", tmpRect);
        tmpRect.y += 50;
        drawTextResize("ESCUDO:", tmpRect);
        tmpRect.y += 50;
        drawTextResize("ANILLO:", tmpRect);
        
        tmpy = 300;
        tmpRect.x = 151;
        tmpRect.y = tmpy;
        tmpRect.w = 200;
        tmpRect.y += 50;
       if (alternate3 !="")drawTextResize(alternate3, tmpRect);

        tmpRect.y += 50;
        if (alternate2 != "") drawTextResize(alternate2, tmpRect);

        tmpRect.y += 50;
        if (itemHelmet != "")drawTextResize(itemHelmet, tmpRect);

        tmpRect.y += 50;
        if (alternate != "")drawTextResize(alternate, tmpRect);

        tmpRect.y += 50;
        if (itemRing != "")drawTextResize(itemRing, tmpRect);


        target.y = 650;
        drawSquare(target, { 200,200,200 });
        drawTextBlock(itemDescription, target);
}


void game::screenHero()
{
    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    drawButtonSrc(achievementsButton, buttonStarsTexture);

    tmpRect.y = gScreenSurface->h / 4;
    tmpRect.h = 100;

    tmpRect.x = gScreenSurface->w / 2 - 450;
    tmpRect.w = 900;
    drawTextResize("Heroe inmortal", tmpRect);

    tmpRect.y = gScreenSurface->h / 2;
    drawTextBlock("Has sido derrotado, pero cuando piensas que ya todo está perdido una voz en tu interior te llama y sientes que nuevas energías recorren tu cuerpo,  ¿Te rindes o vas a seguir luchando?", tmpRect);
}


void game::addSkill(std::string skill)
{
    if(!hasSkill(skill)){
    skills.push_back(skill);
    addNotification("Has aprendido la habilidad: "+skill, { 0,0,0 });
    }
}

bool game::hasSkill(std::string skill)
{
    bool returnValue = false;

    for (std::string askill : skills) {
        if (askill == skill){
            returnValue = true;
            break;
        }
    }

    return returnValue;
}


void game::screenAchievements()
{ 
    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    int combat = 0, missions = 0, options = 0, hidden = 0;
    int aCounter = 0;
    SDL_Rect square;

    square.x = gScreenSurface->w / 2;
    square.y = 450;
    square.w = 200;
    square.h = 50;

    for (achievement currentAchievement : achievements) {

        switch (currentAchievement.type)
        {
        case my_enums::_COMBAT_:
        {
            combat++;
            if (achievementGroup == 0) {
                square.y = 450+ aCounter*50;
                drawText(currentAchievement.name, square);
                aCounter++;
            }
            break;
        }
        case my_enums::_MISSIONS_:
        {
            missions++;
            if (achievementGroup == 1) {
                square.y = 450 + aCounter * 50;
                drawText(currentAchievement.name, square);
                aCounter++;
            }
            break;
        }


        case my_enums::_OPTIONS_:
        {
            options++;
            if (achievementGroup == 2) {
                square.y = 450 + aCounter * 50;
                drawText(currentAchievement.name, square);
                aCounter++;
            }
            break;
        }

        case my_enums::_HIDDEN_:
        {
            hidden++;
            if (achievementGroup == 3) {
                square.y = 450 + aCounter * 50;
                drawText(currentAchievement.name, square);
                aCounter++;
            }
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


    tmpRect.x = gScreenSurface->w / 2 - 250;
    tmpRect.y = 100;

    tmpRect.w = 100;
    tmpRect.y = 200;
    drawTextL("Combate", tmpRect);
    tmpRect.y = 250;
    drawTextL("Missiones", tmpRect);
    tmpRect.y = 300;
    drawTextL("Opciones", tmpRect);
    tmpRect.y = 350;
    drawTextL("Ocultos", tmpRect);

    drawIMGBox(gScreenSurface->w / 2 - 250, 200, combat, 10, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2 - 250, 250, missions, 10, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2 - 250, 300, options, 10, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2 - 250, 350, hidden, 10, { 200,0,0,0 });


    square.x = gScreenSurface->w / 2;
    square.y =  200;
    square.w = 200;
    square.h = 200;
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 200, 0);
    SDL_RenderDrawRect(gRenderer, &square);
    drawTextResize(achievementsButton.getCaption(), square);


    SDL_SetRenderDrawColor(gRenderer, 200, 0, 0, 0);
    square.x = gScreenSurface->w / 2 - 400;
    square.y = 200+achievementGroup*50;
    square.h = 50;
    square.w = 400;
   
    SDL_RenderDrawRect(gRenderer, &square);

}



void game::screenFight()
{

    drawButtonSrc(exitButton, buttonCloseTexture);


    if ((SDL_GetTicks() - lastTurn) > 1000) {
    //All players can use normal attacks
    drawButtonSrc(fightButton, buttonSwordTexture);

    if (hasSkill("ARQUERO"))drawButtonSrc(bowButton, buttonBowTexture);
    if (hasSkill("SUBTERFUGIO"))drawButtonSrc(hideButton, buttonHideTexture);

    if (power > 0) {
        if (hasSkill("MAGIA"))drawButtonSrc(spellButton, buttonSpellTexture);
        if (hasSkill("DRENAR"))drawButtonSrc(drainButton, buttonDrainTexture);
        if (hasSkill("INVOCAR"))drawButtonSrc(summonButton, buttonSummonTexture);
    }

    if (potions_health > 0) {
        drawButtonSrc(potionHealthButton, buttonPotionHealthTexture);
    }

    if (potions_power > 0) {
        drawButtonSrc(potionMagicButton, buttonPotionMagicTexture);
    }

}

    if ((SDL_GetTicks() - lastTurn) > 1000) {
        fightPlayer = "Preparado...";
        fightFoe = "";
    }

    SDL_Rect target;
    target.x = gScreenSurface->w / 2 +100;
    target.y = gScreenSurface->h / 2 +100;
    target.w = gScreenSurface->w/2 -100;
    target.h = 200;
    drawSquare(target, { 200,200,200 });
    drawTextBlock(fightPlayer, target);

    target.x = 1;
    target.y = gScreenSurface->h / 2 +100;
    target.w = gScreenSurface->w / 2 -100;
    target.h = 200;
    drawSquare(target, { 200,200,200 });
    drawTextBlock(fightFoe, target);

    target.x = gScreenSurface->w / 2 - 200;
    target.y = 0;
    target.w = 400;
    target.h = 50;
   
    drawTextResize("¡¡COMBATE!!", target);


    int tmp = 0;

  
    tmpRect.x = 100;
    SDL_Rect square;
    square.x = 1;
    square.w = 500;
    square.h = 100;

    for (list<NPC>::iterator it = tmpNPCs.begin(); it != tmpNPCs.end(); it++)
    {
        tmp++;
        
        tmpRect.w = 100;
        tmpRect.h = 100;
        tmpRect.x = 1;

        tmpRect.y = tmp * 100;
        drawTileset(tmpRect, playersTexture, it->tile,20);

        tmpRect.x = 101;
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
        tmpRect.y = 75 + tmp * 100;
        drawText("Poder", tmpRect);

        drawIMGBox(200, 25 + tmp * 100, it->skill, it->skill, { 0,0,200,0 });
        drawIMGBox(200, 50 + tmp * 100, it->stamina, it->stamina, { 200,0,0,0 });
        drawIMGBox(200, 75 + tmp * 100, it->power, it->power, { 128,0,128,0 });

        square.y = tmp * 100;
        SDL_SetRenderDrawColor(gRenderer, 200, 0, 0, 0);
        SDL_RenderDrawRect(gRenderer, &square);

    }

    tmpRect.w = 100;
    tmpRect.h = 50;
    tmpRect.x = 100;
    tmpRect.x = gScreenSurface->w / 2;
    
    tmpRect.y = gScreenSurface->h / 2 - 250;
    drawText(playerName, tmpRect);
   
    
    tmpRect.y = gScreenSurface->h / 2 - 225;
    drawTextL("Habilidad", tmpRect);
    drawIMGBox(gScreenSurface->w / 2, gScreenSurface->h / 2 - 225, skill, max_skill, { 0,0,200,0 });

    tmpRect.y = gScreenSurface->h/2 - 200;
    drawTextL("Vitalidad:", tmpRect);
    drawIMGBox(gScreenSurface->w / 2, gScreenSurface->h / 2 - 200, stamina, max_stamina, { 200,0,0,0 });
    

    tmpRect.y = gScreenSurface->h/2 - 175;
    drawTextL("Poder:", tmpRect);
    drawIMGBox(gScreenSurface->w / 2, gScreenSurface->h / 2 - 175, power, max_power, { 128,0,128,0 });


    tmpRect.y = gScreenSurface->h / 2 - 250; 
    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.h = 100;
    drawTileset(tmpRect, playersTexture, playerTile, 20);

    square.x = gScreenSurface->w / 2 - 200;
    square.y = gScreenSurface->h / 2 - 250;
    square.w = 600;
    SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 0);
    SDL_RenderDrawRect(gRenderer, &square);

    tmpRect.y = gScreenSurface->h / 2 - 50;
    //melee
    

    tmpRect.x = gScreenSurface->w / 2;
    tmpRect.y = gScreenSurface->h / 2 + 200;
    drawTextL("Turno:", tmpRect);
    drawText(to_string(turn), tmpRect);

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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    Mix_PlayChannel(-1, audioButton, 0);
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
                
            }
            if (continueButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                setState(my_enums::S_ARCHETYPES_);

            }

            if (backButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                setState(my_enums::S_ATTRIBUTES_);

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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    Mix_PlayChannel(-1, audioButton, 0);
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }

            }
            if (continueButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                Mix_PlayMusic(musicTOWN, -1);
                setState(my_enums::S_HOMETOWN_);

                //Add skill depending on archetype
                switch (currentArchetype)
                {
                case my_enums::_FIGHTER_:
                {
                    addSkill("MELEE");
                    break;
                }
                case my_enums::_ARCHER_:
                {
                    addSkill("ARQUERO");
                    break;
                }

                case my_enums::_THIEVE_:
                {
                    addSkill("SUBTERFUGIO");
                    break;
                }

                case my_enums::_MAGE_:
                {
                    addSkill("MAGIA");
                    break;
                }

                case my_enums::_NECROMANCER_:
                {
                    addSkill("DRENAR");
                    break;
                }

                case my_enums::_SUMMONER_:
                {
                    addSkill("INVOCAR");
                    break;
                }
                default:
                {
                    // is likely to be an error
                }
                };

                //Update attributes depending on the race
                switch (currentRace)
                {
                case my_enums::_HUMAN_:
                {
                    max_skill += 1;
                    skill = max_skill;
                    break;
                }
                case my_enums::_ELF_:
                {
                    max_power += 1;
                    power = max_power;
                    break;
                }

                case my_enums::_DARF_:
                {
                    max_stamina += 2;
                    stamina = max_stamina;
                    break;
                }

                case my_enums::_HALFING_:
                {
                    max_luck += 2;
                    luck = max_luck;
                    break;
                }

                case my_enums::_HALFORC_:
                {
                    max_skill += 2;
                    skill = max_skill;
                    max_power -= 1;
                    power = max_power;
                }

                case my_enums::_BEHOLDER_:
                {
                    max_power += 2;
                    power = max_power;
                    max_skill -= 1;
                    skill = max_skill;
                    break;
                }

                default:
                {
                    // is likely to be an error
                }
                };

                if (debugMode) {
                    addSkill("MELEE");
                
                    addSkill("ARQUERO");
                
                    addSkill("SUBTERFUGIO");
              
                    addSkill("MAGIA");
               
                    addSkill("DRENAR");
                
                    //addSkill("INVOCAR");

                    skillPoints = 30;
                    coins = 5000;

                    
                    addItem("ANILLO VUELO", "Este anillo abre la puerta a un mundo elemental", 1, 0, 226, my_enums::_OTHER_, 0);
                    addItem("ANILLO RESISTENCIA FUEGO", "Este anillo abre la puerta a un mundo elemental", 1, 0, 229, my_enums::_OTHER_, 0);
                    addItem("ANILLO RESPIRAR AGUA", "Este anillo abre la puerta a un mundo elemental", 1, 0, 230, my_enums::_OTHER_, 0);
                    addItem("ANILLO DE LA TIERRA", "Este anillo abre la puerta a un mundo elemental", 1, 0, 210, my_enums::_OTHER_, 0);
                    addItem("LLAVE PUERTA ESTE", "Esta llave abre la puerta del este", 1, 0, 340, my_enums::_OTHER_, 0);
                    addItem("MEDALLON OSCURO", "Esta llave abre la puerta del este", 1, 0, 340, my_enums::_OTHER_, 0);
                    addItem("POCION ROJA", "Esta llave abre la puerta del este", 1, 0, 340, my_enums::_OTHER_, 0);

                    //****
                }

                changeMap();

            }//continue

            if (backButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                SDL_StartTextInput();
                setState(my_enums::S_RACES_);

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
                    //currentArchetype = my_enums::_SUMMONER_;
                    currentArchetype = my_enums::_FIGHTER_;
                    break;
                }
              
                /*
                case my_enums::_SUMMONER_:
                {
                    currentArchetype = my_enums::_FIGHTER_;
                    break;
                }
                */

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
                   // currentArchetype = my_enums::_SUMMONER_;
                    currentArchetype = my_enums::_NECROMANCER_;
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

                /*
                case my_enums::_SUMMONER_:
                {
                    currentArchetype = my_enums::_NECROMANCER_;
                    break;
                }
                */

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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    Mix_PlayChannel(-1, audioButton, 0);
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }

            }
            if (continueButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                setState(my_enums::S_RACES_);

            }//continue

            if (backButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                SDL_StartTextInput();
                setState(my_enums::S_NAME_);


            }//back

            if (rollButton.clicked(mousex, mousey)) {
                Mix_PlayChannel(-1, audioButton, 0);
                randomAttributes();

            }//continue
           

            
        }

    }
}



void game::eventsMaster()
{
    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    Mix_PlayChannel(-1, audioButton, 0);
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
            }
            if (continueButton.clicked(mousex, mousey)) {
                setState(previousScreen);


            }//continue

            if (skillButton.clicked(mousex, mousey)) {
                if ((skillPoints > 0) && (max_skill<30)){
                    skillPoints--;
                    max_skill += 1;
                    skill = max_skill;
               }
            }

            if (powerButton.clicked(mousex, mousey)) {
                if ((skillPoints > 0) && (max_power < 30)) {
                    skillPoints--;
                    max_power += 1;
                    power = max_power;

                }
            }

            if (staminaButton.clicked(mousex, mousey)) {
                if (skillPoints > 0) {
                    skillPoints--;
                    max_stamina += 2;
                    stamina = max_stamina;

                }
            }

            if (luckButton.clicked(mousex, mousey)) {
                if ((skillPoints > 0) && (max_luck < 30)) {
                    skillPoints--;
                    max_luck += 1;
                    luck = max_luck;
                }
            }

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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    Mix_PlayChannel(-1, audioButton, 0);
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
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
    max_stamina = stamina;
    max_power = power;
    max_skill = skill;
    max_luck = luck;
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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
            }

            if (muteButton.clicked(mousex, mousey)) {
                if (musicON == true) {
                    musicON = false;
                    muteButton.setCaption("MUSIC OFF");
                    Mix_Volume(-1, 0);
                    Mix_VolumeMusic(0);
                    //addNotification("Sonido apagado");
                    addAchievement("La ley del silencio", my_enums::_OPTIONS_);
                    volumeSoundButton.setCaption("SOUND " + std::to_string(volumeSound * 100 / 128) + "%");
                    volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
                }
                else {
                    musicON = true;
                    muteButton.setCaption("MUSIC ON");

                    Mix_Volume(-1, volumeSound);
                    Mix_VolumeMusic(volumeMusic);
                    //addNotification("Sonido activado");
                    addAchievement("Música para mis oídos", my_enums::_OPTIONS_);
                    volumeSoundButton.setCaption("SOUND " + std::to_string(volumeSound * 100 / 128) + "%");
                    volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
                }
            }

            if (debugMode) {
            if (musicButton.clicked(mousex, mousey)) {
                switch (currentMusic) {

                case 1:
                    currentMusic = 2;
                    Mix_PlayMusic(musicDARK, -1);
                    break;
                case 2:
                    currentMusic = 3;
                    Mix_PlayMusic(musicFOREST, -1);
                    break;
                case 3:
                    currentMusic = 4;
                    Mix_PlayMusic(musicTOWN, -1);
                    break;

                case 4:
                    currentMusic = 5;
                    Mix_PlayMusic(musicBATTLE, -1);
                    break;

                case 5:
                    currentMusic = 6;
                    Mix_PlayMusic(musicBOSS, -1);
                    break;

                case 6:
                    currentMusic = 7;
                    Mix_PlayMusic(musicCAVE, -1);
                    break;

                case 7:
                    currentMusic = 8;
                    Mix_PlayMusic(musicHERO, -1);
                    break;

                case 8:
                    currentMusic = 9;
                    Mix_PlayMusic(musicMYSTICAL, -1);
                    break;

                case 9:
                    currentMusic = 1;
                    Mix_PlayMusic(musicINTRO, -1);
                    break;

                }
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
                addAchievement("Más ruido", my_enums::_OPTIONS_);
            }

            if (volumeSoundDownButton.clicked(mousex, mousey)) {
                volumeSound -= 8;
                if (volumeSound < 0) { volumeSound = 0; }
                Mix_Volume(-1, volumeSound);
                Mix_VolumeMusic(volumeMusic);
                volumeSoundButton.setCaption("SOUND " + std::to_string(volumeSound * 100 / 128) + "%");
                addAchievement("Menos ruido", my_enums::_OPTIONS_);                   
            }

            if (volumeMusicUpButton.clicked(mousex, mousey)) {
                volumeMusic += 8;
                if (volumeMusic > 128) { volumeMusic = 128; }
                Mix_Volume(-1, volumeSound);
                Mix_VolumeMusic(volumeMusic);
                volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
                addAchievement("Más música", my_enums::_OPTIONS_);
            }

            if (volumeMusicDownButton.clicked(mousex, mousey)) {
                volumeMusic -= 8;
                if (volumeMusic < 0) { volumeMusic = 0; }
                Mix_Volume(-1, volumeSound);
                Mix_VolumeMusic(volumeMusic);
                volumeMusicButton.setCaption("MUSIC " + std::to_string(volumeMusic * 100 / 128) + "%");
                addAchievement("Menos música", my_enums::_OPTIONS_);
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

            if (titleButton.clicked(mousex, mousey)) {
                addAchievement("El nombre del juego", my_enums::_HIDDEN_);
            }

            if (tuto1Button.clicked(mousex, mousey)) {
                addAchievement("Ayuda", my_enums::_HIDDEN_);
                playTutorial();
            }

            if (tuto2Button.clicked(mousex, mousey)) {
                addAchievement("Ayuda", my_enums::_HIDDEN_);
                playTutorialHomeTown();
            }

        }//mouse button

 

        //******************
    }


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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
            }

            if (continueButton.clicked(mousex, mousey)) {
                setState(previousScreen);

            }

            
            if (itemSelectedButton.clicked(mousex, mousey)) {
                int y = mousey - 350;
                int itemSelected = (y / 50) + 1;

                if (mousex > gScreenSurface->w / 2 + 200)itemSelected += 10;

                //itemSelectedButton.setButton(gScreenSurface->w / 2 - 200, 350, 850, gScreenSurface->h - 350, " ");

                if (items.size() >= itemSelected) {
                      std::list<item>::iterator it = items.begin();
                    if (itemSelected > 1) {
                        std::advance(it, itemSelected-1);
                    }
                    itemDescription = it->description;
                    switch (it->type) {
                    
                    case my_enums::_WEAPON_1H_:
                    {
                        Mix_PlayChannel(-1, weapon, 0);
                        alternate3 = it->name;
                        break;
                    }

                    case my_enums::_WEAPON_2H_:
                    {
                        Mix_PlayChannel(-1, weapon, 0);
                        alternate3 = it->name;
                        alternate = "";
                        break;
                    }

                    case my_enums::_WEAPON_RANGED_:
                    {
                        Mix_PlayChannel(-1, weapon, 0);
                        alternate3 = it->name;
                        alternate = "";
                        break;
                    }

                    case my_enums::_ARMOR_:
                    {
                        Mix_PlayChannel(-1, clothes, 0);
                        alternate2 = it->name;
                        break;
                    }

                    case my_enums::_SHIELD_:
                    {
                        Mix_PlayChannel(-1, clothes, 0);
                        if ((alternate3 =="") ||(getItem(alternate3).type == my_enums::_WEAPON_1H_)){
                        alternate = it->name;
                        }
                        break;
                    }

                    case my_enums::_HELMET_:
                    {
                        Mix_PlayChannel(-1, clothes, 0);
                        itemHelmet = it->name;
                        break;
                    }

                    case my_enums::_RING_:
                    {
                        Mix_PlayChannel(-1, ring, 0);
                        int ringL = it->description.length() - 7;
                        if (ringL == 7)ringL = 5;
                        if (ringL == 8)ringL = 6;
                        if (ringL == 11)ringL = 9;
                        string ringD = it->description.substr(7, ringL);

                        if (itemRing != "") {
                            skill -= skillBonus;
                            max_skill -= skillBonus;
                            stamina -= staminaBonus;
                            max_stamina -= staminaBonus;
                            power -= powerBonus;
                            max_power -= powerBonus;
                            luck -= luckBonus;
                            max_luck -= luckBonus;

                            skillBonus=0;
                            staminaBonus=0;
                            powerBonus=0;
                            luckBonus=0;
                        }

                            if(ringD=="HABILIDAD"){
                                skill += it->bonus;
                                max_skill+= it->bonus;
                                skillBonus = it->bonus;
                            }
                            if (ringD == "VITALIDAD") {
                                stamina += it->bonus;
                                max_stamina += it->bonus;
                                staminaBonus = it->bonus;
                            }
                            if (ringD == "PODER") {
                                power += it->bonus;
                                max_power += it->bonus;
                                powerBonus = it->bonus;
                            }
                            if (ringD == "SUERTE") {
                                luck += it->bonus;
                                max_luck += it->bonus;
                                luckBonus = it->bonus;
                            }
                      
                       



                        itemRing = it->name;
                        break;
                    }


                    }//switch

                }//items size

            }//itemselected button
            

        }

        //******************
    }



}

//*x =


bool game::existNode(int x, int y, std::list <node> nodes)
{
    bool found = false;
    for (list<node>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if ((it->x == x) & (it->y == y)) {
            found = true;
            break;
        }

    }
    return found;
}

void game::getpath(int* startx, int* starty, int endx, int endy)
{
    int MAX_LEVEL = 20;
        int tmpx = *startx;
        int tmpy = *starty;

        int tx = 0;
        int ty = 0;

    std::list <node> baseNodes;
    baseNodes.push_back({ 0,-1,0,0,"NULL",0,0 });//north
    baseNodes.push_back({ 0,+1,0,0,"NULL",0,0 });//south
    baseNodes.push_back({ -1,0,0,0,"NULL",0,0 });//west
    baseNodes.push_back({ +1,0,0,0,"NULL",0,0 });//east

    std::list <node> nodes;

    int currentLevel = 0;
    bool keepLevel = true;


    bool exit = false;
    int parent = 0;
    int max_id = 1;
    int current_id = 0;

    int min_M = 100;
    int emergency = 0;
    int emergencySearch = 0;

    //create starting node
    node currentNode = { tmpx,tmpy,0,manhattan(tmpx,tmpy,endx,endy),"NULL",max_id,0 };
    nodes.push_back(currentNode);
    
    //while (((currentLevel <= MAX_LEVEL)&&(currentLevel>=0)  )||(exit == false))
    bool found = false;
    while ((emergency <150) && (exit == false))
    {   
        emergency++;
        found = false;
        //check if we found destination node
        if ((currentNode.x == endx) && (currentNode.y == endy)) {
            exit = true;
            break;
        }
        else {
            //not found, tag current node as visited
            for (list<node>::iterator it = nodes.begin(); it != nodes.end(); it++)
            {
                if (it->id == currentNode.id) {
                    it->S = "Y";
                    break;
                }
            }//tag Y as visited

            //We add nodes surrounding current node
            for (list<node>::iterator it = baseNodes.begin(); it != baseNodes.end(); it++)
            {
                tx = currentNode.x + it->x;
                ty = currentNode.y + it->y;
                if (!existNode(tx, ty, nodes) && !collide(tx, ty, false)) {
                    nodes.push_back({ tx,ty,currentNode.L + 1,manhattan(tx,ty,endx,endy),"NULL",++max_id,currentNode.id });
                }
            }

            // we define next current node
           
            currentLevel = 0;
           // int currentID = 0;
           // while ((!found)||(currentID<150) ){
            emergencySearch = 0;
            while ((!found)&& (emergencySearch < 150)) {
                emergencySearch++;
                keepLevel = false;
               // currentID = 0;
                for (list<node>::iterator it = nodes.begin(); it != nodes.end(); it++)
                {
                    //if ((it->id == currentID) && (it->L == currentLevel)&&(it->S == "NULL")) {
                    if ((it->L == currentLevel) && (it->S == "NULL")) {
                        currentNode.x = it->x;
                        currentNode.y = it->y;
                        currentNode.L = it->L;
                        currentNode.M = it->M;
                        currentNode.S = it->S;
                        currentNode.id = it->id;
                        currentNode.parent_id = it->parent_id;
                        found = true;
                        break;
                    }
                }
                //currentID++;
                if (keepLevel == false)currentLevel++;
            }//found currentnode
        }//current node is not last one    
    }//exit from main loop
    

    //we have found the target, traverse path in reverse ordert to find next NPC step
    if (exit == true) {
        int id = 0;
        bool foundPath = false;

        //first we get the parent id for the last node
        for (list<node>::iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            if ((it->x == endx) && (it->y == endy)){
                id = it->parent_id;
                break;
            }
        }

        while (foundPath == false) {
            for (list<node>::iterator it = nodes.begin(); it != nodes.end(); it++)
            {
                if (it->id == id) {
                    if (manhattan(*startx, *starty, it->x, it->y) == 1) {
                        foundPath = true;
                        *startx = it->x;
                        *starty = it->y;
                        break;
                    }
                    id = it->parent_id;
                }//found if
            }

        }


    }//found the target


}//


void game::processAI()
{
    //Process NPC AIs
    for (list<NPC>::iterator it = NPCs.begin(); it != NPCs.end(); it++)
    {
        //only update NPCs in current map
        if (it->map == currentState) {
            if (it->NPCAI == my_enums::_ENEMY_RANDOM_ || it->NPCAI == my_enums::_FRIENDLY_RANDOM_) {
                int tmpx = it->x;
                int tmpy = it->y;
                switch (dice(4, 1)) {
               
                case 1:
                {
                    tmpx++;
                    break;
                }

                case 2:
                {
                    tmpx--;
                    break;
                }

                case 3:
                {
                    tmpy++;
                    break;
                }

                case 4:
                {
                    tmpy--;
                    break;
                }

                }//switch
                if (tmpx < 0)tmpx = 1;
                if (tmpy < 0)tmpy = 1;
                if (tmpx > 250)tmpx = 250;
                if (tmpy > 250)tmpy = 250;
                if (!collide(tmpx, tmpy, false)) {
                    it->x = tmpx;
                    it->y = tmpy;
                }

            }

            if ((it->NPCAI == my_enums::_ENEMY_FOLLOW_)|| (it->NPCAI == my_enums::_ENEMY_FOLLOW_MAGE_)){

                //if(getDistance(px,py, it->x, it->y) <= 15){// only follow if distance is less or equal than 15
                int m = manhattan(px, py, it->x, it->y);
                if (m <= 15) {// only follow if distance is less or equal than 15
                    if (isAround(it->x, it->y)) {
                        //force enter combat
                        it->x = px;
                        it->y = py;
                    }
                    else {
                        if (m < 8) {
                            getpath(&it->x, &it->y, px, py);
                        }
                        else {
                            int tmpx = it->x;
                            int tmpy = it->y;

                            node north = { 0,-1,0,0,"",0,0 };
                            node south = { 0,+1,0,0,"",0,0 };
                            node west = { -1,0,0,0,"",0,0 };
                            node east = { +1,0,0,0,"",0,0 };

                            node finalNode = { 0,0,0,0,"",0,0 };

                            std::list <node> nodes;
                            if (dice(2, 1) == 1) {
                                nodes.push_back(north);
                                nodes.push_back(south);
                                nodes.push_back(east);
                                nodes.push_back(west);
                            }
                            else {
                                nodes.push_back(west);
                                nodes.push_back(east);
                                nodes.push_back(south);
                                nodes.push_back(north);
                            }

                            std::list<node>::iterator itn = nodes.begin();
                            //while (itn != nodes.end())
                            while (itn != nodes.end())
                            {
                                bool erased = false;
                                if (!collide(tmpx + itn->x, tmpy + itn->y, false)) {
                                    itn->M = manhattan(tmpx + itn->x, tmpy + itn->y, px, py);
                                    if (finalNode.M > itn->M) {
                                        finalNode.M = itn->M;
                                        finalNode.x = itn->x;
                                        finalNode.y = itn->y;

                                    }
                                }
                                else {
                                    itn = nodes.erase(itn);
                                    erased = true;
                                }
                                if ((itn != nodes.end()) && (erased == false))itn++;

                            }

                            it->x = tmpx + finalNode.x;
                            it->y = tmpy + finalNode.y;

                        }
                    }
                    
  
            }//distance less than 15
            }//process basic follow AI



            if (it->NPCAI == my_enums::_FRIENDLY_FOLLOW_) {

                if (manhattan(px, py, it->x, it->y) <= 15){// only follow if distance is less or equal than 15
                    if(!isAround(it->x, it->y)){
                        int m = manhattan(px, py, it->x, it->y);
                        if (m < 8) {
                            getpath(&it->x, &it->y, px, py);
                        }
                        else {





                            int tmpx = it->x;
                            int tmpy = it->y;

                            node north = { 0,-1,0,0,"",0,0 };
                            node south = { 0,+1,0,0,"",0,0 };
                            node west = { -1,0,0,0,"",0,0 };
                            node east = { +1,0,0,0,"",0,0 };

                            node finalNode = { 0,0,0,0,"",0,0 };

                            std::list <node> nodes;
                            if (dice(2, 1) == 1) {
                                nodes.push_back(north);
                                nodes.push_back(south);
                                nodes.push_back(east);
                                nodes.push_back(west);
                            }
                            else {
                                nodes.push_back(west);
                                nodes.push_back(east);
                                nodes.push_back(south);
                                nodes.push_back(north);
                            }

                            std::list<node>::iterator itn = nodes.begin();
                            //while (itn != nodes.end())
                            while (itn != nodes.end())
                            {
                                bool erased = false;
                                if (!collide(tmpx + itn->x, tmpy + itn->y, false)) {
                                    itn->M = manhattan(tmpx + itn->x, tmpy + itn->y, px, py);
                                    if (finalNode.M > itn->M) {
                                        finalNode.M = itn->M;
                                        finalNode.x = itn->x;
                                        finalNode.y = itn->y;

                                    }
                                }
                                else {
                                    itn = nodes.erase(itn);
                                    erased = true;
                                }
                                if ((itn != nodes.end()) && (erased == false))itn++;

                            }

                            it->x = tmpx + finalNode.x;
                            it->y = tmpy + finalNode.y;

                        }


                       // getpath(&it->x, &it->y, px, py);
                        /*
                        int tmpx = it->x;
                        int tmpy = it->y;

                        node north = { 0,0,-1 };
                        node south = { 0,0,+1 };
                        node west = { 0,-1,0 };
                        node east = { 0,+1,0 };

                        node finalNode = { 100,0,0 };

                        std::list <node> nodes;
                        if (dice(2, 1) == 1) {
                            nodes.push_back(north);
                            nodes.push_back(south);
                            nodes.push_back(east);
                            nodes.push_back(west);
                        }
                        else {
                            nodes.push_back(west);
                            nodes.push_back(east);
                            nodes.push_back(south);
                            nodes.push_back(north);
                        }

                        std::list<node>::iterator itn = nodes.begin();
                        //while (itn != nodes.end())
                        while (itn != nodes.end())
                        {
                            bool erased = false;
                            if (!collide(tmpx + itn->x, tmpy + itn->y, false)) {
                                itn->cost = manhattan(tmpx + itn->x, tmpy + itn->y, px, py);
                                if (finalNode.cost > itn->cost) {
                                    finalNode.cost = itn->cost;
                                    finalNode.x = itn->x;
                                    finalNode.y = itn->y;

                                }
                            }
                            else {
                                itn = nodes.erase(itn);
                                erased = true;
                            }
                            if ((itn != nodes.end()) && (erased == false))itn++;

                        }

                        it->x = tmpx + finalNode.x;
                        it->y = tmpy + finalNode.y;
                        */
                }//we are not too close
                }//distance less than 15
            }//process basic follow AI


        }

    }

    //Process global events
        std::list<GLOBAL_EVENT>::iterator it = GLOBAL_EVENTs.begin();
        while (it != GLOBAL_EVENTs.end())
        {
            bool erased = false;
        if (it->map == currentState) {//only process events in current map
            if (it->description == "CHANGE_AI_FRIENDLY_FOLLOW") {

               
                    if (insideBoundaries(px, py, it->location)) { //inside event boundaries
                        for (list<NPC>::iterator itNPC = NPCs.begin(); itNPC != NPCs.end() && erased == false; itNPC++)
                        {
                            if (itNPC->map == currentState) {//on the current map
                                if (itNPC->id == it->NPCID) { //id match
                                   itNPC->NPCAI = my_enums::_FRIENDLY_FOLLOW_;
                                     addNotification(itNPC->description + " comienza a seguirte", { 0,0,0 });
                                     //GLOBAL_EVENTs.erase(it);
                                     it = GLOBAL_EVENTs.erase(it);
                                     erased = true;
                                 }
                            }
                        }
                }//event boundaries

            }else if (it->description == "CHANGE_AI_FRIENDLY_STATIC") {

                for (list<NPC>::iterator itNPC = NPCs.begin(); itNPC != NPCs.end() && erased == false; itNPC++)
                {
                    if (itNPC->map == currentState) {//on the current map
                        if (itNPC->id == it->NPCID) { //id match
                            if (insideBoundaries(itNPC->x, itNPC->y, it->location)) { //inside event boundaries
                                itNPC->NPCAI = my_enums::_FRIENDLY_STATIC_;
                                addNotification(itNPC->description + " deja de seguirte", { 0,0,0 });

                                if (itNPC->id == 10001)addAchievement("Rescataste a TOR", my_enums::_MISSIONS_);
                                it = GLOBAL_EVENTs.erase(it);
                                erased = true;
                            }
                        }

                    }
                }//

            }//
            else if (it->description == "REMOVE_CHAT") {

                for (list<NPC>::iterator itNPC = NPCs.begin(); itNPC != NPCs.end() && erased == false; itNPC++)
                {
                    if (itNPC->map == currentState) {//on the current map
                        if (itNPC->id == it->NPCID) { //id match
                            if (insideBoundaries(itNPC->x, itNPC->y, it->location)) { //inside event boundaries                               
                                if (itNPC->id == 10001)removeChat(119, 142, 2);
                                it = GLOBAL_EVENTs.erase(it);
                                erased = true;
                            }
                        }

                    }
                }//

            }//removechat
            else if (it->description == "ADD_CHAT") {

                for (list<NPC>::iterator itNPC = NPCs.begin(); itNPC != NPCs.end() && erased == false; itNPC++)
                {
                    if (itNPC->map == currentState) {//on the current map
                        if (itNPC->id == it->NPCID) { //id match
                            if (insideBoundaries(itNPC->x, itNPC->y, it->location)) { //inside event boundaries                               
                                if (itNPC->id == 10001) {
                                    addChat(my_enums::S_HOMETOWN_, 119, 142, 2, "Sobre Tor", "¡Muchas gracias!, con la llave que te he dado puedes abrir el camino que lleva a la playa");
                                    removeChat(109, 142, 5);
                                    addChat(my_enums::S_HOMETOWN_, 109, 142, 5, "Sobre el mago del templo elemental", "El mago del templo elemental esta muy agradecido, sigue así y se abriran más puertas");
                                }
                                    
                                    
                                it = GLOBAL_EVENTs.erase(it);
                                erased = true;
                            }
                        }

                    }
                }//

            }//removechat
            else if (it->description == "ADD_ITEM") {

                for (list<NPC>::iterator itNPC = NPCs.begin(); itNPC != NPCs.end() && erased == false; itNPC++)
                {
                    if (itNPC->map == currentState) {//on the current map
                        if (itNPC->id == it->NPCID) { //id match
                            if (insideBoundaries(itNPC->x, itNPC->y, it->location)) { //inside event boundaries                               
                                if (itNPC->id == 10001)addItem("LLAVE PUERTA ESTE", "Esta llave abre la puerta del este", 1, 0, 340, my_enums::_OTHER_, 0);
                                it = GLOBAL_EVENTs.erase(it);
                                erased = true;
                            }
                        }

                    }
                }//

            }//add item
            else if (it->description == "NPC_DEFEATED") {
                if (!findNPC(it->NPCID)) {
                    if (it->NPCID = 3000) {
                        if(it->map == my_enums::S_HOMETOWN_){
                        addAchievement("Derrotaste al necromante", my_enums::_MISSIONS_);
                        removeChat(109, 142, 4);
                        addChat(my_enums::S_HOMETOWN_, 109, 142, 4, "Sobre el necromante...", "Gracias por derrotar al nigromante, ahora el pueblo vuelve a ser un sitio seguro");
                        removeChat(119, 142, 1);
                        addChat(my_enums::S_HOMETOWN_, 119, 142, 1, "<<MISIÓN>>Objetos perdidos...", "He perdido mis gafas de sol, la última vez que las usé estaba en la costa al este");
                        //it = GLOBAL_EVENTs.erase(it);
                       // erased = true;
                        }

                        if (it->map == my_enums::S_COAST_WORLD_) {
                            addItem("GAFAS DE SOL", "Te suena alguien que las necesita...", 1, 0, 337, my_enums::_OTHER_, 0);
                          }

                        if (it->map == my_enums::S_ELEMENTAL_WATER_WORLD_) {
                            addItem("ANILLO VUELO", "Este anillo abre la puerta a un mundo elemental", 1, 0, 226, my_enums::_OTHER_, 0);
                            
                        
                        }

                        if (it->map == my_enums::S_ELEMENTAL_FIRE_WORLD_) {
                            addItem("MEDALLON OSCURO", "No sabes que función puede tener", 1, 0, 245, my_enums::_OTHER_, 0);
                        
                        }

                        if (it->map == my_enums::S_ELEMENTAL_EARTH_WORLD_) {                         
                            addItem("ANILLO RESPIRAR AGUA", "Este anillo abre la puerta a un mundo elemental", 1, 0, 230, my_enums::_OTHER_, 0);
                        
                        }

                        if (it->map == my_enums::S_ELEMENTAL_WIND_WORLD_) {
                            addItem("ANILLO RESISTENCIA FUEGO", "Este anillo abre la puerta a un mundo elemental", 1, 0, 229, my_enums::_OTHER_, 0);
                             
                        }

                        if (it->map == my_enums::S_FOREST_WORLD_) {
                            addItem("LIBRO RECETAS", "Libro con recetas malignas", 1, 0, 282, my_enums::_OTHER_, 0);
                            addAchievement("Cocinando el mal", my_enums::_MISSIONS_);

                            //move npc
                            //change chat
                            for (list<NPC>::iterator it = NPCs.begin(); it != NPCs.end(); it++)
                            {
                                if (currentState == my_enums::S_FOREST_WORLD_) {
                                    if ((it->id == 2000) && (it->map == currentState)) {//update wise person location
                                        int x = 1, y = 1;
                                        baseMap.getLocation(&x, &y, 573);
                                        it->x = x + 4;
                                        it->y = y - 1;
                                    }
                                }
                            }

                            removeChat(230, 50, 4);
                            addChat(my_enums::S_FOREST_WORLD_, 230, 50, 4, "<<MISIÓN>>¡Tengo un libro de alquimia oscura!", "Con este libro tan solo necesitas dos pasos más 1)Encuentra un alquimista que te prepare la poción, 2)Encuentra un mago elemental que la active");

                            for (list<CHAT>::iterator it = CHATs.begin(); it != CHATs.end(); it++)
                            {
                                if (currentState == my_enums::S_FOREST_WORLD_) {
                                    if (it->map == currentState) {//update wise person location
                                        int x = 1, y = 1;
                                        baseMap.getLocation(&x, &y, 573);
                                        it->x = x + 4;
                                        it->y = y - 1;
                                    }
                                }
                            }

                              



                        }

                        
                    }

                    if (it->NPCID = 5000) {
                       
                        
                        if (it->map == my_enums::S_NECRO_WORLD_) {
                            addAchievement("Asesino de reyes", my_enums::_HIDDEN_);
                            int achievementCounter = achievements.size();
                            if (achievementCounter != 40) {
                                addAchievement("Undead", my_enums::_HIDDEN_);
                                setState(my_enums::S_HERO_);
                                changeMusic();
                                deleteNPCs(px, py);
                                //Mix_PlayMusic(musicGameOver, -1);
                                addAchievement("Tu personaje ha muerto", my_enums::_HIDDEN_);
                                screenDarkFate();

                            }
                            else {
                                setState(my_enums::S_MAINMENU_);
                                changeMusic();
                                deleteNPCs(px, py);
                                screenWIN();
                            }

                        }

              
                        
                    }
                   

                    
                    it = GLOBAL_EVENTs.erase(it);
                    erased = true;

                }

            }//npc defeated
            else if (it->description.substr(0, 9) == "HAVE_ITEM") {
                if (insideBoundaries(px, py, it->location)) { //inside event boundaries  
                    string itemName = it->description.substr(10, it->description.length() - 10);
                    if (findItem(itemName)) {
                        if (itemName == "GAFAS DE SOL") {
                            addAchievement("Sol y playa", my_enums::_MISSIONS_);
                            removeChat(119, 142, 1);
                            removeChat(119, 142, 2);
                            addChat(my_enums::S_HOMETOWN_, 119, 142, 1, "Sobre el templo", "Te había subestimado, si superas los 4 portales podrás cruzar el portal al necromundo, donde el señor oscuro nos acecha...");                         
                            addChat(my_enums::S_HOMETOWN_, 119, 142, 2, "<<MISIÓN>>Portal elemental de tierra", "Con el anillo que te he dado podrás atravesar el primer portal, busca el secuaz del señor oscuro y arrebátale el anillo para abrir el siguiente portal...");                           
                            addItem("ANILLO DE LA TIERRA", "Este anillo abre la puerta a un mundo elemental", 1, 0, 210, my_enums::_OTHER_, 0);
                            removeItem("GAFAS DE SOL");
                        }

                        if (itemName == "ANILLO RESPIRAR AGUA") {
                            addAchievement("Poseidón", my_enums::_MISSIONS_);
                           removeChat(119, 142, 2);
                            addChat(my_enums::S_HOMETOWN_, 119, 142, 2, "<<MISIÓN>>Portal elemental de agua", "Con el anillo que te he dado podrás atravesar el segundo portal, busca el secuaz del señor oscuro y arrebátale el anillo para abrir el siguiente portal...");
                           
                        }

                        if (itemName == "ANILLO VUELO") {
                            addAchievement("Águila", my_enums::_MISSIONS_);
                            removeChat(119, 142, 2);
                            addChat(my_enums::S_HOMETOWN_, 119, 142, 2, "<<MISIÓN>>Portal elemental de aire", "Con el anillo que te he dado podrás atravesar el tercer portal, busca el secuaz del señor oscuro y arrebátale el anillo para abrir el siguiente portal...");
                        }

                            if (itemName == "ANILLO RESISTENCIA FUEGO") {
                                addAchievement("Lilith", my_enums::_MISSIONS_);
                                removeChat(119, 142, 2);
                                addChat(my_enums::S_HOMETOWN_, 119, 142, 2, "<<MISIÓN>>Portal elemental de fuego", "Con el anillo que te he dado podrás atravesar el último portal, busca el secuaz del señor oscuro y arrebátale el anillo para abrir el siguiente portal...");
                            }

                            if (itemName == "MEDALLON OSCURO") {
                                addAchievement("Rey de los planos", my_enums::_MISSIONS_);
                                removeChat(119, 142, 2);
                                addChat(my_enums::S_HOMETOWN_, 119, 142, 2, "<<MISIÓN>>El bosque oscuro", "No paras de sorprenderme, ese collar que has conseguido... creo que en realidad es una llave para abrir una puerta a un edificio donde se aloja el mal, creo que es hora que te acerques al bosque oscuro, sigue el camino al moroeste del pueblo");
                            }

                            if (itemName == "LIBRO RECETAS") {
                                addItem("POCION NEGRA", "Esta poción tiene una pinta muy sospechosa", 1, 0, 270, my_enums::_OTHER_, 0);                             
                            }

                            if (itemName == "POCION NEGRA") {
                                addItem("POCION ROJA", "Esta poción tiene una pinta muy sospechosa", 1, 0, 276, my_enums::_OTHER_, 0);
                                removeItem("POCION NEGRA");
                                addAchievement("Poción del mal", my_enums::_MISSIONS_);
                            }

                            if (itemName == "POCION ROJA") {                              
                                removeItem("POCION ROJA");

                                for (list<NPC>::iterator itNPC = NPCs.begin(); itNPC != NPCs.end() && erased == false; itNPC++)
                                {
                                    if (itNPC->map == currentState) {//on the current map
                                        if (itNPC->id == it->NPCID) { //id match
                                            if (insideBoundaries(itNPC->x, itNPC->y, it->location)) { //inside event boundaries
                                                itNPC->NPCAI = my_enums::_ENEMY_FOLLOW_;
                                                addNotification("¡¡¡"+itNPC->description + " se despierta y te ataca!!!", {0,0,0});

                                                addAchievement("Despertar al leviatán", my_enums::_MISSIONS_);
                                                it = GLOBAL_EVENTs.erase(it);
                                                erased = true;
                                            }
                                        }

                                    }
                                }//
                                
                            }


                           // addGlobalEvent(13, my_enums::S_NECRO_WORLD_, { 128, 157,130, 159 }, { 0,0,0,0 }, 0, 500, "HAVE_ITEM POCION ROJA");


                            
                            


                        it = GLOBAL_EVENTs.erase(it);
                        erased = true;

                    }
                    //  addItem("LLAVE PUERTA ESTE", "Esta llave abre la puerta del este", 1, 0, 340, my_enums::_OTHER_, 0);
                  //  it = GLOBAL_EVENTs.erase(it);
                  //  erased = true;
                }
                    }//have item
        


        }//on the same map


       if ((it != GLOBAL_EVENTs.end())&& (erased == false))it++;
        

    }//Process global events


}

/*
bool game::compareByCost(const node& a, const node& b)
{
    return a.cost < b.cost;
}*/

int game::manhattan(int start_x, int start_y, int end_x, int end_y)
{
    return (abs(start_x - end_x) + abs(start_y - end_y));
}

bool game::findNPC(int id)
{
    bool found = false;
    for (list<NPC>::iterator itNPC = NPCs.begin(); itNPC != NPCs.end(); itNPC++)
    {
        if ((itNPC->id == id)&& (itNPC->map == currentState))found = true;
    }
    return found;
}

void game::timeEvents()
{
    
    if ((getState() != my_enums::S_FIGHT_) || (getState() != my_enums::S_SHOP_) || (getState() != my_enums::S_CHAT_) || (getState() != my_enums::S_MASTER_)){
        if ((SDL_GetTicks() - ticksAI) >= 1000) {
            ticksAI = SDL_GetTicks();
            processAI();
            monsterGenerator();
        }
    }
myTime = (int)( (SDL_GetTicks()- timer) / 1000);

//Power recharges with time...
if (getState() != my_enums::S_FIGHT_)//You dont recharge magic while fighting
{
    if ((myTime != prevTime)&& (myTime >= 1)){
        prevTime = myTime;

        powerRegeneration++;
        if (powerRegeneration >= (30 - max_power)) {
            power++;
            if (power > max_power) power = max_power;
            powerRegeneration = 0;
        }
    }
}
}//timeEvents


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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
            }

            if (continueButton.clicked(mousex, mousey)) {
                //Reset everything except achievements
                notifications.clear();
                skills.clear();

                NPCs.clear(); //All NPCs
                tmpNPCs.clear(); //NPCs found in our location

                SHOPs.clear(); // All Shops
                tmpSHOPs.clear(); // Shops Found in our location

                CHATs.clear(); // All chat options
                tmpCHATs.clear(); // chat options Found in our location

                EVENTs.clear(); // All Shops
                tmpEVENTs.clear(); // Shops Found in our location

                items.clear();

coins = 50; //Player currency
food = 10; //Player food rations 
potions_health = 1;
potions_power = 1;
powerRegeneration = 0;

//Player attributes
skill = 1; //Combat Skill
stamina = 1; //Player hitpoins and posion defense
power = 1; //magic points and magic regeneration rate
luck = 1; //skill improves buy and sell values and posibility to discover how difficult are enemies
max_skill = 1;
max_stamina = 1;
max_power = 1;
max_luck = 1;

string alternate3 = "";
string  itemArmor = "";
string itemHelmet = "";
string shield = "";
string itemRing = "";
string alternate = "";
string alternate2 = "";

loadPlayerDefault();
loadNPCs();
loadShops();
loadChats();
loadEvents();

Mix_PlayMusic(musicINTRO, -1);
addAchievement("Heroe Inmortal", my_enums::_HIDDEN_);
setState(my_enums::S_MAINMENU_);


            }

            if (achievementsButton.clicked(mousex, mousey)) {
                setState(my_enums::S_ACHIEVEMENTS_);
                previousScreen = my_enums::S_HERO_;

            }

        }

        //******************
    }

}

void game::locationEvents()
{
    if ((currentState == my_enums::S_HOMETOWN_) || (currentState == my_enums::S_COAST_WORLD_) || (currentState == my_enums::S_FOREST_WORLD_) || (currentState == my_enums::S_NECRO_WORLD_) || (currentState == my_enums::S_ELEMENTAL_FIRE_WORLD_) || (currentState == my_enums::S_ELEMENTAL_WATER_WORLD_) || (currentState == my_enums::S_ELEMENTAL_EARTH_WORLD_) || (currentState == my_enums::S_ELEMENTAL_WIND_WORLD_)) {
        tmpEVENTs = getEvents(px, py);
        if (tmpEVENTs.size() > 0) {
            bool erase = false;
            std::string tmpStr;
            for (list<EVENT>::iterator it = tmpEVENTs.begin(); it != tmpEVENTs.end(); it++)
            {
                if (it->description == "TELEPORT") {
                    px = it->newx;
                    py = it->newy;

                    if (it->map != it->newMap) {
                        addAchievement("Cambiador de planos", my_enums::_HIDDEN_);
                        currentState = it->newMap;
                        changeMap();
                    }


                }

                if (it->description == "GOLD") {

                    if (it->value > 0) {
                        tmpStr = "Ganas ";
                    }
                    else {
                        tmpStr = "Pierdes ";
                    }
                    tmpStr = tmpStr + to_string(it->value);
                    tmpStr = tmpStr + " monedas";
                    addNotification(tmpStr, { 0,0,0 });
                    coins += it->value;
                    if (coins < 0) { coins = 0; }
                    erase = true;

                }
            }
            if (erase)cleanEvents(px, py);
        }//there are events to process
    }
}

void game::enemyAttack() {
    int enemyDamage;

    //Process enemy attack
    if (tmpNPCs.size() > 0) {
        if (((tmpNPCs.begin()->NPCAI == my_enums::_ENEMY_FOLLOW_MAGE_)|| (tmpNPCs.begin()->NPCAI == my_enums::_ENEMY_STATIC_MAGE_)) && (tmpNPCs.begin()->power >0)) {
            //process magic attack
            addAnimation(1, 100, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonSpellTexture);
            Mix_PlayChannel(-1, magic, 1);
//            int damage = 1;
            int attackPower = dice(tmpNPCs.begin()->power, 1);
            enemyDamage = attackPower;
            stamina -= enemyDamage;
            tmpNPCs.begin()->power -= attackPower;

            fightFoe = tmpNPCs.begin()->description + " te ha herido por " + to_string(attackPower) + " puntos de vida";
            if (stamina <= 0) {
                fightFoe += ", Has sido derrotado por " + tmpNPCs.begin()->description + "!!";
                //addNotification("Has sido derrotado por " + tmpNPCs.begin()->description + "!!", { 0,0,0 });
                setState(my_enums::S_HERO_);
                deleteNPCs(px, py);
                //Mix_PlayMusic(musicGameOver, -1);
                addAchievement("Tu personaje ha muerto", my_enums::_HIDDEN_);
                Mix_PlayMusic(musicHERO, -1);
                timerGameOver = SDL_GetTicks();
            }


        }
        else {
           //process melee atack
            addAnimation(1, 100, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonSwordTexture);
            Mix_PlayChannel(-1, sword, 1);
            int good = dice(10, 1) + skill;
            if (hasSkill("MELEE")) { good++; }
            if (alternate2 != "") { good += getItem(alternate2).bonus; }
            if (itemHelmet != "") { good += getItem(itemHelmet).bonus; }
            if (alternate != "") { good += getItem(alternate).bonus; }

            int bad = dice(10, 1) + tmpNPCs.begin()->skill;
            if (bad > good) {
                enemyDamage = tmpNPCs.begin()->damage;
                stamina -= enemyDamage;
                //addNotification(tmpNPCs.begin()->description + " te ha herido!!", { 0,0,0 });
                fightFoe = tmpNPCs.begin()->description + " te ha herido por " + to_string(enemyDamage) + " puntos de vida";
                if (stamina <= 0) {
                    fightFoe += ", Has sido derrotado por " + tmpNPCs.begin()->description + "!!";
                    //addNotification("Has sido derrotado por " + tmpNPCs.begin()->description + "!!", { 0,0,0 });
                    setState(my_enums::S_HERO_);
                    deleteNPCs(px, py);
                    //Mix_PlayMusic(musicGameOver, -1);
                    addAchievement("Tu personaje ha muerto", my_enums::_HIDDEN_);
                    Mix_PlayMusic(musicHERO, -1);
                    timerGameOver = SDL_GetTicks();
                }

            }
            else {
                fightFoe = tmpNPCs.begin()->description + " ha fallado ";
            }

        }
    }

}

void game::eventsFight()
{
    //If we got pending an enemy attack, process it
    if (((SDL_GetTicks() - lastTurn) > 1000) && enemyAttacked == true) {
        enemyAttacked = false;
        lastTurn = SDL_GetTicks();
        enemyAttack();
    }

   

    //Event handler
    SDL_Event e;

    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                    deleteNPCs(px, py);
                }
            }

            if ((SDL_GetTicks() - lastTurn) > 1000) {
            //melee atack
            if (fightButton.clicked(mousex, mousey)) {
                addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100, 1, buttonSwordTexture);
                Mix_PlayChannel(-1, sword, 1);
                turn++;
                lastTurn = SDL_GetTicks();
                int good = dice(10, 1) + skill;
                int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                // playerDice = good;
               //  foeDice = bad;
                int damage = 1;
                int enemyDamage = 1;

                if (hasSkill("MELEE")) { good++; }
                if (alternate3 != "") { damage += getItem(alternate3).bonus; }

                //Process player attack
                if (good > bad) {
                    NPC tmpNPC = tmpNPCs.front();

                    fightPlayer = "Has herido a " + tmpNPC.description + " por " + to_string(damage) + " puntos de vida";
                    tmpNPC.stamina -= damage;
                    if (tmpNPC.stamina <= 0) {
                        fightPlayer += ", Has derrotado a " + tmpNPC.description + "!!";
                        //Mix_PlayChannel(-1, audioMaleDeath, 0);
                        addExp(tmpNPC.exp);
                        dropLoot(tmpNPC.x, tmpNPC.y, tmpNPC.exp);
                        tmpNPCs.pop_front();
                        killCount++;
                        addAchievement("Primera victoria", my_enums::_COMBAT_);
                        if (killCount == 10)addAchievement("Le estas pillando el punto", my_enums::_COMBAT_);
                        if (killCount == 50)addAchievement("Massacre", my_enums::_COMBAT_);
                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }
                }
                else {
                    fightPlayer = "Has fallado";
                }


                //Counter attack from enemy
                enemyAttacked = true;
                //enemyAttack();

            }//fightbutton click

            //ranged attack
            if (bowButton.clicked(mousex, mousey)) {
                addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100, 1, buttonBowTexture);
                Mix_PlayChannel(-1, bow, 1);
                turn++;
                lastTurn = SDL_GetTicks();
                int good = dice(10, 1) + skill;
                int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                // playerDice = good;
               //  foeDice = bad;
                int damage = 1;


                if (alternate3 != "") { damage += getItem(alternate3).bonus; }

                //Process player attack
                if (good > bad) {
                    NPC tmpNPC = tmpNPCs.front();
                    fightPlayer = "Has herido a " + tmpNPC.description + " por " + to_string(damage) + " puntos de vida";
                    tmpNPC.stamina -= damage;
                    if (tmpNPC.stamina <= 0) {
                        fightPlayer += ", Has derrotado a " + tmpNPC.description + "!!";
                        //Mix_PlayChannel(-1, audioMaleDeath, 0);
                        addExp(tmpNPC.exp);
                        dropLoot(tmpNPC.x, tmpNPC.y, tmpNPC.exp);
                        tmpNPCs.pop_front();


                        killCount++;
                        addAchievement("Guillermo Tell", my_enums::_COMBAT_);
                        if (killCount == 10)addAchievement("Le estas pillando el punto", my_enums::_COMBAT_);
                        if (killCount == 50)addAchievement("Massacre", my_enums::_COMBAT_);
                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }
                }
                else {
                    fightPlayer = "Has fallado";
                }

                //Counter attack from enemy
                enemyAttacked = true;
                //enemyAttack();

            }//bow button click

            //subterfuge attack
            if (hideButton.clicked(mousex, mousey)) {
                addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100, 1, buttonHideTexture);
                Mix_PlayChannel(-1, sword, 1);
                turn++;
                lastTurn = SDL_GetTicks();
                NPC tmpNPC = tmpNPCs.front();
                int damage = 2;
                if (alternate3 != "") { damage += getItem(alternate3).bonus; }


                int goodLuck = dice(10, 1) + luck;
                int badLuck = dice(10, 1) + tmpNPCs.begin()->luck;
                if (goodLuck > badLuck) {
                    fightPlayer = "Ataque por sorpresa a " + tmpNPC.description + " por " + to_string(damage+1) + " puntos de vida";
                    tmpNPC.stamina -= (damage + 1);
                    if (tmpNPC.stamina <= 0) {
                        fightPlayer += ", Has derrotado a " + tmpNPC.description + "!!";
                        addExp(tmpNPC.exp);
                        dropLoot(tmpNPC.x, tmpNPC.y, tmpNPC.exp);
                        tmpNPCs.pop_front();

                        killCount++;
                        addAchievement("Asesino", my_enums::_COMBAT_);
                        if (killCount == 10)addAchievement("La fuerza crece en ti", my_enums::_COMBAT_);
                        if (killCount == 50)addAchievement("Desintegrar", my_enums::_COMBAT_);


                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }
                }
                else {
                    
                    int enemyDamage = tmpNPCs.begin()->damage;
                    stamina -= enemyDamage;
                    fightFoe = tmpNPCs.begin()->description + "Te han descubierto y te dañan por " + to_string(enemyDamage) + " puntos de vida";
                    if (stamina <= 0) {
                        fightFoe += ", Has sido derrotado por " + tmpNPCs.begin()->description + "!!";
                        setState(my_enums::S_HERO_);
                        changeMusic();
                        deleteNPCs(px, py);
                        addAchievement("Tu personaje ha muerto", my_enums::_HIDDEN_);
                    }
                }



            }//stealth button click

            //Health potion
            if (potions_health > 0) {
                if (potionHealthButton.clicked(mousex, mousey)) {
                    Mix_PlayChannel(-1, heal, 1);
                    addAnimation(potionHealthButton.getRect().x, potionHealthButton.getRect().y, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonPotionHealthTexture);
                    potions_health--;
                    stamina += 10;
                    fightPlayer = "Te curas por 10 puntos de vida";
                    if (stamina > max_stamina)stamina = max_stamina;
                    turn++;
                    lastTurn = SDL_GetTicks();
                    addAchievement("Poción de vida", my_enums::_OPTIONS_);

                    int damage = 1;
                    //Counter attack from enemy
                    enemyAttacked = true;
                    //enemyAttack();
                }
            }

            //Power potion
            if (potions_power > 0) {

                if (potionMagicButton.clicked(mousex, mousey)) {
                    Mix_PlayChannel(-1, heal, 1);
                    addAnimation(potionMagicButton.getRect().x, potionMagicButton.getRect().y, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonPotionMagicTexture);
                    potions_power--;
                    power += 10;
                    if (power > max_power)power = max_power;
                    turn++;
                    fightPlayer = "Recuperas 10 puntos de magia";
                    lastTurn = SDL_GetTicks();
                    addAchievement("Poción de mágia", my_enums::_OPTIONS_);




                    int damage = 1;
                    //Counter attack from enemy
                    enemyAttacked = true;
                    //enemyAttack();
                }
            }

            //magic attack
            if (spellButton.clicked(mousex, mousey)) {
                if (power > 0) {
                    Mix_PlayChannel(-1, magic, 1);
                    addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100, 1, buttonSpellTexture);
                    turn++;
                    lastTurn = SDL_GetTicks();
                    int damage = 1;
                    int attackPower = dice(power, 1);

                    NPC tmpNPC = tmpNPCs.front();
                    if (attackPower > tmpNPC.stamina)attackPower = tmpNPC.stamina;
                    fightPlayer = "Has herido a " + tmpNPC.description + " por " + to_string(attackPower) + " puntos de vida";
                    tmpNPC.stamina -= attackPower;
                    power -= attackPower;
                    if (tmpNPC.stamina <= 0) {
                        fightPlayer += ", Has derrotado a " + tmpNPC.description + "!!";
                        addExp(tmpNPC.exp);
                        dropLoot(tmpNPC.x, tmpNPC.y, tmpNPC.exp);
                        tmpNPCs.pop_front();

                        magicKill++;
                        addAchievement("Abracadabra", my_enums::_COMBAT_);
                        if (magicKill == 10)addAchievement("La fuerza crece en ti", my_enums::_COMBAT_);
                        if (magicKill == 50)addAchievement("Desintegrar", my_enums::_COMBAT_);


                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }

                    //Counter attack from enemy
                    enemyAttacked = true;
                    //enemyAttack();

                }//player has power
            }//magic attack

            //drain attack
            if (drainButton.clicked(mousex, mousey)) {
                if (power > 0) {
                    Mix_PlayChannel(-1, magic, 1);
                    addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100, 1, buttonDrainTexture);
                    turn++;
                    lastTurn = SDL_GetTicks();
                    int damage = 1;
                    int attackPower = dice(power/2, 1);

                    NPC tmpNPC = tmpNPCs.front();
                    if (attackPower > tmpNPC.stamina)attackPower = tmpNPC.stamina;

                    fightPlayer = "Has herido a " + tmpNPC.description + " por " + to_string(attackPower) + " puntos de vida";
                    
                    if (attackPower < 1)attackPower = 1;
                    tmpNPC.stamina -= attackPower;
                    stamina += attackPower;
                    if (stamina > max_stamina)stamina = max_stamina;

                    power -= attackPower;
                    if (tmpNPC.stamina <= 0) {
                        fightPlayer += ", Has derrotado a " + tmpNPC.description + "!!";
                        addExp(tmpNPC.exp);
                        dropLoot(tmpNPC.x, tmpNPC.y, tmpNPC.exp);
                        tmpNPCs.pop_front();

                        magicKill++;
                        addAchievement("Necromante", my_enums::_COMBAT_);
                        if (magicKill == 10)addAchievement("La fuerza crece en ti", my_enums::_COMBAT_);
                        if (magicKill == 50)addAchievement("Desintegrar", my_enums::_COMBAT_);


                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }

                    //Counter attack from enemy
                    enemyAttacked = true;
                    //enemyAttack();


                }//player has power
            }//drain attack

        }

            if (tmpNPCs.size() == 0) {
                turn = 0;
                deleteNPCs(px, py);
                setState(previousScreen);
                changeMusic();
                clearAnimations();
                enemyAttacked = false;
            }

        }

        //******************
    }//events

}

void game::addExp(int xp)
{
    exp += xp;
    //increase level;
    if (exp >= ((level * level) * 100)) {
        skillPoints+=2;
        exp -= ((level * level) * 100);
        level++;
        addNotification("Subes al nivel:" + to_string(level), { 0,0,0 });
        Mix_PlayChannel(-1, win, 0);
        addAchievement("Level Up!", my_enums::_COMBAT_);
    }
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
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            //******
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                    addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
                }
            }

            if (continueButton.clicked(mousex, mousey)) {
                setState(previousScreen);

            }

            if (achievementGroup0Button.clicked(mousex, mousey)) {
                achievementGroup = 0;
                addAchievement("Naturaleza curiosa", my_enums::_HIDDEN_);
            }

            if (achievementGroup1Button.clicked(mousex, mousey)) {
                achievementGroup = 1;
                addAchievement("Naturaleza curiosa", my_enums::_HIDDEN_);
            }

            if (achievementGroup2Button.clicked(mousex, mousey)) {
                achievementGroup = 2;
                addAchievement("Naturaleza curiosa", my_enums::_HIDDEN_);
            }

            if (achievementGroup3Button.clicked(mousex, mousey)) {
                achievementGroup = 3;
                addAchievement("Naturaleza curiosa", my_enums::_HIDDEN_);

             
            }

           

        }

        //******************
    }


}

void game::eventsHomeTown()
{
    bool foundNPC = false;
    left = false;
    right = false;
    up = false;
    down = false;

    SDL_Event e;
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Mix_PlayChannel(-1, audioButton, 0);
            setState(my_enums::S_GAMEOVER_);
            Mix_PlayMusic(musicGameOver, -1);
            timerGameOver = SDL_GetTicks();
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&mousex, &mousey);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            foundNPC = checkNPC(mousex,mousey);

            if (exitButton.clicked(mousex, mousey)) {
                if (confirmExit() == true) {
                    Mix_PlayChannel(-1, audioButton, 0);
                    setState(my_enums::S_GAMEOVER_);
                    Mix_PlayMusic(musicGameOver, -1);
                    timerGameOver = SDL_GetTicks();
                }
            }//exit button
            

            if (configButton.clicked(mousex, mousey)) {
                previousScreen = getStringState(currentState);
                setState(my_enums::S_CONFIGMENU_);
            }//config button

            if (achievementsButton.clicked(mousex, mousey)) {
                previousScreen = getStringState(currentState);
                setState(my_enums::S_ACHIEVEMENTS_);
            }

            if (inventoryButton.clicked(mousex, mousey)) {
                previousScreen = getStringState(currentState);
                setState(my_enums::S_INVENTORY_);
            }//inventory button

            if (playerButton.clicked(mousex, mousey)) {             
                previousScreen = getStringState(currentState);
                setState(my_enums::S_PLAYER_);
            }//inventory button

            if(food>0){
            if (foodButton.clicked(mousex, mousey)) {
                int tx = px - cam_x;
                int ty = py - cam_y;

                addAnimation(foodButton.getRect().x, foodButton.getRect().y, (gScreenSurface->w / cam_size_x) * tx, (gScreenSurface->h / cam_size_y) * ty, gScreenSurface->w / cam_size_x, gScreenSurface->h / cam_size_y, 1, buttonFoodTexture);
                addAchievement("Estomago lleno", my_enums::_OPTIONS_);

                food--;
                stamina += 4;
                if (stamina > max_stamina)stamina = max_stamina;
                
            }//food button
            }

            if (moveRightButton.clicked(mousex, mousey)) {
                right = true;
            }//config button

            if (moveLeftButton.clicked(mousex, mousey)) {
                left = true;
            }//config button

            if (moveDownButton.clicked(mousex, mousey)) {
                down = true;
            }//config button

            if (moveUpButton.clicked(mousex, mousey)) {
                up = true;
            }//config button

            if (mapButton.clicked(mousex, mousey)) {
                drawMiniMap();
                addAchievement("Topógrafo", my_enums::_OPTIONS_);
            }//map button


            if (newMapButton.clicked(mousex, mousey)) {
                
                //int width, height;
                //width = 256;
               // height = 256;
              //  baseMap.mymap.generate(rand() % 6 + 1, rand() % 6 + 1, 0.5f, 1, 1, width, height);

                switch (getState())
                {
                
                case my_enums::S_FOREST_WORLD_:
                {
                    currentState = my_enums::S_HOMETOWN_;
                    changeMap();
                    break;
                }

                case my_enums::S_ELEMENTAL_WATER_WORLD_:
                {
                    currentState = my_enums::S_FOREST_WORLD_;
                    px = 248;
                    py = 51;
                    changeMap();
                    break;
                }

                case my_enums::S_ELEMENTAL_FIRE_WORLD_:
                {
                    currentState = my_enums::S_ELEMENTAL_WATER_WORLD_;
                    changeMap();
                    break;
                }

                case my_enums::S_COAST_WORLD_:
                {
                    currentState = my_enums::S_ELEMENTAL_FIRE_WORLD_;
                    changeMap();
                    break;
                }
                case my_enums::S_HOMETOWN_:
                {
                    currentState = my_enums::S_COAST_WORLD_;
                    changeMap();
                    break;
                }

                default:
                {
                    // is likely to be an error
                }
                };


               // baseMap.mymap.to_surface(baseMap.imageSurface,getState());
               // baseMap.generateTiles(currentState);
               
               // updateMap();
               // baseMap.blur();

                changeMap();

            }//new map button

        }
        else if (e.type == SDL_KEYDOWN)
        {
            //Select surfaces based on key press
            switch (e.key.keysym.sym)
            {
            case SDLK_a:
                left = true;
                break;

            case SDLK_w:
                up = true;
                break;

            case SDLK_s:
                down = true;
                break;

            case SDLK_d:
                right = true;
                break;


            default:
                  break;
            }
        }//event management
        
    }

    tmpx = px;
    tmpy = py;
    if (right)tmpx++;
    if (left)tmpx--;
    if (up)tmpy--;
    if (down)tmpy++;
    if (!foundNPC)phaseNPCs();
}

bool game::checkNPC(int x, int y)
{
    bool foundNPC = false;
    int tmpx = cam_x + (x / (gScreenSurface->w / cam_size_x));
    int tmpy = cam_y + (y / (gScreenSurface->h / cam_size_y));
     
    tmpNPCs = getNPCs(tmpx, tmpy);
    if (tmpNPCs.size() > 0) {
        int Xx = 1;
        if (tmpNPCs.begin()->NPCAI == my_enums::_FRIENDLY_CHAT_) {
            tmpCHATs = getChat(tmpx, tmpy);
            previousScreen = getStringState(currentState);
            setState(my_enums::S_CHAT_);
            foundNPC = true;
        }
        else if (tmpNPCs.begin()->NPCAI == my_enums::_FRIENDLY_SHOP_) {
            tmpSHOPs = getShops(tmpx, tmpy);
            previousScreen = getStringState(currentState);
            setState(my_enums::S_SHOP_);
            foundNPC = true;
        }else if (tmpNPCs.begin()->NPCAI == my_enums::_FRIENDLY_MASTER_) {
            previousScreen = getStringState(currentState);
            setState(my_enums::S_MASTER_);
            foundNPC = true;
        }
    }
    return foundNPC;

}

void game::updateNPCandEVENTS(my_enums::gameState state) {
    if ((state != my_enums::S_HOMETOWN_)&& (state != my_enums::S_NECRO_WORLD_)){//only for maps outside hometown and Necroworld

        
        //Update teleport events
        if (state != my_enums::S_FOREST_WORLD_) {
        for (list<EVENT>::iterator it = EVENTs.begin(); it != EVENTs.end(); it++)
        {
            if ((it->description == "TELEPORT") && (it->map == state)) {//update teleporter location
                int x = 1, y = 1;
                baseMap.getLocation(&x, &y, 56);
                it->x = x;
                it->y = y;
            }

        }
    }

        if (state == my_enums::S_FOREST_WORLD_) {
            for (list<EVENT>::iterator it = EVENTs.begin(); it != EVENTs.end(); it++)
            {
                if ((it->description == "IF**MEDALLON OSCURO") && (it->map == state)) {//update teleporter location
                    int x = 1, y = 1;
                    baseMap.getLocation(&x, &y, 573);
                    it->x = x+3;
                    it->y = y;
                }

            }
        }

        //Update NPC locations
        for (list<NPC>::iterator it = NPCs.begin(); it != NPCs.end(); it++)
        {
            if ((it->id == 1000) && (it->map == state)) {//update shop location
                int x = 1, y = 1;
                baseMap.getLocation(&x, &y, 300);
                it->x = x;
                it->y = y;
            }

            if (state != my_enums::S_FOREST_WORLD_) {
                if ((it->id == 2000) && (it->map == state)) {//update wise person location
                    int x = 1, y = 1;
                    baseMap.getLocation(&x, &y, 570);
                    it->x = x;
                    it->y = y;
                }
            }

            if ((it->id == 3000) && (it->map == state)) {//update monster location
                int x = 1, y = 1;
                baseMap.getLocation(&x, &y, 573);
                it->x = x;
                it->y = y;
            }

        }


        //update chat lines location
        if (state != my_enums::S_FOREST_WORLD_) {
            for (list<CHAT>::iterator it = CHATs.begin(); it != CHATs.end(); it++)
            {
                if (it->map == state) {
                    int x = 1, y = 1;
                    baseMap.getLocation(&x, &y, 570);
                    it->x = x;
                    it->y = y;
                }
            }
        }

        //update shop items location
        for (list<SHOP>::iterator it = SHOPs.begin(); it != SHOPs.end(); it++)
        {
            if (it->map == state) {
                int x = 1, y = 1;
                baseMap.getLocation(&x, &y, 300);
                it->x = x;
                it->y = y;
            }
        }



        }

}

void game::changeMap()
{
    //Update music
    changeMusic();

    //**********************


    int width, height;
    width = 256;
    height = 256;
    baseMap.mymap.generate(rand() % 4 + 4, rand() % 7 +2 , 0.5f, width, height);

    baseMap.mymap.to_surface(baseMap.imageSurface, getState());
    baseMap.generateTiles(getState());

    updateNPCandEVENTS(getStringState(currentState));

    updateMap();
    baseMap.blur();
}

void game::addAnimation(int startx, int starty, int endx, int endy, int w, int h, int s, SDL_Texture* texture)
{
    animation anime;
    anime.startx = startx;
    anime.starty = starty;
    anime.endy = endy;
    anime.endx = endx;
    anime.w = w;
    anime.h = h;
    anime.texture = texture;
    anime.seconds = s;
    anime.ticks = SDL_GetTicks();
    anime.incx = abs(endx - startx) / desiredFPS;
    anime.incy = abs(endy - starty) / desiredFPS;

    animations.push_back(anime);

}

void game::playAnimations() {
    if(animations.size() >0) {
    for (list<animation>::iterator it = animations.begin(); it != animations.end(); it++)
    {
        if (it->startx > it->endx)it->startx-= it->incx;
        if (it->startx < it->endx)it->startx+= it->incx;
        if (it->starty > it->endy)it->starty-= it->incy;
        if (it->starty < it->endy)it->starty+= it->incy;

    }

    std::list<animation>::iterator i = animations.begin();
    while (i != animations.end())
    {

        bool animationEnd = (SDL_GetTicks() - (i)->ticks) > ((i)->seconds * 1000);
        if (animationEnd)
        {
            animations.erase(i++);  
        }
        else
        {
           
            SDL_Rect tmprect;
            tmprect.x = (i)->startx;
            tmprect.y = (i)->starty;
            tmprect.w = (i)->w;
            tmprect.h = (i)->h;
            SDL_RenderCopy(gRenderer, (i)->texture, NULL, &tmprect);
            ++i;
        }
    }
    }

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

    drawTileset(target, playersTexture, playerTile,20);
    
    drawIMGBoxSmall(target.x, target.y - 40, gScreenSurface->w / cam_size_x , 20 , stamina, max_stamina, { 200,0,0,0 });
    drawIMGBoxSmall(target.x, target.y - 20, gScreenSurface->w / cam_size_x, 20, power, max_power, { 128,0,128,0 });


}


void game::drawNPCs()
{
    SDL_Color NPCColor = { 200,0,0,0 };
    int tx, ty;
    SDL_Rect target;

    for (list<NPC>::iterator it = NPCs.begin(); it != NPCs.end(); it++)
    {
        if (currentState == it->map) {
            tx = it->x - cam_x;
            ty = it->y - cam_y;


            target.x = (gScreenSurface->w / cam_size_x) * tx;
            target.y = (gScreenSurface->h / cam_size_y) * ty;
            target.w = gScreenSurface->w / cam_size_x;
            target.h = gScreenSurface->h / cam_size_y;

           

         
            drawTileset(target, playersTexture, it->tile, 20);
            if ((it->NPCAI == my_enums::_FRIENDLY_CHAT_) || (it->NPCAI == my_enums::_FRIENDLY_SHOP_) || (it->NPCAI == my_enums::_FRIENDLY_MASTER_)) {
                drawTransparentSquare(target, { 0,200,0,0 });
                drawTransparentSquare({ target.x + 1,target.y + 1,target.w - 2,target.h - 2, }, { 0,200,0,0 });
            }

            if ((it->NPCAI == my_enums::_ENEMY_FOLLOW_) || (it->NPCAI == my_enums::_ENEMY_FOLLOW_MAGE_) || (it->NPCAI == my_enums::_ENEMY_RANDOM_) || (it->NPCAI == my_enums::_ENEMY_STATIC_) || (it->NPCAI == my_enums::_ENEMY_STATIC_MAGE_)) {
                drawTransparentSquare(target, { 200,0,0,0 });
                drawTransparentSquare({ target.x+1,target.y + 1,target.w  -2,target.h - 2, }, { 200,0,0,0 });
            }


            if ((it->NPCAI == my_enums::_FRIENDLY_CHAT_)|| (it->NPCAI == my_enums::_FRIENDLY_SHOP_) || (it->NPCAI == my_enums::_FRIENDLY_MASTER_)) {
                ty--;
                target.y = (gScreenSurface->h / cam_size_y) * ty;     
               
                SDL_RenderCopy(gRenderer, talkTexture, NULL, &target);


                SDL_Color fg = { 0,0,0,0 };
                SDL_Color bg = { 150,150,150,0 };
                SDL_Surface* text = TTF_RenderUTF8_Blended_Wrapped(smallFont, it->description.c_str(), fg, target.w);
                SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(gRenderer, text);
               
                SDL_RenderCopy(gRenderer, txtTexture, NULL, &target);

                SDL_FreeSurface(text);
                SDL_DestroyTexture(txtTexture);
                text = NULL;
                txtTexture = NULL;

                }
        }
    }
}//end

void game::drawTileset(SDL_Rect target,SDL_Texture *texture, int player, int columns)
{
    if (player != 0) {
        int sx;
        int sy;
        sy = player / columns;
        sx = player % columns;

        SDL_Rect playerSrc;
       // playerSrc.x = sx * 32 + 1;
       // playerSrc.y = sy * 32 + 1;
        playerSrc.x = sx * 32 ;
        playerSrc.y = sy * 32 ;
        playerSrc.w = 32;
        playerSrc.h = 32;
        SDL_RenderCopy(gRenderer,texture, &playerSrc, &target);


    }
}//End 

void game::checkBoundaries()
{
    if (px < 1) { px = 1; }
    if (py < 1) { py = 1; }
    if (px > 254) { px = 254; }
    if (py > 254) { py = 254; }

    if (px <= cam_x+1) { cam_x--; }
    if (py <= cam_y+1) { cam_y--; }
    if (px >= (cam_x-1 + cam_size_x - 1)) { cam_x++; }
    if (py >= (cam_y-1 + cam_size_y - 1)) { cam_y++; }

}


void game::addItem(string name, string description, int count, int value, int tile, my_enums::itemTypes type, int bonus)
{
    item aItem;
    aItem.name = name;
    aItem.description = description;
    aItem.count = count;
    aItem.value = value;
    aItem.tile = tile;
    aItem.type = type;
    aItem.bonus = bonus;

    items.push_back(aItem);
    addNotification("Has conseguido:" + name, { 0,0,0 });
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


item game::getItem(string name)
{

    bool found = false;
    item aItem;
    for (list<item>::iterator it = items.begin(); it != items.end(); it++)
    {
        if (it->name == name)
        {
            found = true;
            aItem.name = it->name;
            aItem.bonus = it->bonus;
            aItem.count = it->count;
            aItem.type = it->type;
            aItem.tile = it->tile;
            aItem.value = it->value;
            break;
        }
    }
    return aItem;

}
