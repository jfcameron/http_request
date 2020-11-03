// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/curl_get.h>

using namespace jfc;

http::curl_get::curl_get(http::curl_context::worker_task_queue_ptr pWorkerTaskQueue,
    const std::string &aURL,
    const std::string &aUserAgent,
    const size_t aTimeoutMiliseconds,
    const std::vector<std::string> &aHeaders,
    const http::request::response_handler_functor &aResponseHandler,
    const http::request::failure_handler_functor &aFailureHandler)
: http::curl_request::curl_request(pWorkerTaskQueue, 
    aURL, 
    aUserAgent, 
    aTimeoutMiliseconds,
    aHeaders, 
    aResponseHandler, 
    aFailureHandler)
{}

