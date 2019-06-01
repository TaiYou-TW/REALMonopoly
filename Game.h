#pragma once
#include <cstdint>
#include <vector>
#include "Building.h"
#include "Player.h"

using namespace std;

typedef enum { dice, buy } GameStatus;
class Game {
public:
    Game();
    Game(const string &f);

    void loadFile(const string &f);
    void saveFile() const;

    const bool isEnd() const;
    const int32_t rollDice() const;

    void draw() const;
    void rollDicePhase();
    void estatePhase(const string &action);
    void fateChancePhase();

    const int32_t getRounds() const;
    const int32_t getNumOfPlayers() const;
    Player &getPlayer();
    Player &getPlayer(const int32_t &p);

    void setRounds(const int32_t &r);
    void setPlayers(const int32_t &p);
    void setNowPlaying(const int32_t &p);

protected:
    string mapName;
    vector<Building> map;
    vector<Player> playerList;
    int32_t nowPlaying;
    int32_t numOfPlayers;
    int32_t rounds;
    GameStatus status;
};
