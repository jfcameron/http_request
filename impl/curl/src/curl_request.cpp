// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/curl_request.h>

#include <stdexcept>

using namespace jfc;

http::curl_request::curl_request()
: m_handle([]()
{
    auto handle = curl_easy_init();

    if (!handle) throw std::runtime_error("could not initialize a new curl handle");

    return  handle;
}()
, [](CURL *p)
{
    curl_easy_cleanup(p);
})
{}

