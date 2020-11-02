// © 2020 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <string>
#include <thread>

#include <jfc/http/context.h>

using namespace jfc;

int main(int count, char **args)
{
    auto pContext = http::context::make(http::context::implementation::curl);

    auto pRequest = pContext->make_get("archlinux.org", "libcurl-agent/1.0",
        //Headers
        {
            //"Travis-API-Version: 3"
        });

    pRequest->fetch();

    bool bShouldClose(false);

    std::vector<std::thread> workers;
    
    for (int i = 0; i < 4; ++i) workers.push_back(std::thread([&]()
    {
        while(true) pContext->worker_update();
    }));

    while (!bShouldClose)
    {
        pContext->main_update();

        //bShouldClose = true;
    }

    return EXIT_SUCCESS;
}

