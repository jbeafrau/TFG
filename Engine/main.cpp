#include <exception>
#include <string>
#include <iostream>
#include<fstream>
#include "game.h"
using namespace std;

/*
    ACHIEVEMENT MASTER
    TFG Ingeniería informática
    Jorge Bea Frau
    Curso 2024-2025
*/


int main( int argc, char * argv[] )
{
game Game;

//Enable for debugging
Game.debugMode = true;

if (Game.initSDL() == false) {
    Game.setState(my_enums::S_GAMEOVER_);
}

Game.loadMedia(Game.getBasePath());

Game.start();

while (Game.getState() != my_enums::S_GAMECLOSE_){
    
    Game.events();

    Game.screenClear();

    Game.drawBackground();
    
    Game.drawScreens();

    Game.playAnimations();

    Game.eventsNotifications();

    Game.screenNotifications();

    Game.screenFlip();

    Game.adjustFPS();

    Game.locationEvents();

    Game.timeEvents();
}

Game.closeSDL();

return 0;
}
