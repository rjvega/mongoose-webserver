#include "info.h"

Info::Info (void) : localDb(0)
{
}

Info::~Info ()
{
}

string Info::getKey ()
{
    return this->key;
}

string Info::getText ()
{
    return this->text;
}

string Info::getNumber ()
{
    return this->number;
}

string Info::getData ()
{
    return this->data;
}

string Info::getDate ()
{
    return this->date;
}

void Info::setKey (string _key)
{
    this->key = _key;
}

void Info::setText (string _text)
{
    this->text = _text;
}

void Info::setNumber (string _number)
{
    this->number = _number;
}

void Info::setData (string _data)
{
    this->data = _data;
}

void Info::setDate (string _date)
{
    this->date = _date;
}

list<Info*> Info::getInfoList ()
{
    return this->listInfo;
}

void Info::setInfoList (list<Info*> pInfo)
{
    this->listInfo = pInfo;
}

