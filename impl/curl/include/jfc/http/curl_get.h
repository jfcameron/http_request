// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CURL_GET_H
#define JFC_HTTP_CURL_GET_H

#include <jfc/http/curl_request.h>
#include <jfc/http/curl_context.h>

namespace jfc::http
{
    class curl_get final : public curl_request
    {
    public:

    private:

    public:
        curl_get(http::curl_context::worker_task_queue_ptr pWorkerTaskQueue,
            const std::string &aURL, 
            const std::string &aUserAgent,
            const std::vector<std::string> &aHeaders);

        ~curl_get() = default;
    };
}

#endif

