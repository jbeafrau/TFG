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
            if (it->id == id){
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
        if ((x == it->x) && (y == it->y)) {
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
         //addNotification("There are NPCS here...");
         //currentMusic = 5;
        
         if (tmpNPCs.begin()->NPCAI == my_enums::_FRIENDLY_CHAT_) {
             tmpCHATs = getChat(tmpx, tmpy);
             previousScreen = my_enums::_HOMETOWN_;
             setState(my_enums::_CHAT_);

         }
         else if (tmpNPCs.begin()->NPCAI == my_enums::_FRIENDLY_SHOP_) {
             tmpSHOPs = getShops(tmpx, tmpy);
             previousScreen = my_enums::_HOMETOWN_;
             setState(my_enums::_SHOP_);

         }else if (tmpNPCs.begin()->NPCAI == my_enums::_ENEMY_STATIC_ || tmpNPCs.begin()->NPCAI == my_enums::_ENEMY_RANDOM_ || tmpNPCs.begin()->NPCAI == my_enums::_ENEMY_FOLLOW_) {
             if (getState() != my_enums::_FIGHT_) {
                 px = tmpx;
                 py = tmpy;
                 checkBoundaries();
                 updateMap();
                 Mix_PlayMusic(musicBATTLE, -1);
                 previousScreen = my_enums::_HOMETOWN_;
                 setState(my_enums::_FIGHT_);
             }
         }
        


     }
     else {
         if(!collide(tmpx,tmpy)){
         px = tmpx;
         py = tmpy;
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
    aNPC.exp = ((aNPC.skill + aNPC.stamina) / 10) + 1;
    aNPC.NPCAI = NPCAI;
    aNPC.tile = tile;
    aNPC.boundaries = boundaries;
    NPCs.push_back(aNPC);

}

void game::addShop(int id, int x, int y, int option, std::string description, int value, std::string  description2, int value2, int tile, my_enums::itemTypes type, int bonus)
{
    SHOP aShop;
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
        if ((x == it->x) && (y == it->y)) {

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


void game::addChat( int x, int y, int option, std::string question, std::string  answer)
{
    CHAT achat;
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
                                    //popup(tmpStr);
                                    addNotification(tmpStr);
                                    //break;
                                }
                                else {
                                    tmpStr = "No tienes eso..";
                                    //popup(tmpStr);
                                    addNotification(tmpStr);
                                    //break;
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
                                        //popup(tmpStr);
                                        addNotification(tmpStr);
                                        food += it->value;
                                        // break;
                                    }
                                    else  if (it->description == "POCION VITALIDAD") {
                                        tmpStr = tmpStr + it->description;
                                        //popup(tmpStr);
                                        addNotification(tmpStr);
                                        potions_health += it->value;
                                        // break;
                                    }else  if (it->description == "POCION PODER") {
                                        tmpStr = tmpStr + it->description;
                                        //popup(tmpStr);
                                        potions_power += it->value;
                                        // break;
                                    }else                               
                                    {
                                        tmpStr = tmpStr + it->description;
                                        //popup(tmpStr);
                                        addNotification(tmpStr);


                                        addItem(it->description, it->description, it->value, it->value2, it->tile, it->type, it->bonus);

                                        //addItem(it->description, it->value);
                                       // myBook.cleanShop(currentlocation, it->option);

                                        shop_x = tmpx;
                                        shop_y = tmpy;
                                        shop_option = it->option;
                                        //cleanShop(tmpx, tmpy, it->option);
                                       // tmpShop = -1;
                                      //  tmpSHOPs = getShops(tmpx, tmpy);
                                       // break;

                                    }

                                }
                                else {
                                    addNotification("No tienes dinero suficiente");
                                }//gold
                                if (it->description2 == "FOOD" and food >= it->value2) {
                                    tmpStr = "Compras ";
                                    if (it->description == "FOOD") {
                                        tmpStr = tmpStr + " raciones de comida";
                                        //popup(tmpStr);
                                        addNotification(tmpStr);
                                        food += it->value;
                                    }
                                    else {
                                        // addItem(it->description, it->value);
                                        addItem(it->description, it->description, it->value, it->value2, it->tile, it->type, it->bonus);
                                        tmpStr = tmpStr + it->description;
                                    }
                                    //popup(tmpStr);
                                    addNotification(tmpStr);
                                    food -= it->value2;
                                    shop_x = tmpx;
                                    shop_y = tmpy;
                                    shop_option = it->option;
                                    //cleanShop(tmpx,tmpy, it->option);
                                   // tmpShop = -1;
                                    
                                    


                                }//food
                            }//compra / venta
                            
                            //tmpSHOPs.size();
                            //tmpShops = myBook.getShops(currentlocation);
                           // tmpSHOPs = getShops(tmpx, tmpy);
                            //tmpSHOPs.size();
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

    //Info to cleanup shop options
    //int shop_x = 0;
    //int shop_y = 0;
    //int shop_option = 0;

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
                        //addNotification(it->answer);
                        currentChat = it->answer;
                        
                      
                    }  //found option
                }//for shops

               // if (shop_x != 0) {
               //     cleanShop(shop_x, shop_y, shop_option);
                //}

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


void game::monsterGenerator()
{
    //todo
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

}


bool game::collide(int x, int y)
{
    bool tmp = false;
    if (baseMap.get_cell(x,y) == 9) tmp = true;
    if (baseMap.get_cell(x, y) == 579) tmp = true;


    std::list<EVENT>::iterator it = EVENTs.begin();
    while (it != EVENTs.end())
    {
        bool erased = false;
        if (it->map == currentState) {//only process events in current map
            if ((it->description.substr(0, 4) == "IF**") && (it->x == x) && (it->y == y)) {


                if (it->map == currentState) {//on the current map
                    if (findItem(it->description.substr(4, it->description.length() - 4))) {
                        addNotification("Se abre el camino");
                        //GLOBAL_EVENTs.erase(it);
                        it = EVENTs.erase(it);
                        erased = true;
                        tmp = false;
                    }
                    else {
                        tmp = true;

                        addNotification("No puedes pasar");
                    }
                }


            }
        }
        if ((it != EVENTs.end()) && (erased == false))it++;
    }


    return tmp;
}

