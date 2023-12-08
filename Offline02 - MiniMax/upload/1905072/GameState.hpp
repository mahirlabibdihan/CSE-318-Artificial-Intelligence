#ifndef _GAME_STATE_H
#define _GAME_STATE_H
#include <vector>
#include "State.hpp"
#include "Constant.hpp"
using namespace std;
class Heuristic;
class GameState : public State
{
    int hole[N_ROWS][N_HOLES];
    int stones_in_storage[N_ROWS];
    int total_stones[N_ROWS];
    int extra_moves[N_ROWS];
    int stones_captured[N_ROWS];
    int moves[N_ROWS];
    int player_id;
    bool players_switched;

public:
    GameState(int player_id);
    void switchPlayers();
    void dontSwitchPlayers();
    bool playersHaveSwitched();
    int getPlayerId();
    int getWinner();
    int getCapturedStones(int row);
    int getTotalStones(int row);
    int getExtraMoves(int row);
    bool isValid();
    int getInvalidMove();
    vector<int> getValidMoves();
    State *getNextState(int n);
    void clearHole(int n);
    void captureStone(int n);
    void makeMove(int n); // Returns last position
    int getCloseToStorage(int row);
    int &stonesAt(int row, int col);
    int stonesAt(pair<int, int> pos);
    int &getStorage(int row);
    void addToStorage(int row);
    bool isTerminal();
    int getActualValue();
    int getHeuristicValue(Heuristic *heuristic);
    void wrapup();
    void print();
};
#endif