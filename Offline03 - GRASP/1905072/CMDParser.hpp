#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;
class CMDParser
{
    map<string, string> args;

public:
    vector<string> tokenize(string arg, char delm)
    {
        vector<string> tokens;
        istringstream iss(arg);
        string token;

        while (getline(iss, token, delm))
        {
            tokens.push_back(token);
        }
        return tokens;
    }
    void add(string arg, string value)
    {
        args[arg] = value;
    }
    map<string, string> parse(int argc, char *argv[])
    {
        for (int i = 1; i < argc; i++)
        {
            string arg(argv[i]);
            if (arg.find("--") == 0)
            {
                vector<string> tokens = tokenize(arg.substr(2), '=');
                args[tokens[0]] = tokens.size() > 1 ? tokens[1] : "";
            }
            else
            {
                // Ignore
            }
        }
        return args;
    }
};