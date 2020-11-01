// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CURL_CONTEXT_H
#define JFC_HTTP_CURL_CONTEXT_H

#include <jfc/http/context.h>

namespace jfc::http
{
    class curl_context final : public http::context
    {
    public:
        virtual request_shared_ptr make_get() override;

        curl_context();

        virtual ~curl_context();
    };
}

#endif

