#include <iostream>
#include <memory>
#include "thread_manager.h"
#include "forkbase_restful_operation.h"
#include "httpserver.h"

using namespace std;

int main()
{
    uri_builder uri("http://192.168.2.111:34679");
    auto addr = uri.to_uri().to_string();
    auto pHandler = make_shared<CHttpHandler>(addr);

    CHttpService pServerToken;
    pServerToken.bindHandler(pHandler);
    pHandler->open().wait();
    std::cout << "Press ENTER to exit." << std::endl;

    std::string line;
    std::getline(std::cin, line);
    return 0;

    return 0;
}