void game::loadNPCs()
{
    NPCs.clear();


    addNPC(1, 110, 170, my_enums::_HOMETOWN_, "Monstruo", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_STATIC_, dice(300, 2), {1,1,255,255});
    addNPC(2, 111, 170, my_enums::_HOMETOWN_, "Monstruo", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_FOLLOW_, dice(300, 2), { 1,1,255,255 });
    addNPC(3, 112, 170, my_enums::_HOMETOWN_, "Monstruo poderoso", dice(10, 10), dice(10, 10), dice(10, 5), dice(10, 5), dice(3, 2), my_enums::_ENEMY_STATIC_, dice(300, 2), { 1,1,255,255 });
    addNPC(4, 113, 170, my_enums::_HOMETOWN_, "Monstruo", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_STATIC_, dice(300, 2), { 1,1,255,255 });
   
    addNPC(10, 136,155, my_enums::_HOMETOWN_, "Esqueleto", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, 219, { 130,150,150,170 });
    addNPC(10, 136,161, my_enums::_HOMETOWN_, "Esqueleto", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, 219, { 130,150,150,170 });
    addNPC(10, 142,155, my_enums::_HOMETOWN_, "Esqueleto", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, 219, { 130,150,150,170 });
    addNPC(10, 142,161, my_enums::_HOMETOWN_, "Esqueleto", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, 219, { 130,150,150,170 });
    addNPC(11, 139, 167, my_enums::_HOMETOWN_, "Nigromante", dice(5, 10), dice(5, 15), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, 160, { 130,150,150,170 });

    addNPC(5, 106, 155, my_enums::_HOMETOWN_, "Tienda de comida", 1, 1, 1,1,1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(6, 112, 155, my_enums::_HOMETOWN_, "Tienda de armas", 1, 1,1,1,1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(7, 106, 161, my_enums::_HOMETOWN_, "Tienda de armaduras", 1, 1,1,1,1, my_enums::_FRIENDLY_SHOP_, 74, { 1,1,255,255 });
    addNPC(8, 112, 161, my_enums::_HOMETOWN_, "Tienda de pociones", 1, 1,1,1,1, my_enums::_FRIENDLY_SHOP_, 79, { 1,1,255,255 });


    //Add a dog
    addNPC(10001, 118, 122, my_enums::_HOMETOWN_, "Tor el perro", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_FRIENDLY_STATIC_, 269, { 1,1,255,255 });


    //Add 30 peasants in the town
    for (int x = 0; x < 30;x++)addNPC(1, dice(30, 90), dice(50, 120), my_enums::_HOMETOWN_, "Campesino", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_FRIENDLY_RANDOM_, 74, { 90,130,120,170 });
    


    addNPC(19, 109, 142, my_enums::_HOMETOWN_, "Sabio del pueblo", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(20, 119, 142, my_enums::_HOMETOWN_, "Mago del templo elemental", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 83, { 1,1,255,255 });
    addNPC(21, 107, 143, my_enums::_HOMETOWN_, "Tutorial equipo", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(21, 111, 143, my_enums::_HOMETOWN_, "Tutorial magia", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(22, 107, 145, my_enums::_HOMETOWN_, "Tutorial combate", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });
    addNPC(23, 111, 145, my_enums::_HOMETOWN_, "Tutorial tiendas", 1, 1, 1, 1, 1, my_enums::_FRIENDLY_CHAT_, 77, { 1,1,255,255 });


    //addNPC(8, 83, 71, my_enums::_HOMETOWN_, "Tienda del pueblo5", 1, 1,1,1, my_enums::_FRIENDLY_SHOP_, 74);

}

void game::loadEvents()
{
    addEvent(139, 163, my_enums::_HOMETOWN_, "TELEPORT", 0, 0, 139, 166, my_enums::_HOMETOWN_, 0);
    addEvent(139, 165, my_enums::_HOMETOWN_, "TELEPORT", 0, 0, 139, 162, my_enums::_HOMETOWN_, 0);

    addEvent(139, 167, my_enums::_HOMETOWN_, "GOLD", 50, 0, 0, 0, my_enums::_HOMETOWN_, 344);
    addEvent(139, 168, my_enums::_HOMETOWN_, "GOLD", 50, 0, 0, 0, my_enums::_HOMETOWN_, 344);

    addEvent(251, 151, my_enums::_HOMETOWN_, "IF**LLAVE PUERTA ESTE", 50, 0, 0, 0, my_enums::_HOMETOWN_, 0);

    addEvent(253, 151, my_enums::_HOMETOWN_, "TELEPORT", 0, 0, 10, 10, my_enums::_COAST_WORLD_, 0);

    addGlobalEvent(1, my_enums::_HOMETOWN_, { 117,121,119,123 }, { 0,0,0,0 }, 0, 10001, "CHANGE_AI_FRIENDLY_FOLLOW");
    addGlobalEvent(2, my_enums::_HOMETOWN_, { 118,141,120,143 }, { 0,0,0,0 }, 0, 10001, "CHANGE_AI_FRIENDLY_STATIC");
    addGlobalEvent(3, my_enums::_HOMETOWN_, { 118,141,120,143 }, { 0,0,0,0 }, 0, 10001, "REMOVE_CHAT");
    addGlobalEvent(4, my_enums::_HOMETOWN_, { 118,141,120,143 }, { 0,0,0,0 }, 0, 10001, "ADD_CHAT");
    addGlobalEvent(5, my_enums::_HOMETOWN_, { 118,141,120,143 }, { 0,0,0,0 }, 0, 10001, "ADD_ITEM");

    
}


void game::loadChats()
{
    CHATs.clear();
    addChat(109,142, 1, "¿Que está pasando en el pueblo?", "Un mal ha venido de otro plano y esta generando caos en el pueblo");
    addChat(109, 142, 2, "¿Que puedo hacer?", "Equipate y preparate para luchar contra el mal");
    addChat(109, 142, 3, "Cuentame algo del pueblo", "Al sur encontraras varias tiendas, al este la costa y siguiendo el camino del noroeste, el bosque oscuro");
    addChat(109, 142, 4, "¿Por donde comienzo?", "Al sur y al este del pueblo hay enemigos que no permiten salir a los ciudadanos ");

    addChat(119, 142, 1, "¿Que es esto?", "Esta es la sala de los elementos, pero aun no estas preparado para luchar al mal");
    addChat(119, 142, 2, "¿Puedo ayudar?", "Tor, mi perro esta en una choza al norte, con tantos monstruos no me atrevo a ir a por el, ¿Podrias traermelo por favor??, si lo haces te daré la llave que abre la puerta del este");


    addChat(107, 143, 1, "¿Como accedo a mi equipo?", "Pulsa el icono de la mochila en la pantalla de juego, está en la parte de arriba de la pantalla");
    addChat(107, 143, 2, "¿Que quiere decir EQUIPADO?", "Hay piezas de equipo que puedes usar para mejorar tus habilidades como armas, armaduras u anillos mágicos, para equiparla, tan solo tienes que pinchar sobre el equipo, no puedes usar escudos cuando usas armas de dos manos o de ataques a distancia");
    addChat(107, 143, 3, "¿Como puedo obtener equipo?", "De tres maneras diferentes: comprandolo en las tiendas, derrotando enemigos y a veces también lo encontrarás en el mapa");
    
    addChat(111, 143, 1, "¿Como se gasta la magia?", "La magia engloba varias habilidades que suponen ventajas en el combate, tanto los magos, necromantes e invocadores pueen gastar esas habilidades");
    addChat(111, 143, 2, "¿Como saber lo poderoso que soy?", "La magia depende de un atributo: Poder, cuanto más tengas más fuerte seran tus ataques mágicos y más ataques podras realizar antes de agotar tu reserva");
    addChat(111, 143, 3, "¿Tiene límite la magia?", "Lamentablemente la magia consume tu poder a grán velocidad y cuando se agota, no podrás usarla");
    addChat(111, 143, 4, "¿Se puede recuperar el poder?", "Cuando no estás en combate la mágia se regenera a un ritmo de un punto cada 30 segundos (- el poder máximo), durante el combate si usas una poción de poder recuperas instantáneamente 10 puntos");

    addChat(107, 145, 1, "¿Como funciona el combate?", "En el mapa verás enemigos, si intentas entrar en su ubicación u ellos te alcanzan entrarás en combate, una vez entres en combate en la parte izquierda veras los enemigos con sus habilidades, el combate es por turnos y solo termina cuando terminas con tus enemigos u ellos contigo");
    addChat(107, 145, 2, "¿Como ataco?", "Todos los arquetipos pueden realizar ataques de melee, para hacerlo tan solo hay que pulsar el boton de la espada, además otros personajes tienen otro tipos de ataques, como los ataques a distancia, con subterfugio o magia ");
    addChat(107, 145, 3, "¿Cuando derroto a los enemigos?", "Cuando la barra roja de los enemigos se consume los derrotas, lo mismo sirve para tu personaje");
    addChat(107, 145, 4, "¡Ayuda!, ¡Me han herido!", "Hay dos maneras de recuperar vitalidad, fuera de combate consumiendo raciones de comida (el icono del pán que recupera 4 puntos de vida por ración) y dentro del combate las pociones de vitalidad que recuperan 10 puntos de vida");

    addChat(111, 145, 1, "¿Como funcionan las tiendas?", "En el mapa verás personajes amigos, si intentas desplazarte en su dirección la pantalla de la tienda se abre");
    addChat(111, 145, 1, "¿Como compro el quipo?", "Es tan sencillo como pulsar sobre el objeto deseado, si tienes suficiente dinero perderas dicho dinero pero se añade a tu equipo el objeto");
    addChat(111, 145, 1, "¿Solo se puede pagar con dinero?", "A veces podrás cambiar un objeto por otro");
    addChat(111, 145, 1, "¿Se puede vender equipo?", "A veces los comerciantes te compraran equipo especial, como pueden ser gemas");



}

void game::loadShops()
{

    SHOPs.clear();

    addShop(1, 106, 155, 1, "FOOD", 1, "GOLD", 4, 207,my_enums::_OTHER_,0);

    addShop(2, 112, 155, 1, "ESPADA", 1, "GOLD", 10, 44, my_enums::_WEAPON_1H_, 1);
    addShop(2, 112, 155, 2, "ESPADA 2M", 1, "GOLD", 25, 40, my_enums::_WEAPON_2H_, 2);
    addShop(2, 112, 155, 3, "DAGA", 1, "GOLD", 5, 30, my_enums::_WEAPON_1H_, 0);
    addShop(2, 112, 155, 4, "ARCO", 1, "GOLD", 10, 59, my_enums::_WEAPON_RANGED_, 1);
    addShop(2, 112, 155, 5, "BALLESTA", 1, "GOLD", 25, 60, my_enums::_WEAPON_RANGED_, 2);

    addShop(3, 106, 161, 1, "ARMADURA CUERO", 1, "GOLD", 10, 114, my_enums::_ARMOR_, 1);
    addShop(3, 106, 161, 2, "CASCO CUERO", 1, "GOLD", 5, 160, my_enums::_HELMET_, 1);
    addShop(3, 106, 161, 3, "ESCUDO CUERO", 1, "GOLD", 5, 151, my_enums::_SHIELD_, 1);

    addShop(4, 112, 161, 1, "POCION VITALIDAD", 1, "GOLD", 25, 276, my_enums::_OTHER_, 0);
    addShop(4, 112, 161, 2, "POCION PODER", 1, "GOLD", 25, 273, my_enums::_OTHER_, 0);
    addShop(4, 112, 161, 3, "ANILLO HABILIDAD", 1, "GOLD", 50, 220, my_enums::_RING_, 0);

    /*
        addShop(36, 79, 71, 1 , "**ANILLO DE INVISIBILIDAD" , 1 , "GOLD", 10,229);
        addShop(36, 79, 71, 2 , "**ANILLO DE FUEGO" , 1 , "GOLD", 8,238);
        addShop(36, 79, 71, 3 , "**ANILLO DE HIELO" , 1 , "GOLD", 7,231);
        addShop(52, 80, 71, 1 , "**CUCHILLO ARROJADIZO" , 1 , "GOLD", 4,30);
        addShop(52, 80, 71, 2 , "**CUERDA ESCALAR" , 1 , "GOLD", 2,1);
        addShop(52, 80, 71, 3 , "**GANCHO DE CARNICERO" , 1 , "GOLD", 2,30);
        addShop(52, 80, 71, 4 , "**PINCHO DE HIERRO" , 1 , "GOLD", 1,30);
        addShop(52, 80, 71, 5 , "**LINTERNA" , 1 , "GOLD", 3,329);
        addShop(287, 81, 71, 1 , "FOOD" , 1 , "GOLD", 4,207);
        addShop(293, 82, 71, 1 , "**PETALOS MAGICOS" , 10 , "FOOD", 2,1);
        addShop(293, 82, 71, 2 , "**PETALOS MAGICOS" , 10 , "GOLD", 2,1);
        addShop(302, 83, 71, 1 , "**GEMA" , -2 , "GOLD", 9,338);
            */

}

void game::setButtonDefaults()
{
    //Set button´s size, position and caption
    titleButton.setButton(gScreenSurface->w / 2 - 450, gScreenSurface->h / 4, 900, 100, "");

    itemSelectedButton.setButton(gScreenSurface->w / 2 - 200, 350, 400, gScreenSurface->h - 350, " ");

    achievementGroup0Button.setButton(gScreenSurface->w / 2 - 250, 200, 100, 50, "");
    achievementGroup1Button.setButton(gScreenSurface->w / 2 - 250, 250, 100, 50, "");
    achievementGroup2Button.setButton(gScreenSurface->w / 2 - 250, 300, 100, 50, "");
    achievementGroup3Button.setButton(gScreenSurface->w / 2 - 250, 350, 100, 50, "");
    achievementGroup4Button.setButton(gScreenSurface->w / 2 - 250, 400, 100, 50, "");

    /*
    moveLeftButton.setButton(0, gScreenSurface->h / 2, 128, 128, "A");
    moveLeftButton.setColor(0, 0, 200);
    moveRightButton.setButton(276 , gScreenSurface->h / 2, 128, 128, "D");
    moveRightButton.setColor(0, 0, 200);
    moveUpButton.setButton(138, gScreenSurface->h / 2 -138, 128, 128, "W");
    moveUpButton.setColor(0, 0, 200);
    moveDownButton.setButton(138, gScreenSurface->h / 2, 128, 128, "S");
    moveDownButton.setColor(0, 0, 200);
    */

    moveLeftButton.setButton(gScreenSurface->w - btnx * 3, gScreenSurface->h / 4 * 3, btnx, btny, "A");
    moveLeftButton.setColor(0, 0, 200);
    moveDownButton.setButton(gScreenSurface->w - btnx * 2, gScreenSurface->h / 4 * 3, btnx, btny, "S");
    moveDownButton.setColor(0, 0, 200);
    moveRightButton.setButton(gScreenSurface->w - btnx, gScreenSurface->h / 4 * 3, btnx, btny, "D");
    moveRightButton.setColor(0, 0, 200);
    moveUpButton.setButton(gScreenSurface->w - btnx * 2, gScreenSurface->h / 4 * 3 - btny, btnx, btny, "W");
    moveUpButton.setColor(0, 0, 200);

    /*
    nextButton.setButton(gScreenSurface->w / 2 +128, gScreenSurface->h /2, 128, 128, "Siguiente");
    nextButton.setColor(128, 128, 128);
    prevButton.setButton(gScreenSurface->w / 2 - 256, gScreenSurface->h /2, 128, 128, "Anterior");
    prevButton.setColor(128, 128, 128);
    */
    nextButton.setButton(gScreenSurface->w / 2 + 128, gScreenSurface->h / 2, btnx * 2, btny * 2, "Siguiente");
    nextButton.setColor(128, 128, 128);
    prevButton.setButton(gScreenSurface->w / 2 - 256, gScreenSurface->h / 2, btnx * 2, btny * 2, "Anterior");
    prevButton.setColor(128, 128, 128);

    //exitButton.setButton(gScreenSurface->w - 128, 0, 128, 128, "Salir");
    exitButton.setButton(gScreenSurface->w - btnx, 0, btnx, btny, " ");
    exitButton.setColor(200, 0, 0);

    //  continueButton.setButton(gScreenSurface->w / 2 +32 , gScreenSurface->h - 128, 128, 128, "Continuar");
    continueButton.setButton(gScreenSurface->w / 2 - btnx, gScreenSurface->h / 4 * 3, btnx * 2, btny * 2, "Continuar");
    continueButton.setColor(0, 0, 200);

    // backButton.setButton(gScreenSurface->w / 2 - 160 , gScreenSurface->h - 128, 128, 128, "Volver");
    backButton.setButton(gScreenSurface->w / 2 - btnx * 3, gScreenSurface->h / 4 * 3, btnx * 2, btny * 2, "Volver");
    backButton.setColor(0, 0, 200);

    // rollButton.setButton(gScreenSurface->w / 2 - 64, gScreenSurface->h /2, 128, 128, "Volver a tirar");
    rollButton.setButton(gScreenSurface->w / 2 - btnx, gScreenSurface->h / 2, btnx * 2, btny * 2, "Volver a tirar");
    rollButton.setColor(0, 0, 200);

    //startButton.setButton(gScreenSurface->w / 2 -64, gScreenSurface->h / 2 -64, 128, 128, "Jugar");
    //startButton.setButton(gScreenSurface->w / 2 - btnx, gScreenSurface->h / 2 - 64, btnx*2, btny*2, "Jugar");
    startButton.setButton(gScreenSurface->w / 2 - btnx, gScreenSurface->h / 4 * 3, btnx * 2, btny * 2, "Jugar");

    startButton.setColor(100, 100, 100);

    player1Button.setButton(gScreenSurface->w / 2, gScreenSurface->h / 2 + 100, 200, 50, "Nombre Jugador");
    player1Button.setColor(100, 100, 100);

    /*
    configButton.setButton(1, gScreenSurface->h -128, 128, 128, "Configuración");
    configButton.setColor(100, 100, 100);
    inventoryButton.setButton(129, gScreenSurface->h - 128, 128, 128, "Inventario");
    inventoryButton.setColor(0, 0, 200);
    playerButton.setButton(257, gScreenSurface->h - 128, 128, 128, "Personaje");
    playerButton.setColor(0, 0, 200);
    mapButton.setButton(385, gScreenSurface->h - 128, 128, 128, "View map");
    mapButton.setColor(200, 200, 200);*/
    //foodButton.setButton(gScreenSurface->w - 128, gScreenSurface->h - 128, 128, 128, "Comer");

     //  achievementsButton.setButton(513, gScreenSurface->h - 128, 128, 128, "0 %");
    //achievementsButton.setButton(gScreenSurface->w / 2 - btnx, 1, btnx * 2, btny * 2, "0 %");
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




    newMapButton.setButton(gScreenSurface->w - 128, gScreenSurface->h / 2, 128, 128, "New map");
    newMapButton.setColor(200, 200, 200);

    musicButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 100, 200, 50, "Cambia canción");
    musicButton.setColor(100, 100, 100);
    muteButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 150, 200, 50, "MUSIC ON");
    muteButton.setColor(100, 100, 100);
    volumeSoundDownButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 200, 50, 50, "+");
    volumeSoundDownButton.setColor(100, 100, 100);
    volumeSoundButton.setButton(gScreenSurface->w / 2 + 50 - 100, gScreenSurface->h / 3 + 200, 100, 50, "SOUND 100%");
    volumeSoundButton.setColor(200, 200, 200);
    volumeSoundUpButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 3 + 200, 50, 50, "-");
    volumeSoundUpButton.setColor(100, 100, 100);
    volumeMusicDownButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 250, 50, 50, "+");
    volumeMusicDownButton.setColor(100, 100, 100);
    volumeMusicButton.setButton(gScreenSurface->w / 2 + 50 - 100, gScreenSurface->h / 3 + 250, 100, 50, "MUSIC 100%");
    volumeMusicButton.setColor(200, 200, 200);
    volumeMusicUpButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 3 + 250, 50, 50, "-");
    volumeMusicUpButton.setColor(100, 100, 100);
    blurButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 3 + 300, 200, 50, "BLUR ON");
    blurButton.setColor(100, 100, 100);

    playerUpButton.setButton(gScreenSurface->w / 2 - 100, gScreenSurface->h / 2 + 250, 50, 50, "+");
    playerUpButton.setColor(100, 100, 100);
    playerDownButton.setButton(gScreenSurface->w / 2 + 150 - 100, gScreenSurface->h / 2 + 250, 50, 50, "-");
    playerDownButton.setColor(100, 100, 100);



    mouseButton.setButton(gScreenSurface->w / 2 - 100, 0, 200, 50, "X:" + std::to_string(mousex) + " Y:" + std::to_string(mousey));
    mouseButton.setColor(100, 100, 100);

    /*
    fightButton.setButton(1,   gScreenSurface->h -128, 128, 128, "Atacar");
    bowButton.setButton(128, gScreenSurface->h - 128, 128, 128, "Arco");
    spellButton.setButton(256, gScreenSurface->h -128, 128, 128, "Magia");
    drainButton.setButton(384, gScreenSurface->h - 128, 128, 128, "Drenar");
    hideButton.setButton(512, gScreenSurface->h - 128, 128, 128, "Subterfugio");
    summonButton.setButton(640, gScreenSurface->h - 128, 128, 128, "Invocar");
    */
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

    chat0.setButton(50, gScreenSurface->h / 2, gScreenSurface->w - 100, 50, "0");
    chat1.setButton(50, gScreenSurface->h / 2 + 50, gScreenSurface->w - 100, 50, "1");
    chat2.setButton(50, gScreenSurface->h / 2 + 100, gScreenSurface->w - 100, 50, "2");
    chat3.setButton(50, gScreenSurface->h / 2 + 150, gScreenSurface->w - 100, 50, "3");
    chat4.setButton(50, gScreenSurface->h / 2 + 200, gScreenSurface->w - 100, 50, "4");
    chat5.setButton(50, gScreenSurface->h / 2 + 250, gScreenSurface->w - 100, 50, "5");

}

