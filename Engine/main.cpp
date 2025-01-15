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
//Game.debugMode = true;

//Init SDL subsystems
if (Game.initSDL() == false) {
    Game.setState(my_enums::S_GAMEOVER_);
}

//Load media (images/audio)
Game.loadMedia(Game.getBasePath());

Game.start();

while (Game.getState() != my_enums::S_GAMECLOSE_){
    
    //Process input events for current state
    Game.events();

    //clear screen
    Game.screenClear();

    //draw background 
    Game.drawBackground();
    

    //draw screen for current state
    Game.drawScreens();

    //update and play ongoing animations
    Game.playAnimations();

    //Updates notifications
    Game.eventsNotifications();

    //Draw notifications
    Game.screenNotifications();

    //Draw achievements
    Game.drawAchievements();

    //Darw mouse location
    Game.drawMouse();

    //Flip generated frame on screen
    Game.screenFlip();

    //Process events related to current player location
    Game.locationEvents();

    //Process global events
    Game.timeEvents();

    //Adjust frame rate
    Game.adjustFPS();
}

//Close SDL subsystems
Game.closeSDL();

return 0;
}
