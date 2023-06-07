#ifndef _BOARD_H
#define _BOARD_H

#include <vector>
#include <cmath>
#include <string>
using namespace std;

class Board
{
    vector<vector<int>> grid;
    int n_dim;
    int n_moves;
    int b_row, b_col;
    int h_cost, m_cost;
    Board *parent;

public:
    Board(vector<vector<int>> &grid)
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
    Board *slideDown()
    {
        if (b_row == 0)
        {
            return nullptr;
        }
        else if (parent != nullptr && parent->b_row == b_row - 1 && parent->b_col == b_col)
        {
            return nullptr;
        }
        else
        {
            Board *new_board = new Board(*this);

            new_board->m_cost -= new_board->distRowCol(new_board->b_row - 1, new_board->b_col);
            new_board->h_cost -= new_board->isWrongPosition(new_board->b_row - 1, new_board->b_col);

            swap(new_board->grid[b_row][b_col], new_board->grid[b_row - 1][b_col]);
            new_board->b_row--;

            new_board->m_cost += new_board->distRowCol(new_board->b_row + 1, new_board->b_col);
            new_board->h_cost += new_board->isWrongPosition(new_board->b_row + 1, new_board->b_col);

            new_board->parent = this;
            new_board->n_moves++;
            return new_board;
        }
    }
    Board *slideUp()
    {
        if (b_row == n_dim - 1)
        {
            return nullptr;
        }
        else if (parent != nullptr && parent->b_row == b_row + 1 && parent->b_col == b_col)
        {
            return nullptr;
        }
        else
        {
            Board *new_board = new Board(*this);

            new_board->m_cost -= new_board->distRowCol(new_board->b_row + 1, new_board->b_col);
            new_board->h_cost -= new_board->isWrongPosition(new_board->b_row + 1, new_board->b_col);

            swap(new_board->grid[b_row][b_col], new_board->grid[b_row + 1][b_col]);
            new_board->b_row++;

            new_board->m_cost += new_board->distRowCol(new_board->b_row - 1, new_board->b_col);
            new_board->h_cost += new_board->isWrongPosition(new_board->b_row - 1, new_board->b_col);

            new_board->parent = this;
            new_board->n_moves++;
            return new_board;
        }
    }
    Board *slideRight()
    {

        if (b_col == 0)
        {
            return nullptr;
        }
        else if (parent != nullptr && parent->b_row == b_row && parent->b_col == b_col - 1)
        {
            return nullptr;
        }
        else
        {
            Board *new_board = new Board(*this);

            new_board->m_cost -= new_board->distRowCol(new_board->b_row, new_board->b_col - 1);
            new_board->h_cost -= new_board->isWrongPosition(new_board->b_row, new_board->b_col - 1);

            swap(new_board->grid[b_row][b_col], new_board->grid[b_row][b_col - 1]);
            new_board->b_col--;

            new_board->m_cost += new_board->distRowCol(new_board->b_row, new_board->b_col + 1);
            new_board->h_cost += new_board->isWrongPosition(new_board->b_row, new_board->b_col + 1);

            new_board->parent = this;
            new_board->n_moves++;
            return new_board;
        }
    }
    Board *slideLeft()
    {
        if (b_col == n_dim - 1)
        {
            return nullptr;
        }
        else if (parent != nullptr && parent->b_row == b_row && parent->b_col == b_col + 1)
        {
            return nullptr;
        }
        else
        {
            Board *new_board = new Board(*this);

            new_board->m_cost -= new_board->distRowCol(new_board->b_row, new_board->b_col + 1);
            new_board->h_cost -= new_board->isWrongPosition(new_board->b_row, new_board->b_col + 1);

            swap(new_board->grid[b_row][b_col], new_board->grid[b_row][b_col + 1]);
            new_board->b_col++;

            new_board->m_cost += new_board->distRowCol(new_board->b_row, new_board->b_col - 1);
            new_board->h_cost += new_board->isWrongPosition(new_board->b_row, new_board->b_col - 1);

            new_board->parent = this;
            new_board->n_moves++;
            return new_board;
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
    vector<vector<int>> getGrid()
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
        // cout << "(" << i << "," << j << ")=>" << this->get(i, j) << endl;
        int n = this->getDimension();
        int row = (this->get(i, j) - 1) / n;                             // O index
        int col = this->get(i, j) % n ? this->get(i, j) % n - 1 : n - 1; // O index
        // cout << "(" << row << "," << col << ")" << endl;
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
                    // cout << this->get(i, j) << " " << cost << endl;
                }
            }
        }
        return cost;
    }
};

#endif