#ifndef _BOARD_H
#define _BOARD_H

#include <vector>
#include <cmath>
#include <string>
using namespace std;

class Board
{
    vector<vector<uint8_t>> grid;
    int n_dim;
    int n_moves;
    int b_row, b_col;
    int h_cost, m_cost;
    Board *parent;

public:
    Board(vector<vector<uint8_t>> &grid)
    {
        this->grid = grid;
        this->n_dim = grid.size();
        this->parent = nullptr;
        this->n_moves = 0;

        for (int i = 0; i < n_dim; i++)
        {
            for (int j = 0; j < n_dim; j++)
            {
                if (grid[i][j] == 0)
                {
                    b_row = i;
                    b_col = j;
                    break;
                }
            }
        }

        this->h_cost = this->hammingDistance();
        this->m_cost = this->manhattanDistance();
    }
    Board(const Board &board)
    {
        this->grid = board.grid;
        this->n_dim = board.n_dim;
        this->parent = nullptr;
        this->b_row = board.b_row;
        this->b_col = board.b_col;
        this->n_moves = board.n_moves;
        this->h_cost = board.h_cost;
        this->m_cost = board.m_cost;
    }
    ~Board()
    {
    }
    int getDimension()
    {
        return n_dim;
    }
    int getMoves()
    {
        return n_moves;
    }
    int get(int i, int j)
    {
        return grid[i][j];
    }
    void setParent(Board *parent)
    {
        this->parent = parent;
    }
    Board *getParent()
    {
        return parent;
    }
    // slide down   : -1  0
    // slide up     :  1  0
    // slide right  :  0  1
    // slide left   :  0 -1
    Board *slide(int dx, int dy)
    {
        int new_row = b_row - dx;
        int new_col = b_col - dy;
        int old_row = b_row;
        int old_col = b_col;

        if (new_row < 0 || new_row >= n_dim || new_col < 0 || new_col >= n_dim) // Check valid
        {
            return nullptr;
        }
        else if (parent != nullptr && parent->b_row == new_row && parent->b_col == new_col) // Check parent
        {
            return nullptr;
        }
        else
        {
            try
            {
                Board *new_board = new Board(*this);
                new_board->m_cost -= new_board->distRowCol(new_row, new_col);
                new_board->h_cost -= new_board->isWrongPosition(new_row, new_col);

                swap(new_board->grid[old_row][old_col], new_board->grid[new_row][new_col]);
                new_board->b_row = new_row;
                new_board->b_col = new_col;

                new_board->m_cost += new_board->distRowCol(old_row, old_col);
                new_board->h_cost += new_board->isWrongPosition(old_row, old_col);

                new_board->parent = this;
                new_board->n_moves++;
                return new_board;
            }
            catch (const std::bad_alloc &e)
            {
                return nullptr;
            }
        }
    }

    int getInversionCount()
    {
        int count = 0;
        for (int i = 0; i < n_dim; i++)
        {
            for (int j = 0; j < n_dim; j++)
            {
                if (grid[i][j] != 0)
                {
                    for (int p = i; p < n_dim; p++)
                    {
                        for (int q = (p == i ? j : 0); q < n_dim; q++)
                        {
                            if (grid[p][q] != 0)
                            {
                                count += grid[i][j] > grid[p][q];
                            }
                        }
                    }
                }
            }
        }
        return count;
    }
    int getBlankRowDistance()
    {
        return n_dim - 1 - b_row;
    }
    bool isSolvable()
    {
        if (n_dim % 2)
        {
            return getInversionCount() % 2 == 0;
        }
        else
        {
            return (getInversionCount() + getBlankRowDistance()) % 2 == 0;
        }
    }
    string getBoard()
    {
        string board;
        for (int i = 0; i < n_dim; i++)
        {
            for (int j = 0; j < n_dim; j++)
            {
                board += to_string(get(i, j)) + " ";
            }
            board += "\n";
        }
        return board;
    }
    vector<vector<uint8_t>> getGrid()
    {
        return grid;
    }
    int getHammingDistance()
    {
        return h_cost;
    }
    int hammingDistance()
    {
        int cost = 0;
        int n = this->getDimension();
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (this->get(i, j) != 0)
                {
                    cost += isWrongPosition(i, j);
                }
            }
        }
        return cost;
    }
    int isWrongPosition(int i, int j)
    {
        int n = this->getDimension();
        int correct = i * n + j + 1;
        return correct != this->get(i, j);
    }
    int distRowCol(int i, int j)
    {
        int n = this->getDimension();
        int row = (this->get(i, j) - 1) / n;                             // O index
        int col = this->get(i, j) % n ? this->get(i, j) % n - 1 : n - 1; // O index
        return abs(row - i) + abs(col - j);
    }
    int getManhattanDistance()
    {
        return m_cost;
    }
    int manhattanDistance()
    {
        int cost = 0;
        int n = this->getDimension();
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (this->get(i, j) != 0)
                {
                    cost += distRowCol(i, j);
                }
            }
        }
        return cost;
    }
};

#endif