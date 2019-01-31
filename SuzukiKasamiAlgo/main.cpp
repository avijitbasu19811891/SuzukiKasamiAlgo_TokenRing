//
//  main.cpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 04/11/18.
//

#include <iostream>
#include <stdio.h>
#include <string>
#include "GlobalConfig.hpp"
#include "SuzukiKasamiAlgo.hpp"
#include "TransportLayer.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    int i,j,time,pid;
    string str;

    auto topology = Topology::instance();

    if (topology == nullptr) {
        std::cout << "Failed to install topology" << std::endl;
        return 0;
    }
    time=0;

    cout<<"Simulating SuzukiKasamiAlgo on a toplogy of "<<GlobalConfig::NumNode
       << " number of nodes" << endl;
    cout<<"Initial state\n"<<endl;
    topology->description(std::cout);
    printf("\n");

    DataSet* dataSet = DataSet::instance();
    dataSet->description(std::cout);

    TransportLayer transportLayer;
    std::shared_ptr<EventData> eventPtr;
    while(eventPtr = transportLayer.waitForEvent())
    {
        str = eventPtr->decode();
        if(str=="REQ")
        {
            std::cout
                << std::endl << std::endl
                << " ============================" << std::endl;
            cout<<"Application EVENT To enter CS : "<<str<<" on site : "<< eventPtr->siteId() <<endl<<endl;
            topology->recvReqEvent( eventPtr->siteId());
            topology->description(std::cout);
            std::cout << std::endl << " ============================" << std::endl;
        }
        else if(str=="REL")
        {
            std::cout
                << std::endl << std::endl
                << " ============================" << std::endl;
            cout<<"ApplicationEVENT To exit CS :"<<str<<" "<< eventPtr->siteId() <<endl<<endl;
            topology->recvRelEvent(eventPtr->siteId());
            topology->description(std::cout);
            std::cout << std::endl << " ============================" << std::endl;
        }
        else {
            break;
        }
    }
    
    topology->printSummary(std::cout);
    return 0;
}
