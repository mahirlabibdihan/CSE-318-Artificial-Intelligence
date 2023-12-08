#ifndef __CONSTRUCTION_H
#define __CONSTRUCTION_H
#include <iostream>
#include <set>
#include <vector>
#include "Edge.hpp"
#include <random>
#include <climits>
#include <unordered_set>
#include <algorithm>
using namespace std;

class Construction
{
protected:
    int64_t max_cut;
    int n, m;
    vector<int> partition;
    set<Edge> edge_list;
    vector<vector<pair<int, int>>> adj;
    std::mt19937 gen;

public:
    Construction()
    {
        this->gen = std::mt19937(1905072);
    }
    void setup(vector<vector<pair<int, int>>> &adj, int &n, int &m)
    {
        this->n = n;
        this->m = m;
        this->adj = adj;
        partition.resize(n);
        for (int u = 0; u < n; u++)
        {
            for (pair<int, int> v : adj[u])
            {
                if (u < v.first)
                {
                    edge_list.insert(Edge(u, v.first, v.second));
                }
            }
        }
    }
    virtual vector<int> createPartition() = 0;
    void calculate(int64_t &v_to_T, int64_t &v_to_S, int &v)
    {
        for (auto p : adj[v])
        {
            if (partition[p.first] == +1) // In T
            {
                v_to_T += p.second; // v to T
            }
            else if (partition[p.first] == -1) // In S
            {
                v_to_S += p.second; // v to S
            }
        }
    }
    void reset()
    {
        max_cut = 0;
    }
    int64_t getMaxcut()
    {
        return max_cut;
    }
};

class RandomizedMaxcut : public Construction
{
    std::uniform_int_distribution<int> dis;

public:
    RandomizedMaxcut()
    {
        // Create a uniform distribution for generating 0 or 1
        this->dis = std::uniform_int_distribution<int>(0, 1);
    }
    void reset()
    {
        Construction::reset();
        std::fill(partition.begin(), partition.end(), 0);
    }
    vector<int> createPartition()
    {
        reset();
        // Choose first edge
        int idx = rand() % m;
        set<Edge>::iterator itr = edge_list.begin();
        std::advance(itr, idx);
        // Add its two end nodes to two partitions

        partition[itr->from] = -1;
        partition[itr->to] = +1;

        max_cut = 0;
        for (int u = 0; u < n; u++)
        {
            if (!partition[u])
            {
                if (dis(gen))
                {
                    partition[u] = -1; // Add to S

                    // Increase max-cut
                    for (pair<int, int> p : adj[u])
                    {
                        if (partition[p.first] == +1) // In T
                        {
                            max_cut += p.second;
                        }
                    }
                }
                else
                {
                    partition[u] = +1; // Add to T

                    // Increase max-cut
                    for (pair<int, int> p : adj[u])
                    {
                        if (partition[p.first] == -1) // In T
                        {
                            max_cut += p.second;
                        }
                    }
                }
            }
        }
        return partition;
    }
};

class GreedyMaxcut : public Construction
{
    unordered_set<int> U;

public:
    GreedyMaxcut()
    {
    }
    void setup(vector<vector<pair<int, int>>> &adj, int &n, int &m)
    {
        Construction::setup(adj, n, m);
    }
    void reset()
    {
        Construction::reset();
        U.clear();
        for (int i = 0; i < n; i++)
        {
            U.insert(i);
        }
        std::fill(partition.begin(), partition.end(), 0);
    }
    vector<int> createPartition()
    {
        reset();
        auto itr = edge_list.rbegin();
        // Add its two end nodes to two partitions
        U.erase(itr->from), partition[itr->from] = -1;
        U.erase(itr->to), partition[itr->to] = +1;

        for (int count = 2; count < n; count++)
        {
            int64_t idx_to_T = -1, idx_to_S = -1;
            int idx = -1;
            int64_t w_max = LLONG_MIN;
            for (int v : U)
            {
                int64_t node_to_T = 0, node_to_S = 0;
                calculate(node_to_T, node_to_S, v);
                if (max(node_to_T, node_to_S) > w_max)
                {
                    idx = v;
                    idx_to_T = node_to_T;
                    idx_to_S = node_to_S;
                }
            }

            // Decide in which partition to put idx
            if (idx_to_T > idx_to_S)
            {
                U.erase(idx), partition[idx] = -1;
                max_cut += idx_to_T;
            }
            else
            {
                U.erase(idx), partition[idx] = +1;
                max_cut += idx_to_S;
            }
        }
        return partition;
    }
};

