//
//  InputDataSet.cpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 05/11/18.
//

#include "InputDataSet.hpp"
#include "GlobalConfig.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include "GlobalConfig.hpp"

///
/// Similates the Request from upper layer application running on a given site.
///
///
/// This includes a set of commands, of format
/// <cmd> SiteId
/// cmd can be following
///   - REQ : application in this node, request to enter CS.
///   - REL : application in this node request to exit CS
///   - END : complete algo.
///

void
DataSet::description(std::ostream &os) {
    int i;
    std::cout << "Set of Events to triggers to above toplogy ->"
    << std::endl << std::endl;
    for (auto it = _eventList.begin(); it != _eventList.end(); it++) {
        os <<  *it << std::endl;
    }
    os << std::endl << std::endl;
}

void DataSet::readConfigFile( void) {
    std::ifstream file("EventList.txt");
    if (file.is_open()) {
        std::cout << "Parsing config file" << std::endl;
        std::string line;
        while (getline(file, line)) {
            // using printf() in all tests for consistency
            printf("%s \n", line.c_str());
            _eventList.push_back(line);
        }
        file.close();
    }
}

///
/// @brief wait for next set of event from application on the nodes.
///
std::shared_ptr<EventData> DataSet::getNextEvent (void) {
    if (_eventList.empty()) {
        return nullptr;
    }
    std::string line = _eventList.front();
    _eventList.pop_front();
    std::shared_ptr<EventData> eventData =
    std::make_shared<EventData>(line);
    return eventData;
}

DataSet::DataSet (void) {
    readConfigFile();
}
