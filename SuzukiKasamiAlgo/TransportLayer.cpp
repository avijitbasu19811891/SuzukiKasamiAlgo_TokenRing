//
//  TransportLayer.cpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 04/11/18.
//

#include "TransportLayer.hpp"
#include "SuzukiKasamiAlgo.hpp"

template <>
int TransportLayer::sendToNode(int destId,Msg msg, Topology *topologyPtr) {
    msg._destPid = destId;
    transmit(msg);
    return 0;
}

int TransportLayer::recvMsg(Msg msg) {
    Topology *topology = Topology::instance();

    if (topology == nullptr) {
        std::cout << "Could not recolve topology"<< std::endl;
        return -1;
    }
    topology->recvEvent(msg._destPid, msg);
    return 0;
}

int TransportLayer::transmit (Msg msg) {
    return receive(msg);
}

int TransportLayer::receive (Msg msg) {
    return recvMsg(msg);
}
