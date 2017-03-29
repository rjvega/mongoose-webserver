#ifndef DATA_H
#define DATA_H

#include <string>
#include <sqlite3.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>


#include "config.h"
#include "info.h"

using std::string;

#define DATABASE "/home/rvega/main/db/data.db3"

static char sqlCreateDataTable[] =
    "CREATE TABLE IF NOT EXISTS data ("
    "key TEXT PRIMARY KEY, "
    "text TEXT, "
    "number TEXT, "
    "data TEXT, "
    "date TEXT, "
    "UNIQUE (key, text) ON CONFLICT REPLACE"
    ")";

static char sqlWriteDataTable[] = 
    "INSERT OR REPLACE INTO data ("
    "key, "
    "text, "
    "number, "
    "data, "
    "date"
    ") VALUES(?,?,?,?,?)";

static char sqlReadDataTable[] =
    "SELECT key, text, number, data, date FROM data ";


class Data
{
private:
    sqlite3 *localDb;
    Data ();
   ~Data ();
    bool dbClose ();

public:
    void dbOpen ();
    bool dbCreateTable ();
    bool dbWrite (Info *);
    bool dbRead (Info *, int *);

    // test functions for debugging
    bool test_dbWrite ();
    bool test_dbRead ();

    static Data& Instance ();
};

#endif
