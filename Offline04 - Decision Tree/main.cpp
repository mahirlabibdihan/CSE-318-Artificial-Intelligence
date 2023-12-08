#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <set>

using namespace std;

vector<pair<string, vector<string>>> all_attributes = {
    {"buying", {"vhigh", "high", "med", "low"}},
    {"maint", {"vhigh", "high", "med", "low"}},
    {"doors", {"2", "3", "4", "5more"}},
    {"persons", {"2", "4", "more"}},
    {"lug_boot", {"small", "med", "big"}},
    {"safety", {"low", "med", "high"}}};

int depth = 0;
class Node
{
public:
    int attribute;
    string label;
    unordered_map<string, Node *> children;

    Node(int attr = -1, string lbl = "")
    {
        attribute = attr;
        label = lbl;
    }
};

class Dataset
{
public:
    vector<vector<string>> matrix; // Data matrix
    Dataset subset(int attr_index, string attr_value) const
    {
        Dataset subset;
        for (int i = 0; i < matrix.size(); i++)
        {
            if (matrix[i][attr_index] == attr_value)
            {
                subset.matrix.push_back(matrix[i]);
            }
        }
        return subset;
    }
};

Dataset loadDataset()
{
    Dataset dataset;

    ifstream file("car.data");
    if (!file.is_open())
    {
        cerr << "Failed to open the dataset file." << endl;
        return dataset;
    }

    string line;
    while (getline(file, line))
    {
        vector<string> row_tokens;
        istringstream row_stream(line);
        string token;

        while (getline(row_stream, token, ','))
        {
            row_tokens.push_back(token);
        }

        dataset.matrix.push_back(row_tokens);
    }
    return dataset;
}

void splitDataset(Dataset dataset, Dataset &training_set, Dataset &testing_set)
{
    // random_shuffle(dataset.matrix.begin(), dataset.matrix.end());
    int split_idx = dataset.matrix.size() * 0.8;
    training_set.matrix.insert(training_set.matrix.begin(), dataset.matrix.begin(), dataset.matrix.begin() + split_idx);
    testing_set.matrix.insert(testing_set.matrix.begin(), dataset.matrix.begin() + split_idx, dataset.matrix.end());
}

string getPluralityValue(const Dataset &dataset)
{
    unordered_map<string, int> label_counts;
    string majority_label;
    int max_count = 0;
    for (const auto &instance : dataset.matrix)
    {
        string label = instance.back();

        label_counts[label]++;
        if (label_counts[label] > max_count)
        {
            max_count = label_counts[label];
            majority_label = label;
        }
    }
    return majority_label;
}

double calculateEntropy(const Dataset &dataset)
{
    unordered_map<string, int> label_counts;
    for (const auto &instance : dataset.matrix)
    {
        label_counts[instance.back()]++;
    }

    double entropy = 0.0;
    for (const auto &label_count : label_counts)
    {
        double probability = 1.0 * label_count.second / dataset.matrix.size();
        entropy -= probability * log2(probability);
    }

    return entropy;
}

double calculateInformationGain(const Dataset &dataset, int attr_idx)
{
    int n_examples = dataset.matrix.size();
    double E_bs = calculateEntropy(dataset);

    unordered_map<string, Dataset> data_splits;
    for (int i = 0; i < n_examples; ++i)
    {
        string attr_value = dataset.matrix[i][attr_idx];
        data_splits[attr_value].matrix.push_back(dataset.matrix[i]);
    }

    double E_as = 0.0;

    for (const auto &i : data_splits)
    {
        double probability = 1.0 * i.second.matrix.size() / n_examples;
        E_as += probability * calculateEntropy(i.second);
    }

    return E_bs - E_as;
}

int getBestAttribute(const Dataset &dataset, const unordered_set<int> &attributes)
{
    double max_gain = -1.0;
    int best_attr = -1;
    for (int attr_idx : attributes)
    {
        double gain = calculateInformationGain(dataset, attr_idx);
        if (gain > max_gain)
        {
            max_gain = gain;
            best_attr = attr_idx;
        }
    }

    return best_attr;
}

