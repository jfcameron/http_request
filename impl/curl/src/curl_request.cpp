// © 2020 Joseph Cameron - All Rights Reserved

#include <stdexcept>
#include <iostream>
#include <string>

#include <jfc/http/curl_request.h>

#include <moody/concurrentqueue.h>

using namespace jfc;

static http::request::error curlcode_to_requesterror(const CURLcode aCurlCode)
{
    http::request::error return_value = http::request::error::unhandled_error;

    switch (aCurlCode)
    {
    case(CURLE_OK): return_value = http::request::error::none; break;
    case(CURLE_UNSUPPORTED_PROTOCOL): return_value = http::request::error::unsupported_protocol; break;
    }

    return return_value;
}

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

http::curl_request::curl_request(std::weak_ptr<http::curl_context> pContext,
    const std::string &aURL,
    const std::string &aUserAgent,
    const size_t aTimeoutMiliseconds,
    const std::vector<std::string> &aHeaders,
    const http::request::response_handler_functor &aResponseHandler,
    const http::request::failure_handler_functor &aFailureHandler)
: m_pContext(pContext)    
, m_pHandle([]()
    {
        auto handle = curl_easy_init();

        if (!handle) throw std::runtime_error("could not initialize a new curl handle");

        return  handle;
    }()
    , [](CURL *p)
    {
        curl_easy_cleanup(p);
    })
, m_ResponseHandler(aResponseHandler)
, m_FailureHandler(aFailureHandler)
{
    m_bResponseLocked.test_and_set();

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

    curl_easy_setopt(m_pHandle.get(), CURLOPT_TIMEOUT_MS, aTimeoutMiliseconds);
}

void http::curl_request::on_enqueue_extra_worker_configuration(CURL *const) {}

void http::curl_request::worker_fetch_task()
{
    m_bResponseLocked.test_and_set();
    
    m_ResponseBody.clear();

    curl_easy_setopt(m_pHandle.get(), CURLOPT_WRITEDATA, reinterpret_cast<void *>(&m_ResponseBody)); 

    on_enqueue_extra_worker_configuration(m_pHandle.get());

    m_RequestError = curlcode_to_requesterror(curl_easy_perform(m_pHandle.get()));
    
    m_bResponseLocked.clear();
}

bool http::curl_request::try_enqueue() 
{
    if (!m_bDoNotEnqueue)
    {
        m_bDoNotEnqueue = true;

        if (auto pContext = m_pContext.lock())
            pContext->enqueue(shared_from_this());
        else 
            return false;

        return true;
    }

    return false;
}

bool http::curl_request::main_try_run_handlers()
{
    if (!m_bResponseLocked.test_and_set())
    {
        if (m_RequestError == http::request::error::none) m_ResponseHandler(m_ResponseBody);
        else m_FailureHandler(m_RequestError);

        m_bDoNotEnqueue = false;

        return true;
    }

    return false;
}

bool http::curl_request::getIsEnqueued() const
{
    return m_bDoNotEnqueue;
}

