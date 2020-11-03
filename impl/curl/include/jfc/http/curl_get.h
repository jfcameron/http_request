// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CURL_GET_H
#define JFC_HTTP_CURL_GET_H

#include <jfc/http/curl_request.h>
#include <jfc/http/curl_context.h>

namespace jfc::http
{
    /// \brief curl implementation of a http GET
    class curl_get final : public curl_request
    {
    public:

    private:

    public:
        curl_get(http::curl_context::worker_task_queue_ptr pWorkerTaskQueue,
            const std::string &aURL, 
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            const http::request::response_handler_functor &aResponseHandler,
            const http::request::failure_handler_functor &);

        ~curl_get() = default;
    };
}

#endif

