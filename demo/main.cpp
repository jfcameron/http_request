// © 2020 Joseph Cameron - All Rights Reserved

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <optional>

#include <jfc/http/context.h>

using namespace jfc;

int main(int count, char **args)
{
    auto pHttp = http::context::make(http::context::implementation::curl);
    
    // Simple interface example:
    // pro: less verbose, con: main must process the server's response
    http::context::request_shared_ptr pGet = pHttp->make_get(
        "https://localhost/get_endpoint",
        "Mozilla/5.0 (Android 4.4; Mobile; rv:41.0) Gecko/41.0 Firefox/41.0", //User Agent
        300000, //Timeout MS
        //Headers
        {
            "Connection: close",
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

    // Custom handler interface:
    // pro: worker can process server's response, con: more lines of code
    class json_handler : public http::reponse_handler
    {
        std::optional<size_t> m_size;

    public:
        virtual void worker_on_success(std::vector<char> data) override
        {
            m_size = data.size();

            std::cout << "worker succeeded. Data processed: " << data.size() << "\n";
        }

        virtual void main_on_success() override
        {
            std::cout << "main succeeded, value is: " <<
                (m_size.has_value()
                    ? std::to_string(*m_size)
                    : std::string("the server's response could not be processed!"));
        }

        virtual void main_on_failure(const http::request::error e) override
        {
            std::cout << "main failed\n";
        }

        json_handler() = default;
    };

    auto pPost = pHttp->make_post(
        "https://localhost/post_endpoint",
        "libcurl-agent/1.0", //User Agent
        300000, //Timeout MS
        //Headers
        {
            "Travis-API-Version: 3",
        },
        "good=morning",
        std::make_unique<json_handler>());


    /// === DEMO PROGRAM ===

    pGet->try_enqueue();
    pPost->try_enqueue();

    std::atomic<bool> bProgramShouldClose(false);

    std::vector<std::thread> workers;
    
    for (size_t i(0); i < 4; ++i) workers.push_back(std::thread([&]()
    {
        while (!bProgramShouldClose)
        {
            if (!pHttp->worker_try_perform_enqueued_request_fetches()) 
                std::this_thread::yield();
        }
    }));

    while (pHttp->enqueued_request_count())
    {
        if (!pHttp->main_try_handle_completed_requests())
            std::this_thread::yield();
    }

    bProgramShouldClose = true;

    for (auto &worker : workers) worker.join();

    return EXIT_SUCCESS;
}

