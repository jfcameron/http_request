// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CONTEXT_H
#define JFC_HTTP_CONTEXT_H

#include <memory>
#include <string>
#include <vector>

#include <jfc/http/request.h>

namespace jfc::http
{
    class context
    {
    public:
        using context_shared_ptr = std::shared_ptr<http::context>;
        using request_shared_ptr = std::shared_ptr<http::request>;

        /// \brief used to tell context factor what implementation to use
        enum class implementation
        {
            curl, /// !< use libcurl impl
            //wasm_browser_apis /// !< use browser apis if compiled to wasm
        };

        /// \brief context factory
        static context_shared_ptr make(const implementation &);

        virtual void main_update() = 0;

        virtual void worker_update() = 0;

        /// \brief create a GET request
        virtual request_shared_ptr make_get(const std::string &aURL,
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            const http::request::response_handler_functor &,
            const http::request::failure_handler_functor &) = 0;

        virtual ~context() = default;

    protected:
        context() = default;
    };
}

#endif

