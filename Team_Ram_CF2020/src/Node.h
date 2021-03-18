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

#ifndef __MESH3_NODE_H_
#define __MESH3_NODE_H_

#include <omnetpp.h>
#include <bitset>
#include "DataLoader.h"
#include "SlidingWindow.h"
#include "RemoteMessage_m.h"
#include "SelfMessage_m.h"
#include "hamming.h"
#include <sstream>
#include <iomanip>
#include "Stats.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Node : public cSimpleModule
{
    int peer=-1;
    DataLoader loader;
    bool shouldWork = true;
    SlidingWindow window;
    list<string> data_recv;
    vector<SelfMessage_Base> timer_msgs;
    SelfMessage_Base *init_smsg;
    SelfMessage_Base send_smsg;
    static Stats stats;
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    void endScheduledEvents();
    void endTransmission();
    string hammingReceive(RemoteMessage_Base *msg);
    vector<bool> hammingSend(string data);
public:
    bool loadFile();
    void selfInit();
    void setPeer(int p);
    int getPeer();
    void actualReceive(RemoteMessage_Base *msg);
    void actualSend(string data, unsigned int seq, unsigned int ack);
    void startTimer(unsigned int id);
    void stopTimer(unsigned int id);

    static string printStats();
};

#endif
