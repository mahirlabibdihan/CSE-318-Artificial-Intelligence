#ifndef __EDGE_H
#define __EDGE_H
class Edge
{
public:
    int from, to, weight;
    Edge() {}
    Edge(int from, int to, int weight)
    {
        this->from = from;
        this->to = to;
        this->weight = weight;
    }
    bool operator<(const Edge &e) const
    {
        return this->weight < e.weight;
    }
};
#endif