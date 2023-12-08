#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <map>
#include <functional>
#include "GameManager.hpp"
#include "GameState.hpp"
#include "Constant.hpp"
#include "Color.hpp"
#include "Player.hpp"
#include <chrono>
using namespace std;

GameManager::GameManager(Agent *a1, Agent *a2) : players(N_PLAYERS)
{
    players[0] = new Player(0, a1);
    players[1] = new Player(1, a2);
}
GameManager::~GameManager()
{
    delete players[0];
    delete players[1];
}
void GameManager::gameOver()
{
    game->wrapup();
    int p1_point = game->getStorage(0), p2_point = game->getStorage(1);
    if (p1_point > p2_point)
    {
        cout << endl;
        cout << GREEN << " 😀 Player #1: " << p1_point << endl;
        cout << RED << " 🙂 Player #2: " << p2_point << endl;
        cout << RESET << endl;
    }
    else if (p1_point < p2_point)
    {
        cout << endl;
        cout << RED << " 🙂 Player #1: " << p1_point << endl;
        cout << GREEN << " 😀 Player #2: " << p2_point << endl;
        cout << RESET << endl;
    }
    else
    {
        cout << endl;
        cout << YELLOW << " 🙂 Player #1: " << p1_point << endl;
        cout << YELLOW << " 🙂 Player #2: " << p2_point << endl;
        cout << RESET << endl;
    }
}
void GameManager::autoplay(int n_matches)
{
    int n_p1 = 0, n_p2 = 0;
    while (n_matches--)
    {
        game = new GameState(0);
        loop(game);
        game->print();

        if (game->getWinner() == 0)
        {
            cout << "#1 Win" << endl;
            n_p1++;
        }
        else if (game->getWinner() == 1)
        {
            cout << "#2 Win" << endl;
            n_p2++;
        }
        else
        {
            cout << "Draw" << endl;
        }
        // cout << n_matches << endl;
        delete game;
        // if (RANDOM)
        //     Heuristic::init();
    }
    cout << "Player #1: " << n_p1 << endl;
    cout << "Player #2: " << n_p2 << endl;
}
void GameManager::loop(GameState *game)
{
    double time[] = {0, 0};
    while (game->isValid())
    {
        int turn = game->getPlayerId();
        auto startTime = chrono::high_resolution_clock::now();
        game->makeMove(players[turn]->getMove(game));
        auto endTime = chrono::high_resolution_clock::now();
        time[turn] += chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count() / 1000000000.0;
    }
    cout << " Player #1 Time: " << time[0] << " seconds" << endl;
    cout << " Player #2 Time: " << time[1] << " seconds" << endl;
}
void GameManager::play()
{
    game = new GameState(0); // Initial State
    loop(game);
    if (CLEAR)
        system("clear");
    game->print();
    gameOver();
    delete game;
}