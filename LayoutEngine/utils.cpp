#include "utils.hpp"

#include <fstream>
#include <iostream>

using namespace std;

string readFile(string path)
{
    ifstream iff(path.c_str(), ios::in);

    string grammarStr = "";
    if (iff.is_open())
    {
        while (iff)
        {
            string holder;
            getline(iff, holder);
            grammarStr += holder;
            grammarStr += "\n";
        }
        iff.close();
    }
    else
        cout << "Failed to open grammar file '" << path << "'\n";

    return grammarStr;
}
