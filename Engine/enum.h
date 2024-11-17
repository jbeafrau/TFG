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
        _RACES_ = 4,
        _ARCHETYPES_ = 5,
        _CONFIGMENU_ = 6,
        _HOMETOWN_ = 10,
        _TOWN_SHOP_ = 20,
        _TOWN_ELDER_ = 30,
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
}
#endif