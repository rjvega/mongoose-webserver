/*
 * 03/31/04 Created this file
 *          Installed boost. sudo aptitude install libboost-all-dev
 *          Files are located in /usr/include/boost, and /usr/lib/libboost*
 * 04/11/04 Implement Config::initConfigfile()
 *          to read the mwserver.ini file
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

#include "config.h"
#include "data.h"
#include "web.h"

int main (int argc, char *argv[])
{
    char section[] = "database";
    char key[]     = "dbName";

    // read ini configurations
    Config& conf = Config::Instance ();
    string result = conf.getString (section, key);
    printf ("CALL %s getString (%s, %s)\n", result.c_str(), section, key);

    Data& data = Data::Instance ();
    Web::Instance ();
    getchar();
}
