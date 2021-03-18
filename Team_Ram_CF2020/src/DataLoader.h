
#ifndef DATALOADER_H_
#define DATALOADER_H_

#include <list>
#include <string>
using namespace std;
class DataLoader
{
    list<string> data;
public:
    DataLoader() { }

    bool loadFile(string& filename);

    bool peek(string& str);
    bool pop();
};


#endif /* DATALOADER_H_ */
