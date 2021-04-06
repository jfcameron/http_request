// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/curl_get.h>

using namespace jfc;

http::curl_get::curl_get(std::weak_ptr<http::curl_context> pContext,
    const std::string &aURL,
    const std::string &aUserAgent,
    const size_t aTimeoutMiliseconds,
    const std::vector<std::string> &aHeaders,
    std::unique_ptr<http::reponse_handler> && aHandler)
: http::curl_request::curl_request(pContext, 
    aURL, 
    aUserAgent, 
    aTimeoutMiliseconds,
    aHeaders, 
    std::move(aHandler))
{}

bool http::curl_get::try_enqueue()
{
    return curl_request::try_enqueue();
}