void game::start()
{
    
    btnx = gScreenSurface->w / 32;
    btny = gScreenSurface->h / 16;


    //Initialize timers
    timer.start();
    FPStimer.start();
    timer.pause();
    timerGameOver.start();
    timerGameOver.pause();

    ticksAI = SDL_GetTicks();

    loadPlayerDefault();

    loadNPCs();

    loadShops();

    loadChats();

    loadEvents();

   

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
    baseMap.generateTiles();

    baseMap.imageSurface = SDL_ConvertSurface(baseMap.imageSurface, gScreenSurface->format, 0);

    //baseMap.SetSurface();

    updateMap();

 
    // baseMap.targetSurface = SDL_ConvertSurface(baseMap.targetSurface, gScreenSurface->format, 0);

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

    case my_enums::_SHOP_:
    {
        eventsShops();
        break;
    }

    case my_enums::_CHAT_:
    {
        eventsChat();
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

    case my_enums::_SHOP_:
    {
        screenShops();
        break;
    }

    case my_enums::_CHAT_:
    {
        screenChat();
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


    aFile = images + "items_alpha.png";
    itemsSurface = loadSurface(aFile);
    SDL_SetColorKey(itemsSurface, SDL_TRUE, // enable color key (transparency)
        //SDL_MapRGB(playersSurface->format, 0xFF, 0, 0xFF)); // This is the color that should be taken as being the 'transparent' part of the image
        SDL_MapRGB(itemsSurface->format, 71, 108, 108)); // This is the color that should be taken as being the 'transparent' part of the image

    itemsTexture = SDL_CreateTextureFromSurface(gRenderer, itemsSurface);


    aFile = images + "terrain_alpha.png";
    locationsSurface = loadSurface(aFile);
    SDL_SetColorKey(locationsSurface, SDL_TRUE, // enable color key (transparency)
        //SDL_MapRGB(playersSurface->format, 0xFF, 0, 0xFF)); // This is the color that should be taken as being the 'transparent' part of the image
        SDL_MapRGB(locationsSurface->format, 255, 0, 255)); // This is the color that should be taken as being the 'transparent' part of the image

    locationsTexture = SDL_CreateTextureFromSurface(gRenderer, locationsSurface);
   
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
buttonBackTexture     = loadTexture(images + "anticlockwise-rotation.png");


buttonSwordTexture = loadTexture(images + "sword-brandish.png");
buttonBowTexture = loadTexture(images + "archer.png");
buttonSpellTexture = loadTexture(images + "spell-book.png");
buttonDrainTexture = loadTexture(images + "dead-head.png");
buttonHideTexture = loadTexture(images + "invisible.png");
buttonSummonTexture = loadTexture(images + "magic-hat.png");


buttonSoundOnTexture  = loadTexture(images + "speaker.png");
buttonSoundOffTexture = loadTexture(images + "speaker-off.png");

//classButton bowButton;
//classButton drainButton;

buttonUpTexture = loadTexture(images + "up-button.png");
buttonDownTexture = loadTexture(images + "down-button.png");
buttonLeftTexture = loadTexture(images + "left-button.png");
buttonRightTexture = loadTexture(images + "right-button.png");

buttonPlayerTexture = loadTexture(images + "histogram.png");


buttonPotionHealthTexture = loadTexture(images + "potion-health.png");
buttonPotionMagicTexture = loadTexture(images + "potion-magic.png");
buttonFoodTexture = loadTexture(images + "bread.png");


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
    /*destRect.x = 1;
    destRect.y = 1;
    destRect.w = 256;
    destRect.h = 256;

    */

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

    screenFlip();


    //SDL_Delay(3000);



    bool quit = false;
    while (!quit)
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

void game::drawIMGBoxSmall(int x, int y, int w, int h, int value, int max, SDL_Color color)
{
    int tmp;
    // SDL_Texture* IMGTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_Rect target;
    target.x = x;
    target.y = y;
   // target.w = 20;
    target.w = w / max;

    target.h = h;
    //SDL_RenderCopy(gRenderer, IMGTexture, NULL, &target);
    //drawBo
    if (value > 0) drawSquare(target, color);
    // SDL_Color fg = { 0,0,0,0 };

    while (value > 0) {
        //SDL_RenderCopy(gRenderer, IMGTexture, NULL, &target);
        drawSquare(target, color);
        target.x += w / max;
        value--;
    }

    color = { 0,0,0,0 };
    target.x = x;
    tmp = max;
    while (tmp > 0) {
        //drawTransparentSquare(&target ,color);
      //SDL_RenderCopy( gRenderer, surface, NULL, &target );
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

   /* tmpRect.w = 32;
    tmpRect.h = 32;
    tmpRect.x = gScreenSurface->w / 8;
    tmpRect.y = gScreenSurface->h / 8 - 50;
    tmpRect.w = 256;
    tmpRect.h = 50;
    tmpRect.x = gScreenSurface->w / 8;
    tmpRect.y = gScreenSurface->h / 8;
    */
    tmpRect.x = gScreenSurface->w / 2 - 250;
    tmpRect.y = gScreenSurface->h / 4;
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

    //drawPlayerTileset(tmpRect, playerTile);

    drawTileset(tmpRect, playersTexture, playerTile,20);

    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;

    drawTextResize("CAMBIAR NOMBRE E IMAGEN", tmpRect);
    tmpRect.y = 200;
    drawText("Introduce el nombre del jugador", tmpRect);

    tmpRect.y = 250;
    drawText(playerName, tmpRect);
    //tmpRect.y = 300;
   // tmpRect.y = gScreenSurface->h / 2 - 50;
    tmpRect.y = gScreenSurface->h / 4*3 -50;
    drawText("Cambia la imagen y luego pulsa continuar...", tmpRect);
}


void game::screenPlayerAttributes()
{



   // drawButton(exitButton);
   // drawButton(continueButton);
   // drawButton(rollButton);

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
/*
drawIMGBox(100, 200, skill, max_skill, { 0,0,200,0 });
  drawIMGBox(100, 250, stamina, max_stamina, { 200,0,0,0 });
  drawIMGBox(100, 300, power, max_power, { 128,0,128,0 });
  drawIMGBox(100, 350, luck, max_luck, { 0,200,200,0 });
*/
  drawIMGBox(gScreenSurface->w / 2 - 200, 200, skill, max_skill, { 0,0,200,0 });
  drawIMGBox(gScreenSurface->w / 2 - 200, 250, stamina, max_stamina, { 200,0,0,0 });
  drawIMGBox(gScreenSurface->w / 2 - 200, 300, power, max_power, { 128,0,128,0 });
  drawIMGBox(gScreenSurface->w / 2 - 200, 350, luck, max_luck, { 0,200,200,0 });

  
}


void game::screenPlayer()
{


    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
   // drawButtonSrc(backButton, buttonCancelTexture);

   // drawButtonSrc(rollButton, buttonRollDiceTexture);

    int tmpy = 100;
    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 0;
    tmpRect.w = 400;
    tmpRect.h = 50;
    //SDL_Rect target;
    //target.x = tmpRect.x - 50;
    //target.w = tmpRect.w + 40;
    //target.h = tmpRect.h + 25;


    drawTextResize("Ficha del personaje: " +playerName, tmpRect);


    //tmpRect.x = 100;
    tmpRect.x = gScreenSurface->w/2 - 200;
    tmpRect.y = 100;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawTextResize(getRaceName(currentRace) + " / "+ getArchetypeName(currentArchetype), tmpRect);
  //  drawTextResize("Raza: " + getRaceName(currentRace), tmpRect);

   // tmpRect.y = 150;
    //drawTextResize("Arquetipo: " + getArchetypeName(currentArchetype), tmpRect);

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


    drawIMGBox(gScreenSurface->w / 2 - 200, 200, skill, max_skill, { 0,0,200,0 });
    drawIMGBox(gScreenSurface->w / 2 - 200, 250, stamina, max_stamina, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2 - 200, 300, power, max_power, { 128,0,128,0 });
    drawIMGBox(gScreenSurface->w / 2 - 200, 350, luck, max_luck, { 0,200,200,0 });


    tmpRect.y = 500;
    drawText("Nivel:" + to_string(level), tmpRect);
    tmpRect.y = 550;
    tmpRect.w = 200;
    drawText("Experiencia:" + to_string(exp) + "/" + to_string((level*level)*100), tmpRect);


    tmpRect.y = 200;
    tmpRect.x = gScreenSurface->w / 2 + 200;
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

    //drawButtonSrc(exitButton, closeTexture);
    //drawButtonSrc(continueButton, buttonTexture);
   
    //drawButtonSrc(inventoryButton, buttonTexture);

    

    std::string tmpStr;

    SDL_Rect target;
    tmpRect.x = gScreenSurface->w /2 -200;
    tmpRect.y = 50;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawTextResize(tmpNPCs.begin()->description, tmpRect);

    
    target.x = tmpRect.x - 50;
    target.y = tmpRect.y - 25;
    target.w = tmpRect.w + 40;
    target.h = tmpRect.h + 25;
   // SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
   // drawText(aLoc.name + "(" + to_string(aLoc.id) + ")", tmpRect);
    tmpRect.y = 100;
    //tmpRect.w = gScreenRect.w - 100;
    target.x = tmpRect.x - 25;
    target.y = tmpRect.y - 25;
    target.w = tmpRect.w + 100;
    target.h = tmpRect.h + 50;
    //drawTextBlockBG(aLoc.description, tmpRect);

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
                // cout << it->name.substr(2,it->name.length() -2) << " (" << it->count << ")" << endl;
            }
            else {
                tmpStr = tmpStr + it->description;
                // cout << it->name << " (" << it->count << ")" << endl;
            }

            // tmpStr = tmpStr + it->description;
        }
        tmpStr = tmpStr + " por " + to_string(it->value2);
        if (it->description2 == "GOLD") { tmpStr = tmpStr + " monedas de oro"; }
        if (it->description2 == "FOOD") { tmpStr = tmpStr + " raciones de comida"; }
        // cout << endl;

        switch (itC) {
        case 0:
            shop0.setCaption(tmpStr);
            drawButtonSrc(shop0, buttonAcceptTexture);
            drawTileset({ 0, gScreenSurface->h / 2,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 1:
            shop1.setCaption(tmpStr);
            drawButtonSrc(shop1, buttonAcceptTexture);
            drawTileset({ 0, gScreenSurface->h / 2 +50 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 2:
            shop2.setCaption(tmpStr);
            drawButtonSrc(shop2, buttonAcceptTexture);
            drawTileset({ 0, gScreenSurface->h / 2 + 100 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 3:
            shop3.setCaption(tmpStr);
            drawButtonSrc(shop3, buttonAcceptTexture);
            drawTileset({ 0, gScreenSurface->h / 2 + 150 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 4:
            shop4.setCaption(tmpStr);
            drawButtonSrc(shop4, buttonAcceptTexture);
            drawTileset({ 0, gScreenSurface->h / 2 + 200 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        case 5:
            shop5.setCaption(tmpStr);
            drawButtonSrc(shop5, buttonAcceptTexture);
            drawTileset({ 0, gScreenSurface->h / 2 + 250 ,50,50 }, itemsTexture, it->tile, 20);
            break;
        }
        itC++;
        // cout <<  "-> " << it->description << "(" << it->targetId << ")" << endl;
    }



}

void game::screenChat()
{

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);

    //drawButtonSrc(exitButton, closeTexture);
    //drawButtonSrc(continueButton, buttonTexture);

    //drawButtonSrc(inventoryButton, buttonTexture);



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
    // SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
    // drawText(aLoc.name + "(" + to_string(aLoc.id) + ")", tmpRect);
    tmpRect.y = 100;
    //tmpRect.w = gScreenRect.w - 100;
    target.x = tmpRect.x - 25;
    target.y = tmpRect.y - 25;
    target.w = tmpRect.w + 100;
    target.h = tmpRect.h + 50;
    //drawTextBlockBG(aLoc.description, tmpRect);

    int itC = 0;
    for (list<CHAT>::iterator it = tmpCHATs.begin(); it != tmpCHATs.end(); it++) {

        tmpStr = it->question;

        switch (itC) {
        case 0:
            chat0.setCaption(tmpStr);
            drawButtonSrc(chat0, buttonAcceptTexture);
         
            break;
        case 1:
            chat1.setCaption(tmpStr);
            drawButtonSrc(chat1, buttonAcceptTexture);
         
            break;
        case 2:
            chat2.setCaption(tmpStr);
            drawButtonSrc(chat2, buttonAcceptTexture);
         
            break;
        case 3:
            chat3.setCaption(tmpStr);
            drawButtonSrc(chat3, buttonAcceptTexture);
         
            break;
        case 4:
            chat4.setCaption(tmpStr);
            drawButtonSrc(chat4, buttonAcceptTexture);
         
            break;
        case 5:
            chat5.setCaption(tmpStr);
            drawButtonSrc(chat5, buttonAcceptTexture);
         
            break;
        }
        itC++;
        // cout <<  "-> " << it->description << "(" << it->targetId << ")" << endl;
    }




    target.x = 50;
    target.y = gScreenSurface->h / 4;
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


void game::screenNotifications()
{
    tmpRect.h = 30;
    //tmpRect.x = 0;
    //tmpRect.x = gScreenSurface->w / 2 - 250;
    tmpRect.x = 1;

    tmpRect.w = 500;
    int tmpY = 0;
    if (notifications.size() > 0) {

        int base_y = gScreenSurface->h - (notifications.size() * 30 + 30);
        if (base_y < gScreenSurface->h /2) { base_y = gScreenSurface->h  /2; }
        for (std::string notification : notifications) {

            //tmpRect.y = ((gScreenSurface->h / 4) * 3) + (tmpY * 30);
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
                //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
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
                addAchievement("Comenzando el juego", my_enums::_HIDDEN_);
                SDL_StartTextInput();
                setState(my_enums::_NAME_);
            }



        }
  

        //******************
    }

   // myTime = (int)(timer.getTicks() / 1000);
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
    /*tmpRect.x = gScreenSurface->w / 8 - 1;
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
    */
    tmpRect.x = gScreenSurface->w / 2 - 250;
    tmpRect.y = gScreenSurface->h / 8 - 50;
    tmpRect.w = 500;
    tmpRect.h = 100;
    drawTextResize("Menu Configuración", tmpRect);
    /*
    tmpRect.x = gScreenSurface->w - 500;
    tmpRect.y = gScreenSurface->h - 50;
    tmpRect.w = 500;
    tmpRect.h = 50;
    */
    //drawText("Tiempo jugando: minutos:" + std::to_string(myTime / 60) + " segundos:" + std::to_string(myTime % 60), tmpRect);


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

    drawButtonSrc(startButton, buttonAcceptTexture);
}

void game::screenHomeTown()
{

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


    drawButtonSrc(moveLeftButton, buttonLeftTexture);
    drawButtonSrc(moveRightButton, buttonRightTexture);
    drawButtonSrc(moveUpButton, buttonUpTexture);
    drawButtonSrc(moveDownButton, buttonDownTexture);
    
    //drawButton(newMapButton);

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
   // drawButtonSrc(backButton, buttonCancelTexture);
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

    //tmpRect.y = 250;
    //drawText(playerName, tmpRect);
   // tmpRect.y = 300;
    tmpRect.y = gScreenSurface->h / 4 * 3 - 50;
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
   // drawButtonSrc(backButton, buttonCancelTexture);
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

   // tmpRect.y = 300;
    tmpRect.y = gScreenSurface->h / 4 * 3 - 50;
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
        //SDL_Rect target;
        //target.x = tmpRect.x - 50;
        //target.w = tmpRect.w + 40;
        //target.h = tmpRect.h + 25;


        drawTextResize("Inventario", tmpRect);


        string tmpString = "";
        tmpString = "Monedas de oro: " + to_string(coins);
        tmpy += 50;
        tmpRect.y = tmpy;
        //target.y = tmpRect.y - 25;
        //SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
        drawText(tmpString, tmpRect);


        tmpString = "Raciones de comida: " + to_string(food);
        tmpy += 50;
        tmpRect.y = tmpy;
        //target.y = tmpRect.y - 25;
        //SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
        drawText(tmpString, tmpRect);
        drawTileset({ gScreenSurface->w / 2 - 250,tmpy,50,50 }, itemsTexture, 207, 20);

        tmpString = "Pociones de vitalidad: " + to_string(potions_health);
        tmpy += 50;
        tmpRect.y = tmpy;
       // target.y = tmpRect.y - 25;
        //SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
        drawText(tmpString, tmpRect);
        drawTileset({ gScreenSurface->w / 2 - 250,tmpy,50,50 }, itemsTexture, 276, 20);

        tmpString = "Pociones de poder: " + to_string(potions_power);
        tmpy += 50;
        tmpRect.y = tmpy;
       // target.y = tmpRect.y - 25;
        //SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
        drawText(tmpString, tmpRect);
        drawTileset({ gScreenSurface->w / 2 - 250,tmpy,50,50 }, itemsTexture, 273, 20);  
        

        for (list<item>::iterator it = items.begin(); it != items.end(); it++)
        {
            tmpy += 50;
            tmpRect.y = tmpy;
            //target.y = tmpRect.y - 25;
            //target.x = tmpRect.x - 50;

            tmpRect.x = gScreenSurface->w / 2 - 250;
            tmpRect.w = 50;
            drawTileset(tmpRect, itemsTexture, it->tile,20);

            if (it->name.substr(0, 2) == "**") {
                tmpString = it->name.substr(2, it->name.length() - 2) + " (" + to_string(it->count) + ") valor:" + to_string(it->value);
                // cout << it->name.substr(2,it->name.length() -2) << " (" << it->count << ")" << endl;
            }
            else {
                tmpString = it->name + " (" + to_string(it->count) + ") valor:" + to_string(it->value);
                // cout << it->name << " (" << it->count << ")" << endl;
            }
            //SDL_RenderCopy(gRenderer, streetTexture, NULL, &target);
            tmpRect.w = 400;
            tmpRect.x = gScreenSurface->w / 2 - 200;
            drawText(tmpString, tmpRect);

        }

        tmpy = 300;
        tmpRect.x = gScreenSurface->w / 4 * 3-100;
        tmpRect.y = tmpy;
        drawTextResize("ITEMS EQUIPADOS", tmpRect);

        tmpRect.x = gScreenSurface->w / 4 * 3 -150;
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

        //itemShield = "";
        
        tmpy = 300;
        tmpRect.x = gScreenSurface->w / 4 * 3;
        tmpRect.y = tmpy;
        tmpRect.w = 200;
        tmpRect.y += 50;
       if (alternate3 !="")drawTextResize(alternate3, tmpRect);

        tmpRect.y += 50;
        //if (alternate2 != "") drawTextResize(alternate2, tmpRect);
        if (alternate2 != "") drawTextResize(alternate2, tmpRect);

        tmpRect.y += 50;
        if (itemHelmet != "")drawTextResize(itemHelmet, tmpRect);

        tmpRect.y += 50;
        //if (shield != "")drawTextResize(shield, tmpRect);
        if (alternate != "")drawTextResize(alternate, tmpRect);

        tmpRect.y += 50;
        if (itemRing != "")drawTextResize(itemRing, tmpRect);
        

}


void game::screenHero()
{
    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    drawButtonSrc(achievementsButton, buttonStarsTexture);

  /*  tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 0;
    tmpRect.w = 400;
    tmpRect.h = 50;
    drawText("Heroe inmortal", tmpRect);
*/
    tmpRect.y = gScreenSurface->h / 4;
    tmpRect.h = 100;

    tmpRect.x = gScreenSurface->w / 2 - 450;
    tmpRect.w = 900;
    drawTextResize("Heroe inmortal", tmpRect);

    tmpRect.y = gScreenSurface->h / 2;
    //drawTextResize("¿Te rindes o vas a seguir luchando?", tmpRect);
    drawTextBlock("Has sido derrotado, pero cuando piensas que ya todo está perdido una voz en tu interior te llama y sientes que nuevas energías recorren tu cuerpo,  ¿Te rindes o vas a seguir luchando?", tmpRect);
 
}


void game::addSkill(std::string skill)
{
    if(!hasSkill(skill)){
    skills.push_back(skill);
    addNotification("Has aprendido la habilidad: "+skill);
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
    
    //drawButton(continueButton);
    //drawButton(exitButton);

    drawButtonSrc(exitButton, buttonCloseTexture);
    drawButtonSrc(continueButton, buttonAcceptTexture);
    int combat = 0, missions = 0, npcs = 0, options = 0, hidden = 0;
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

        case my_enums::_NPCS_:
        {
            npcs++;
            if (achievementGroup == 2) {
                square.y = 450 + aCounter * 50;
                drawText(currentAchievement.name, square);
                aCounter++;
            }
            break;
        }

        case my_enums::_OPTIONS_:
        {
            options++;
            if (achievementGroup == 3) {
                square.y = 450 + aCounter * 50;
                drawText(currentAchievement.name, square);
                aCounter++;
            }
            break;
        }

        case my_enums::_HIDDEN_:
        {
            hidden++;
            if (achievementGroup == 4) {
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
    drawTextL("NPCs", tmpRect);
    tmpRect.y = 350;
    drawTextL("Opciones", tmpRect);
    tmpRect.y = 400;
    drawTextL("Ocultos", tmpRect);

    //combat = 0, missions = 0, npcs = 0, options = 0, hidden = 0;

    drawIMGBox(gScreenSurface->w / 2 - 250, 200, combat, 10, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2 - 250, 250, missions, 10, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2 - 250, 300, npcs, 10, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2 - 250, 350, options, 10, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2 - 250, 400, hidden, 10, { 200,0,0,0 });

    /*drawIMGBox(100, 200, combat, 10, {200,0,0,0});
    drawIMGBox(100, 250, missions, 10, { 200,0,0,0 });
    drawIMGBox(100, 300, npcs, 10, { 200,0,0,0 });
    drawIMGBox(100, 350, options, 10, { 200,0,0,0 });
    drawIMGBox(100, 400, hidden, 10, { 200,0,0,0 });
    */
    //SDL_Rect square;

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
    
    //All players can use normal attacks
    drawButtonSrc(fightButton, buttonSwordTexture);
    
    if(hasSkill("ARQUERO"))drawButtonSrc(bowButton, buttonBowTexture);
    if (hasSkill("SUBTERFUGIO"))drawButtonSrc(hideButton, buttonHideTexture);
    
    if (power>0) {
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

    tmpRect.x = gScreenSurface->w / 2 - 200;
    tmpRect.y = 0;
    tmpRect.w = 400;
    tmpRect.h = 50;
   
    drawTextResize("¡¡COMBATE!!", tmpRect);


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
        //drawPlayerTileset(tmpRect, it->tile);
        drawTileset(tmpRect, playersTexture, it->tile,20);

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
    //tmpRect.x = 100;
    tmpRect.x = gScreenSurface->w / 2;
    
    tmpRect.y = gScreenSurface->h / 2 - 250;
    drawTextL(playerName, tmpRect);
    tmpRect.y = gScreenSurface->h/2 - 225;
  //  drawTextL("Tu ataque", tmpRect);
    drawTextL("Vitalidad:", tmpRect);
    //drawIMGBox(100, gScreenSurface->h/2 - 250, stamina, max_stamina, { 200,0,0,0 });
    drawIMGBox(gScreenSurface->w / 2, gScreenSurface->h / 2 - 225, stamina, max_stamina, { 200,0,0,0 });
    

    tmpRect.y = gScreenSurface->h/2 - 200;
    drawTextL("Poder:", tmpRect);
    //drawIMGBox(100, gScreenSurface->h/2 - 200, power, max_power, { 128,0,128,0 });
    drawIMGBox(gScreenSurface->w / 2, gScreenSurface->h / 2 - 200, power, max_power, { 128,0,128,0 });
 //   drawTextL("Ataque enemigo", tmpRect);


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

//    drawIMGBox(100, gScreenSurface->h - 250, playerDice, 18, { 0,0,200,0 });
 //   drawIMGBox(100, gScreenSurface->h - 200, foeDice, 18, { 200,0,0,0 });
    tmpRect.x = gScreenSurface->w / 2;
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

                //addItem("ESPADA", "Una espada mellada", 1, 1, 40);
                //addItem("ARMADURA CUERO", "Armadura ligera de cuero", 1, 1, 114);
               // addShop()
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
                
                    addSkill("INVOCAR");
                    //****
                }

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
                //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
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
                   // addAchievement("Musica DARK", my_enums::_OPTIONS_);
                    break;
                case 2:
                    currentMusic = 3;
                    Mix_PlayMusic(musicFOREST, -1);
                    //addAchievement("Musica FOREST", my_enums::_OPTIONS_);
                    break;
                case 3:
                    currentMusic = 4;
                    Mix_PlayMusic(musicTOWN, -1);
                  //  addAchievement("Musica TOWN", my_enums::_OPTIONS_);
                    break;

                case 4:
                    currentMusic = 5;
                    Mix_PlayMusic(musicBATTLE, -1);
                    //addAchievement("Musica BATTLE", my_enums::_OPTIONS_);
                    break;

                case 5:
                    currentMusic = 6;
                    Mix_PlayMusic(musicBOSS, -1);
                  //  addAchievement("Musica BOSS", my_enums::_OPTIONS_);
                    break;

                case 6:
                    currentMusic = 7;
                    Mix_PlayMusic(musicCAVE, -1);
                   // addAchievement("Musica CAVE", my_enums::_OPTIONS_);
                    break;

                case 7:
                    currentMusic = 8;
                    Mix_PlayMusic(musicHERO, -1);
                   // addAchievement("Musica HERO", my_enums::_OPTIONS_);
                    break;

                case 8:
                    currentMusic = 9;
                    Mix_PlayMusic(musicMYSTICAL, -1);
                  //  addAchievement("Musica MYSTICAL", my_enums::_OPTIONS_);
                    break;

                case 9:
                    currentMusic = 1;
                    Mix_PlayMusic(musicINTRO, -1);
                  //  addAchievement("Musica INTRO", my_enums::_OPTIONS_);
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

            
            if (itemSelectedButton.clicked(mousex, mousey)) {
                int y = mousey - 350;
                int itemSelected = (y / 50) + 1;
                if (items.size() >= itemSelected) {
                    //todo

                    //item aItem = getItem(items[itemSelected]);

                    std::list<item>::iterator it = items.begin();
                    if (itemSelected > 1) {
                        std::advance(it, itemSelected-1);
                    }
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
                      //  itemEscudo = "";
                        alternate = "";
                        break;
                    }

                    case my_enums::_WEAPON_RANGED_:
                    {
                        Mix_PlayChannel(-1, weapon, 0);
                        alternate3 = it->name;
                     //   itemEscudo = "";
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
                        //itemEscudo = it->name;
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
                if (!collide(tmpx, tmpy)) {
                    it->x = tmpx;
                    it->y = tmpy;
                }

            }

            if (it->NPCAI == my_enums::_ENEMY_FOLLOW_) {

                if(getDistance(px,py, it->x, it->y) <= 15){// only follow if distance is less or equal than 15
                int tmpx = it->x;
                int tmpy = it->y;


                if (((px < it->x) || (px > it->x)) && (py == it->y)) {
                    if (px < it->x) { tmpx--; }
                    if (px > it->x) { tmpx++; }
                }
                else if (((py < it->y) || (py > it->y)) && (px == it->x)) {
                    if (py < it->y) { tmpy--; }
                    if (py > it->y) { tmpy++; }
                }
                else if ((px != it->x) && (py != it->y)) {
                    int d = dice(2, 1);
                    if (d == 1) {
                        if (px < it->x) { tmpx--; }
                        if (px > it->x) { tmpx++; }
                    }
                    else {
                        if (py < it->y) { tmpy--; }
                        if (py > it->y) { tmpy++; }
                    }
                }
                /*
                if (px < it->x) { tmpx--; }
                if (py < it->y) { tmpy--; }
                if (px > it->x) { tmpx++; }
                if (py > it->y) { tmpy++; }
                */

                if (!collide(tmpx, tmpy)) {
                    if (insideBoundaries(tmpx, tmpy, it->boundaries)) {
                        it->x = tmpx;
                        it->y = tmpy;
                    }
                }
            }//distance less than 15
            }//process basic follow AI



            if (it->NPCAI == my_enums::_FRIENDLY_FOLLOW_) {

                if (getDistance(px, py, it->x, it->y) <= 15) {// only follow if distance is less or equal than 15
                    if(!isAround(it->x, it->y)){
                    int tmpx = it->x;
                    int tmpy = it->y;


                    if (((px < it->x) || (px > it->x)) && (py == it->y)) {
                        if (px < it->x) { tmpx--; }
                        if (px > it->x) { tmpx++; }
                    }
                    else if (((py < it->y) || (py > it->y)) && (px == it->x)) {
                        if (py < it->y) { tmpy--; }
                        if (py > it->y) { tmpy++; }
                    }
                    else if ((px != it->x) && (py != it->y)) {
                        int d = dice(2, 1);
                        if (d == 1) {
                            if (px < it->x) { tmpx--; }
                            if (px > it->x) { tmpx++; }
                        }
                        else {
                            if (py < it->y) { tmpy--; }
                            if (py > it->y) { tmpy++; }
                        }
                    }


                    if (!collide(tmpx, tmpy)) {
                        if (insideBoundaries(tmpx, tmpy, it->boundaries)) {
                            it->x = tmpx;
                            it->y = tmpy;
                        }
                    }
                }//we are not too close
                }//distance less than 15
            }//process basic follow AI


        }

    }

    //Process global events
    //for (list<GLOBAL_EVENT>::iterator it = GLOBAL_EVENTs.begin(); it != GLOBAL_EVENTs.end(); it++)
   // {


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
                                     addNotification(itNPC->description + " comienza a seguirte");
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
                                addNotification(itNPC->description + " deja de seguirte");

                                if (itNPC->id == 10001)addAchievement("¡¡Rescataste a TOR!!", my_enums::_NPCS_);
                                //GLOBAL_EVENTs.erase(it);
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
                                if (itNPC->id == 10001)addChat(119, 142, 2, "Sobre Tor", "¡Muchas gracias!, con la llave que te he dado puedes abrir el camino que lleva a la playa");
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
            
        


        }//on the same map


       if ((it != GLOBAL_EVENTs.end())&& (erased == false))it++;
        

    }//Process global events


}

void game::timeEvents()
{
    
    if ((getState() != my_enums::_FIGHT_) || (getState() != my_enums::_SHOP_)){
        if ((SDL_GetTicks() - ticksAI) > 1000) {
            ticksAI = SDL_GetTicks();
            processAI();

            /*
            tmpNPCs = getNPCs(px, py);
           // if ((tmp.size() > 0)&&((tmp.begin()->NPCAI == my_enums::_ENEMY_RANDOM_)|| (tmp.begin()->NPCAI == my_enums::_ENEMY_FOLLOW_))) {
            if ((tmpNPCs.size() > 0) && ((tmpNPCs.begin()->NPCAI == my_enums::_ENEMY_RANDOM_) || (tmpNPCs.begin()->NPCAI == my_enums::_ENEMY_FOLLOW_))) {
                
                    Mix_PlayMusic(musicBATTLE, -1);
                    previousScreen = my_enums::_HOMETOWN_;
                   // tmpNPCs = getNPCs(px, py);
                    setState(my_enums::_FIGHT_);
                
            }
            */
        }
    }
myTime = (int)(timer.getTicks() / 1000);

//Power recharges with time...
if (getState() != my_enums::_FIGHT_)//You dont recharge magic while fighting
{
    if (myTime != prevTime) {
        prevTime = myTime;

        if (debugMode) {
            if (NPCs.size()<20){
            addNPC(1, dice(10, 80), dice(10, 80), my_enums::_HOMETOWN_, "MALO", dice(10, 1), dice(10, 5), dice(10, 5), dice(10, 5), dice(3, 1), my_enums::_ENEMY_RANDOM_, dice(300, 2), { 1,1,255,255 });
        }
        }

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
                //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
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
                setState(my_enums::_MAINMENU_);
                

            }

            if (achievementsButton.clicked(mousex, mousey)) {
                setState(my_enums::_ACHIEVEMENTS_);
                previousScreen = my_enums::_HERO_;

            }

        }

        //******************
    }

   // myTime = (int)(timer.getTicks() / 1000);

}

void game::locationEvents()
{
    if (currentState == my_enums::_HOMETOWN_) {
        tmpEVENTs = getEvents(px,py);
        //cout << "Eventos cargados" << tmpEvents.size() << endl;
        bool erase = false;
        std::string tmpStr;
        for (list<EVENT>::iterator it = tmpEVENTs.begin(); it != tmpEVENTs.end(); it++)
        {
            if (it->description == "TELEPORT") {
                px = it->newx;
                py = it->newy;
                
                if (it->map != it->newMap) {
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
                addNotification(tmpStr);
                coins += it->value;
                if (coins < 0) { coins = 0; }
                erase = true;

            }
        }
        if (erase)cleanEvents(px, py);

    }
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
                //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
            }

   

            if (fightButton.clicked(mousex, mousey)) {
                addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100, 1,buttonSwordTexture);
                Mix_PlayChannel(-1,sword, 1);
                turn++;
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
                    addNotification("Has herido a "+ tmpNPC.description +"!!");
                    tmpNPC.stamina -= damage;
                    if (tmpNPC.stamina <= 0) {
                        //   cout << "Has derrotado a " << tmpFoe.description << endl;
                         addNotification("Has derrotado a "+ tmpNPC.description +"!!");
                        //Mix_PlayChannel(-1, audioMaleDeath, 0);
                         exp += tmpNPC.exp;
                        tmpNPCs.pop_front();


                        killCount++;
                        addAchievement("Primera victoria", my_enums::_COMBAT_);
                        if(killCount == 10)addAchievement("Le estas pillando el punto", my_enums::_COMBAT_);
                        if (killCount == 100)addAchievement("Massacre", my_enums::_COMBAT_);
                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }
                }

                //Process enemy attack
                if (tmpNPCs.size() > 0){
                    addAnimation( 1, 100, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonSwordTexture);
                    int good = dice(10, 1) + skill ;
                    if (hasSkill("MELEE")) { good++; }
                    if (alternate2 != "") { good += getItem(alternate2).bonus; }
                    if (itemHelmet != "") { good += getItem(itemHelmet).bonus; }
                    if (alternate != "") { good += getItem(alternate).bonus; }
                   
                    int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                    if (bad > good) {
                        enemyDamage = tmpNPCs.begin()->damage;
                        stamina -= enemyDamage;
                        addNotification(tmpNPCs.begin()->description + " te ha herido!!");
                        if (stamina <= 0) {
                            addNotification("Has sido derrotado por " + tmpNPCs.begin()->description + "!!");
                            setState(my_enums::_HERO_);
                            deleteNPCs(px, py);
                            //Mix_PlayMusic(musicGameOver, -1);
                            addAchievement("Tu personaje ha muerto", my_enums::_HIDDEN_);
                            Mix_PlayMusic(musicHERO, -1);
                            timerGameOver.start();
                            timerGameOver.reset();
                            //addNotification("Saliendo del juego");
                            //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
                        }

                    }

                }
               
              
            }//fightbutton click


            //ranged attack
            if (bowButton.clicked(mousex, mousey)) {
                addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100, 1, buttonBowTexture);
                Mix_PlayChannel(-1, bow, 1);
                turn++;
                int good = dice(10, 1) + skill;
                int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                // playerDice = good;
               //  foeDice = bad;
                int damage = 1;

                
                if (alternate3 != "") { damage += getItem(alternate3).bonus; }

                //Process player attack
                if (good > bad) {
                    NPC tmpNPC = tmpNPCs.front();
                    addNotification("Has herido a " + tmpNPC.description + "!!");
                    tmpNPC.stamina -= damage;
                    if (tmpNPC.stamina <= 0) {
                        //   cout << "Has derrotado a " << tmpFoe.description << endl;
                        addNotification("Has derrotado a " + tmpNPC.description + "!!");
                        //Mix_PlayChannel(-1, audioMaleDeath, 0);
                        exp += tmpNPC.exp;
                        tmpNPCs.pop_front();


                        killCount++;
                        addAchievement("Guillermo Tell", my_enums::_COMBAT_);
                        if (killCount == 10)addAchievement("Le estas pillando el punto", my_enums::_COMBAT_);
                        if (killCount == 100)addAchievement("Massacre", my_enums::_COMBAT_);
                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }
                }

                //Process enemy attack
                if (tmpNPCs.size() > 0) {
                    int goodLuck = dice(10, 1) + luck;
                    int badLuck = dice(10, 1) + tmpNPCs.begin()->luck;
                    if (goodLuck > badLuck) {
                        addNotification("Evitas el ataque");
                    }else{
                    addAnimation(1, 100, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonSwordTexture);
                    int good = dice(10, 1) + skill;
                    if (hasSkill("MELEE")) { good++; }
                    if (alternate2 != "") { good += getItem(alternate2).bonus; }
                    if (itemHelmet != "") { good += getItem(itemHelmet).bonus; }
                    if (alternate != "") { good += getItem(alternate).bonus; }

                    int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                    if (bad > good) {
                        int enemyDamage = tmpNPCs.begin()->damage;
                        stamina -= enemyDamage;
                        addNotification(tmpNPCs.begin()->description + " te ha herido!!");
                        if (stamina <= 0) {
                            addNotification("Has sido derrotado por " + tmpNPCs.begin()->description + "!!");
                            setState(my_enums::_HERO_);
                            deleteNPCs(px, py);
                            //Mix_PlayMusic(musicGameOver, -1);
                            addAchievement("Tu personaje ha muerto", my_enums::_HIDDEN_);
                            Mix_PlayMusic(musicHERO, -1);
                            timerGameOver.start();
                            timerGameOver.reset();
                            //addNotification("Saliendo del juego");
                            //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
                        }

                    }
                }//Luck

                }


            }//bow button click


            //subterfuge attack
            if (hideButton.clicked(mousex, mousey)) {
                addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100, 1, buttonHideTexture);
                Mix_PlayChannel(-1, sword, 1);
                turn++;

                NPC tmpNPC = tmpNPCs.front();
                int damage = 2;
                if (alternate3 != "") { damage += getItem(alternate3).bonus; }
                    

                int goodLuck = dice(10, 1) + luck;
                int badLuck = dice(10, 1) + tmpNPCs.begin()->luck;
                if (goodLuck > badLuck) {
                    addNotification("Ataque por sorpresa");
                    tmpNPC.stamina -= (damage +1);
                    if (tmpNPC.stamina <= 0) {
                        //   cout << "Has derrotado a " << tmpFoe.description << endl;
                        addNotification("Has derrotado a " + tmpNPC.description + "!!");
                        //Mix_PlayChannel(-1, audioMaleDeath, 0);
                        exp += tmpNPC.exp;
                        tmpNPCs.pop_front();

                        magicKill++;
                        addAchievement("Asesino", my_enums::_COMBAT_);
                        if (magicKill == 10)addAchievement("La fuerza crece en ti", my_enums::_COMBAT_);
                        if (magicKill == 100)addAchievement("Desintegrar", my_enums::_COMBAT_);


                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }
                }
                else {
                    addNotification("Te han descubierto ");
                    int enemyDamage = tmpNPCs.begin()->damage;
                    stamina -= enemyDamage;
                }



            }//stealth button click


            //Health potion
            if (potions_health > 0) {
                if (potionHealthButton.clicked(mousex, mousey)) {
                    Mix_PlayChannel(-1, heal, 1);
                    addAnimation(potionHealthButton.getRect().x, potionHealthButton.getRect().y, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100,1, buttonPotionHealthTexture);
                    potions_health--;
                    stamina += 10;
                    if (stamina > max_stamina)stamina = max_stamina;
                    turn++;

                    int damage = 1;
                    //Process enemy attack
                    if (tmpNPCs.size() > 0) {
                        addAnimation(1, 100, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonSwordTexture);

                        int good = dice(10, 1) + skill;
                        if (alternate2 != "") { good += getItem(alternate2).bonus; }
                        if (itemHelmet != "") { good += getItem(itemHelmet).bonus; }
                        if (alternate != "") { good += getItem(alternate).bonus; }

                        int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                        if (bad > good) {
                            int enemyDamage = tmpNPCs.begin()->damage;
                            stamina -= enemyDamage;
                            addNotification(tmpNPCs.begin()->description + " te ha herido!!");
                            if (stamina <= 0) {
                                turn = 0;
                                addNotification("Has sido derrotado por " + tmpNPCs.begin()->description + "!!");
                                setState(my_enums::_HERO_);
                                deleteNPCs(px, py);
                                //Mix_PlayMusic(musicGameOver, -1);
                                addAchievement("Tu personaje ha muerto", my_enums::_HIDDEN_);
                                Mix_PlayMusic(musicHERO, -1);
                                timerGameOver.start();
                                timerGameOver.reset();
                                //addNotification("Saliendo del juego");
                                //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
                            }

                        }

                    }//enemy attack
                }
            }

            //Power potion
            if (potions_power > 0) {
                
                if (potionMagicButton.clicked(mousex, mousey)) {
                    Mix_PlayChannel(-1, heal, 1);
                    addAnimation(potionMagicButton.getRect().x, potionMagicButton.getRect().y, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1,buttonPotionMagicTexture);
                    potions_power--;
                    power += 10;
                    if (power > max_power)power = max_power;
                    turn++;

                    int damage = 1;
                    //Process enemy attack
                    if (tmpNPCs.size() > 0) {
                        addAnimation(1, 100, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonSwordTexture);

                        int good = dice(10, 1) + skill;
                        if (alternate2 != "") { good += getItem(alternate2).bonus; }
                        if (itemHelmet != "") { good += getItem(itemHelmet).bonus; }
                        if (alternate != "") { good += getItem(alternate).bonus; }

                        int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                        if (bad > good) {
                            int enemyDamage = tmpNPCs.begin()->damage;
                            stamina -= enemyDamage;
                            addNotification(tmpNPCs.begin()->description + " te ha herido!!");
                            if (stamina <= 0) {
                                turn = 0;
                                addNotification("Has sido derrotado por " + tmpNPCs.begin()->description + "!!");
                                setState(my_enums::_HERO_);
                                deleteNPCs(px, py);
                                //Mix_PlayMusic(musicGameOver, -1);
                                addAchievement("Tu personaje ha muerto", my_enums::_HIDDEN_);
                                Mix_PlayMusic(musicHERO, -1);
                                timerGameOver.start();
                                timerGameOver.reset();
                                //addNotification("Saliendo del juego");
                                //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
                            }

                        }

                    }//enemy attack
                }
            }

            //magic attack
            if (spellButton.clicked(mousex, mousey)){
                if (power > 0) {
                    Mix_PlayChannel(-1, magic, 1);
                    addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100,1, buttonSpellTexture);
                    turn++;
                    int damage = 1;
                    int attackPower = dice(power, 1);
                    
                    NPC tmpNPC = tmpNPCs.front();
                    if (attackPower > tmpNPC.stamina)attackPower = tmpNPC.stamina;


                    addNotification("Has herido a " + tmpNPC.description + "!!");
                    tmpNPC.stamina -= attackPower;
                    power -= attackPower;
                    if (tmpNPC.stamina <= 0) {
                        //   cout << "Has derrotado a " << tmpFoe.description << endl;
                        addNotification("Has derrotado a " + tmpNPC.description + "!!");
                        //Mix_PlayChannel(-1, audioMaleDeath, 0);
                        exp += tmpNPC.exp;
                        tmpNPCs.pop_front();

                        magicKill++;
                        addAchievement("Abracadabra", my_enums::_COMBAT_);
                        if (magicKill == 10)addAchievement("La fuerza crece en ti", my_enums::_COMBAT_);
                        if (magicKill == 100)addAchievement("Desintegrar", my_enums::_COMBAT_);


                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }

                    //Process enemy attack
                    if (tmpNPCs.size() > 0) {
                        addAnimation(1, 100, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonSwordTexture);

                        int good = dice(10, 1) + skill;
                        if (alternate2 != "") { good += getItem(alternate2).bonus; }
                        if (itemHelmet != "") { good += getItem(itemHelmet).bonus; }
                        if (alternate != "") { good += getItem(alternate).bonus; }

                        int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                        if (bad > good) {
                            int enemyDamage = tmpNPCs.begin()->damage;
                            stamina -= enemyDamage;
                            addNotification(tmpNPCs.begin()->description + " te ha herido!!");
                            if (stamina <= 0) {
                                turn = 0;
                                addNotification("Has sido derrotado por " + tmpNPCs.begin()->description + "!!");
                                setState(my_enums::_HERO_);
                                deleteNPCs(px, py);
                                //Mix_PlayMusic(musicGameOver, -1);
                                Mix_PlayMusic(musicHERO, -1);
                                timerGameOver.start();
                                timerGameOver.reset();
                                //addNotification("Saliendo del juego");
                                //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
                            }

                        }

                    }



                }//player has power
            }//magic attack


            //drain attack
            if (drainButton.clicked(mousex, mousey)) {
                if (power > 0) {
                    Mix_PlayChannel(-1, magic, 1);
                    addAnimation(gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 1, 100, 100, 100, 1, buttonDrainTexture);
                    turn++;
                    int damage = 1;
                    int attackPower = dice(power, 1);

                    NPC tmpNPC = tmpNPCs.front();
                    if (attackPower > tmpNPC.stamina)attackPower = tmpNPC.stamina;


                    addNotification("Has herido a " + tmpNPC.description + "!!");
                    attackPower = attackPower / 2;
                    if (attackPower < 1)attackPower = 1;
                    tmpNPC.stamina -= attackPower;
                    stamina += attackPower;
                    if (stamina > max_stamina)stamina = max_stamina;

                    power -= attackPower;
                    if (tmpNPC.stamina <= 0) {
                        //   cout << "Has derrotado a " << tmpFoe.description << endl;
                        addNotification("Has derrotado a " + tmpNPC.description + "!!");
                        //Mix_PlayChannel(-1, audioMaleDeath, 0);
                        exp += tmpNPC.exp;
                        tmpNPCs.pop_front();

                        magicKill++;
                        addAchievement("Necromante", my_enums::_COMBAT_);
                        if (magicKill == 10)addAchievement("La fuerza crece en ti", my_enums::_COMBAT_);
                        if (magicKill == 100)addAchievement("Desintegrar", my_enums::_COMBAT_);


                    }
                    else {
                        tmpNPCs.pop_front();
                        tmpNPCs.push_front(tmpNPC);
                    }

                    //Process enemy attack
                    if (tmpNPCs.size() > 0) {
                        addAnimation(1, 100, gScreenSurface->w / 2 - 200, gScreenSurface->h / 2 - 250, 100, 100, 1, buttonSwordTexture);

                        int good = dice(10, 1) + skill;
                        if (alternate2 != "") { good += getItem(alternate2).bonus; }
                        if (itemHelmet != "") { good += getItem(itemHelmet).bonus; }
                        if (alternate != "") { good += getItem(alternate).bonus; }

                        int bad = dice(10, 1) + tmpNPCs.begin()->skill;
                        if (bad > good) {
                            int enemyDamage = tmpNPCs.begin()->damage;
                            stamina -= enemyDamage;
                            addNotification(tmpNPCs.begin()->description + " te ha herido!!");
                            if (stamina <= 0) {
                                turn = 0;
                                addNotification("Has sido derrotado por " + tmpNPCs.begin()->description + "!!");
                                setState(my_enums::_HERO_);
                                deleteNPCs(px, py);
                                //Mix_PlayMusic(musicGameOver, -1);
                                Mix_PlayMusic(musicHERO, -1);
                                timerGameOver.start();
                                timerGameOver.reset();
                                //addNotification("Saliendo del juego");
                                //addAchievement("Saliendo del juego", my_enums::_OPTIONS_);
                            }

                        }

                    }



                }//player has power
            }//drain attack


            if (tmpNPCs.size() == 0) {
                turn = 0;
                Mix_PlayMusic(musicTOWN, -1);
                deleteNPCs(px, py);
                setState(previousScreen);

            }


            //    drawIMGBox(100, gScreenSurface->h - 250, playerDice, 18, { 0,0,200,0 });
 //   drawIMGBox(100, gScreenSurface->h - 200, foeDice, 18, { 200,0,0,0 });

        }

        //******************
    }//events

   // myTime = (int)(timer.getTicks() / 1000);

    

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

            if (achievementGroup0Button.clicked(mousex, mousey)) {
                achievementGroup = 0;
            }

            if (achievementGroup1Button.clicked(mousex, mousey)) {
                achievementGroup = 1;
            }

            if (achievementGroup2Button.clicked(mousex, mousey)) {
                achievementGroup = 2;
            }

            if (achievementGroup3Button.clicked(mousex, mousey)) {
                achievementGroup = 3;
            }

            if (achievementGroup4Button.clicked(mousex, mousey)) {
                achievementGroup = 4;
            }

        }

        //******************
    }

    //myTime = (int)(timer.getTicks() / 1000);

}

