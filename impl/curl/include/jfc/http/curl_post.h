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

        virtual bool try_update_postdata(const std::string &aPostData) override;
    ///@}

        curl_post(std::weak_ptr<http::curl_context> pContext,
            const std::string &aURL, 
            const std::string &aUserAgent,
            const size_t aTimeoutMiliseconds,
            const std::vector<std::string> &aHeaders,
            const std::string &aPostData,
            std::unique_ptr<http::reponse_handler> && aHandler);

        ~curl_post() = default;
    
    private:
        virtual void worker_on_enqueued_perform_extra_configuration(CURL *const pCURL) override;

        std::string m_PostData;
    };
}

#endif

