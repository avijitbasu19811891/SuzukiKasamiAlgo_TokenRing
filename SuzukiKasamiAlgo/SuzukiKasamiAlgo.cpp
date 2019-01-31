//
//  SuzukiKasamiAlgo.cpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 04/11/18.
//

#include <stdio.h>
#include <string>
#include <iostream>
#include <memory>
#include <map>
#include "SuzukiKasamiAlgo.hpp"
#include "Node.hpp"
#include "GlobalConfig.hpp"
#include "TransportLayer.hpp"

Topology::Topology (void) {
    initialize();
}

void Topology::initialize()
{
    int i;
    token.init();
    for(i=0;i<GlobalConfig::NumNode;i++)
    {
        _nodeList[i].init(i);
    }
    ///
    /// Let initial token be with node Id 0.
    ///
    transferIdealToken(0);
}

void Topology::bcastTokenReq(Msg msg) {
    int i;
    std::cout << "///////  Initiating Token Req to peer Nodes ///////"
              << std::endl;
    std::cout << "Broadcasting Req from site : " << msg._srcPid << std::endl;
    std::cout << "///////                                     ///////"
              << std::endl;
    for(i=0;i<GlobalConfig::NumNode;i++)
    {
        ///
        /// Send to all nodes except self node.
        ///
        if(i!=msg._srcPid) {
            ///
            /// perfrom unicast transmit to all sites.
            ///
            TransportLayer transport = TransportLayer::instance();
            transport.sendToNode(i, msg, this);
        }
    }
}

///
/// @brief responsible for processing msg received at th node.
///        Lower transport layer will invoke this API.
///         when a msg is rceived with dest same as the node id.
///
int Topology::recvEvent(int siteId, Msg msg) {
    if (!isSiteIdValid(siteId)) {
        return -1;
    }
    std::cout << "Algo layer , site : " << siteId
    << " received msg from site : " << msg._srcPid << std::endl;
    _nodeList[siteId].processReq(msg, this);
    return 0;
}

///
/// @brief Upper layer sofware on node siteId, request for entry to enter CS>
///
void Topology::recvReqEvent (int siteId) {
    if (!isSiteIdValid(siteId)) {
        return ;
    }
    _nodeList[siteId].reqToEnterCS(this);
}

///
/// @brief Upper layer in this node(siteId), has completed CS.
///
void Topology::recvRelEvent (int siteId) {
    if (!isSiteIdValid(siteId)) {
        return ;
    }
    _nodeList[siteId].csExit(this);
}

bool
Topology::isCorrect (void) {
    
    int i;
    int inCSNodeCount = 0;
    for(i=0;i<GlobalConfig::NumNode;i++)
    {
        if (_nodeList[i].isExecutingCs()) {
            inCSNodeCount++;
        }
    }
    if (inCSNodeCount > 1) {
        return std::false_type::value;
    }
    return std::true_type::value;
}
void
Topology::description(std::ostream& os) {
    int i,j,k=0;
    queue <int> temp;
    std::cout << std::endl << std::endl;
    std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
    os << "Displaying LN Array STATE" << std::endl;
    os << "TOKEN HOLDER : " << tokenHolder << std::endl;
    os << "State of TOKEN Req Q: " << std::endl;
    if(token.token_q.empty())
    {
        os << "No Pending Request" << std::endl;
        j=0;
    }
    else
    {
        j=token.token_q.size();
    }
    while(k<j)
    {
        i=token.token_q.front();
        token.token_q.pop();
        token.token_q.push(i);
        printf("%d ",i);
        k++;
    }
    
    os << "Global LN array state:" << std::endl;
    for(i=0;i<GlobalConfig::NumNode;i++)
        os << token.ln[i];
    os << std::endl;

    os << "Per Node RN Array :" << std::endl;
    int csSiteId = -1;
    for(i=0;i<GlobalConfig::NumNode;i++)
    {
        printf(" S%d :",i);
        for(j=0;j<GlobalConfig::NumNode;j++)
            printf(" %d ",_nodeList[i].rn[j]);
        printf("\n");
        if (_nodeList[i].isExecutingCs()) {
            csSiteId = i;
        }
    }

    std::cout << std::endl;
    if (!isCorrect()) {
        os << "Algo is not correct "
        << "Multiple nodes in CS at same time" << std::endl;
        numOfInstancesImproper++;
    } else {
        os << "Algo working fine \n \t";
        std::string s1;
        if (csSiteId > 0) {
            os <<  "siteId : " << csSiteId << "in CS" << std::endl;
        } else {
            os << "No Site in CS" << std::endl;
        }
    }
    std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
    std::cout << std::endl << std::endl;
}
