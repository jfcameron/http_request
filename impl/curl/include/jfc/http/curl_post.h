// © Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CURL_POST_H
#define JFC_HTTP_CURL_POST_H

#include <jfc/http/curl_request.h>

#include <jfc/http/post.h>

namespace jfc::http
{
    /// \brief curl implementation of a http GET
    class curl_post final : public post, public curl_request
    {
    public:
    /// \name external interface
    ///@{
    //
        virtual bool try_enqueue() override;

        virtual void update_postdata(const std::string &aPostData) override;
    ///@}

        curl_post(std::weak_ptr<http::curl_context> pContext,
            const std::string &aURL, 
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            const std::string &aPostData,
            const http::request::response_handler_functor &aResponseHandler,
            const http::request::failure_handler_functor &);

        ~curl_post() = default;
    
    private:
        virtual void on_enqueue_extra_worker_configuration(CURL *const pCURL) override;

        std::string m_PostData;
    };
}

#endif

