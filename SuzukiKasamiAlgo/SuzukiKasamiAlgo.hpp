//
//  SuzukiKasamiAlgo.hpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 04/11/18.
//

#ifndef SuzukiKasamiAlgo_hpp
#define SuzukiKasamiAlgo_hpp

#include <stdio.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <array>
#include "SuzukiKasamiAlgo.hpp"
#include "Node.hpp"
#include "GlobalConfig.hpp"
#include "TransportLayer.hpp"

///
/// This is the full set of nodes, that are running this algo.
/// classes of this file provides functionality to application running at each site
/// to enter and exit CS.
///
///
/// @class Toplogy of a set of nodes.
///
class Topology {
    Topology (void);
    void initialize (void);
    ///
    /// Global Token used by this toplogy of nodes.
    ///
    Token token;
    ///
    /// List of nodes in this toplogy.
    ///
    std::array<Node, GlobalConfig::NumNode> _nodeList;
    int tokenHolder = GlobalConfig::InitialTokeHolder;
    bool isSiteIdValid (int siteId) {
        if ((siteId < 0) ||
            (siteId > GlobalConfig::NumNode)){
            return std::false_type::value;
        }
        return std::true_type::value;
    }
public:
    Token* getLNToken(void) {
        return &token;
    }
    ///
    /// @brief Get singleton isntance of this topology.
    ////
    static Topology* instance () {
        static Topology topology;
        return &topology;
    };
    ///
    /// @brief Assign token to this Site.
    ///
    void assignToken (int pid) {
        tokenHolder = pid;
    }
    ///
    /// @brief retirve Id of node that holds token now.
    ///
    int TokenHolder (void) { return tokenHolder;};
    bool isNodeTokenHolder (int siteId) {
        return( TokenHolder() == siteId);
    }
    ///
    /// @brief broadcast a token msg to all nodes in the toplogy.
    ///        except self
    ///
    void bcastTokenReq(Msg msg);

    ///
    /// @brief transfer ownership of token to a site.
    ///
    void transferToken (int srcId, int destsiteId) {
        if (! isNodeTokenHolder(srcId)) {
            std::cout << "Improper Token transfer msg"
                      << "Site : " << srcId << " is not token holder"
                      << std::endl ;
            return;
        }
        _nodeList[destsiteId].hastoken=1;
        _nodeList[destsiteId].exec=1;
        _nodeList[destsiteId].isreq=0;
        assignToken(destsiteId);
    }
    ///
    /// @brief iniitliaze ideal token site.
    ///
    void transferIdealToken (int destsiteId) {
        _nodeList[destsiteId].hastoken=1;
        _nodeList[destsiteId].exec=0;
        _nodeList[destsiteId].isreq=0;
        assignToken(destsiteId);
    }
    ///
    /// Transport layer Interface
    ///
    int recvEvent(int sitId, Msg msg);
    
    ///
    /// Interface to individual nodes.
    ///

    ///
    /// @brief user/application on a node wants to enter CS.
    ///
    void recvReqEvent (int siteId);
    ///
    /// @brief user/application on a node wants to exit CS.
    ///
    void recvRelEvent (int siteId);
    void description (std::ostream& os);
    
    bool isCorrect (void);
    
    ///
    /// Stats
    ///
    int numOfInstancesImproper = 0;
    
    bool isAlgoNeverFailed (void) {
        return (numOfInstancesImproper == 0);
    }
    
    void printSummary (std::ostream& os) {
        int i;
        os << "Final token with site : " << TokenHolder() << std::endl;
        auto isTokenProper = _nodeList[tokenHolder].holdsToken();
        os << "Token state proper: " << isTokenProper <<std::endl;
        os << "Per Site Stats:" << std::endl;
        for(i=0;i<GlobalConfig::NumNode;i++) {
            _nodeList[i].description(os);
        }
        if (isAlgoNeverFailed()) {
            std::cout << "Algo never failed" << std::endl;
        } else {
            std::cout << "Algo failed in some instanced"
            << " multiple sites entered CS at same time" << std::endl;
        }
    }
};

#endif /* SuzukiKasamiAlgo_hpp */
