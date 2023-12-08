#ifndef __GRASP_H
#define __GRASP_H
#include <chrono>
#include "Construction.hpp"
#include "LocalSearch.hpp"
class Grasp
{
    int max_iter, iter;
    int64_t max_cut, best_iter;
    vector<int> max_partition;
    std::chrono::high_resolution_clock::time_point start_time;

    SemiGreedyMaxcut semigreedy;
    LocalSearchFirst local_search_first;
    LocalSearchBest local_search_best;

    double alpha;

private:
    int64_t calculateMaxcut(vector<vector<pair<int, int>>> &adj, vector<int> partition)
    {
        int n = partition.size();
        int64_t max_cut = 0;
        for (int v = 0; v < n; v++)
        {
            if (partition[v] == -1) // In S
            {
                for (pair<int, int> p : adj[v])
                {
                    if (partition[p.first] == +1) // In T
                    {
                        max_cut += p.second;
                    }
                }
            }
        }
        return max_cut;
    }
    void init(vector<vector<pair<int, int>>> &adj, int &n, int &m)
    {
        start_time = std::chrono::high_resolution_clock::now();
        semigreedy.setup(adj, n, m);
        local_search_first.setup(adj, n, m);
        local_search_best.setup(adj, n, m);
    }

public:
    Grasp(int max_sideway_moves) : local_search_first(max_sideway_moves), local_search_best(max_sideway_moves)
    {
    }
    void iteration(SemiGreedyMaxcut *semigreedy, LocalSearch *local_search)
    {
        semigreedy->setAlpha(alpha);
        vector<int> partition = semigreedy->createPartition();
        local_search->findLocalOptima(partition);
        int64_t cut = local_search->getMaxCut() + semigreedy->getMaxcut();
        if (cut > max_cut)
        {
            max_cut = cut;
            best_iter = iter;
            max_partition = partition;

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            clog << "At +" << duration.count() / 1e6 << "s: Iter: " << iter << "/" << max_iter << ", Maximum cut: " << max_cut << ", alpha: " << semigreedy->getAlpha() << ", Local Moves: " << local_search->getMoves() << endl;
        }
    }
    void setup(int max_iter, double alpha)
    {
        this->max_iter = max_iter;
        this->alpha = alpha;
    }
    int64_t getMaxCut()
    {
        return max_cut;
    }
    int getBestIter()
    {
        return best_iter;
    }
    void findSolution(vector<vector<pair<int, int>>> &adj, int &n, int &m)
    {
        init(adj, n, m);
        max_cut = LLONG_MIN;
        for (iter = 1; iter <= max_iter; iter++)
        {
            if (rand() % 2)
            {
                iteration(&semigreedy, &local_search_first);
            }
            else
            {
                iteration(&semigreedy, &local_search_best);
            }
        }
    }
};
#endif