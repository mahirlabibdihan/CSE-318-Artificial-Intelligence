#include <iostream>
#include "NPuzzle.hpp"
using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<vector<uint8_t>> grid(n, vector<uint8_t>(n));
    for (vector<uint8_t> &row : grid)
    {
        for (uint8_t &cell : row)
        {
            int a;
            cin >> a;
            cell = a;
        }
    }

    NPuzzle puzzle(grid);
    puzzle.solve(NPuzzle::HeuristicType::MANHATTAN_DISTANCE);
    puzzle.printSolution();
    cout << "Manhattan: \nExplored: " << puzzle.getExplored() << "\nExpanded: " << puzzle.getExpanded() << endl
         << endl;
    puzzle.solve(NPuzzle::HeuristicType::HAMMING_DISTANCE);
    // puzzle.printSolution();
    cout << "Hamming: \nExplored: " << puzzle.getExplored() << "\nExpanded: " << puzzle.getExpanded() << endl
         << endl;
}