// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/context.h>

#include <jfc/http/curl_context.h>
#include <jfc/http/null_context.h>

using namespace jfc;

class basic_response_handler final : public http::reponse_handler
{
public:
    using success_functor_type = std::function<void(std::vector<char>)>;
    using failure_functor_type = std::function<void(const http::request::error)>;

    virtual void worker_on_success(std::vector<char> a) override { m_ResponseData = a; }
    virtual void main_on_success() override { m_success_functor(m_ResponseData); }
    virtual void main_on_failure(const http::request::error e) override { m_fail_functor(e); }

    basic_response_handler(success_functor_type aOnSuccess, failure_functor_type aOnFail)
        : m_success_functor(aOnSuccess)
        , m_fail_functor(aOnFail)
    {}

    ~basic_response_handler() = default;

private:
    std::vector<char> m_ResponseData;

    success_functor_type m_success_functor;
    failure_functor_type m_fail_functor;
};

http::context::request_shared_ptr http::context::make_get(const std::string& aURL,
    const std::string& aUserAgent,
    const size_t aTimeoutMiliseconds,
    const std::vector<std::string>& aHeaders,
    std::function<void(std::vector<char>)> aOnSuccess,
    std::function<void(const http::request::error)> aOnFailure)
{
    return make_get(aURL,
        aUserAgent,
        aTimeoutMiliseconds,
        aHeaders,
        std::make_unique<basic_response_handler>(aOnSuccess, aOnFailure));
}

std::shared_ptr<http::post> http::context::make_post(const std::string& aURL,
    const std::string& aUserAgent,
    const size_t aTimeoutMiliseconds,
    const std::vector<std::string>& aHeaders,
    const std::string& aPostData,
    std::function<void(std::vector<char>)> aOnSuccess,
    std::function<void(const http::request::error)> aOnFailure)
{
    return make_post(aURL,
        aUserAgent,
        aTimeoutMiliseconds,
        aHeaders,
        aPostData,
        std::make_unique<basic_response_handler>(aOnSuccess, aOnFailure));
}

http::context::context_shared_ptr http::context::make(const implementation &a)
{
    switch (a)
    {
        case context::implementation::curl:
        return http::context::context_shared_ptr(new http::curl_context());

        default:
        return http::context::context_shared_ptr(new http::null_context());
    }
}

