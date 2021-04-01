// © Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_POST_H
#define JFC_HTTP_POST_H

#include <jfc/http/request.h>

#include <functional>
#include <string>
#include <vector>

namespace jfc::http
{
    /// \brief HTTP POST
    class post : public request
    {
    public:
        /// \brief update the post data
        /// \note fails if called while the POST is enqueued
        virtual bool try_update_postdata(const std::string &aPostData) = 0;

        ~post() = default;
    };
}

#endif

