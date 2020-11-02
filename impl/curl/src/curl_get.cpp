// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/http/curl_get.h>

using namespace jfc;

http::curl_get::curl_get(http::curl_context::worker_task_queue_ptr pWorkerTaskQueue,
    const std::string &aURL,
    const std::string &aUserAgent,
    const std::vector<std::string> &aHeaders)
: http::curl_request::curl_request(pWorkerTaskQueue, aURL, aUserAgent, aHeaders)
{}

