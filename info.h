#ifndef INFO_H
#define INFO_H

#include <string>
#include <list>
#include "sqlite3.h"

using std::string;
using std::list;

class Info
{
private:
    static string buffer;
    sqlite3* localDb;

    string key;
    string text;
    string number;
    string data;
    string date;

    list<Info*> listInfo;

public:
    Info ();
   ~Info ();
    string getKey ();
    string getText ();
    string getNumber ();
    string getData ();
    string getDate ();

    void setKey (string);
    void setText (string);
    void setNumber (string);
    void setData (string);
    void setDate (string);

    list<Info*> getInfoList ();
    void setInfoList (list<Info*>);
};

#endif
