[![Build Status](https://travis-ci.org/jfcameron/jfc-http_request.svg?branch=master)](https://travis-ci.org/jfcameron/jfc-http_request)[![Documentation](https://img.shields.io/badge/documentation-doxygen-blue.svg)](https://jfcameron.github.io/jfc-http_request/)

## jfc-http_request

Concurrent http request library, uses libcurl for implementation.

A single thread makes requests then handles their responses asynchronously. Any number of worker threads perform the requests concurrently. 

This project does not create threads or keep a thread group itself, this is done to allow the user to decide what threading strategy makes the most sense in the dependent project.

### state of project

currently supports GET and POST requests.

CI build scripts confirm Linux and Mac compatibility.

I am currently deciding how best to pull in libCURL for Windows.. likely submodule then static linking.

### usage

See demo for usage.

### building

see .travis.yml