void game::eventsHomeTown()
{
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

            if(food>0){
            if (foodButton.clicked(mousex, mousey)) {
                int tx = px - cam_x;
                int ty = py - cam_y;

                addAnimation(foodButton.getRect().x, foodButton.getRect().y, (gScreenSurface->w / cam_size_x) * tx, (gScreenSurface->h / cam_size_y) * ty, gScreenSurface->w / cam_size_x, gScreenSurface->h / cam_size_y, 1, buttonFoodTexture);

                food--;
                stamina += 4;
                if (stamina > max_stamina)stamina = max_stamina;
                
            }//food button
            }

            if (moveRightButton.clicked(mousex, mousey)) {
                right = true;
                //px++;
                //updateMap();
                
                
            }//config button

            if (moveLeftButton.clicked(mousex, mousey)) {
                left = true;
                //px--;
                //updateMap();
                //baseMap.blur();
            }//config button

            if (moveDownButton.clicked(mousex, mousey)) {
                down = true;
                //py++;
                //updateMap();
                //baseMap.blur();
            }//config button

            if (moveUpButton.clicked(mousex, mousey)) {
                up = true;
                //py--;
                //updateMap();
               // baseMap.blur();
            }//config button

            if (mapButton.clicked(mousex, mousey)) {
                drawMiniMap();
            }//map button


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
                baseMap.generateTiles();
               
                updateMap();
                baseMap.blur();

            }//new map button

        }
        else if (e.type == SDL_KEYDOWN)
        {
            //Select surfaces based on key press
            switch (e.key.keysym.sym)
            {
            case SDLK_a:
                left = true;
                //px--;
                //updateMap();
              
                break;

            case SDLK_w:
                up = true;
                //py--;
                //updateMap();
                break;

            case SDLK_s:
                down = true;
                //py++;
                //updateMap();
                break;

            case SDLK_d:
                right = true;
                //px++;
                //updateMap();
                break;


            default:
                  break;
            }
        }//event management
        
         /*
        tmpx = px;
        tmpy = py;
        if (right)tmpx++;
        if (left)tmpx--;
        if (up)tmpy--;
        if (down)tmpy++;
        phaseNPCs();
        */


    }

    tmpx = px;
    tmpy = py;
    if (right)tmpx++;
    if (left)tmpx--;
    if (up)tmpy--;
    if (down)tmpy++;
    phaseNPCs();
   // myTime = (int)(timer.getTicks() / 1000);
}

