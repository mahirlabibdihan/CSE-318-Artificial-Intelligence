#ifndef _STATE_H
#define _STATE_H
#include <vector>
#include <functional>
using namespace std;

class Heuristic;
class State
{
public:
    virtual int getHeuristicValue(Heuristic *) = 0;
    virtual int getActualValue() = 0;
    virtual int getInvalidMove() = 0;
    virtual vector<int> getValidMoves() = 0;
    virtual State *getNextState(int move) = 0;
    virtual void makeMove(int move) = 0;
    virtual bool playersHaveSwitched() = 0;
    virtual bool isTerminal() = 0;
    virtual ~State() {}
};
#endif