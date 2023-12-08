#include "Heuristic.hpp"
#include "GameState.hpp"
#include <cstdlib>
Heuristic::Heuristic(Mode h)
{
    // 9, 2, 6, 7, 9
    W[0] = W[1] = W[2] = W[3] = W[4] = 0;
    switch (h)
    {
    case H1:
        W[0] = 1;
        break;
    case H2:
        W[0] = 9, W[1] = 2;
        break;
    case H3:
        W[0] = 5, W[1] = 9, W[2] = 6;
        break;
    case H4:
        W[0] = 9, W[1] = 2, W[2] = 6, W[3] = 7;
        break;
    case H5:
        W[0] = 9, W[1] = 2, W[2] = 6, W[3] = 7, W[4] = 9;
        break;
    }
}
Heuristic::Heuristic()
{
    W[0] = rand() % 10; // storage difference
    W[1] = rand() % 10; // holes difference
    W[2] = rand() % 10; // extra moves
    W[3] = rand() % 10; // captured stones
    W[4] = rand() % 10;
}
int Heuristic::getValue(GameState *state, int id)
{
    return W[0] * (state->getStorage(id) - state->getStorage(!id)) +
           W[1] * (state->getTotalStones(id) - state->getTotalStones(!id)) +
           W[2] * state->getExtraMoves(id) +
           W[3] * state->getCapturedStones(id);
    //    + W[4] * (state->getCloseToStorage(id) - state->getCloseToStorage(!id));
}

void Heuristic::randomize()
{
    W[0] = rand() % 10;
    W[1] = rand() % 10;
    W[2] = rand() % 10;
    W[3] = rand() % 10;
    W[4] = rand() % 10;
}