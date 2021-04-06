// © Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_NULL_CONTEXT_H
#define JFC_HTTP_NULL_CONTEXT_H

#include <jfc/http/context.h>

namespace jfc::http
{
    class null_get : public http::request
    {
    public:
        virtual bool try_enqueue() override { return false; }

        null_get() = default;
    };

    class null_post : public http::post
    {
    public:
        virtual bool try_enqueue() override { return false; }
        
        virtual bool try_update_postdata(const std::string &aPostData) override 
        { 
            return false; 
        }
        
        null_post() = default;
    };

    class null_context final : public http::context
    {
    public:
    /// \name external interface
    ///@{
    //
        virtual request_shared_ptr make_get(const std::string &aURL,
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            const http::request::response_handler_functor &,
            const http::request::failure_handler_functor &) override
        {
            return request_shared_ptr(new null_get());
        }

        virtual std::shared_ptr<http::post> make_post(const std::string &aURL,
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            const std::string &aPostData,
            const http::request::response_handler_functor &,
            const http::request::failure_handler_functor &) override
        {
            return std::shared_ptr<http::post>(new null_post());
        }

        virtual bool main_try_handle_completed_requests() override { return false; }

        virtual bool worker_try_perform_enqueued_request_fetches() override { return false; }
        
        virtual size_t enqueued_request_count() override { return 0; }
    ///@}

        null_context() = default;

        ~null_context() = default;
    };
}

#endif

