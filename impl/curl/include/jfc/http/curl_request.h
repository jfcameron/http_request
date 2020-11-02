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
    class curl_request : public http::request
    {
    public:
        using curl_handle_ptr_type = std::shared_ptr<CURL>;

        using curl_linked_list_ptr_type = std::shared_ptr<curl_slist>;

    private:
        std::atomic_flag m_bResponseLocked;

        curl_handle_ptr_type m_pHandle;

        curl_linked_list_ptr_type m_pHeaders;

        http::curl_context::worker_task_queue_ptr m_pWorkerTaskQueue;

        //
        http::request::response_data_type m_ResponseBody;

        error m_RequestError = error::none;

        http::request::response_handler_functor m_ResponseHandler;
        
        failure_handler_functor m_FailureHandler;

    public:
        virtual void fetch() override;

        void main_update();

        void worker_update();

    protected:
        curl_request(http::curl_context::worker_task_queue_ptr pWorkerTaskQueue,
            const std::string &aURL, 
            const std::string &aUserAgent,
            const std::vector<std::string> &aHeaders,
            const http::request::response_handler_functor &aResponseHandler,
            const http::request::failure_handler_functor &);

    public:
        virtual ~curl_request() = default;
    };
}

#endif