void game::changeMap()
{
    int width, height;
    width = 256;
    height = 256;
    // baseMap.mymap.init();
    baseMap.mymap.generate(rand() % 6 + 1, rand() % 6 + 1, 0.5f, 1, 1, width, height);

    baseMap.mymap.to_surface(baseMap.imageSurface, getState());
    baseMap.generateTiles();

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

        //if (currentState == it->map) {
        //    // s_Foe aFoe;
        //    tx = it->x - cam_x;
        //        }

    }

    std::list<animation>::iterator i = animations.begin();
    while (i != animations.end())
    {
        //int difx = abs((i)->startx - (i)->endx);
        //int dify = abs((i)->starty - (i)->endy);
      //  bool animationEnd = (difx < 10)&& (dify < 10);

        bool animationEnd = (SDL_GetTicks() - (i)->ticks) > ((i)->seconds * 1000);
        //int s,
        if (animationEnd)
        {
            animations.erase(i++);  // alternatively, i = items.erase(i);
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

    

    //drawSquare(target,player);
    //drawPlayerTileset(target, playerTile);
    drawTileset(target, playersTexture, playerTile,20);
    
    drawIMGBoxSmall(target.x, target.y - 40, gScreenSurface->w / cam_size_x , 20 , stamina, max_stamina, { 200,0,0,0 });
    drawIMGBoxSmall(target.x, target.y - 20, gScreenSurface->w / cam_size_x, 20, power, max_power, { 128,0,128,0 });


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
            drawTileset(target, playersTexture, it->tile,20);
           // tmp.push_back(aFoe);
        }
    }
}//end


