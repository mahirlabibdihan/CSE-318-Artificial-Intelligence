#ifndef _PLAYER_H
#define _PLAYER_H
#include "Heuristic.hpp"
#include "Agent.hpp"
#include <algorithm>
#include <functional>
using namespace std;
class Player
{

private:
    int id;
    Agent *agent;

public:
    Player(int id, Agent *agent)
    {
        this->id = id;
        this->agent = agent;
    }
    int getId()
    {
        return id;
    }
    int getMove(GameState *game)
    {
        if (CLEAR)
            system("clear");
        if (SIMULATION)
        {
            game->print();
        }

        int move;
        while (true)
        {
            if (SIMULATION)
            {
                cout << CYAN << " Player #" << id + 1 << "[1-6]: " << RESET;
                cout.flush();
            }
            move = agent->findBestMove(game, id);
            if (0 <= move && move < N_HOLES && game->stonesAt(id, move) != 0)
            {
                break;
            }
            else
            {
                cout << RED << " Error: Invalid Move" << endl;
            }
        }
        return move;
    }
};
#endif