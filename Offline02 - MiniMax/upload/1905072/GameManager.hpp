#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H
#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <map>
#include <functional>
#include "Constant.hpp"
#include "Color.hpp"
using namespace std;

class Player;
class GameState;
class Agent;
class GameManager
{
    vector<Player *> players;
    GameState *game;

public:
    GameManager(Agent *a1, Agent *a2);
    ~GameManager();
    void gameOver();
    void autoplay(int n_matches);
    void loop(GameState *game);
    void play();
};
#endif