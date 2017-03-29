#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <utility>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;
using std::string;
using std::map;

class Config
{
public:
    static Config& Instance ();
    bool initialize ();
    string getString (string section, string key);

private: 
    std::map <string, string> keyInfoMap;
    boost::property_tree::ptree  m_propTree;
    Config ();
   ~Config ();
    bool initConfigFile (string config_file, boost::property_tree::ptree& rtree);
    static Config* m_config;
};

#endif
