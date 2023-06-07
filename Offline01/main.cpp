#include <iostream>
#include "NPuzzle.hpp"
using namespace std;

template <typename T>
istream &operator>>(istream &in, vector<T> &a)
{
    for (T &i : a)
    {
        cin >> i;
    }
    return in;
}

int main()
{
    int n;
    cin >> n;
    vector<vector<int>> grid(n, vector<int>(n));
    cin >> grid;

    NPuzzle puzzle(grid);
    puzzle.solve(NPuzzle::HeuristicType::MANHATTAN_DISTANCE);
    puzzle.printSolution();
    // puzzle.solve(NPuzzle::HeuristicType::HAMMING_DISTANCE);
    // puzzle.printSolution();
}