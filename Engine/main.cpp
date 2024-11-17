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


if (Game.initSDL() == false) {
    Game.setState(_GAMEOVER_);
}

Game.loadMedia(Game.getBasePath());

Game.start();

while (Game.getState() != _GAMECLOSE_){
    
    Game.events();

    Game.screenClear();

    Game.drawScreens();

    Game.eventsNotifications();

    Game.screenNotifications();

    Game.screenFlip();

    Game.adjustFPS();
}

Game.closeSDL();

return 0;
}
