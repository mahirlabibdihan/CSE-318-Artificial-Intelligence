#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>
#include "State.hpp"
#include "GameState.hpp"
#include "Constant.hpp"
#include "Heuristic.hpp"
using namespace std;

GameState::GameState(int player_id)
{
    this->player_id = player_id;
    for (int i = 0; i < N_ROWS; i++)
    {
        total_stones[i] = 0;
        for (int j = 0; j < N_HOLES; j++)
        {
            hole[i][j] = N_STONES;
            total_stones[i] += N_STONES;
        }
        stones_in_storage[i] = 0;
        extra_moves[i] = 0;
        moves[i] = 0;
    }
}
void GameState::switchPlayers()
{
    players_switched = true;
    moves[player_id]++;
    player_id = !player_id;
}
void GameState::dontSwitchPlayers()
{
    moves[player_id]++;
    players_switched = false;
}
bool GameState::playersHaveSwitched()
{
    return players_switched;
}
int GameState::getPlayerId()
{
    return player_id;
}
int GameState::getWinner()
{
    if (isValid())
    {
        return -1;
    }

    int p1_point = this->getTotalStones(0), p2_point = this->getTotalStones(1);
    if (p1_point > p2_point)
    {
        return 0;
    }
    else if (p1_point < p2_point)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
int GameState::getCapturedStones(int row)
{
    return stones_captured[row];
}
int GameState::getTotalStones(int row)
{
    return total_stones[row];
}
int GameState::getExtraMoves(int row)
{
    return extra_moves[row];
}
bool GameState::isValid()
{
    return total_stones[0] && total_stones[1];
}
int GameState::getInvalidMove()
{
    return -1;
}
vector<int> GameState::getValidMoves()
{
    vector<int> valid_moves;
    for (int move = 0; move < N_HOLES; move++)
    {
        if (this->stonesAt(player_id, move))
        {
            valid_moves.push_back(move);
        }
    }
    random_shuffle(valid_moves.begin(), valid_moves.end());
    return valid_moves;
}
State *GameState::getNextState(int n)
{
    GameState *new_state = new GameState(*this);
    new_state->makeMove(n);
    if (new_state->isTerminal())
    {
        new_state->wrapup();
    }
    return new_state;
}

void GameState::clearHole(int n)
{
    total_stones[player_id] -= hole[player_id][n];
    hole[player_id][n] = 0;
}

void GameState::captureStone(int n)
{
    stones_captured[player_id] += hole[!player_id][N_HOLES - n - 1];
    total_stones[player_id] -= hole[player_id][n];
    total_stones[!player_id] -= hole[!player_id][N_HOLES - n - 1];
    stones_in_storage[player_id] += hole[player_id][n] + hole[!player_id][N_HOLES - n - 1];
    hole[player_id][n] = hole[!player_id][N_HOLES - n - 1] = 0;
}
void GameState::makeMove(int n) // Returns last position
{
    int row = player_id;
    int steps = hole[row][n];
    clearHole(n);
    while (steps--)
    {
        // Go forward
        if (n == N_HOLES)
        {
            row = !row; // Switch row
            n = 0;      // Current move first hole
        }
        else
        {
            n++;
        }

        // Evaluate after n++
        if (n == N_HOLES)
        {
            if (row == player_id)
            {
                stones_in_storage[row]++; // Add to storage
            }
            else
            {
                row = !row;     // Switch row
                n = 0;          // Current move first hole
                hole[row][n]++; // Add to first hole
                total_stones[row]++;
            }
        }
        else
        {
            hole[row][n]++; // Add to next hole
            total_stones[row]++;
        }
    }
    if (row == player_id && n != N_HOLES && hole[row][n] == 1 && hole[!row][N_HOLES - n - 1] > 0) // Captures stones
    {
        captureStone(n);
    }

    // Last stone at storage
    // Get a free turn
    if (n == N_HOLES)
    {
        extra_moves[row]++;
        dontSwitchPlayers();
        return;
    }
    switchPlayers();
}

int GameState::getCloseToStorage(int row)
{
    int cnt = 0;
    for (int i = 0; i < N_HOLES; i++)
    {
        cnt += min(N_HOLES - i, hole[row][i]);
    }
    return cnt;
}
int &GameState::stonesAt(int row, int col)
{
    return hole[row][col];
}
int GameState::stonesAt(pair<int, int> pos)
{
    return hole[pos.first][pos.second];
}
int &GameState::getStorage(int row)
{
    return stones_in_storage[row];
}
void GameState::addToStorage(int row)
{
    stones_in_storage[row]++;
}
bool GameState::isTerminal()
{
    return (!this->isValid() || this->getStorage(player_id) > N_HOLES * N_STONES || this->getStorage(!player_id) > N_HOLES * N_STONES);
}
int GameState::getActualValue()
{
    if (isTerminal())
    {
        if (!this->isValid())
        {
            if (this->getWinner() == player_id)
            {
                return INT_MAX;
            }
            else if (this->getWinner() == !player_id)
            {
                return INT_MIN;
            }
            else
            {
                // Draw
                return 0;
                // return heuristic(game, id);
            }
        }
        if (this->getStorage(player_id) > N_HOLES * N_STONES)
        {
            // cout << "Player going to win" << endl;
            return INT_MAX; // Max will win anyway
        }
        if (this->getStorage(!player_id) > N_HOLES * N_STONES)
        {
            // cout << "Opponent going to win" << endl;
            return INT_MIN; // Min will win anyway
        }
    }
    cout << "Not a Terminal" << endl;
    return 0;
}
int GameState::getHeuristicValue(Heuristic *heuristic)
{
    return heuristic->getValue(this, player_id);
}
void GameState::wrapup()
{
    if (total_stones[0] == 0)
    {
        stones_in_storage[1] += total_stones[1];
    }
    if (total_stones[1] == 0)
    {
        stones_in_storage[0] += total_stones[0];
    }
}
void GameState::print()
{
    // cout << moves[0] << " " << moves[1] << endl;
    printf("                                              \n");
    printf(" P         6    5    4    3    2    1        P\n");
    printf(" L ╔════╦════╦════╦════╦════╦════╦════╦════╗ L\n");
    printf(" A ║    ║ %02d ║ %02d ║ %02d ║ %02d ║ %02d ║ %02d ║    ║ A\n",
           hole[1][5], hole[1][4], hole[1][3], hole[1][2], hole[1][1], hole[1][0]);
    printf(" Y ║ %02d ╠════╬════╬════╬════╬════╬════╣ %02d ║ Y\n", stones_in_storage[1], stones_in_storage[0]);
    printf(" E ║    ║ %02d ║ %02d ║ %02d ║ %02d ║ %02d ║ %02d ║    ║ E\n",
           hole[0][0], hole[0][1], hole[0][2], hole[0][3], hole[0][4], hole[0][5]);
    printf(" R ╚════╩════╩════╩════╩════╩════╩════╩════╝ R\n");
    printf(" 2         1    2    3    4    5    6        1\n");
    printf("                                              \n");
}