//void game::drawPlayerTileset(int x, int y, Uint8 player)
void game::drawTileset(SDL_Rect target,SDL_Texture *texture, int player, int columns)
{
    if (player != 0) {
        int sx;
        int sy;
        sy = player / columns;
        sx = player % columns;

        SDL_Rect playerSrc;
        playerSrc.x = sx * 32 + 1;
        playerSrc.y = sy * 32 + 1;
        playerSrc.w = 32;
        playerSrc.h = 32;
       // SDL_RenderCopy(gRenderer, playersTexture, &playerSrc, &target);
        SDL_RenderCopy(gRenderer,texture, &playerSrc, &target);

        //drawIMGBox(100, 250, stamina, max_stamina, { 200,0,0,0 });
        //drawIMGBox(100, 300, power, max_power, { 128,0,128,0 });

       

    }
}//End 

void game::checkBoundaries()
{
    if (px < 1) { px = 1; }
    if (py < 1) { py = 1; }
    if (px > 256) { px = 256; }
    if (py > 256) { py = 256; }

    if (px <= cam_x) { cam_x--; }
    if (py <= cam_y) { cam_y--; }
    if (px >= (cam_x + cam_size_x - 1)) { cam_x++; }
    if (py >= (cam_y + cam_size_y - 1)) { cam_y++; }

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
