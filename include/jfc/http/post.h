// © Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_POST_H
#define JFC_HTTP_POST_H

#include <jfc/http/request.h>

#include <functional>
#include <string>
#include <vector>

namespace jfc::http
{
    class post : public request
    {
    public:
        virtual void update_postdata(const std::string &aPostData) = 0;
    };
}

#endif

