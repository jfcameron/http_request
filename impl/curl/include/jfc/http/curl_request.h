// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CURL_REQUEST_H
#define JFC_HTTP_CURL_REQUEST_H

#include <atomic>
#include <functional>
#include <memory>
#include <vector>

#include <jfc/http/request.h>
#include <jfc/http/curl_context.h>

#include <curl/curl.h>

namespace jfc::http
{
    /// \brief curl implementation of http request
    class curl_request : public http::request
    {
    public:
        /// \brief wrapper for curl handle
        using curl_handle_ptr_type = std::shared_ptr<CURL>;
        
        /// \brief wrapper for curl linked list
        using curl_linked_list_ptr_type = std::shared_ptr<curl_slist>;

    private:
        /// \brief bool that protects shared data
        std::atomic_flag m_bResponseLocked;

        /// \brief handle to the request in the curl context
        curl_handle_ptr_type m_pHandle;

        /// \brief ptr to a curl linked list used to store header entries
        curl_linked_list_ptr_type m_pHeaders;

        /// \brief ptr to the worker task queue in the http::curl_context that made this request
        http::curl_context::worker_task_queue_ptr m_pWorkerTaskQueue;

        /// \brief shared response data returned by last request
        http::request::response_data_type m_ResponseBody;

        /// \brief shared error state of the last request
        error m_RequestError = error::none;

        /// \brief user provided functor, called if request is successful
        http::request::response_handler_functor m_ResponseHandler;
        
        /// \brief user provided functor, called if request fails
        failure_handler_functor m_FailureHandler;

    public:
        /// \brief enqueues the request, to be performed by a worker
        virtual void fetch() override;

        /// \brief called by the main thread, determines if the
        /// success/failure functor should be called
        void main_update();

        /// \brief called by worker threads, performs enqueued requests
        void worker_update();

    protected:
        /// \brief ctor, inits all common request work
        curl_request(http::curl_context::worker_task_queue_ptr pWorkerTaskQueue,
            const std::string &aURL, 
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            const http::request::response_handler_functor &aResponseHandler,
            const http::request::failure_handler_functor &);

    public:
        virtual ~curl_request() = default;
    };
}

#endif

