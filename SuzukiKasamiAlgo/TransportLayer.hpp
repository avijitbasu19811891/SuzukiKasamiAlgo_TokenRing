//
//  TransportLayer.hpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 04/11/18.
//

#ifndef TransportLayer_hpp
#define TransportLayer_hpp

#include <stdio.h>
#include <iostream>
#include <memory.h>

#include "InputDataSet.hpp"

class Msg;

class TransportLayer {
public:
    static TransportLayer& instance (void) {
        static TransportLayer transport;
        return transport;
    }
    
    ///
    /// Apis exposed to upper layer algorithm.
    ///
    
    ///
    /// @brief send a msg to a dest site.
    ///
    template <class T>
    int sendToNode(int destId,Msg msg, T *topologyPtr);
    
    ///
    /// @brief received a msg from lower layer
    ///
    int recvMsg(Msg msg);

    ///
    /// Apis to lower layer to trigger tx and rx
    ///
    int transmit (Msg msg);
    int receive (Msg msg);
    ///
    /// @brief wait for events.
    ///
    std::shared_ptr<EventData>
    waitForEvent (void) {
        DataSet *fp = DataSet::instance();
        
        if (fp != nullptr) {
            return (fp->getNextEvent());
        }
        return nullptr;
    }
};

class Msg {
public:
    enum Type {
        REQ =1,
    };
    int _srcPid;
    int _destPid;
    int _seqNo;
    Type _type;
    Msg (int pid, int seqNo) {
        _srcPid = pid;
        _seqNo = seqNo;
        _type = Msg::Type::REQ;
    }
};
#endif /* TransportLayer_hpp */
