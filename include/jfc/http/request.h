// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_REQUEST_H
#define JFC_HTTP_REQUEST_H

#include <jfc/http/request.h>

#include <functional>
#include <vector>

namespace jfc::http
{
    class request
    {
    public:
        using response_data_type = std::vector<char>;
        
        using response_handler_functor = std::function<void(response_data_type)>;
        
        enum class error
        {
            none,
            network_error,
            unhandled_error //!< used when no other information is available
        };

        using failure_handler_functor = std::function<void(error)>;
        
        //using response_succeeded_handler_type = std::function<void(std::vector<char>)>;
        //using response_failed_handler_type = std::functional<void()>;

        virtual void fetch() = 0;
    };
}

#endif

