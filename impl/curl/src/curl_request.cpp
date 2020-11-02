// © 2020 Joseph Cameron - All Rights Reserved

#include <stdexcept>
#include <iostream>
#include <string>

#include <jfc/http/curl_request.h>

#include <moody/concurrentqueue.h>

using namespace jfc;

static size_t WriteMemoryCallback(unsigned char *const contentPointer, 
    const size_t contentItemSize, 
    const size_t contentItemCount, 
    void *const userPointer)
{
    const size_t contentByteCount(contentItemSize *contentItemCount);

    auto pResponseBuffer(static_cast<http::request::response_data_type *const>(userPointer));

    pResponseBuffer->insert(pResponseBuffer->end(), contentPointer, contentPointer + contentByteCount);

    return contentByteCount;
}

http::curl_request::curl_request(http::curl_context::worker_task_queue_ptr pWorkerTaskQueue,
    const std::string &aURL,
    const std::string &aUserAgent,
    const std::vector<std::string> &aHeaders)
: m_pHandle([]()
    {
        auto handle = curl_easy_init();

        if (!handle) throw std::runtime_error("could not initialize a new curl handle");

        return  handle;
    }()
    , [](CURL *p)
    {
        curl_easy_cleanup(p);
    })
, m_pWorkerTaskQueue(pWorkerTaskQueue)
{
    curl_easy_setopt(m_pHandle.get(), CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    curl_easy_setopt(m_pHandle.get(), CURLOPT_URL, aURL.c_str());

    curl_easy_setopt(m_pHandle.get(), CURLOPT_USERAGENT, aUserAgent.c_str());
    
    if (!aHeaders.empty())
    {
        m_pHeaders = {[&]()
        {
            curl_slist *headerlist = curl_slist_append(nullptr, aHeaders[0].c_str());

            if (aHeaders.size() > 1) for (size_t i(1); i < aHeaders.size(); ++i)
                headerlist = curl_slist_append(headerlist, aHeaders[i].c_str());

            curl_easy_setopt(m_pHandle.get(), CURLOPT_HTTPHEADER, headerlist);

            return headerlist;
        }()
        , [](curl_slist *p)
        {
            curl_slist_free_all(p);
        }};
    }
    
    //curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 10L); //TODO: provide param
}

void http::curl_request::fetch() 
{
    std::cout << "fetch\n";

    m_pWorkerTaskQueue->enqueue([&]()
    {
        std::cout << "doing taskl\n";

        m_bResponseReady.clear();

        response_data_type buffer;
        curl_easy_setopt(m_pHandle.get(), CURLOPT_WRITEDATA, reinterpret_cast<void *>(&buffer)); 
        
        const CURLcode curlResult = curl_easy_perform(m_pHandle.get());
        
        //TODO: handle failure
        m_Response = buffer;
        
        m_bResponseReady.test_and_set();
    });

    // call appropriate response functor
    //if (curlResult == CURLE_OK) aHandler(buffer);
    //else aFailedHandler();
}

void http::curl_request::main_update()
{
    if (m_bResponseReady.test_and_set())
    {
        //TODO: need to param it
        //m_SuccessfulResponseHandler(m_Response);

        std::string blar(m_Response.begin(), m_Response.end());

        std::cout << blar << "\n";
    }
}

void http::curl_request::worker_update()
{
    if (curl_context::worker_task_type task; m_pWorkerTaskQueue->try_dequeue(task)) task();
}

