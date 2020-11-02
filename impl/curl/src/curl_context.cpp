// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/curl_context.h>
#include <jfc/http/curl_get.h>
#include <jfc/http/curl_request.h>

#include <iostream>

using namespace jfc;

http::curl_context::curl_context()
{
    //TODO: think about global init. shared counter to init/deinit?
    //curl_global_init
}

http::curl_context::~curl_context()
{
    //curl_global_cleanup
}

http::context::request_shared_ptr http::curl_context::make_get(const std::string &aURL,
    const std::string &aUserAgent,
    const std::vector<std::string> &aHeaders,
    const http::request::response_handler_functor &aResponseHandler,
    const http::request::failure_handler_functor &aFailureHandler)
{
    std::shared_ptr<http::curl_request> p(new jfc::http::curl_get(m_worker_task_queue,
        aURL, aUserAgent, aHeaders, aResponseHandler, aFailureHandler));

    m_requests.push_back(std::weak_ptr<http::curl_request>(p));

    return std::static_pointer_cast<jfc::http::context::request_shared_ptr::element_type>(p);
}

void http::curl_context::main_update()
{
    for (size_t i(0); i < m_requests.size(); ++i)
    {
        auto request = m_requests[i];

        if (auto p = request.lock()) p->main_update();
        else
            m_requests.erase(m_requests.begin() + i--);
    }
}

void http::curl_context::worker_update()
{
    for (size_t i(0); i < m_requests.size(); ++i)
    {
        auto request = m_requests[i];

        if (auto p = request.lock()) p->worker_update();
        else
            m_requests.erase(m_requests.begin() + i--);
    }
}

