//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Pairer.h"
#include "Node.h"
#include <list>
#include <iterator>
using namespace std;

Define_Module(Pairer);

void Pairer::initialize()
{
    // TODO - Generated method body
    EV << "Pairer init\n";
    cMessage *msg = new cMessage("");
    scheduleAt(simTime()+simtime_t(0,SIMTIME_S), msg);

}

void Pairer::handleMessage(cMessage *msg)
{
    int rand = intuniform(0, 9);
    EV << "Pairer execute";
    if(rand < 5)
    {
        list<int> free_nodes;
        // TODO - Generated method body
        cModule *parent = getParentModule();
        cModule::SubmoduleIterator it(parent);
        it++;//ignore self
        for (; !it.end(); it++)
        {
          cModule *submodule = *it;
          Node * node = check_and_cast<Node*>(submodule);
          if(node->getPeer() == -1)
          {
              int index = submodule->getIndex();
              free_nodes.emplace_back(index);
          }
        }
        if(free_nodes.size() >= 2)
        {
            int i1 = intuniform(0, free_nodes.size()-1);
            list<int>::iterator it1 = free_nodes.begin();
            advance(it1, i1);
            int n1 = *(it1);
            free_nodes.erase(it1);
            int i2 = intuniform(0, free_nodes.size()-1);
            list<int>::iterator it2 = free_nodes.begin();
            advance(it2, i2);
            int n2 = *(it2);
            free_nodes.erase(it2);
            Node * node1 = check_and_cast<Node*>(parent->getSubmodule("nodes", n1));
            Node * node2 = check_and_cast<Node*>(parent->getSubmodule("nodes", n2));
            cMessage *m2 = new cMessage(to_string(n2).c_str());
            cMessage *m1 = new cMessage(to_string(n1).c_str());

            sendDirect(m2, node1, "pairer");
            sendDirect(m1, node2, "pairer");
            EV << " paired " << n1 << " and " << n2 << ".\n";
        }
        else
        {
            EV << " none.\n";
        }

    }
    else
    {
        EV << " rand false.\n";
    }

    //EV << "Simtime > 180 : " << (simTime().inUnit(SIMTIME_S) > 180) << endl;
    if(!stats_done && simTime().inUnit(SIMTIME_S) >= 180)
    {
        EV << Node::printStats();
        stats_done = true;
    }
    scheduleAt(simTime()+simtime_t(30,SIMTIME_S), msg);

}
