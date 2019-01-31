//
//  Node.cpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 04/11/18.
//

#include "Node.hpp"
#include "SuzukiKasamiAlgo.hpp"

///
/// @brief Process Request msg received from a peer node.
///
template <>
void Node::processReq (Msg msg, Topology *topology)
{
    int i;
    int srcPid = msg._srcPid;
    int seqno = msg._seqNo;
    Token* token = topology->getLNToken();

    rn[srcPid]=std::max(rn[srcPid],seqno);
    if(hastoken==1)
    {
        std::cout << "///////// Token Req Event in Token holder site ///////"
                  << std::endl;
        std::cout << "Token holder Site : " << _pid << " Received token request from site : "
                  << msg._srcPid << std::endl;
        if(exec==0 && token->ln[srcPid]+1==rn[srcPid])
        {
            std::cout << "\t transfering token " << std::endl;
            hastoken=0;
            topology->assignToken(srcPid);
        }
        else if(token->ln[srcPid]+1==rn[srcPid])
        {
            std::cout << "\t In CS not transfering token " << std::endl;
            token->token_q.push(srcPid);
            if(token->token_q.empty())
            {
                std::cout << "Error in queu push" << std::endl;
            } else {
                std::cout << "\t Saved token Req to Q" << std::endl << std::endl;
            }
        }
        std::cout << "/////////                                      ///////"
        << std::endl;
    }
}

///
/// @brief application on this node, requested entry to CS>
///        Resolve the token.
///
template <>
void Node::reqToEnterCS (Topology* topology)
{
    int i,seqno;
    seqno= ++rn[_pid];
    std::cout << "site Id : " << _pid << " Token request seqNo : "
               << seqno << std::endl << std::endl;
    //Checking whether it has already requested
    if(isreq==1 || exec==1)
    {
        printf("SITE %d is already requesting \n",_pid);
        return;
    }
    isreq=1;
    ///
    /// If local site has ideal token.
    ///
    if(topology->isNodeTokenHolder(_pid))
    {
        isreq=0;
        exec=1;
        std::cout << " //////    CS Enter with Idle Token    //////" << std::endl;
        printf("SITE %d already has the token and it enters the critical section\n",_pid);
        std::cout << " //////                                //////" << std::endl;
        return;
    }
   
    ///
    /// Local node is not token holder.
    /// broadcast request to other nodes.
    ///
    if(topology->TokenHolder() != _pid)
    {
        Msg msg {_pid, seqno};
        topology->bcastTokenReq(msg);
    }
    
    ///
    /// Wait for some time till request send and receive of reply is complete.
    ///
    
    ///
    /// Checking if requesting site did get the token
    ///
    std::cout << std::endl << std::endl;
    std::cout << "//////// Result of Token Request /////////" << std::endl;
    if(topology->TokenHolder() == _pid)
    {
        ackEntryToCS();
    }
    else
    {
        std::cout << "Token holder site : " << topology->TokenHolder()
        << " executing critical section" << std::endl;
        std::cout << " requsting site : " << _pid << " is waiting in req q" << std::endl;
    }
    std::cout << "////////                       /////////" << std::endl;
    _reqCount++;
}

///
/// @brief application on this node has completed CS and existing CS.
///        Check the request Q, if any pending request, transfer token to the
///          reeusting site.
///        If no pending request, continue to hold ideal token.
///
template <>
void Node::csExit (Topology* topology)
{
    if(exec!=1)
    {
        std::cout << "SITE : " << _pid
                  << "is not currently executing the critical section"
                  << std::endl;
        return;
    }
    int i,siteid;
    Token *token = topology->getLNToken();
    token->updateLn(_pid, rn[_pid]);
    exec=0;
    _csExitCount++;
    std::cout << " //////    CS  EXIT Event    //////" << std::endl;
    std::cout << "SITE : " << _pid << " exists critical section" << std::endl;
    std::cout << " //////                      //////" << std::endl;
    ///
    /// Checking if deffred requests are there by checking token queue
    /// And Passing the token if queue is non empty
    ///
    if(!token->token_q.empty())
    {
        siteid=token->token_q.front();
        token->token_q.pop();
        token->id=siteid;
        hastoken=0;
        topology->transferToken(_pid, siteid);
        std::cout << " //////    Token Transfer Event    //////" << std::endl;
        std::cout << "siteId : " << _pid << " transfered token to site : " << siteid
                  << std::endl;
        std::cout << " //////                           //////" << std::endl;
        return;
    }
    
    std::cout << "SITE : " << _pid << " still holds idle token" << std::endl;
}
