#ifndef MY_ENUMS
#define MY_ENUMS
namespace my_enums
{

    enum gameState
    {
        S_NULL_ = 0,
        S_INTRO_ = 1,
        S_MAINMENU_ = 2,
        S_NAME_ = 3,
        S_ATTRIBUTES_ = 4,
        S_RACES_ = 5,
        S_ARCHETYPES_ = 6,
        S_CONFIGMENU_ = 7,
        S_INVENTORY_ = 8,
        S_ACHIEVEMENTS_ = 9,
        S_PLAYER_ = 10,
        S_SHOP_ = 11,
        S_CHAT_ = 12,
        S_MASTER_ = 13,
        S_HOMETOWN_ = 15,  
        S_FIGHT_ = 30,
        S_HERO_ = 35,
        S_FOREST_WORLD_ = 110,
        S_COAST_WORLD_ = 120,
        S_ELEMENTAL_FIRE_WORLD_ = 130,
        S_ELEMENTAL_WATER_WORLD_ = 140,
        S_ELEMENTAL_EARTH_WORLD_ = 150,
        S_ELEMENTAL_WIND_WORLD_ = 160,
        S_NECRO_WORLD_ = 170,
        S_FINAL_ARENA_ = 200,
        S_GAMEOVER_ = 900,
        S_GAMECLOSE_ = 999
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
        _FRIENDLY_PET_ = 4,
        _FRIENDLY_MASTER_ = 7,
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
        _OPTIONS_ = 30,
        _HIDDEN_ = 40
    };

    enum itemTypes
    {
        _WEAPON_1H_ = 1,
        _WEAPON_2H_ = 2,
        _WEAPON_RANGED_ = 3,
        _ARMOR_ = 4,
        _SHIELD_ = 5,
        _HELMET_ =6,
        _RING_ = 7,
        _OTHER_ = 10


    };

}
#endif