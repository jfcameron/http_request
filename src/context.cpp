// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/context.h>

#include <jfc/http/curl_context.h>

using namespace jfc;

http::context::context_shared_ptr http::context::make(const implementation &)
{
    return http::context::context_shared_ptr(new http::curl_context());
}

