#ifndef MY_ENUMS
#define MY_ENUMS
namespace my_enums
{
    enum gameState
    {
        _NULL_ = -1,
        _INTRO_ = 1,
        _MAINMENU_ = 2,
        _NAME_ = 3,
        _ATTRIBUTES_ = 4,
        _RACES_ = 5,
        _ARCHETYPES_ = 6,
        _CONFIGMENU_ = 7,
        _INVENTORY_ = 8,
        _ACHIEVEMENTS_ = 9,
        _PLAYER_ = 10,
        _SHOP_ = 11,
        _HOMETOWN_ = 15,
        _TOWN_SHOP_ = 20,
        _TOWN_ELDER_ = 25,
        _FIGHT_ = 30,
        _HERO_ = 35,
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

    enum AItypes
    {
        _FRIENDLY_STATIC_ = 1,
        _FRIENDLY_RANDOM_ = 2,
        _FRIENDLY_FOLLOW_ = 3,
        _FRIENDLY_SHOP_ = 8,
        _FRIENDLY_CHAT_ = 9,
        _FRIENDLY_SKILL_ = 10,
        _ENEMY_STATIC_ = 20,
        _ENEMY_RANDOM_ = 21,
        _ENEMY_FOLLOW_ = 22
    };

    enum Achievements
    {
        _COMBAT_ = 10,
        _MISSIONS_ = 20,
        _NPCS_ = 30,
        _OPTIONS_ = 40,
        _HIDDEN_ = 50
    };

}
#endif