bool isSameClassification(const Dataset &dataset)
{
    string unique = dataset.matrix[0].back();
    for (const auto &instance : dataset.matrix)
    {
        if (unique != instance.back())
        {
            return false;
        }
    }
    return true;
}

string pad()
{
    string p;
    for (int i = 0; i < depth; i++)
    {
        p += "--";
    }
    return p;
}
Node *buildDecisionTree(const Dataset &dataset, unordered_set<int> attributes)
{
    if (isSameClassification(dataset))
    {
        Node *leaf = new Node(-1, dataset.matrix[0].back());
        clog << pad() << leaf->label << " (Leaf: All same label)" << endl;
        return leaf;
    }
    else if (attributes.empty())
    {
        Node *leaf = new Node();
        leaf->label = getPluralityValue(dataset);
        clog << pad() << leaf->label << " (Leaf: No more attributes)" << endl;
        return leaf;
    }
    int best_attr = getBestAttribute(dataset, attributes);

    clog << pad() << all_attributes[best_attr].first << " (Test)" << endl;
    Node *decision_node = new Node();
    decision_node->attribute = best_attr;
    attributes.erase(best_attr);

    for (const auto &attr_value : all_attributes[best_attr].second)
    {
        Dataset subset = dataset.subset(best_attr, attr_value);
        clog << pad() << all_attributes[best_attr].first << "." << attr_value << " (Branch)" << endl;
        if (subset.matrix.empty())
        {
            Node *leaf = new Node();
            leaf->label = getPluralityValue(dataset);
            depth++;
            decision_node->children[attr_value] = leaf;
            clog << pad() << leaf->label << " (Leaf: No more examples)" << endl;
            depth--;
        }
        else
        {
            depth++;
            decision_node->children[attr_value] = buildDecisionTree(subset, attributes);
            depth--;
        }
    }

    return decision_node;
}
Node *train(const Dataset &dataset, const Dataset &training_set)
{
    unordered_set<int> attributes;
    for (int i = 0; i < all_attributes.size(); i++)
    {
        attributes.insert(i);
    }
    Node *decision_tree = buildDecisionTree(training_set, attributes);
    return decision_tree;
}
string predictLabel(Node *node, const vector<string> &instance)
{
    if (node->label != "")
    {
        return node->label;
    }
    string attr_value = instance[node->attribute];
    auto it = node->children.find(attr_value);
    return predictLabel(it->second, instance);
}
double classify(Node *decision_tree, Dataset testing_set)
{
    int correct_predictions = 0;
    int n_tests = testing_set.matrix.size();
    for (int i = 0; i < n_tests; i++)
    {
        string true_label = testing_set.matrix[i].back();
        string predictedLabel = predictLabel(decision_tree, testing_set.matrix[i]);

        if (predictedLabel == true_label)
        {
            correct_predictions++;
        }
    }
    double accuracy = 1.0 * correct_predictions / n_tests;
    return accuracy;
}

void destruct(Node *node)
{
    for (auto &i : node->children)
    {
        destruct(i.second);
    }
    delete node;
}
int main()
{
    srand(1905072);
    vector<double> accuracies;
    const int n_experiments = 1;
    Dataset dataset = loadDataset();
    for (int i = 0; i < n_experiments; ++i)
    {
        Dataset training_set, testing_set;
        splitDataset(dataset, training_set, testing_set);
        Node *decision_tree = train(dataset, training_set);
        double accuracy = classify(decision_tree, testing_set);
        destruct(decision_tree);
        accuracies.push_back(accuracy);
        cout << "Accuracy: " << accuracy * 100 << "%" << endl;
        // clog << accuracy * 100 << endl;
    }

    double avg_accuracy = accumulate(accuracies.begin(), accuracies.end(), 0.0) / n_experiments;
    double sum_squared_deviation = 0.0;
    for (double acc : accuracies)
    {
        sum_squared_deviation += pow(acc - avg_accuracy, 2);
    }
    double variance = sum_squared_deviation / (n_experiments);
    double std_deviation = sqrt(variance);

    cout << "Mean Accuracy: " << avg_accuracy * 100 << "%" << endl;
    cout << "Standard Deviation: " << std_deviation * 100 << "%" << endl;

    return EXIT_SUCCESS;
}
