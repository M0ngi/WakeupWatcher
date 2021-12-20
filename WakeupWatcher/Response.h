#pragma once

#include <cpprest/json.h>

class Client;

class Response
{
public:
    int response_code;
    web::json::value resp;
    Client* source;

    Response(int, web::json::value, Client*);
};

