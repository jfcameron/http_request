// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CONTEXT_H
#define JFC_HTTP_CONTEXT_H

#include <memory>
#include <string>
#include <vector>

#include <jfc/http/request.h>
#include <jfc/http/post.h>

namespace jfc::http
{
    class reponse_handler
    {
    public:
        virtual void worker_on_success(std::vector<char>) = 0;
        virtual void main_on_success() = 0;
        virtual void main_on_failure(const http::request::error) = 0;

        virtual ~reponse_handler() = default;
    };

    /// \brief library entry point.
    class context
    {
    public:
        using context_shared_ptr = std::shared_ptr<http::context>;
        using request_shared_ptr = std::shared_ptr<http::request>;

        /// \brief used to tell context factory what implementation to use
        enum class implementation
        {
            curl, /// !< use libcurl impl
            null /// !< testing, & when porting dependent code somewhere unsupported
            //TODO: wasm_browser_apis /// !< use browser apis if compiled to wasm
        };

        /// \brief create a context
        static context_shared_ptr make(const implementation &);
        
        /// \brief create a GET request
        /// worker processes response before returning to main
        virtual request_shared_ptr make_get(const std::string &aURL,
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            std::unique_ptr<http::reponse_handler> &&) = 0;

        /// \brief create a GET request
        /// worker returns the raw response directly to main
        request_shared_ptr make_get(const std::string& aURL,
            const std::string& aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string>& aHeaders,
            std::function<void(std::vector<char>)> aOnSuccess,
            std::function<void(const http::request::error)> aOnFailure);
        
        /// \brief create a POST request
        /// worker processes response before returning to main
        virtual std::shared_ptr<http::post> make_post(const std::string &aURL,
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            const std::string &aPostData,
            std::unique_ptr<http::reponse_handler> &&) = 0;

        /// \brief create a POST request
        /// worker processes response before returning to main
        std::shared_ptr<http::post> make_post(const std::string& aURL,
            const std::string& aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string>& aHeaders,
            const std::string& aPostData,
            std::function<void(std::vector<char>)> aOnSuccess,
            std::function<void(const http::request::error)> aOnFailure);
        
        /// \brief call handlers for completed requests 
        /// \warn must be called by the single "main" thread
        /// \note returns true if a request was handled
        virtual bool main_try_handle_completed_requests() = 0;

        /// \brief performs enqueued fetches.
        /// \note threadsafe.
        /// \warn must be called by at least one thread.
        /// \warn "main" can participate but this will cause blocking on main
        /// \note returns true if a fetch was performed
        virtual bool worker_try_perform_enqueued_request_fetches() = 0;
        
        /// \brief returns the number of requests currently enqueued
        virtual size_t enqueued_request_count() = 0;

        virtual ~context() = default;
    };
}

#endif

