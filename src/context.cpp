// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/context.h>

#include <jfc/http/curl_context.h>
#include <jfc/http/null_context.h>

using namespace jfc;

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

