#include "DataLoader.h"
#include <fstream>

bool DataLoader::loadFile(string& filename)
{
    ifstream file(filename);
    string str;
    data.clear();
    if (!file)
    {
        return false;
    }
    while (getline(file, str))
    {
        data.push_back(str);
    }
    file.close();
    return true;
}

bool DataLoader::peek(string &str)
{
    if (data.size() == 0)
    {
        return false;
    }
    str = data.front();
    return true;
}

bool DataLoader::pop()
{
    if (data.size())
    {
        data.pop_front();
        return true;
    }
    return false;
}
