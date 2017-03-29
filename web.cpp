#include <stdio.h>
#include <string.h>
#include <stdexcept>

#include "web.h"
using std::exception;

Web* Web::m_instance = NULL;

void Web::AjaxCallback (struct mg_connection* conn, void* data) 
{ 
    printf ("%s\n", __FUNCTION__); 

    const struct mg_request_info *request_info = mg_get_request_info(conn);
    char content[100];

    // Prepare the message we're going to send
    int content_length = snprintf(content, sizeof(content),
                                "hello from mongoose! Remote port: %d",
                                request_info->remote_port);

    // Send HTTP reply to the client
    mg_printf(conn,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "\r\n"
            "%s",
            content_length, content);
}


// This function will be called by mongoose on every new request.
static int begin_request_handler(struct mg_connection *conn) 
{
    printf ("%s\n", __FUNCTION__);

    try 
    {
        Web::Instance()->AjaxCallback (conn, NULL);
    }
    catch (exception &e)
    {
        printf ("%s\n", e.what());
    }

    // Returning non-zero tells mongoose that our function has replied to
    // the client, and mongoose should not send client any more data.
    return 1;
}

Web::Web()
{
    printf ("%s\n", __FUNCTION__);
    struct mg_callbacks callbacks;
    const char* options[] = { "listening_ports", "8080", NULL };

    memset (&callbacks, 0, sizeof (callbacks));
    callbacks.begin_request = begin_request_handler;

    m_context = mg_start (&callbacks, NULL, options);
    // Wait until user hits "enter". Server is running in separate thread.
    // Navigating to http://localhost:8080 will invoke begin_request_handler().
    // getchar();

    // Stop the server.
    // mg_stop(m_context);

}

Web::~Web() 
{
    printf ("%s\n", __FUNCTION__);
    mg_stop (m_context);
}

Web* Web::Instance()
{
    printf ("%s\n", __FUNCTION__);
    if (m_instance == NULL) 
    {
        m_instance = new Web;
    }

    return m_instance;
}


