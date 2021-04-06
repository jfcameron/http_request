// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/curl_context.h>
#include <jfc/http/curl_get.h>
#include <jfc/http/curl_post.h>
#include <jfc/http/curl_request.h>

#include <iostream>

using namespace jfc;

static size_t c_context_instance_counter(0);

http::curl_context::curl_context()
{
    if (!c_context_instance_counter++)
        curl_global_init(CURL_GLOBAL_ALL);
}

http::curl_context::~curl_context()
{
    if (!--c_context_instance_counter)
    {
        curl_global_cleanup();

        c_context_instance_counter = 0;
    }
}

size_t http::curl_context::enqueued_request_count()
{
    return m_unhandled_requests.size();
}

http::context::request_shared_ptr http::curl_context::make_get(const std::string &aURL,
    const std::string &aUserAgent,
    const size_t aTimeoutMiliseconds,
    const std::vector<std::string> &aHeaders,
    std::unique_ptr<http::reponse_handler> && aHandler)
{
    std::shared_ptr<http::curl_get> p(new jfc::http::curl_get(weak_from_this(), 
        aURL, aUserAgent, aTimeoutMiliseconds, aHeaders, std::move(aHandler)));

    return std::static_pointer_cast<jfc::http::context::request_shared_ptr::element_type>(p);
}

std::shared_ptr<http::post> http::curl_context::make_post(const std::string &aURL,
    const std::string &aUserAgent,
    const size_t aTimeoutMiliseconds,
    const std::vector<std::string> &aHeaders,
    const std::string &aPostData,
    std::unique_ptr<http::reponse_handler> && aHandler)
{
    std::shared_ptr<http::curl_post> p(new jfc::http::curl_post(weak_from_this(),
        aURL, aUserAgent, aTimeoutMiliseconds, aHeaders, aPostData, std::move(aHandler)));

    return std::static_pointer_cast<http::post>(p);
}

bool http::curl_context::main_try_handle_completed_request()
{
    for (size_t i(0); i < m_unhandled_requests.size(); ++i)
    {
        if (m_unhandled_requests[i]->main_try_run_handlers())
        {
            m_unhandled_requests.erase(m_unhandled_requests.begin() + i);

            return true;
        }
    }

    return false;
}

bool http::curl_context::worker_try_perform_enqueued_request()
{
    if (curl_context::worker_task_type task; m_worker_task_queue->try_dequeue(task)) 
    {
        task();

        return true;
    }

    return false;
}

void http::curl_context::enqueue(std::shared_ptr<http::curl_request> aRequest)
{
    m_unhandled_requests.push_back(aRequest);

    m_worker_task_queue->enqueue(
        std::bind(&http::curl_request::worker_fetch_task, aRequest.get()));
}

