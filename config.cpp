/*
 * 04/01/04 Created this file
 * 04/12/04 Implement Config::initConfigFile()
 *          to read the config.ini file
 *
 */

#include "config.h"
#include <string.h>
using namespace std;
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>


Config* Config::m_config = NULL;

Config::Config ()
{
}

Config& Config::Instance ()
{
    if (!m_config) {
        m_config = new Config ();
        m_config->initialize ();
    }
    return *m_config;
}

bool Config::initialize ()
{
    bool retval = true;
    boost::property_tree::ptree propTree;

    try
    {
        retval = initConfigFile ("config.ini", propTree);
        m_propTree = propTree;
    }
    catch (std::exception& ex)
    {
        printf ("Exception occured: %s\n", ex.what());
        retval = false;
    }
    return retval;
}

bool Config::initConfigFile (string config_file, boost::property_tree::ptree& rtree)
{
    if (boost::filesystem::exists (config_file))
    {
        std::string key, value;

        /* read the ini file */
        boost::property_tree::ini_parser::read_ini(config_file, rtree);
        boost::property_tree::ptree::const_iterator end = rtree.end();
        for (boost::property_tree::ptree::const_iterator it = rtree.begin(); it != end; ++it)
        {
            boost::property_tree::ptree sectionTree;
            sectionTree = it->second;

            BOOST_FOREACH (const boost::property_tree::ptree::value_type &v, sectionTree.get_child ("")) 
            {
                key = v.first.c_str();
                value = v.second.get_value<std::string>().c_str();
                keyInfoMap[key] = value;
                printf ("Add key %s: %s\n", key.c_str(), value.c_str()); 
            }
        }
    }
    else
    {
        printf ("File %s  not found.\n", config_file.c_str());
        return false;
    }
    return true;
}

string Config::getString (string section, string key)
{
    boost::property_tree::ptree &iniTree = m_propTree;
    boost::property_tree::ptree::const_iterator end = iniTree.end();
    bool found = false;
    string result = "";

    for (boost::property_tree::ptree::const_iterator it = iniTree.begin(); it != end; ++it)
    {
        if (boost::iequals (it->first, section) && it->second.get_value<std::string>() == "")
        {
            boost::property_tree::ptree sectionTree;
            sectionTree = it->second;

            BOOST_FOREACH (const boost::property_tree::ptree::value_type &v, sectionTree.get_child ("")) 
            {
                if (boost::iequals (v.first, key))
                {
                    result = v.second.get_value<std::string>();
                    found = true;
                    break;
                }
            }
            break;
        }
    }
    if (found)
    {
        int pos = -1;
        pos = result.find ("#");
        result = (pos >- 1) ? result.substr (0, pos-1) : result;
        boost::algorithm::trim (result);
    }

    return result;
}


