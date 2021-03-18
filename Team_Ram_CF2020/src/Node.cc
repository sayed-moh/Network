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

#include "Node.h"
//#define MAX_SEQ 3   //window size -1
//#define SEND_POLL 1 //time between every frame is sent in seconds
//#define TIMER_DELAY 7 //timeout every n seconds

Define_Module(Node);

Stats Node::stats;

void Node::initialize()
{
    init_smsg = new SelfMessage_Base("");
    init_smsg->setType(SM_Init);
    scheduleAt(simTime(), init_smsg);
    EV << "Node init\n";

    send_smsg.setType(SM_Send);
}

void Node::endScheduledEvents()
{
    for(unsigned int i = 0; i < timer_msgs.size(); i++)
    {
        stopTimer(i);
    }

    if (send_smsg.isScheduled())
        cancelEvent(&send_smsg);
    if (init_smsg && init_smsg->isScheduled())
        cancelEvent(init_smsg);
}

void Node::finish()
{
    endScheduledEvents();
}

void Node::setPeer(int p)
{
    peer = p;
    if(p==-1)
        return;
    //schedule starting transmission.
    scheduleAt(simTime(), &send_smsg);

}

int Node::getPeer()
{
    return peer;
}

bool Node::loadFile()
{

    string filename = "D:/Uni/Senior-1/Networks/omnetpp-5.6.2-src-windows/omnetpp-5.6.2/samples/Mesh3/src/data/node" + to_string(getIndex()) + ".txt";
    return loader.loadFile(filename);
}

void Node::selfInit()
{
    EV << "Self init msg: ";
    bool success = loadFile();
    if(!success)
    {
        EV << "failed to load data file.";
        shouldWork = false;
    }
    else
    {
        EV << "loaded data file successfully.";
    }

    window.setMaxSeq(par("max_seq").intValue());
    window.setOwner(this);

    EV << " Sliding window size " << par("max_seq").intValue()+1 << ".";

    timer_msgs.resize(par("max_seq").intValue()+1);
    for(int i = 0; i < par("max_seq").intValue()+1; i++)
    {
        timer_msgs[i].setType(SM_Timer);
    }
}

void Node::handleMessage(cMessage *cmsg)
{
    if (cmsg->isSelfMessage())
    {
        SelfMessage_Base *msg = check_and_cast<SelfMessage_Base *>(cmsg);
        enum SM_Type type = (SM_Type)msg->getType();
        if(type == SM_Init)
        {
            EV << "SM_Init" << endl;
            selfInit();
            delete init_smsg;
            init_smsg = 0;
        }
        else if(type == SM_Timer)
        {
            EV << "SM_Timer" << endl;
            for(unsigned int i = 0 ; i < window.outStanding(); i++)
            {
                stats.addRetrans();
            }
            window.timeout();
        }
        else if(type == SM_Send)
        {
            EV << "SM_Send" << endl;
            //sent portion of data if possible,
            //otherwise, scheduleAt SM_Send self message after given time
            string str;
            if(!loader.peek(str))
            {
                //finished transmitting
                //TODO: tell other node transmission done.
                int id = getIndex();
                int gate_id = peer < id ? peer : peer-1;

                if(window.outStanding())
                {
                    scheduleAt(simTime()+simtime_t(par("send_poll").intValue(),SIMTIME_S), &send_smsg);
                }
                else
                {
                    RemoteMessage_Base* msg = new RemoteMessage_Base("");
                    msg->setType(RM_End);
                    send(msg, "outs", gate_id);
                    endTransmission();
                }
            }
            else
            {
                if(window.canSend())
                {
                    stats.addUseful();
                    EV << "Window can send." << endl;
                    window.send(str);
                    loader.pop();
                }
                else
                {
                    EV << "Window cannot send." << endl;
                }
                scheduleAt(simTime()+simtime_t(par("send_poll").intValue(),SIMTIME_S), &send_smsg);
            }
        }


    }
    else if(cmsg->arrivedOn("pairer"))
    {
        setPeer(stoi(cmsg->getName()));
        delete cmsg;
    }
    else
    {
        if(peer == -1)
        {
            delete cmsg;
        }
        else
        {
            RemoteMessage_Base *msg = check_and_cast<RemoteMessage_Base *>(cmsg);
            if(msg->getType() == RM_End)
            {
                endTransmission();
                delete msg;
            }
            else
            {
                Frame r(msg);

                string data = hammingReceive(msg);
                EV << data;
                string ack_str = data.substr(0, 8);
                unsigned int ack;
                try{
                    ack = stoi(ack_str, nullptr, 16);
                }
                catch (...)
                {

                    EV << "couldn't parse ack_str : " << msg->getDataArraySize() << " " << ack_str << endl << data << endl;
                    return;
                }
                data.erase(0, 8);
                EV << data;
                msg->setStrData(data.c_str());
                msg->setAck(ack);
                //received message
                window.receive(r);
            }
        }
    }
}

