#include "httpserver.h"
#include "define.h"
CHttpHandler::CHttpHandler(utility::string_t url):m_listener(url)
{

}
CHttpHandler::~CHttpHandler()
{
    //dtor
}

http_listener* CHttpHandler::getListener()
{
    return &m_listener;
}

CHttpService::CHttpService() {
    m_tpool = make_shared<thread_pool>();
    m_tpool->start();
}

CHttpService::~CHttpService() {

}

void CHttpService::bindHandler(shared_ptr<CHttpHandler> &pHandler)
{
    auto listener = pHandler->getListener();
    listener->support(methods::GET, std::bind(&CHttpService::handle_get, this, std::placeholders::_1));
    listener->support(methods::PUT, std::bind(&CHttpService::handle_put, this, std::placeholders::_1));
    listener->support(methods::POST, std::bind(&CHttpService::handle_post, this, std::placeholders::_1));
    listener->support(methods::DEL, std::bind(&CHttpService::handle_delete, this, std::placeholders::_1));
}

void CHttpService::handle_get(http_request message) {
    auto task = new CForkbaseRestfulThreadTask(message, Restful_Method::GET);
    m_tpool->submit(task);
}

void CHttpService::handle_put(http_request message) {
}

void CHttpService::handle_post(http_request message) {
}

void CHttpService::handle_delete(http_request message) {

}
