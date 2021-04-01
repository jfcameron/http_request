// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/curl_get.h>

using namespace jfc;

http::curl_get::curl_get(std::weak_ptr<http::curl_context> pContext,
    const std::string &aURL,
    const std::string &aUserAgent,
    const size_t aTimeoutMiliseconds,
    const std::vector<std::string> &aHeaders,
    const http::request::response_handler_functor &aResponseHandler,
    const http::request::failure_handler_functor &aFailureHandler)
: http::curl_request::curl_request(pContext, 
    aURL, 
    aUserAgent, 
    aTimeoutMiliseconds,
    aHeaders, 
    aResponseHandler, 
    aFailureHandler)
{}

bool http::curl_get::try_enqueue()
{
    return curl_request::try_enqueue();
}