class SemiGreedyMaxcut : public Construction
{
    double alpha;
    std::uniform_real_distribution<double> dis;
    int64_t w_min, w_max;
    unordered_set<int> U;

public:
    SemiGreedyMaxcut()
    {
        this->dis = std::uniform_real_distribution<double>(0.0, 1.0);
    }

    int choose(int64_t &idx_to_S, int64_t &idx_to_T)
    {
        // Calculate threshold of RCL
        int64_t w_min = LLONG_MAX;
        int64_t w_max = LLONG_MIN;

        vector<int64_t> node_to_T(n, 0), node_to_S(n, 0);

        // O(m)
        for (int v : U)
        {
            calculate(node_to_T[v], node_to_S[v], v);
            w_max = max(w_max, max(node_to_T[v], node_to_S[v]));
            w_min = min(w_min, min(node_to_T[v], node_to_S[v]));
        }
        int64_t threshold = w_min + alpha * (w_max - w_min); // Calculate threshold

        vector<int> rcl_v;
        for (int v : U)
        {
            if (max(node_to_T[v], node_to_S[v]) >= threshold) // Can store the max improvement in a set. Then do binary search here.
            {
                rcl_v.push_back(v);
            }
        }

        int idx = rcl_v[rand() % rcl_v.size()];
        idx_to_S = node_to_S[idx];
        idx_to_T = node_to_T[idx];
        return idx;
    }
    void setAlpha(double alpha = -1)
    {
        if (alpha == -1)
        {
            this->alpha = dis(gen);
        }
        else
        {
            this->alpha = alpha;
        }
    }
    double getAlpha()
    {
        return alpha;
    }
    void setup(vector<vector<pair<int, int>>> &adj, int &n, int &m)
    {
        Construction::setup(adj, n, m);
        w_min = edge_list.begin()->weight;
        w_max = edge_list.rbegin()->weight;
    }
    void reset()
    {
        Construction::reset();
        U.clear();
        for (int i = 0; i < n; i++)
        {
            U.insert(i);
        }
        std::fill(partition.begin(), partition.end(), 0);
    }
    vector<int> createPartition()
    {
        reset();
        // Start with a edge
        int64_t threshold = w_min + alpha * (w_max - w_min); // Calculate threshold

        set<Edge>::iterator itr = edge_list.lower_bound(Edge(-1, -1, threshold)); // binary search
        int rcl_length = distance(itr, edge_list.end());
        int idx = rand() % rcl_length; // Edge selected
        max_cut = itr->weight;         // Choose an edge
        // Add its two end nodes to two partitions
        U.erase(itr->from), partition[itr->from] = -1;
        U.erase(itr->to), partition[itr->to] = +1;

        // Now add remaining n-2 one by one
        // O(nm)
        for (int count = 2; count < n; count++)
        {
            int64_t idx_to_S, idx_to_T;
            int idx = choose(idx_to_S, idx_to_T);
            // Decide in which partition to put idx
            if (idx_to_T > idx_to_S)
            {
                U.erase(idx), partition[idx] = -1;
                max_cut += idx_to_T;
            }
            else
            {
                U.erase(idx), partition[idx] = +1;
                max_cut += idx_to_S;
            }
        }
        return partition;
    }
};

#endif