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

#ifndef _NETWORK_PROJECT_NODE0_H
#define _NETWORK_PROJECT_NODE0_H

#include <omnetpp.h>
#include <fstream>
#include <deque>
#include <vector>
#include "Messages_m.h"
using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Node0: public cSimpleModule {
    int First_time;
    std::string time;
    int Sender = 1;
    int WS=0;   // window size sender
    int WR=0;//Window reciever
    int currentSeq=1; // ptr at the begging of the slidding window
    int counterUntillWindowSize=0;
    cMessage *event;
    int Was_Message_sent = 1;
    std::vector<std::string> mystring;
    std::string message_to_send;
    std::string message_4bits;
    std::vector<std::string> Not_processed;
    std::vector<std::string> Bits;
    std::vector<int>VectACKS;
    std::vector<Messages_Base *> RecieVector;
    std::vector<Messages_Base *> senderVector;

    std::string message_to_send2;
    std::vector<Messages_Base *>Verror;
    Messages_Base *nmsg;
    Messages_Base *msgerror;
    Messages_Base *msgLOSS;
    int timer_value=0;
    std::vector<std::string> inversemystring; // to send the message with the correct order



    /////// FOR RECIVER
    std::string message_to_byte_stuff = "";


protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif
