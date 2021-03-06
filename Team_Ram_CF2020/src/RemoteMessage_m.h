//
// Generated file, do not edit! Created by nedtool 5.6 from RemoteMessage.msg.
//

#ifndef __REMOTEMESSAGE_M_H
#define __REMOTEMESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
    #include <vector>
	typedef std::vector<bool> BoolVector;
	enum RM_Type {
		RM_DataAck,
		RM_Ack,
		RM_End
	};
// }}

/**
 * Class generated from <tt>RemoteMessage.msg:28</tt> by nedtool.
 * <pre>
 * packet RemoteMessage
 * {
 *     \@customize(true);  // see the generated C++ header for more info
 *     unsigned int type;
 *     unsigned int seq;
 *     unsigned int ack;
 *     unsigned int char_count;
 *     bool data[];
 *     string strData;
 * }
 * </pre>
 *
 * RemoteMessage_Base is only useful if it gets subclassed, and RemoteMessage is derived from it.
 * The minimum code to be written for RemoteMessage is the following:
 *
 * <pre>
 * class RemoteMessage : public RemoteMessage_Base
 * {
 *   private:
 *     void copy(const RemoteMessage& other) { ... }

 *   public:
 *     RemoteMessage(const char *name=nullptr, short kind=0) : RemoteMessage_Base(name,kind) {}
 *     RemoteMessage(const RemoteMessage& other) : RemoteMessage_Base(other) {copy(other);}
 *     RemoteMessage& operator=(const RemoteMessage& other) {if (this==&other) return *this; RemoteMessage_Base::operator=(other); copy(other); return *this;}
 *     virtual RemoteMessage *dup() const override {return new RemoteMessage(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from RemoteMessage_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(RemoteMessage)
 * </pre>
 */
class RemoteMessage_Base : public ::omnetpp::cPacket
{
  protected:
    unsigned int type;
    unsigned int seq;
    unsigned int ack;
    unsigned int char_count;
    bool *data; // array ptr
    unsigned int data_arraysize;
    ::omnetpp::opp_string strData;

  private:
    void copy(const RemoteMessage_Base& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RemoteMessage_Base&);
    // make constructors protected to avoid instantiation
    RemoteMessage_Base(const RemoteMessage_Base& other);
    // make assignment operator protected to force the user override it
    RemoteMessage_Base& operator=(const RemoteMessage_Base& other);

  public:
    virtual ~RemoteMessage_Base();
    RemoteMessage_Base(const char *name=nullptr, short kind=0);
    virtual RemoteMessage_Base *dup() const override {return new RemoteMessage_Base(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getType() const;
    virtual void setType(unsigned int type);
    virtual unsigned int getSeq() const;
    virtual void setSeq(unsigned int seq);
    virtual unsigned int getAck() const;
    virtual void setAck(unsigned int ack);
    virtual unsigned int getChar_count() const;
    virtual void setChar_count(unsigned int char_count);
    virtual void setDataArraySize(unsigned int size);
    virtual unsigned int getDataArraySize() const;
    virtual bool getData(unsigned int k) const;
    virtual void setData(unsigned int k, bool data);
    virtual const char * getStrData() const;
    virtual void setStrData(const char * strData);
};


#endif // ifndef __REMOTEMESSAGE_M_H

