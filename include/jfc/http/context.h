// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_CONTEXT_H
#define JFC_HTTP_CONTEXT_H

#include <memory>

#include <jfc/http/request.h>

namespace jfc::http
{
    class context
    {
    public:
        using context_shared_ptr = std::shared_ptr<http::context>;
        using request_shared_ptr = std::shared_ptr<http::request>;

        enum class implementation
        {
            curl
        };

        static context_shared_ptr make(const implementation &);

        virtual request_shared_ptr make_get() = 0;

        virtual ~context() = default;

    protected:
        context() = default;
    };
}

#endif

