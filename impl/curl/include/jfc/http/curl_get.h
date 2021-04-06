// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CURL_GET_H
#define JFC_HTTP_CURL_GET_H

#include <jfc/http/request.h>
#include <jfc/http/curl_request.h>

namespace jfc::http
{
    /// \brief curl implementation of a http GET
    class curl_get final : public curl_request, public request
    {
    public:
        virtual bool try_enqueue() override;

        curl_get(std::weak_ptr<http::curl_context> pContext,
            const std::string &aURL, 
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            std::unique_ptr<http::reponse_handler> && aHandler);

        ~curl_get() = default;
    };
}

#endif

