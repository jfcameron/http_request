// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_REQUEST_H
#define JFC_HTTP_REQUEST_H

#include <functional>
#include <vector>

namespace jfc::http
{
    class request
    {
    public:
        /// \brief enqueues the request to be performed by a worker. Fails if already enqueued
        virtual bool try_enqueue() = 0;

        enum class error
        {
            none,
            network_error,
            unhandled_error //!< used when no other information is available
        };
        
        using response_data_type = std::vector<char>; //TODO: std::byte? unsure

        using response_handler_functor = std::function<void(response_data_type)>;
        using failure_handler_functor = std::function<void(error)>;
    };
}

#endif

