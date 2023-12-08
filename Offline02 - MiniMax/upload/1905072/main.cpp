#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include "GameManager.hpp"
#include "Strategy.hpp"
#include "Heuristic.hpp"
#include "Agent.hpp"
#include "Color.hpp"
using namespace std;

void setup()
{
    Strategy *algo1 = new MiniMax(11, new Heuristic(Heuristic::Mode::H4));
    Strategy *algo2 = new MiniMax(11, new Heuristic(Heuristic::Mode::H3));
    Agent *ai1 = new AI(algo1);
    Agent *ai2 = new AI(algo2);
    Agent *human1 = new Human();
    Agent *human2 = new Human();

    // Pass 2 agents to GameManager
    // GameManager(ai1, ai2).autoplay(100);
    GameManager(ai1, ai2).play();
    delete algo1, algo2;
    delete ai1, ai2;
    delete human1, human2;
}
int main()
{
    srand(time(0));
    setup();
}

// 9 2 6 7 9
// 3, 0, 9, 4, 8
// 4 4 0 0 2 - Draw