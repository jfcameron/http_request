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
    auto pHttp = http::context::make(http::context::implementation::curl);

    auto pPost = pHttp->make_post(
        "http://localhost/post_endpoint",
        "libcurl-agent/1.0", //User Agent
        300000, //Timeout MS
        //Headers
        {
            //"Travis-API-Version: 3",
        },
        "good=morning",
        [&](http::request::response_data_type data) //Response handler
        {
            std::string s(data.begin(), data.end());

            std::cout << "post succeeded: " << s << "\n";
        },
        [&](http::request::error e) //Fail handler
        {
            std::cout << "post failed\n";
        });

    http::context::request_shared_ptr pGet = pHttp->make_get(
        "http://localhost/post_endpoint",
        "Mozilla/5.0 (Android 4.4; Mobile; rv:41.0) Gecko/41.0 Firefox/41.0", //User Agent
        300000, //Timeout MS
        //Headers
        {
            //"Travis-API-Version: 3",
        },
        [&](http::request::response_data_type data) //Response handler
        {
            std::string s(data.begin(), data.end());

            std::cout << "get response: " << s << "\n";
        },
        [&](http::request::error e) //Fail handler
        {
            std::cout << "get failed\n";
        });

    pGet->try_enqueue();
    pPost->try_enqueue();

    std::vector<std::thread> workers;
    
    for (int i(0); i < 4; ++i) workers.push_back(std::thread([&]()
    {
        pHttp->worker_perform_enqueued_request_fetches();
    }));

    while (auto c = pHttp->enqueued_request_count())
    {
        pHttp->main_handle_completed_requests();
    }

    for (auto &worker : workers) worker.join();

    return EXIT_SUCCESS;
}

