#ifndef _N_PUZZLE_H
#define _N_PUZZLE_H

#include "Board.hpp"
#include <iostream>
#include <queue>
#include <set>
using namespace std;

class NPuzzle
{
    Board *initial_board;
    Board *goal_board;

public:
    NPuzzle(vector<vector<int>> &grid)
    {
        initial_board = new Board(grid);
    }

    enum HeuristicType
    {
        HAMMING_DISTANCE,
        MANHATTAN_DISTANCE
    };

    void solve(HeuristicType type)
    {
        goal_board = nullptr;
        if (initial_board->isSolvable())
        {
            priority_queue<pair<int, Board *>> pq;
            pq.push({-getHeuristicCost(initial_board, type), initial_board});
            int n = initial_board->getDimension();

            while (!pq.empty())
            {
                Board *curr_board = pq.top().second;
                pq.pop();

                if (!getHeuristicCost(curr_board, type))
                {
                    goal_board = curr_board;
                    return;
                }

                int dx[] = {-1, 1, 0, 0};
                int dy[] = {0, 0, -1, 1};
                Board *new_board = nullptr;
                for (int i = 0; i < 4; i++)
                {
                    new_board = curr_board->slide(dx[i], dy[i]);
                    if (new_board != nullptr)
                    {
                        pq.push({-(new_board->getMoves() + getHeuristicCost(new_board, type)), new_board});
                    }
                }
            }
        }
    }
    void printSolution()
    {
        if (goal_board)
        {
            cout << "Minimum number of moves = " << goal_board->getMoves() << endl
                 << endl;

            printPath(goal_board);
        }
        else
        {
            cout << "Unsolvable puzzle" << endl;
        }
    }

private:
    int getHeuristicCost(Board *board, HeuristicType type)
    {
        switch (type)
        {
        case HAMMING_DISTANCE:
            return board->getHammingDistance();
            break;
        case MANHATTAN_DISTANCE:
            return board->getManhattanDistance();
            break;
        default:
            break;
        }
        return -1;
    }
    void printPath(Board *curr)
    {
        if (curr == nullptr)
        {
            return;
        }
        printPath(curr->getParent());
        cout << curr->getBoard() << endl;
    }
};

#endif