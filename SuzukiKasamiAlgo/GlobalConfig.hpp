//
//  GlobalConfig.hpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 04/11/18.
//

#ifndef GlobalConfig_hpp
#define GlobalConfig_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>

class GlobalConfig {
public:
    static int constexpr NumNode = 6;
    static int constexpr InitialTokeHolder = 0;
};

//Description of the token
class Token
{
public:
    int id;                           //Id of the site having token
    std::queue <int> token_q;              //Token queue
    int ln[GlobalConfig::NumNode];                       //Token Array for sequence no
    void init()                       //Initializing token
    {
        id=0;
        for(int i=0; i < GlobalConfig::NumNode; i++)
        {
            ln[i]=0;
        }
    }
    void
    updateLn (int pid, int seqNo) {
        std::cout << "Updating Ln ->" << std::endl;
        std::cout << "\t Site : " << pid << "Ln : " << seqNo << std::endl;
       ln[pid]= seqNo;
    }
};

#endif /* GlobalConfig_hpp */
