//
//  InputDataSet.hpp
//  SuzukiKasamiAlgo
//
//  Created by Avijit Basu on 05/11/18.
//

#ifndef InputDataSet_hpp
#define InputDataSet_hpp

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <list>
#include "GlobalConfig.hpp"
using namespace std;

class EventData {
public:
    string str;
    int    _siteId = -1;
    
    EventData (std::string line) {
        stringstream s(line);
        bool parsedCmd = std::false_type::value;
        if (s >> str) {
            parsedCmd = std::true_type::value;
        }
        if (!parsedCmd) {
            return;
        }
        std::cout << "Parsed Cmd:" << str << std::endl;
        if (s >> _siteId) {
            std::cout << "Parsed SiteId" << _siteId << std::endl;
            if ((_siteId > GlobalConfig::NumNode) ||
                (_siteId < 0)) {
                    str = "End";
                }
        }
    }
    
    bool isValid (void) {
        return (!str.empty());
    }
    
    std::string decode (void) {
        if (isValid()) {
            return str;
        }
        return "END";
    }
    int siteId (void) {
        if (isValid()) {
            return _siteId;
        }
        return -1;
    }
};

class DataSet {
    DataSet (void);
    std::list<string> _eventList;
public:
    std::shared_ptr<EventData> getNextEvent (void);
    
    static DataSet* instance (void) {
        static DataSet dataSet;
        return &dataSet;
    }
    void readConfigFile( void);
    void description (std::ostream& os);
};
#endif /* InputDataSet_hpp */
