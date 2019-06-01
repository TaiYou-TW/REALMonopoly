#include "Game.h"
#include <fstream>
#include <random>
#include <sstream>

using namespace std;

Game::Game() {}
Game::Game(const string &f) {}

void Game::loadFile(const string &f) {
    vector<string> readLine;
    ifstream file;
    file.open(f);
    if (file.is_open()) {
        for (string buf; getline(file, buf);) {
            readLine.push_back(buf);
        }
        file.close();
    }

    if (!readLine.empty()) {
        stringstream ss;
        bool buildMap = true;

        for (uint16_t i = 0; i < readLine.size(); i++) {
            string line = readLine[i];
            ss << line;

            if (i == 0) {  // first line
                ss << readLine[0];
                ss >> mapName >> rounds >> numOfPlayers;
                continue;
            } else if (line[0] == 'p' && line[1] == 'l') {  // player state
                buildMap = false;
                string ignore;
                ss >> ignore >> nowPlaying;
                continue;
            }
            // build map input
            if (buildMap) {
                int32_t loc, type, price;
                string name;
                ss >> loc >> name >> type;
                if (type >= 0) {  // std build
                    ss >> price;
                    vector<int32_t> tax;
                    for (int32_t taxBuf; ss >> taxBuf;) {
                        tax.push_back(taxBuf);
                    }
                    Building b(loc, name, type, price, 0, -1, tax);
                    map.push_back(b);
                } else {  // fate or chance
                    Building b(loc, name, type);
                    map.push_back(b);
                }
            } else {  // player input
                /* TODO */
            }
        }
    }
}
void Game::saveFile() const {}

const bool Game::isEnd() const {}
const int32_t Game::rollDice() const {
    random_device seed;
    mt19937_64 gen(seed());
    uniform_int_distribution<int> distribute(1, 6);
    return distribute(gen);
}

void Game::draw() const { /* TODO */
}
void Game::rollDicePhase() {
    // get player
    Player &now = playerList[nowPlaying];

    // move
    now.setLoc((now.getLoc += rollDice()) % map.size());
}
void Game::estatePhase(const string &action) {
    Player &now = playerList[nowPlaying];

    if (map[now.getLoc()].getType < 0) {  // fate or chance
        fateChancePhase();
    } else {                                     // buy or give tax
        if (map[now.getLoc()].getOwner() > 0) {  // building belongs to a player
            // current player is not the owner
            if (map[now.getLoc()].getOwner() != now.getID()) {
                // give tax to another player
                Player &owner = playerList[map[now.getLoc].getOwner()];
                now.setDeposit(now.getDeposit() - map[now.getLoc()].getTax());
                owner.setDeposit(owner.getDeposit() + map[now.getLoc()].getTax());
            } else {
                if (action == "Y") {
                }
            }
        } else {
            if (action == "Y") {  // player buy the building
                map[now.getLoc()].setOwner(now.getID());
                now.setDeposit(now.getDeposit() - map[now.getLoc()].getPrice());
            }
        }
    }
}
void Game::fateChancePhase() {}

const int32_t Game::getRounds() const { return rounds; }
const int32_t Game::getNumOfPlayers() const { return numOfPlayers; }
Player &Game::getPlayer() { return playerList[nowPlaying]; }
Player &Game::getPlayer(const int32_t &p) { return playerList[p]; }

void Game::setRounds(const int32_t &r) { rounds = r; }
void Game::setPlayers(const int32_t &p) { numOfPlayers = p; }
void Game::setNowPlaying(const int32_t &p) { nowPlaying = p; }
