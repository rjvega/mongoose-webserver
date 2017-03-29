/*
 * To install sqlite3, run the command:
 * sudo apt-get install sqlite3 libsqlite3-dev
 */

#include "config.h"
#include "data.h"
#include "info.h"

static Data* m_instance;

Data::Data (void)
{
    printf ("%s constructor entry.\n", __FUNCTION__);
    dbOpen ();
    dbCreateTable ();

    // test_dbWrite ();
    test_dbRead ();
}

Data::~Data ()
{
    dbClose ();
}

Data& Data::Instance ()
{
    Config& conf = Config::Instance ();
    if (!m_instance) m_instance = new Data ();
    return *m_instance;
}

void Data::dbOpen ()
{
    printf ("%s entry.\n", __FUNCTION__);
    Config& conf = Config::Instance ();
    string dbName = conf.getString ("database", "dbName");
    if (dbName.empty())
    {
        dbName = string (DATABASE);
    }

    int status = sqlite3_open (dbName.c_str(), &localDb);
    if (status != SQLITE_OK || localDb == NULL)
    {
        printf ("%s: problem opening database: status_code: %d", __FUNCTION__, status);
    } else {
        printf ("%s: Open database successfully\n", __FUNCTION__);
    }
}

bool Data::dbClose ()
{
    printf ("%s entry.\n", __FUNCTION__);
    bool ret = true;
    if (localDb != NULL)
    {
        int status = sqlite3_close (localDb);
        if (status != SQLITE_OK)
        {
            ret = false;
        }
    }
    return ret;
}

bool Data::dbCreateTable ()
{
    printf ("%s entry.\n", __FUNCTION__);
    int status;
    sqlite3_stmt* pStmt;

    status = sqlite3_prepare_v2 (localDb, sqlCreateDataTable, -1, &pStmt, NULL);
    if (status != SQLITE_OK)
    {
        printf ("%s: invalid sqlite3_prepare: status_code: %d, sql: %s", \
                __FUNCTION__, status, sqlCreateDataTable);
        return false;
    }

    status = sqlite3_step (pStmt);
    if (status != SQLITE_DONE)
    {
        printf ("%s: invalid sqlite3_step: status_code: %d, sql: %s", \
                __FUNCTION__, status, sqlCreateDataTable);
        return false;
    }

    status = sqlite3_finalize (pStmt);
    if (status != SQLITE_OK)
    {
        printf ("%s: invalid sqlite3_finalize: status_code: %d, sql: %s", \
                __FUNCTION__, status, sqlCreateDataTable);
        return false;
    }
    return true;
}  

bool Data::dbWrite (Info *info)
{
    printf ("%s entry.\n", __FUNCTION__);
    int status;
    sqlite3_stmt* pStmt;

    string dbSql = string (sqlWriteDataTable);

    status = sqlite3_prepare_v2 (localDb, dbSql.c_str(), dbSql.length(), &pStmt, NULL);
    if (status != SQLITE_OK)
    {
        printf ("%s: invalid sqlite3_prepare: status_code: %d, sql: %s", \
                __FUNCTION__, status, dbSql.c_str());
        return false;
    }

    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    const string key = boost::lexical_cast<string>(uuid);

    status = sqlite3_bind_text (pStmt, 1, key.c_str(), key.length(), SQLITE_STATIC);
    status = sqlite3_bind_text (pStmt, 2, info->getText().c_str(), info->getText().length(), SQLITE_STATIC);
    status = sqlite3_bind_text (pStmt, 3, info->getNumber().c_str(), info->getNumber().length(), SQLITE_STATIC);
    status = sqlite3_bind_text (pStmt, 4, info->getData().c_str(), info->getData().length(), SQLITE_STATIC);
    status = sqlite3_bind_text (pStmt, 5, info->getDate().c_str(), info->getDate().length(), SQLITE_STATIC);
   
    status = sqlite3_step (pStmt);
    if (status != SQLITE_DONE)
    {
        printf ("%s: invalid sqlite3_step: status_code: %d, sql: %s", \
                __FUNCTION__, status, sqlWriteDataTable);
        return false;
    }

    status = sqlite3_finalize (pStmt);
    if (status != SQLITE_OK)
    {
        printf ("%s: invalid sqlite3_finalize: status_code: %d, sql: %s", \
                __FUNCTION__, status, sqlCreateDataTable);
        return false;
    }
    return true;
}

bool Data::dbRead (Info *pInfo, int *count)
{
    printf ("%s entry.\n", __FUNCTION__);
    int status;
    sqlite3_stmt* pStmt;

    string dbSql = string (sqlReadDataTable);

    status = sqlite3_prepare_v2 (localDb, dbSql.c_str(), dbSql.length(), &pStmt, NULL);
    if (status != SQLITE_OK)
    {
        printf ("%s: invalid sqlite3_prepare: status_code: %d, sql: %s", \
                __FUNCTION__, status, dbSql.c_str());
        return false;
    }

    int row = 0;
    list<Info*> dbList;
    while (1)
    {
        int s;
        s = sqlite3_step (pStmt);
        if (s == SQLITE_ROW)
        {
            Info *info = new Info();
            const char* value;
            int bytes;
            bytes = sqlite3_column_bytes (pStmt, 0);

            value = (const char*) sqlite3_column_text (pStmt, 0);
            info->setKey (string (value));

            value = (const char*) sqlite3_column_text (pStmt, 1);
            info->setText (string (value));

            value = (const char*) sqlite3_column_text (pStmt, 2);
            info->setNumber (string (value));

            value = (const char*) sqlite3_column_text (pStmt, 3);
            info->setData (string (value));

            value = (const char*) sqlite3_column_text (pStmt, 4);
            info->setDate (string (value));

            dbList.push_back (info);
            row++;
        } 
        else if (s = SQLITE_DONE)
        {
            break;
        }
        else
        {
            printf ("%s: read failed.\n", __FUNCTION__);
            break;
        }
    }
    *count = row;
    pInfo->setInfoList (dbList);

    status = sqlite3_finalize (pStmt);
    if (status != SQLITE_OK)
    {
        printf ("%s: invalid sqlite3_finalize: status_code: %d, sql: %s", \
                __FUNCTION__, status, dbSql.c_str());
        return false;
    }

    return true;
}

bool Data::test_dbWrite ()
{
    printf ("%s entry.\n", __FUNCTION__);
    Info info;
    string text, number, data, date;

    text = "some text";
    info.setText (text);

    number = "1234";
    info.setNumber (number);

    data = "blob data goes here";
    info.setData (data);

    date = "04/20/2014";
    info.setDate (date);

    bool rc = dbWrite (&info);
    if (rc)
        printf ("%s: INSERT successfull.\n", __FUNCTION__);
    else
        printf ("%s: INSERT failed.\n", __FUNCTION__);

    return rc;
}

bool Data::test_dbRead ()
{
    printf ("%s entry.\n", __FUNCTION__);
    Info info;
    int rowCount;
    bool rc = false;

    if (dbRead (&info, &rowCount))
    {
        list<Info*> infoList = info.getInfoList ();
        list<Info*>::iterator iter;

        int i = 0;
        for (iter = infoList.begin(); iter != infoList.end(); ++iter, i++)
        {
            Info *inf = *iter;
            printf (">  %s, %s, %s, %s, %s\n", inf->getKey().c_str(), inf->getText().c_str(), \
                    inf->getNumber().c_str(), inf->getData().c_str(), inf->getDate().c_str());

        }
        rc = true;
    }
    return rc;
}


