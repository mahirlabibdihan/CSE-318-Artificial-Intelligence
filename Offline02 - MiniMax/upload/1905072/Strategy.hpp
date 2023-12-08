#ifndef _STRATEGY_H
#define _STRATEGY_H
#include <iostream>
#include <functional>
#include <climits>
#include <cmath>
#include "State.hpp"
#include "Heuristic.hpp"
using namespace std;

class Strategy
{
public:
    virtual int execute(State *state) = 0;
    virtual ~Strategy() {}
};

class MiniMax : public Strategy
{
    int curr_depth;
    int max_depth;
    bool is_max;
    State *state;
    Heuristic *heuristic;
    int explored;

public:
    MiniMax(int depth, Heuristic *heuristic)
    {
        this->max_depth = depth;
        this->heuristic = heuristic;
    }
    ~MiniMax()
    {
    }
    int execute(State *state)
    {
        // {value, move}
        pair<int, int> best_move = {INT_MIN, state->getInvalidMove()};
        explored = 0;
        for (int move : state->getValidMoves()) // Successor function
        {
            State *new_state = state->getNextState(move);
            curr_depth = max_depth;
            is_max = !new_state->playersHaveSwitched();
            int value = backtrack(new_state, INT_MIN, INT_MAX);
            // Choose best move
            if (best_move.second == state->getInvalidMove() || best_move.first < value)
            {
                best_move = {value, move};
            }
            delete new_state;
        }
        return best_move.second;
    }
    int backtrack(State *state, int alpha, int beta)
    {
        explored++;
        if (curr_depth == 0) // Leaf
        {
            return state->getHeuristicValue(heuristic);
        }
        int best_value = is_max ? INT_MIN : INT_MAX;
        for (int move : state->getValidMoves())
        {
            State *next_state = state->getNextState(move);
            int value;
            if (next_state->isTerminal())
            {
                value = next_state->getActualValue(); // Win: INT_MAX, Lose: INT_MIN, DRAW: 0 // Utility function
            }
            else
            {
                curr_depth--;
                is_max = next_state->playersHaveSwitched() ? !is_max : is_max;
                value = backtrack(next_state, alpha, beta);
                is_max = next_state->playersHaveSwitched() ? !is_max : is_max;
                curr_depth++;
            }
            delete next_state;
            if (is_max)
            {
                best_value = max(best_value, value);
                alpha = max(alpha, best_value);
            }
            else
            {
                best_value = min(best_value, value);
                beta = min(beta, best_value);
            }
            if (beta <= alpha)
            {
                break;
            }
        }
        return best_value;
    }
};
#endif