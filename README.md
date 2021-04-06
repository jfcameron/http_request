[![Build Status](https://travis-ci.org/jfcameron/jfc-http_request.svg?branch=master)](https://travis-ci.org/jfcameron/jfc-http_request)[![Documentation](https://img.shields.io/badge/documentation-doxygen-blue.svg)](https://jfcameron.github.io/jfc-http_request/)

## jfc-http_request

Concurrent http request library, uses libcurl for implementation. Supports Linux, Mac, Windows. currently supports GET and POST requests.

A single thread makes requests then handles their responses asynchronously. Any number of worker threads perform the requests concurrently. Additionally, the worker can optionally process the raw response (e.g: convert json text to a json data model) before handing the processed data back to main.

This project does not create threads or keep a thread group itself, this is done to allow the user to decide what threading strategy makes the most sense in the dependent project.

### usage

See `demo/` for example programs. [Click here for documentation](https://jfcameron.github.io/jfc-http_request/).
Below is a basic overview of how to use the library:
```cpp
// Initializing the library
auto pHttp = http::context::make(http::context::implementation::curl);

// Creating a basic GET request
auto pGet = pHttp->make_get(
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

// Enqueuing the request
pGet->try_enqueue();

// Setting up a worker threadgroup to do the fetch work
std::vector<std::thread> workers;

std::atomic<bool> bProgramShouldClose(false);

for (size_t i(0); i < 4; ++i) workers.push_back(std::thread([&]()
{
    while (!bProgramShouldClose)
    {
        if (!pHttp->worker_try_perform_enqueued_request_fetches()) 
            std::this_thread::yield();
    }
}));

// Handling response on the main thread
while (auto c = pHttp->enqueued_request_count())
{
    if (!pHttp->main_try_handle_completed_requests())
        std::this_thread::yield();
}

bProgramShouldClose = true;
```


### building

see `.travis.yml` for platform specific instructions

