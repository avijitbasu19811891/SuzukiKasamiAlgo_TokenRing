//
//  Node.hpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 04/11/18.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include "GlobalConfig.hpp"
#include "TransportLayer.hpp"

//Description of each Node
class Node
{
    int _pid;
public :
    int rn[GlobalConfig::NumNode];                      //Site's Array for sequence no.
    bool exec;                       //For checking whether site is executing
    bool isreq;                      //For checking whether site is  requesting
    bool hastoken;                   //For checking whether site has token
    int _reqCount =0;
    int _csEntryCount = 0;
    int _csExitCount = 0;
    void init(int pid)                      //Initializing sites
    {
        exec=0;
        isreq=0;
        hastoken=0;
        _pid = pid;
        for(int i=0;i< GlobalConfig::NumNode;i++)
        {
            rn[i]=0;
        }
    }
    template <class T>
    void processReq(Msg msg, T* topology);
    template <class T>
    void reqToEnterCS(T* topology);
    template <class T>
    void csExit(T* topology);
    
    void ackEntryToCS (void) {
        hastoken= std::true_type::value;
        exec= std::true_type::value;
        isreq= std::false_type::value;
        _csEntryCount++;
        std::cout << "Reeusting site : " << _pid << "gets the token and it enters the critical section\n";
    }
    bool isExecutingCs (void) {
        return (exec == std::true_type::value);
    }
    bool holdsIdealToken (void) {
        return ((exec == std::false_type::value) && hastoken);
    }
    bool holdsToken (void) {
        return hastoken;
    }
    void description (std::ostream & os) {
        os << "Site Id : " << _pid << std::endl;
        os << "Stats ->\n";
        os << "\t CS Req Count : " << _reqCount << "\n";
        os << "\t CS Entry success : " << _csEntryCount << "\n";
        os << "\t CS Exit Count : " << _csExitCount << std::endl;
    }
};

#endif /* Node_hpp */
