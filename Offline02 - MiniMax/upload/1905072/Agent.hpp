#ifndef _AGENT_H
#define _AGENT_H
#include "Strategy.hpp"

class Agent
{
public:
    virtual int findBestMove(State *game, int player) = 0;
    virtual ~Agent() {}
};

class AI : public Agent
{
    Strategy *algo;

public:
    AI(Strategy *algo)
    {
        this->algo = algo;
    }
    ~AI()
    {
    }
    int findBestMove(State *game, int player)
    {
        int move = algo->execute(game);
        cout << move + 1;
        if (PAUSE)
        {
            string s;
            getline(cin, s);
        }
        else
        {
            cout << endl;
        }
        return move;
    }
};

class Human : public Agent
{
public:
    int findBestMove(State *game, int player)
    {
        int move;
        cin >> move;
        cin.ignore();
        return move - 1;
    }
    ~Human()
    {
    }
};
#endif