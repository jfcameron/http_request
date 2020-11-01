// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_GET_H
#define JFC_HTTP_GET_H

#include <jfc/http/curl_request.h>

namespace jfc::http
{
    class curl_get final : public curl_request
    {
    public:

    private:

    public:
        curl_get();

        ~curl_get() = default;
    };
}

#endif

