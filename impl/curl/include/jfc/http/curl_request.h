// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_REQUEST_H
#define JFC_HTTP_REQUEST_H

#include <functional>
#include <memory>
#include <vector>

#include <curl/curl.h>

namespace jfc::http
{
    class curl_request
    {
    public:
        using curl_handle_ptr_type = std::shared_ptr<CURL>;

    private:
        curl_handle_ptr_type m_handle;

    protected:
        curl_request();

        //get_handle();

    public:
        virtual ~curl_request() = default;
    };
}

#endif

