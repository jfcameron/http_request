// © Joseph Cameron - All Rights Reserved

#ifndef JFC_HTTP_RESPONSE_HANDLER_H
#define JFC_HTTP_RESPONSE_HANDLER_H

namespace jfc::http
{
    /// \brief interface for user-defined request handler types
    ///
    /// when an enqueued request has been performed, these
    /// methods are called to allow the user to specify how 
    /// to process the server's response.
    class reponse_handler
    {
    public:
        /// \brief called on the worker thread if the request was successful.
        /// Provides an opportunity for user-defined processing to be done on
        /// the worker thread that performed the fetch
        virtual void worker_on_success(std::vector<char>) = 0;

        /// \brief called by the main thread if the request was successful.
        /// guaranteed to be called after worker_on_success has completed
        virtual void main_on_success() = 0;

        /// \brief called by main thread if the request was unsuccessful.
        /// this occurs if for example the fetch timed out or if the server 
        /// returned an error code
        virtual void main_on_failure(const http::request::error) = 0;

        virtual ~reponse_handler() = default;
    };
}

#endif