void Node::endTransmission()
{
    EV << "End transmission." << endl;
    window.reset();
    window.setMaxSeq(par("max_seq").intValue());
    setPeer(-1);
    endScheduledEvents();
    data_recv.clear();
    loadFile();
}

string Node::hammingReceive(RemoteMessage_Base *msg)
{

    vector<bool > sent_data;
    unsigned int sz = msg->getDataArraySize();
    sent_data.resize(sz);
    for(int i =0; i < sz; i++)
    {
        sent_data[i] = msg->getData(i);
    }

    EV<< "hamming recv ch_count: " << msg->getChar_count() << endl;
    vector<bool > received_data = HammingCode(sent_data, msg->getChar_count(), RECEIVER, sent_data);

    string received_message = VectorOfBoolToString(received_data);

    return received_message;
}

void Node::actualReceive(RemoteMessage_Base* msg)
{
    //TODO: implement
    string data = msg->getStrData();
    data_recv.push_back(data);
    EV << "Received data frame no. " << data_recv.size()-1 << " : ";
    EV << data << endl;
    delete msg;
}

vector<bool > Node::hammingSend(string message)
{
    vector<bool > sent_data = StringToVectorOfBool(message);
    vector<bool > received_payload;
    vector<bool > payload = HammingCode(sent_data, sent_data.size(), SENDER, received_payload);
	return payload;
}


void Node::actualSend(string data, unsigned int seq, unsigned int ack)
{
    //TODO: char count
    //TODO: hamming

    int id = getIndex();
    int gate_id = peer < id ? peer : peer-1;


    double loss_prob = par("loss_prob");
    double delay_prob = par("delay_prob");
    double dup_prob = par("dup_prob");
    double mod_prob = par("mod_prob");
    double delay_lambda = par("delay_lambda");
    double delay_interval = 0;

    bool dupl=true;


    double rand;
    //first: loss
    rand = uniform(0,1);
    if(rand < loss_prob)
    {
        stats.addDropped();
        return;
    }

    //second, delays:
    rand = uniform(0,1);
    if(rand < delay_prob)
    {
        dupl = false;
        delay_interval = exponential(1/delay_lambda);
    }




    //third: duplicates
    int num_copies = 1;
    if(dupl)
    {
        rand = uniform(0,1);
        if(rand < dup_prob)
        {
            num_copies++;
        }
    }

    //add ack to data, since ack can have error too
    stringstream ss;
    ss << setfill('0') << setw(8) << hex << ack;
    string ack_str = ss.str();
    data = ack_str + data;
    unsigned int ch_count = data.size();
    //hamming calc
	vector<bool> vb = hammingSend(data);
    string printdata = data;
    //printdata.erase(0,8);

    //fourth: modifications
    rand = uniform(0,1);
    if(rand < mod_prob)
    {
        constexpr int byte_size = 8;
        int size = data.size();
        int loc = intuniform(0, size*byte_size-1);
        int id = loc/byte_size;
        char ch = data[id];
        int localbitloc = loc%byte_size;

        EV << "Modifcation error: ch(" << loc << "," << localbitloc << ") before: " << bitset<byte_size>(ch) << "(" << ch << ") ";
        ch ^= (1<< (byte_size-1-localbitloc));
        EV << "ch after: " << bitset<byte_size>(ch)  << "(" << ch << ")\n";
        data[id] = ch;
        printdata = data;
        vb[loc] = !vb[loc];
    }

    for(int i = 0; i < num_copies; i++)
    {
        RemoteMessage_Base* msg = new RemoteMessage_Base("");

        
        msg->setDataArraySize(vb.size());
        for(int j =0; j < vb.size(); j++)
        {
            msg->setData(j, vb[j]);
        }
        msg->setSeq(seq);
        msg->setAck(ack);
        msg->setChar_count(ch_count*8);
        msg->setType(RM_DataAck);
        EV << "node[" << getIndex() << "] send frame: " << seq << endl;
        EV << "delay: " << delay_interval << " seconds " << endl;
        EV << "send hamming data size: " << msg->getDataArraySize()<<endl;
        EV << printdata << endl;
        sendDelayed(msg, simtime_t(delay_interval*1000, SIMTIME_MS), "outs", gate_id);
    }
}


void Node::startTimer(unsigned int id)
{
    stopTimer(id);
    SelfMessage_Base *msg = &timer_msgs[id];
    scheduleAt(simTime()+simtime_t(par("timer_delay").intValue(),SIMTIME_S), msg);
}

void Node::stopTimer(unsigned int id)
{
    SelfMessage_Base *msg = &timer_msgs[id];
    if (msg->isScheduled())
        cancelEvent(msg);
}

string Node::printStats()
{
    stringstream ss;
    ss << "Statistics: " << endl;
    ss << "Total frames: " << stats.getTotal() << endl;
    ss << "Useful: " << stats.getUseful() << endl;
    ss << "Dropped: " << stats.getDropped() << endl;
    ss << "Retransmitted: " << stats.getRetrans() << endl;
    ss << "Efficiency: " << stats.getEfficiency() << endl;
    return ss.str();
}
