#ifndef _WEB_H
#define _WEB_H
#include "mongoose/mongoose.h"

class Web
{
private:
    Web();
    virtual ~Web();
    static Web* m_instance;

public:
    struct mg_context* m_context;
    static Web* Instance();
    void AjaxCallback (struct mg_connection* conn, void* data);
};

#endif
