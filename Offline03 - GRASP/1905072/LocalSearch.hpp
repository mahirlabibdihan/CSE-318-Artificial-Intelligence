#ifndef __LOCAL_SEARCH_H
#define __LOCAL_SEARCH_H
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
class LocalSearch
{
protected:
    // Setup
    int max_sideway_moves;
    int n, m;
    vector<vector<pair<int, int>>> adj;

    // Reset
    int n_sideway_moves;
    int n_moves;
    vector<bool> visited;
    int64_t max_cut;

    // Local
    int64_t current_improvement, best_improvement;
    bool change;
    int selected;

    // Parameter
    vector<int> partition;

protected:
    void hillClimb(int &v, int64_t &improvement)
    {
        max_cut += improvement;
        partition[v] *= -1;
        ++n_moves;

        if (improvement > 0) // Uphill move
        {
            if (n_sideway_moves > 0) // Previous was sideway
            {
                n_sideway_moves = 0;
                std::fill(visited.begin(), visited.end(), 0);
            }
        }
        else // Sideway move
        {
            ++n_sideway_moves;
            visited[v] = true;
        }
    }
    int64_t calculate(int &v)
    {
        int64_t v_to_T = 0, v_to_S = 0;
        for (auto p : adj[v])
        {
            if (partition[p.first] == +1) // In T
            {
                v_to_T += p.second;
            }
            else if (partition[p.first] == -1) // In S
            {
                v_to_S += p.second;
            }
        }
        return v_to_S - v_to_T;
    }

public:
    LocalSearch(int msm)
    {
        max_sideway_moves = msm;
        n = m = 0;
    }
    void setup(vector<vector<pair<int, int>>> &adj, int &n, int &m)
    {
        this->n = n;
        this->m = m;
        this->adj = adj;
        visited.resize(n);
        partition.resize(n);
    }
    int64_t getMaxCut()
    {
        return max_cut;
    }
    virtual void func1(int &) = 0;
    virtual void func2(int &) = 0;
    void reset()
    {
        max_cut = 0;
        n_sideway_moves = n_moves = 0;
        current_improvement = best_improvement = -1;
        std::fill(visited.begin(), visited.end(), 0);
    }
    int getMoves()
    {
        return n_moves;
    }
    vector<int> shuffleNodes()
    {
        vector<int> V(n);
        std::iota(V.begin(), V.end(), 0);
        random_shuffle(V.begin(), V.end());
        return V;
    }
    void findLocalOptima(vector<int> part)
    {
        reset();
        partition = part;
        int sideway = 0;
        for (change = true; change;) // Loop until improvement not possible
        {
            change = false;
            vector<int> V = shuffleNodes();

            best_improvement = -1;
            selected = -1;
            // O(m)
            for (int &v : V)
            {
                int64_t dw = calculate(v);
                current_improvement = -1 * partition[v] * dw;
                if (current_improvement - visited[v] > -(n_sideway_moves < max_sideway_moves)) // Allow sideway only for non-visited
                {
                    func1(v);
                    // break;
                }
            }

            if (change)
            {
                func2(selected);
            }
        }
        // Reached local maxima
    }
};

class LocalSearchFirst : public LocalSearch
{
public:
    LocalSearchFirst(int msm = 100) : LocalSearch(msm)
    {
    }
    void func1(int &v)
    {
        change = true;
        hillClimb(v, current_improvement);
    }
    void func2(int &v)
    {
    }
};

class LocalSearchBest : public LocalSearch
{
public:
    LocalSearchBest(int msm = 100) : LocalSearch(msm)
    {
    }
    void func1(int &v)
    {
        if (current_improvement > best_improvement)
        {
            best_improvement = current_improvement;
            selected = v;
            change = true;
        }
    }
    void func2(int &v)
    {
        hillClimb(v, best_improvement);
    }
};
#endif