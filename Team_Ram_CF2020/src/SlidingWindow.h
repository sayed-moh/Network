/*
 * SlidingWindow.h
 *
 *  Created on: Jan 8, 2021
 *      Author: mohab
 */

#ifndef SLIDINGWINDOW_H_
#define SLIDINGWINDOW_H_

#include <string>
#include <vector>
#include "RemoteMessage_m.h"

#define inc(k, max_seq) if(k < max_seq) k++; else k=0;
#define dec(k, max_seq) if(k > 0) k--; else k=max_seq;

using namespace omnetpp;
using namespace std;

class Node;

class Frame
{
    RemoteMessage_Base *rmsg;
public:
    Frame(RemoteMessage_Base *msg)
    {
        rmsg = check_and_cast<RemoteMessage_Base *>(msg);
    }

    unsigned int getAck()
    {
        return rmsg->getAck();
    }
    unsigned int getSeq()
    {
        return rmsg->getSeq();
    }

    RemoteMessage_Base *getMsg()
    {
        return rmsg;
    }
};

class SlidingWindow
{
    unsigned int max_seq;//max squence number = 2^n-1
    unsigned int next_frame_to_send;
    unsigned int ack_expected;
    unsigned int frame_expected;
    vector<string> buffer;
    unsigned int nbuffered;

    Node *node;
    void realSend(unsigned int frame_nr, unsigned int frame_expected) const;
    bool between(unsigned int a, unsigned int b, unsigned int c);
public:
    SlidingWindow();

    unsigned int outStanding();
    void reset();
    void setOwner(Node* owner);
    void setMaxSeq(unsigned int max_sequence_number);

    bool canSend() const;
    void send(string& data);
    void receive(Frame& r);

    void timeout();
};


#endif /* SLIDINGWINDOW_H_ */
