#include <iostream>
#include <string>
#include "Building.h"
#include "Game.h"
#include "Player.h"

using namespace std;

int main() {
    Game game;
    for (string inputBuf; !game.isEnd();) {
        game.draw();
        game.rollDicePhase();
        system("PAUSE");

        game.draw();

        // next player
        game.setNowPlaying((game.getPlayer().getID() + 1) % game.getNumOfPlayers());
    }
}