/*
 * SlidingWindow.cc
 *
 *  Created on: Jan 8, 2021
 *      Author: mohab
 */
#include "Node.h"
using namespace omnetpp;

void SlidingWindow::reset()
{
    max_seq = 0;
    next_frame_to_send = 0;
    ack_expected = 0;
    frame_expected = 0;
    nbuffered = 0;
}

void SlidingWindow::realSend(unsigned int frame_nr, unsigned int frame_expected) const
{
    //handle real send
    unsigned int ack = frame_expected;
    dec(ack, max_seq);
    node->actualSend(buffer[frame_nr], frame_nr, ack);
    //start or restart time id frame_nr by using scheduleAt() function
    node->startTimer(frame_nr);
}

unsigned int SlidingWindow::outStanding()
{
    return nbuffered;
}

bool SlidingWindow::between(unsigned int a, unsigned int b, unsigned int c)
{
    return ((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a));
}

SlidingWindow::SlidingWindow()
{
    reset();
}

void SlidingWindow::setOwner(Node* owner)
{
    node = owner;
}

void SlidingWindow::setMaxSeq(unsigned int max_sequence_number)
{
    max_seq = max_sequence_number;
    buffer.resize(max_seq + 1);
}

bool SlidingWindow::canSend() const
{
    return nbuffered < max_seq;
}

void SlidingWindow::send(string& data)
{
    //TODO: send ack only?
    //if we're at our limit, don't send
    if (!canSend())
    {
        throw "Sender buffer full, but send invoked, shouldn't happen!.";
        return;
    }
    buffer[next_frame_to_send] = data;
    nbuffered++;
    EV << "window[" << node->getIndex() << "] send frame: " << next_frame_to_send << endl;
    realSend(next_frame_to_send, frame_expected);
    inc(next_frame_to_send, max_seq);
}

void SlidingWindow::receive(Frame& r)
{
    //receieve ack of next frame (send 0 receieve ack 1)
    unsigned int seq = r.getSeq();
    unsigned int ack = r.getAck();//TODO: will be different if ack inside payload and accepts noise

    if (seq == frame_expected)
    {
        EV << "window[" << node->getIndex() << "] recv frame expected: " << seq << "\n";
        node->actualReceive(r.getMsg());
        inc(frame_expected, max_seq);
    }
    else
    {
        EV << "window[" << node->getIndex() << "] recv frame NOT expected ";
        EV << "frame: " << seq << " expected: " << frame_expected <<"\n";
        delete r.getMsg();
    }

    while (between(ack_expected, ack, next_frame_to_send))
    {
        nbuffered--;
        //stop timer id ack_expected by using cancelEvent() function
        node->stopTimer(ack_expected);
        inc(ack_expected, max_seq);
    }
    EV << "ack expected: " << ack_expected << endl;
}

void SlidingWindow::timeout()
{
    next_frame_to_send = ack_expected;
    for (size_t i = 0; i < nbuffered; i++)
    {
        realSend(next_frame_to_send, frame_expected);
        inc(next_frame_to_send, max_seq);
    }
}
