// © 2020 Joseph Cameron - All Rights Reserved

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include <jfc/http/context.h>

using namespace jfc;

int main(int count, char **args)
{
    auto pContext = http::context::make(http::context::implementation::curl);

    std::atomic<bool> bShouldClose(false);
    
    auto pRequest = pContext->make_get("https://duckduckgo.com/?q=", 
        "libcurl-agent/1.0",
        //Headers
        {
            //"Travis-API-Version: 3",
        },
        [&](http::request::response_data_type data)
        {
            data.push_back('\0');

            std::cout << &data[0] << "\n";

            bShouldClose = true;
        },
        [&](http::request::error e)
        {
            std::cout << "an error has occured\n";

            bShouldClose = false;
        });

    pRequest->fetch();

    std::vector<std::thread> workers;
    
    for (int i = 0; i < 4; ++i) workers.push_back(std::thread([&]()
    {
        while(!bShouldClose) pContext->worker_update();
    }));

    while (!bShouldClose)
    {
        pContext->main_update();
    }

    for (auto &worker : workers) worker.join();

    return EXIT_SUCCESS;
}

