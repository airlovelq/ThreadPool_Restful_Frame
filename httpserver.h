#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <memory>
#include <fstream>
#include "cpprest/http_listener.h"
#include "cpprest/filestream.h"
#include "thread_manager.h"

#include "forkbase_restful_operation.h"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace web::http::experimental::listener;


class CHttpHandler
{
public:
    CHttpHandler() = delete;
    CHttpHandler(utility::string_t url);
    virtual ~CHttpHandler();

public:
    http_listener *getListener();

private:
    http_listener m_listener;

public:
    pplx::task<void> open(){return m_listener.open();}
    pplx::task<void> close(){return m_listener.close();}
};

class CHttpService
{
public:
    CHttpService();
    virtual ~CHttpService();

public:
    virtual void handle_get(http_request message);
    virtual void handle_put(http_request message);
    virtual void handle_post(http_request message);
    virtual void handle_delete(http_request message);
//    virtual void handle_error(pplx::task<void>& t) = 0;
    virtual void bindHandler(shared_ptr<CHttpHandler> &pHandler);

private:
    std::shared_ptr<thread_pool> m_tpool;
};
#endif // HTTPSERVER_H
