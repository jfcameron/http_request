// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CURL_REQUEST_H
#define JFC_HTTP_CURL_REQUEST_H

#include <atomic>
#include <functional>
#include <memory>
#include <vector>

#include <jfc/http/curl_context.h>
#include <jfc/http/request.h>

#include <curl/curl.h>

namespace jfc::http
{
    /// \brief curl implementation of http request
    class curl_request : public std::enable_shared_from_this<curl_request>
    {
    public:
        /// \brief try to enqueue the request
        bool try_enqueue();

        /// \brief fetch task run by worker threads
        void worker_fetch_task();
    
        /// \brief called by the main thread
        bool main_try_run_handlers();
        
        /// \brief wrapper for curl handle
        using curl_handle_ptr_type = std::shared_ptr<CURL>;
        
        /// \brief wrapper for curl linked list
        using curl_linked_list_ptr_type = std::shared_ptr<curl_slist>;

    private:
        std::weak_ptr<http::curl_context> m_pContext;

        /// \brief prevents main thread from enqueueing before running response handler
        bool m_bDoNotEnqueue = false;

        /// \brief bool that protects shared data
        std::atomic_flag m_bResponseLocked;

        /// \brief handle to the request in the curl context
        curl_handle_ptr_type m_pHandle;

        /// \brief ptr to a curl linked list used to store header entries
        curl_linked_list_ptr_type m_pHeaders;

        /// \brief shared response data returned by last request
        http::request::response_data_type m_ResponseBody;

        /// \brief shared error state of the last request
        http::request::error m_RequestError = http::request::error::none;

        /// \brief user provided functor, called if request is successful
        http::request::response_handler_functor m_ResponseHandler;
        
        /// \brief user provided functor, called if request fails
        http::request::failure_handler_functor m_FailureHandler;

    protected:
        /// \brief lets child types test if they are currently enqueued from main thread
        bool getIsEnqueued() const;

        /// \brief concrete request type specific configuration, run by worker
        virtual void on_enqueue_extra_worker_configuration(CURL *const pCURL);

        /// \brief ctor, inits all common request work
        curl_request(std::weak_ptr<http::curl_context> pContext,
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

