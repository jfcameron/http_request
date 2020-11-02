// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CURL_CONTEXT_H
#define JFC_HTTP_CURL_CONTEXT_H

#include <memory>
#include <vector>

#include <moody/concurrentqueue.h>

#include <jfc/http/context.h>

namespace jfc::http
{
    class curl_request;

    class curl_context final : public http::context
    {
    public:
        using curl_context_ptr = std::shared_ptr<http::curl_context>;
        
        using worker_task_type = std::function<void()>;

        using worker_task_queue = moodycamel::ConcurrentQueue<worker_task_type>;

        using worker_task_queue_ptr = std::shared_ptr<worker_task_queue>;

    private:
        std::vector<std::weak_ptr<http::curl_request>> m_requests;

        worker_task_queue_ptr m_worker_task_queue = std::make_shared<worker_task_queue>(worker_task_queue());

    public:
        virtual request_shared_ptr make_get(const std::string &aURL,
            const std::string &aUserAgent,
            const std::vector<std::string> &aHeaders) override;

        virtual void main_update() override;

        virtual void worker_update() override;

        curl_context();

        virtual ~curl_context();
    };
}

#endif

