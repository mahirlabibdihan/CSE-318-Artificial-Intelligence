#ifndef _HEURISTIC_H
#define _HEURISTIC_H
class GameState;
class Heuristic
{
    int W[5];

public:
    enum Mode
    {
        H1,
        H2,
        H3,
        H4,
        H5
    };

public:
    Heuristic();
    Heuristic(Mode h);
    void randomize();
    int getValue(GameState *state, int id);
};
#endif