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
int x = 0;
int dummy1 = 0;

double prevclck;
double currentClck;

#include "Node0.h"
std::bitset<8> checkbitsOfCurrentProcessing(0);
Define_Module(Node0);
int max_seq;
int expected_frame = 0;
int seq_num = 0;
int CurrentseqRec = -1;
bool FlagCurrentNAK = true;
bool isnacked2=false;
int ReciverableToProcess = 1;
int finshed=0;
bool isNACKED = false;

void Node0::initialize()
{
    First_time = 0;

    WS = getParentModule()->par("WS").intValue();
    WR = getParentModule()->par("WR").intValue();
    counterUntillWindowSize = WS;
    max_seq = (2 * WS) - 1;
    RecieVector.resize(max_seq);
    senderVector.resize(max_seq);
    for (int i = 0; i < max_seq; i++) {
        RecieVector[i] = nullptr;
        senderVector[i] = nullptr;
    }
}

void Node0::handleMessage(cMessage *msg) {

    double PT = getParentModule()->par("PT").doubleValue();
    int TD = getParentModule()->par("TD").intValue();
    double DD = getParentModule()->par("DD").doubleValue();
    int ED = getParentModule()->par("ED").intValue();
    double TO=getParentModule()->par("TO").intValue();

    if (timer_value == 0)
    {
        time = std::string(msg->getName());
    }
    std::string senderModuleName = msg->getSenderModule()->getFullName();

    if (senderModuleName == "coordinator" && Sender != 0) {
        Sender = 0;
    }


    if (Sender == 0)
    {
        EV << "sender" << endl;
        //////////////////////////////////

        if (strcmp(msg->getName(), "ack") == 0)
        {
            EV << "ack section sender" << endl;

            Messages_Base *dummy;
            dummy = check_and_cast<Messages_Base*>(msg);
            VectACKS.push_back(dummy->getSeq_Num());
            int i = 0;
            while (VectACKS.size())
            {
                EV << "CHECKING ACK" << endl;
                EV << "CurrentS" << currentSeq << endl;
                EV << "VectAcks" << VectACKS[i] << endl;
                if (currentSeq == VectACKS[i]) {
                    VectACKS.erase(VectACKS.begin() + i);
                    senderVector[i] = nullptr;
                    if (currentSeq == max_seq)
                        currentSeq = 1;
                    else
                        currentSeq++;

                    counterUntillWindowSize++;
                    EV<<"abokero"<<counterUntillWindowSize<<endl;
                }
                if (i + 1 < VectACKS.size()) {
                    i++;
                } else {
                    break;
                }
            }

            //scheduleAt(simTime() + time[1] - '0', new Messages_Base(" up"));   ana w ismail
        }
            else if(strcmp(msg->getName(), "Time out") == 0 )
            {
                EV<<"Time out"<<endl;
                Messages_Base *dummy;
                            dummy = check_and_cast<Messages_Base*>(msg);
                            EV<<"dummy->getSeq_Num()"<< senderVector[dummy->getSeq_Num()]->getM_Payload()<<endl;

                sendDelayed(senderVector[dummy->getSeq_Num()], TD, "out");

            }

         else if (strcmp(msg->getName(), "nack") == 0 ) {
            EV << "nack section sender" << endl;
            Messages_Base *dummy;
            dummy = check_and_cast<Messages_Base*>(msg);
            if (senderVector[dummy->getSeq_Num()] != nullptr)
            {

                EV << "retransmit"
                          << senderVector[dummy->getSeq_Num()]->getM_Payload()
                          << endl;
                dummy = senderVector[dummy->getSeq_Num()];
                senderVector[dummy->getSeq_Num()] = nullptr;

                scheduleAt(simTime(), new Messages_Base("HI"));

                sendDelayed(dummy, TD, "out");
            }

        }
   if(! finshed)
      {
       EV << "kero" << endl;

        ///////////////////////////////////////////////
        nmsg = new Messages_Base();
        msgerror = new Messages_Base();
        int e = 0;
        // set sequence number of every frame

        if (First_time == 0) {

            // Node reads the input file
            timer_value = 1;
            int i = 0;
            scheduleAt(simTime() + time[1] - '0', new Messages_Base("Wake up"));
            std::ifstream myfile;
            myfile.open("Input0.txt");
            std::string trial = "";
            if (myfile.is_open()) {
                char mychar;
                while (myfile) {
                    mychar = myfile.get();

                    if (mychar != '\n') {
                        if (mychar != ' ') {
                            trial += mychar;
                        }
                    } else {
                        mystring.push_back(trial);
                        trial = "";
                    }

                }
            }
            mystring.push_back(trial);

            First_time++;

        } else {

            while (!mystring.empty()) {

                inversemystring.push_back(mystring.back());
                mystring.pop_back();
            }
            // The frames are not vanished
            //START

            if (counterUntillWindowSize && Was_Message_sent == 1)
            {
                EV << "Buffering :" << counterUntillWindowSize << endl;
                counterUntillWindowSize--;
                if (Was_Message_sent == 1) {
                    // EV<<"entered was sent"<<endl;
                    message_to_send = "";
                    message_4bits = "";
                    if (inversemystring.size() != 0) {
                        std::string message = inversemystring.back();
                        inversemystring.pop_back();
                        for (int h = 0; h < message.size(); h++) {
                            if (h < 4) {
                                message_4bits += message[h];
                            } else if (h >= 4) {
                                message_to_send += message[h];

                            }
                        }

                    } else
                    {
                        message_to_send="int";
                        message_4bits = "4444";

                    }
                    e = 1;
                    Was_Message_sent = 0;

                }
            }
            //some parameters
            //EV<<"e"<<e<<endl;
            if (e)
            {
                EV << "sent end pT SIG" << endl;
                scheduleAt(simTime() + PT,
                        new Messages_Base("END processing time"));
                e = 0;
            }
            //Check for any errors
            if (strcmp(msg->getName(), "END processing time") == 0) {
                nmsg->setM_Payload(message_to_send.c_str());
                nmsg->setSeq_Num(seq_num);
                Verror.push_back(nmsg);
                //EV<<"HelloWorld"<<endl;
                Was_Message_sent = 1;
                e = 1;
                if (message_4bits[0] - '0' == 4)
                {
                    EV<<"sender finshed"<<endl;
                    finshed=1;
                    return;

                } else {
                    // EV<<"which error of the rest three"<<endl;
                    // checksum
                    //*
                    std::bitset<8> checkbits(0); //to store 8 bits of checksum
                    for (int i = 0; i < message_to_send.size(); i++) {

                        std::bitset<8> temp(message_to_send[i]); // convert string to bits

                        checkbits = checkbits ^ temp;
                    }

                    nmsg->setTrailer(checkbits);
                    msgerror->setTrailer(checkbits);

                    std::string mypayload = message_to_send;
                    message_to_send2 = mypayload;

                    if ((message_4bits[0] - '0') == 1
                            && (message_4bits[1] - '0') != 1) //Modification
                                    {

                        int randombyte = uniform(0, mypayload.size());

                        mypayload[randombyte] += 1;
                        message_to_send = mypayload;

                    }

                    //framing
                    //*
                    std::string message_to_byte_stuff = "";
                    std::string message_to_byte_stuffwithnoerror = "";
                    for (int i = 0; i < message_to_send.size(); i++)
                    {
                        if (i == 0) {
                            message_to_byte_stuff += '#';
                            message_to_byte_stuff += message_to_send[i];
                            message_to_byte_stuffwithnoerror += '#';
                            message_to_byte_stuffwithnoerror +=
                                    message_to_send2[i];

                        } else if (i == message_to_send.size() - 1) {
                            message_to_byte_stuff += message_to_send[i];
                            message_to_byte_stuff += '#';
                            message_to_byte_stuffwithnoerror +=
                                    message_to_send2[i];
                            message_to_byte_stuffwithnoerror += '#';
                        } else if (message_to_send[i] == '/') {
                            message_to_byte_stuff += '/';
                            message_to_byte_stuff += message_to_send[i];
                            message_to_byte_stuffwithnoerror += '/';
                            message_to_byte_stuffwithnoerror +=
                                    message_to_send2[i];
                        } else if (message_to_send[i] == '#') {
                            message_to_byte_stuff += '/';
                            message_to_byte_stuff += message_to_send[i];
                            message_to_byte_stuffwithnoerror += '/';
                            message_to_byte_stuffwithnoerror +=
                                    message_to_send2[i];
                        } else {
                            message_to_byte_stuff += message_to_send[i];
                            message_to_byte_stuffwithnoerror +=
                                    message_to_send2[i];

                        }
                    }
                    EV << "get payload2" << message_to_byte_stuffwithnoerror
                              << endl;
                    msgerror->setM_Payload(
                                                    message_to_byte_stuffwithnoerror.c_str());
                    if ((message_4bits[1] - '0') != 1) // no error
                            {

                        nmsg->setM_Payload(message_to_byte_stuff.c_str());

                        senderVector[seq_num] = msgerror;
                        if (seq_num == max_seq ) // check circularity of seq_num
                                {
                            seq_num = 0;
                        }
                        nmsg->setSeq_Num(seq_num); // set sequne num
                        msgerror->setSeq_Num(seq_num);
                        Verror.push_back(msgerror);
                        if (counterUntillWindowSize + 1 == WS) {
                           // currentSeq = seq_num + 1;
                        }
                        //We should put if condtion to determine the end of the seq num  TODO
                        seq_num++; ///incrememt seq_num

                        if ((message_4bits[3] - '0') == 1
                                && (message_4bits[2] - '0') == 0
                                && (message_4bits[1] - '0') != 1) // Error delay
                                        {

                            scheduleAt(simTime() + 0.00,
                                    new Messages_Base("HI"));
                            if(ED+TD<TO)
                            {
                            sendDelayed(nmsg, ED + TD, "out");
                            }
                            else
                            {
                                    msgLOSS = new Messages_Base("Time out");
                                    msgLOSS->setSeq_Num(msgerror->getSeq_Num());
                                    scheduleAt(simTime() + TO, msgLOSS);
                                    scheduleAt(simTime(),
                                            new Messages_Base("HI"));
                            }
                        }

                        if ((message_4bits[2] - '0') == 1
                                && (message_4bits[1] - '0') != 1) // Duplicates
                                        {

                            if ((message_4bits[3] - '0') == 1) // Error delay
                                    {
                                scheduleAt(simTime() + 0.00,
                                        new Messages_Base("HI"));
                                sendDelayed(nmsg, ED + TD, "out");
                                scheduleAt(simTime() + 0.00,
                                        new Messages_Base("HI"));
                                if(ED+TD+DD<TO)
                                {
                                sendDelayed(nmsg->dup(), ED + DD + TD, "out");
                                }
                                else
                                {
                                        msgLOSS = new Messages_Base("Time out");
                                        msgLOSS->setSeq_Num(
                                                msgerror->getSeq_Num());
                                        scheduleAt(simTime() + TO, msgLOSS);
                                        scheduleAt(simTime(),
                                                new Messages_Base("HI"));
                                }

                            } else {
                                EV<<nmsg->getM_Payload()<<endl;
                                scheduleAt(simTime() + 0.00,
                                        new Messages_Base("HI"));
                                sendDelayed(nmsg, TD, "out");
                                scheduleAt(simTime() + 0.00,
                                        new Messages_Base("HI"));
                                EV<<nmsg->getM_Payload()<<endl;
                                sendDelayed(nmsg->dup(), DD + TD, "out");

                            }

                        }

                    } else {
                        EV << "LOSS ERROR" << endl;
                        msgLOSS=new Messages_Base("Time out");
                        senderVector[seq_num] = msgerror;
                        EV<<senderVector[seq_num]->getM_Payload()<<endl;
                        msgLOSS->setSeq_Num(msgerror->getSeq_Num());
                        seq_num++;
                        scheduleAt(simTime()+TO,msgLOSS);
                        scheduleAt(simTime(), new Messages_Base("HI"));
                    }
                    if ((message_4bits[1] - '0') == 0
                            && (message_4bits[3] - '0') == 0
                            && (message_4bits[2] - '0') == 0) // No Errors
                                    {
                        EV << "No error in sending" << endl;
                        EV << "get payload" << nmsg->getM_Payload() << endl;
                        scheduleAt(simTime() + 0.00, new Messages_Base("HI"));
                        // EV<<"no errros of the three"<<endl;
                        sendDelayed(nmsg, TD, "out");
                    }
                    //end

                }
            }

        }
    }
    } else if (Sender == 1)
    {
        EV << "RECIVER" << endl;
        nmsg = check_and_cast<Messages_Base*>(msg);
         EV<<"nmsg->getSeq_Num()"<<nmsg->getSeq_Num()<<endl;



        EV << "expected_frame" << expected_frame << endl;

        if(strcmp(nmsg->getM_Payload(), "CorrectFrame") != 0 &&  strcmp(nmsg->getM_Payload(), "InCorrectFrame") != 0)
        {
            //scheduleAt(simTime()+DD, new Messages_Base("checkIfDuplicatesRecived"));
            EV<<"getSeq_Num"<<nmsg->getSeq_Num()<<endl;

            if(  RecieVector[nmsg->getSeq_Num()] != nmsg   &&  strcmp(nmsg->getM_Payload(), "There is a message we need to process") != 0&&  strcmp(nmsg->getM_Payload(), "AT FIRST place a message we need to process") != 0)
                  {
                if(RecieVector[nmsg->getSeq_Num()] !=nullptr)
                {
                    if(strcmp( RecieVector[nmsg->getSeq_Num()]->getM_Payload(), nmsg->getM_Payload() ) ==0 )
                    {
                        return;
                    }
                    else
                    {
                            EV<<"RecieVector[nmsg->getSeq_Num()]->getM_Payload()"<<RecieVector[nmsg->getSeq_Num()]->getM_Payload()<<endl;
                            EV<<"nmsg->getM_Payload()"<<nmsg->getM_Payload()<<endl;
                            EV<<"want to settingg"<<nmsg->getM_Payload()<<"::1" <<endl;
                                      RecieVector[nmsg->getSeq_Num()] = nmsg; // storing reciver message in reciver buffer
                                      EV<<"nmsg" <<nmsg->getM_Payload()<<endl;
                                      EV<< "RecieVector[nmsg->getSeq_Num()]"<<RecieVector[nmsg->getSeq_Num()]->getM_Payload()<<endl;
                    }
                }
                else
                {
                    if(nmsg->getM_Payload() !="")
                    {
                EV<<"want to set"<<nmsg->getM_Payload()<<"::2" <<endl;
                RecieVector[nmsg->getSeq_Num()] = nmsg; // storing reciver message in reciver buffer
                EV<<"nmsg" <<nmsg->getM_Payload()<<endl;
                EV<< "RecieVector[nmsg->getSeq_Num()]"<<RecieVector[nmsg->getSeq_Num()]->getM_Payload()<<endl;
                    }
                }
                  }
        }
        int recentSeqRecived = nmsg->getSeq_Num();

        if (strcmp(nmsg->getM_Payload(), "CorrectFrame") == 0)
        {
            EV << "entered To ack" << endl;
            isNACKED = false;
            Messages_Base *ack = new Messages_Base("ack");
            ack->setM_Type(0);
            ack->setSeq_Num(nmsg->getSeq_Num() + 1);
            EV<<"next frame ack"<<ack->getSeq_Num()<<endl;
            EV << "received message with sequence number ...   ";
            EV << RecieVector[nmsg->getSeq_Num()]->getSeq_Num();
            EV << "  and payload of ... ";
            EV << RecieVector[nmsg->getSeq_Num()]->getM_Payload()<<endl;
            EV << "   and check bits of ...";
            EV << RecieVector[nmsg->getSeq_Num()]->getTrailer().to_string() << endl;
            EV << "ack" << " sequence num = " << ack->getSeq_Num() << endl;


            RecieVector[nmsg->getSeq_Num()] = nullptr;


            EV<<"nmsg->getSeq_Num()ess"<<nmsg->getSeq_Num() + 1<<endl;
            if (nmsg->getSeq_Num() + 1 > 0
                    && nmsg->getSeq_Num() + 1 < max_seq)
            {
                EV<<"entered"<<endl;
                if (RecieVector[nmsg->getSeq_Num() + 1] != nullptr)
                {
                    EV<<"HI after"<<endl;
                    Messages_Base *gg = new Messages_Base("There is a message we need to process");
                    gg->setM_Payload("There is a message we need to process");
                    scheduleAt(simTime(), gg);
                }
            } else {
                if (RecieVector[0] != nullptr) {
                    EV<<"HI begin"<<endl;
                    Messages_Base *gg = new Messages_Base("AT FIRST place a message we need to process");
                                      gg->setM_Payload("AT FIRST place a message we need to process");
                                      scheduleAt(simTime(), gg);
                }
            }

            EV << "SENDINGACK" << endl;
            nmsg->setM_Payload("helloWorld");
            //if(simTime()==19)
            std::cout<<simTime()<<endl;
            std::cout<<"HELLOWESSAM"<<endl;
            sendDelayed(ack, TD, "out");
        }
        else if (strcmp(nmsg->getM_Payload(), "InCorrectFrame") == 0) {
            EV << "entered To Nack" << endl;
            RecieVector[nmsg->getSeq_Num()] = nullptr;
            isnacked2=false;
            Messages_Base *nack = new Messages_Base("nack");
            nack->setM_Type(1);
            EV<<"nmsg->getSeq_Num() at nack"<<nmsg->getSeq_Num()<<endl;
            nack->setSeq_Num(nmsg->getSeq_Num());
            nmsg->setM_Payload("byeWorld");

            sendDelayed(nack, TD, "out");
        }
        else
        {

            int checkToGetFromStockOrNot=0;
            if (expected_frame-1>=0 )
            {
                checkToGetFromStockOrNot=expected_frame-1;
            }
            else
            {
                checkToGetFromStockOrNot=max_seq-1;
            }
            if(RecieVector[checkToGetFromStockOrNot]==nullptr)
            std::cout<<"checkToGetFromStockOrNot"<<checkToGetFromStockOrNot<<endl;

            if (RecieVector[expected_frame] != nullptr && recentSeqRecived == expected_frame
                    || ( RecieVector[expected_frame] != nullptr &&RecieVector[checkToGetFromStockOrNot]==nullptr ) ) // either recent recived or was exisit before
                            {
                EV<<"recentSeqRecived"<<recentSeqRecived<<endl;
                std::cout<<"BYEESSANWESSAM"<<endl;
                std::cout<<"recentSeqRecived"<< recentSeqRecived<<endl;
                std::cout<<"expected_frame"<<expected_frame<<endl;

                if (expected_frame == recentSeqRecived) {
                    EV << "SENT in its order" << endl;
                    message_to_send =
                            RecieVector[expected_frame]->getM_Payload();
                    checkbitsOfCurrentProcessing =
                            RecieVector[expected_frame]->getTrailer();
                } else if (RecieVector[expected_frame] != nullptr) {
                    EV << "got from the stored messages in reciver buffer"
                              << endl;
                    message_to_send =
                            RecieVector[expected_frame]->getM_Payload();
                    checkbitsOfCurrentProcessing =
                            RecieVector[expected_frame]->getTrailer();
                }

                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                //deframing

                for (int i = 0; i < message_to_send.size(); i++) {
                    if (i == 0) {

                    } else if (i == message_to_send.size() - 1) {

                    } else if (message_to_send[i] == '/'
                            && message_to_send[i - 1] == '/') {

                    } else if (message_to_send[i] == '#'
                            && message_to_send[i - 1] == '/') {
                        message_to_byte_stuff += '#';
                    } else
                        message_to_byte_stuff += message_to_send[i];
                }

                std::string Rmessage = message_to_byte_stuff;
                std::bitset<8> checkbits(0);
                EV<<"message_to_byte_stuff"<<message_to_byte_stuff <<endl;

                ///////////////////////////////////////////////////////////////////////////////////////////////

                //checking  checksum
                for (int i = 0; i < Rmessage.size(); i++) {
                    std::bitset<8> temp(Rmessage[i]);
                    checkbits = checkbits ^ temp;
                }

                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                EV<<"checkbits"<<checkbits<<endl;
                EV<<"checkbitsOfCurrentProcessing"<<checkbitsOfCurrentProcessing<<endl;
                // we need to check if the frame recived is correct or not
                if (checkbits == checkbitsOfCurrentProcessing) {
                    EV << "ack" << endl;


                    Messages_Base *gg = new Messages_Base("ack");
                    gg->setM_Payload("CorrectFrame");
                    gg->setSeq_Num(expected_frame );

                    RecieVector[expected_frame]->setM_Payload(message_to_byte_stuff.c_str());
                    message_to_byte_stuff="";


                    if (expected_frame + 1 == max_seq ) // check circularity of seq_num
                            {
                        expected_frame = 0;
                    } else
                        expected_frame++;



                    scheduleAt(simTime() + PT,
                            gg);

                }

                else if(! isnacked2 &&strcmp(RecieVector[expected_frame]->getM_Payload(), "") !=0  ) {
                    EV << "nack  ModifedMessage" << endl;
                    isnacked2=true;
                    Messages_Base *gg = new Messages_Base("nack");
                    gg->setSeq_Num(expected_frame );
                    //RecieVector[expected_frame]=nullptr;
                    gg->setM_Payload("InCorrectFrame");
                    message_to_byte_stuff="";
                    EV<<"at nack expected_frame"<<expected_frame<<endl;
                    scheduleAt(simTime() + PT,
                            gg);

                }

            }

            else if (recentSeqRecived != expected_frame
                    && recentSeqRecived != -1)  // out of order
                            {
                EV << "Out of order for " << recentSeqRecived << endl;
                std::cout<<"BYEESSANWESSAM555555555"<<endl;

                if (!RecieVector[expected_frame] && !isNACKED) {
                    EV << "sending nack that out of order " << endl;
                    Messages_Base *nackk = new Messages_Base("nackk");
                    nackk->setM_Type(1);
                    nackk->setSeq_Num(expected_frame);
                    recentSeqRecived = -1;
                    isNACKED = true;
                   // scheduleAt(simTime() ,new Messages_Base("hi"));
                    sendDelayed(nackk, TD + PT, "out");
                }

            }

        }

    }

}
