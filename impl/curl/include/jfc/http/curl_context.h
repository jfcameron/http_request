// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CURL_CONTEXT_H
#define JFC_HTTP_CURL_CONTEXT_H

#include <memory>
#include <vector>
#include <queue>
#include <map>
#include <list>

#include <moody/concurrentqueue.h>

#include <jfc/http/context.h>

namespace jfc::http
{
    class curl_request;

    /// \brief follows proactor pattern
    class curl_context final : public http::context, public std::enable_shared_from_this<curl_context>
    {
    public:
    /// \name external interface
    ///@{
    //
        virtual request_shared_ptr make_get(const std::string &aURL,
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            std::unique_ptr<http::reponse_handler> &&) override;

        virtual std::shared_ptr<http::post> make_post(const std::string &aURL,
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            const std::string &aPostData,
            std::unique_ptr<http::reponse_handler> &&) override;

        virtual bool main_try_handle_completed_request() override;

        virtual bool worker_try_perform_enqueued_request() override;
        
        virtual size_t enqueued_request_count() override;
    ///@}

    /// \name internal interface
    ///@{
    //
        /// \brief enqueues a request into the proactor system
        void enqueue(std::shared_ptr<http::curl_request> aRequest);
    ///@}

        curl_context();

        ~curl_context();

        using curl_context_ptr = std::shared_ptr<http::curl_context>;
        using worker_task_type = std::function<void()>;
        using worker_task_queue = moodycamel::ConcurrentQueue<worker_task_type>;
        using worker_task_queue_ptr = std::shared_ptr<worker_task_queue>;

    private:
        /// \brief multiplexer: queue distributes fetch tasks to the workers
        worker_task_queue_ptr m_worker_task_queue = std::make_shared<worker_task_queue>(worker_task_queue());
        
        /// \brief demultiplexer: result of completed tasks are used by main via the completion handlers
        std::vector<std::shared_ptr<http::curl_request>> m_unhandled_requests;
    };
}

#endif

