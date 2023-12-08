#include "Grasp.hpp"
#include "CMDParser.hpp"
#include <fstream>
using namespace std;

int max_iter;
double alpha;
string graph;
string dir;
int max_sideway_moves;
map<string, string> args;
void generateReport(int start_case, int end_case)
{
    for (int i = start_case; i <= end_case; i++)
    {
        string input_file = dir + "/g" + to_string(i) + ".rud";
        clog << "Running " + input_file << endl;
        ifstream in(input_file);
        if (!in.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
            return;
        }
        int n, m;
        in >> n >> m;
        vector<vector<pair<int, int>>> adj(n);
        for (int i = 0; i < m; i++)
        {
            int from, to, weight;
            in >> from >> to >> weight;
            adj[from - 1].push_back({to - 1, weight});
            adj[to - 1].push_back({from - 1, weight});
        }
        cout << "G" << i << "," << n << "," << m << ",";
        {
            GreedyMaxcut greedy;
            SemiGreedyMaxcut semigreedy;
            RandomizedMaxcut randomized;

            greedy.setup(adj, n, m);
            semigreedy.setup(adj, n, m);
            randomized.setup(adj, n, m);

            double random_avg = 0, semi_avg = 0, greedy_avg = 0;
            for (int j = 1; j <= 100; j++)
            {
                randomized.createPartition();
                semigreedy.setAlpha(alpha);
                semigreedy.createPartition();
                greedy.createPartition();

                random_avg += randomized.getMaxcut();
                semi_avg += semigreedy.getMaxcut();
                greedy_avg += greedy.getMaxcut();
                // cout << "Construction: " << j << "/100" << endl;
            }
            cout << random_avg / 100 << "," << semi_avg / 100 << "," << greedy_avg / 100 << ",";
            cout.flush();
        }

        {
            LocalSearchFirst ls_first_1(0), ls_first_2(100);
            LocalSearchBest ls_best(100);
            SemiGreedyMaxcut semigreedy;

            ls_first_1.setup(adj, n, m);
            ls_first_2.setup(adj, n, m);
            semigreedy.setup(adj, n, m);
            ls_best.setup(adj, n, m);
            double ls1_avg = 0, ls2_avg = 0, ls1_iter = 0, ls2_iter = 0;
            for (int j = 0; j < 100; j++)
            {
                vector<int> partition = semigreedy.createPartition();
                ls_first_1.findLocalOptima(partition);
                ls1_avg += ls_first_1.getMaxCut() + semigreedy.getMaxcut();
                ls1_iter += ls_first_1.getMoves();
                {
                    if (rand() % 2)
                    {
                        ls_first_2.findLocalOptima(partition);
                        ls2_avg += ls_first_2.getMaxCut() + semigreedy.getMaxcut();
                        ls2_iter += ls_first_2.getMoves();
                    }
                    else
                    {
                        ls_best.findLocalOptima(partition);
                        ls2_avg += ls_best.getMaxCut() + semigreedy.getMaxcut();
                        ls2_iter += ls_best.getMoves();
                    }
                }
            }
            cout << ls1_iter / 100 << "," << ls1_avg / 100 << "," << ls2_iter / 100 << "," << ls2_avg / 100 << ",";
            cout.flush();
        }

        {
            Grasp grasp(max_sideway_moves);
            grasp.setup(max_iter, alpha);
            grasp.findSolution(adj, n, m);
            cout << grasp.getBestIter() << "," << grasp.getMaxCut() << endl;
        }
    }
}
int main(int argc, char *argv[])
{
    srand(1905072);
    CMDParser parser;
    parser.add("maxIter", "100");
    parser.add("alpha", "-1");
    parser.add("graph", "1");
    parser.add("inputDir", "input");
    parser.add("sidewayLimit", "100");
    args = parser.parse(argc, argv);
    if (args.count("help"))
    {
        cout << "Program Options:" << endl;
        cout << "--maxIter:\tMaximum iteration limit of Grasp" << endl;
        cout << "--alpha:\tAlpha for Semigreedy construction" << endl;
        cout << "--graph:\tNumber of the graph test case to run" << endl;
        cout << "--inputDir:\tInput directory of the testcase (g1-54.rud)" << endl;
        cout << "--sidewayLimit:\tMaximum number of consecutive sideway moves" << endl;
        cout << "--report:\tRange of test cases to run to generate report (Ex: --report=1:54)" << endl;
        return EXIT_SUCCESS;
    }
    max_iter = stoi(args["maxIter"]);
    alpha = stod(args["alpha"]);
    graph = args["graph"];
    dir = args["inputDir"];
    max_sideway_moves = stoi(args["sidewayLimit"]);
    if (args.count("report"))
    {
        vector<string> tokens = parser.tokenize(args["report"], ':');
        generateReport(stoi(tokens[0]), stoi(tokens[1]));
    }
    else
    {
        string input_file = dir + "/g" + graph + ".rud";
        ifstream in(input_file);
        if (!in.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
            return EXIT_FAILURE;
        }
        int n, m;
        in >> n >> m;
        vector<vector<pair<int, int>>> adj(n);
        for (int i = 0; i < m; i++)
        {
            int from, to, weight;
            in >> from >> to >> weight;
            adj[from - 1].push_back({to - 1, weight});
            adj[to - 1].push_back({from - 1, weight});
        }

        Grasp grasp(max_sideway_moves);
        grasp.setup(max_iter, alpha);
        grasp.findSolution(adj, n, m);

        cout << "Maximum cut: " << grasp.getMaxCut() << endl;
    }
}